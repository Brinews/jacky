import itertools

from Query.Plan import Plan
from Query.Operators.Join import Join
from Query.Operators.Project import Project
from Query.Operators.Select import Select
from Utils.ExpressionInfo import ExpressionInfo

class Optimizer:
	"""
	A query optimization class.

	This implements System-R style query optimization, using dynamic programming.
	We only consider left-deep plan trees here.

	We provide doctests for example usage only.
	Implementations and cost heuristics may vary.

	>>> import Database
	>>> db = Database.Database()
	>>> try:
	...	 db.createRelation('department', [('did', 'int'), ('eid', 'int')])
	...	 db.createRelation('employee', [('id', 'int'), ('age', 'int')])
	... except ValueError:
	...	 pass

	# Join Order Optimization
	>>> query4 = db.query().fromTable('employee').join( \
				db.query().fromTable('department'), \
				method='block-nested-loops', expr='id == eid').finalize()

	>>> db.optimizer.pickJoinOrder(query4)

	# Pushdown Optimization
	>>> query5 = db.query().fromTable('employee').union(db.query().fromTable('employee')).join( \
				db.query().fromTable('department'), \
				method='block-nested-loops', expr='id == eid')\
				.where('eid > 0 and id > 0 and (eid == 5 or id == 6)')\
				.select({'id': ('id', 'int'), 'eid':('eid','int')}).finalize()

	>>> db.optimizer.pushdownOperators(query5)

	"""

	def __init__(self, db):
		self.db = db
		self.statsCache = {}

	# Caches the cost of a plan computed during query optimization.
	def addPlanCost(self, plan, cost):
		return None

	# Checks if we have already computed the cost of this plan.
	def getPlanCost(self, plan):
		return 10

	# Given a plan, return an optimized plan with both selection and
	# projection operations pushed down to their nearest defining relation
	# This does not need to cascade operators, but should determine a
	# suitable ordering for selection predicates based on the cost model below.
	def pushdownOperators(self, plan):
		plan = Plan(root=self.pushdownOperator(plan.root))
		return plan

	def pushdownOperator(self, op):
		if op.operatorType() == "TableScan":
			return op
		elif op.operatorType() in ["GroupBy", "Sort"]:
			op.subPlan = self.pushdownOperator(op.subPlan)
			return op
		elif op.operatorType() == "UnionAll" or "Join" in op.operatorType():
			op.lhsPlan = self.pushdownOperator(op.lhsPlan)
			op.rhsPlan = self.pushdownOperator(op.rhsPlan)
			return op
		elif op.operatorType() == "Project":
			return self.pushdownProject(op)
		elif op.operatorType() == "Select":
			return self.pushdownSelect(op)
		else:
			print("Unmatched operatorType in pushdownOperator(): " + op.operatorType())
			raise NotImplementedError
			
	def pushdownProject(self, op):
		# First pushdown operators below:
		op.subPlan = self.pushdownOperator(op.subPlan)

		if op.subPlan.operatorType() in ["GroupBy", "TableScan"]:
			return op

		elif op.subPlan.operatorType() == "Project":
			# Attempt to remove redundant projections:
			bools = [op.subPlan.projectExprs[key][0].isAttribute() for key in op.projectExprs]
			if False not in bools:
				op.subPlan = op.subPlan.subPlan
			return self.pushdownOperator(op)

		elif op.subPlan.operatorType() == "Select":
			# Move op below its subplan if op provides all attributes needed for the selectExpr
			selectAttrs = ExpressionInfo(op.subPlan.selectExpr).getAttributes()
			outputAttrs = set(op.projectExprs.keys())
			result = op
			if selectAttrs.issubset(outputAttrs):
				result = op.subPlan
				op.subPlan = result.subPlan
				result.subPlan = self.pushdownOperator(op)
			return result

		elif op.subPlan.operatorType() == "Sort":
			return op

		elif op.subPlan.operatorType() == "UnionAll":
			# Place a copy of op on each side of the union
			result = op.subPlan
			result.lhsPlan = self.pushdownOperator(Project(result.lhsPlan, op.projectExprs))
			result.rhsPlan = self.pushdownOperator(Project(result.rhsPlan, op.projectExprs))
			return result

		elif "Join" in op.subPlan.operatorType():
			# Partition the projections among the input relations, as much as possible
			lhsAttrs = set(op.subPlan.lhsPlan.schema().fields)
			rhsAttrs = set(op.subPlan.rhsPlan.schema().fields)
			lhsProjectExprs = {}
			rhsProjectExprs = {}
			remainingProjectExprs = False

			for attr in op.projectExprs:
				requiredAttrs = ExpressionInfo(op.projectExprs[attr][0]).getAttributes()
				if requiredAttrs.issubset(lhsAttrs):
					lhsProjectExprs[attr] = op.projectExprs[attr]
				elif requiredAttrs.issubset(rhsAttrs):
					rhsProjectExprs[attr] = op.projectExprs[attr]
				else:
					remainingProjectExprs = True

			if lhsProjectExprs:
				op.subPlan.lhsPlan = self.pushdownOperator(Project(op.subPlan.lhsPlan, lhsProjectExprs))
			if rhsProjectExprs:
				op.subPlan.rhsPlan = self.pushdownOperator(Project(op.subPlan.rhsPlan, rhsProjectExprs))

			result = op
			# Remove op from the tree if there are no remaining project expressions, and each side of the join recieved a projection
			if not remainingProjectExprs and lhsProjectExprs and rhsProjectExprs:
				result = op.subPlan
			return result
		else:
			print("Unmatched operatorType in pushdownOperator(): " + op.operatorType())
			raise NotImplementedError

	def pushdownSelect(self, op):
		# First pushdown operators below:
		op.subPlan = self.pushdownOperator(op.subPlan)

		if op.subPlan.operatorType() in ["GroupBy", "TableScan", "Project"]:
			return op

		elif op.subPlan.operatorType() == "Select":
			# Reorder two selects based on 'score'
			useEstimated = True
			opScore = (1 - op.selectivity(useEstimated)) / op.tupleCost
			childScore = (1 - op.subPlan.selectivity(useEstimated)) / op.tupleCost

			result = op
			if childScore > opScore:
				result = op.subPlan
				op.subPlan = result.subPlan
				result.subPlan = self.pushdownOperator(op)
			return result

		elif op.subPlan.operatorType() == "Sort":
			# Always move a select below a sort
			result = op.subPlan
			op.subPlan = result.subPlan
			result.subPlan = self.pushdownOperator(op)
			return result

		elif op.subPlan.operatorType() == "UnionAll":
			# Place a copy of op on each side of the union
			result = op.subPlan
			result.lhsPlan = self.pushdownOperator(Select(result.lhsPlan, op.selectExpr))
			result.rhsPlan = self.pushdownOperator(Select(result.rhsPlan, op.selectExpr))
			return result

		elif "Join" in op.subPlan.operatorType():
			# Partition the select expr as much as possible
			exprs = ExpressionInfo(op.selectExpr).decomposeCNF()
			lhsExprs = []
			rhsExprs = []
			remainingExprs = []

			lhsAttrs = set(op.subPlan.lhsPlan.schema().fields)
			rhsAttrs = set(op.subPlan.rhsPlan.schema().fields)

			for e in exprs:
				attrs = ExpressionInfo(e).getAttributes()
				if attrs.issubset(lhsAttrs):
					lhsExprs.append(e)
				elif attrs.issubset(rhsAttrs):
					rhsExprs.append(e)
				else:
					remainingExprs.append(e)

			if lhsExprs:
				newLhsExpr = ' and '.join(lhsExprs)
				lhsSelect = Select(op.subPlan.lhsPlan, newLhsExpr)
				op.subPlan.lhsPlan = self.pushdownOperator(lhsSelect)

			if rhsExprs:
				newRhsExpr = ' and '.join(rhsExprs)
				rhsSelect = Select(op.subPlan.rhsPlan, newRhsExpr)
				op.subPlan.rhsPlan = self.pushdownOperator(rhsSelect)

			result = None
			if remainingExprs:
				newExpr = ' and '.join(remainingExprs)
				result = Select(op.subPlan, newExpr)
			else:
				result = op.subPlan

			return result
		else:
			print("Unmatched operatorType in pushdownOperator(): " + op.operatorType())
			raise NotImplementedError

	# Returns an optimized query plan with joins ordered via a System-R style
	# dyanmic programming algorithm. The plan cost should be compared with the
	# use of the cost model below.
	def pickJoinOrder(self, plan):
		# Extract all base relations, along with any unary operators immediately above.
		base_relations = set(plan.root.inputs())

		# Extract all joins in original plan, they serve as the set of joins actually necessary.
		joins = plan.root.joins()

		# Define the dynamic programming table.
		optimal_plans = {}

		# Establish optimal access paths.
		for relation in base_relations:
			optimal_plans[frozenset((relation,))] = relation

		# Fill in the table.
		for i in range(2, len(base_relations) + 1):
			for subset in itertools.combinations(base_relations, i):

				# Build the set of candidate joins.
				candidate_joins = set()
				for candidate_relation in subset:
					candidate_joins.add((
						optimal_plans[frozenset(tuple_without(subset, candidate_relation))],
						optimal_plans[frozenset((candidate_relation,))]
					))

				# Find the best of the candidate joins.
				optimal_plans[frozenset(subset)] = self.get_best_join(candidate_joins, joins)

		# Reconstruct the best plan, prepare and return.
		final_plan = Plan(root = optimal_plans[frozenset(base_relations)])
		final_plan.prepare(self.db)
		plan = final_plan
		return final_plan

	def get_best_join(self, candidates, required_joins):
		best_plan_cost = None
		best_plan = None
		for left, right in candidates:

			relevant_expr = None

			# Find the joinExpr corresponding to the current join candidate. If there is none, it's a
			# cartesian product.
			for join in required_joins:
				names = ExpressionInfo(join.joinExpr).getAttributes()
				if set(join.rhsSchema.fields).intersection(names) and set(join.lhsSchema.fields).intersection(names):
					relevant_expr = join.joinExpr
					break
			else:
				relevant_expr = 'True'

			# Construct a join plan for the current candidate, for each possible join algorithm.
			# TODO: Evaluate more than just nested loop joins, and determine feasibility of those methods.
			for algorithm in ["nested-loops", "block-nested-loops", "hash"]:
				test_plan = Plan(root = Join(
					lhsPlan = left,
					rhsPlan = right,
					method = algorithm,
					expr = relevant_expr
				))

				# Prepare and run the plan in sampling mode, and get the estimated cost.
				test_plan.prepare(self.db)
				test_plan.sample(5.0)
				cost = test_plan.cost(estimated = True)

				# Update running best.
				if best_plan_cost is None or cost < best_plan_cost:
					best_plan_cost = cost
					best_plan = test_plan

		# Need to return the root operator rather than the plan itself, since it's going back into the
		# table.
		return best_plan.root

	# Optimize the given query plan, returning the resulting improved plan.
	# This should perform operation pushdown, followed by join order selection.
	def optimizeQuery(self, plan):
		pushedDown_plan = self.pushdownOperators(plan)
		joinPicked_plan = self.pickJoinOrder(pushedDown_plan)

		return joinPicked_plan

if __name__ == "__main__":
	import doctest
	doctest.testmod()
