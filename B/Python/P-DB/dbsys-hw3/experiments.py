import sqlite3
import time

import Database
from Catalog.Schema import DBSchema

import sys
import unittest

import warnings
import copy

import Query.Optimizer as Optimizer

def tests(printOutput):
	db = setup()
	'''select
			sum(l_extendedprice * l_discount) as revenue
		from
			lineitem
		where
			l_shipdate >= 19940101
			and l_shipdate < 19950101
			and l_discount between 0.06 - 0.01 and 0.06 + 0.01
			and l_quantity < 24'''
	query1 = db.query().fromTable('lineitem') \
				.where('L_SHIPDATE >= 19940101') \
		.where('L_SHIPDATE < 19950101') \
		.where('L_DISCOUNT > 0.06 - 0.01') \
		.where('L_DISCOUNT < 0.06 + 0.01') \
		.where('L_QUANTITY < 24') \
		.groupBy(groupSchema=DBSchema('REV', [('dum', 'int')]),
								 aggSchema=DBSchema('SUM', [('SUM', 'double')]),
								 groupExpr=(lambda e: (1)),
								 aggExprs=[(0, lambda acc, e: acc + e.L_EXTENDEDPRICE * e.L_DISCOUNT, lambda x: x)],
								 groupHashFn=(lambda gbVal: hash(gbVal[0]) % 111)) \
		.select({'REVENUE': ('SUM', 'double')}) \
		.finalize()
		
	'''select
				sum(l_extendedprice * (1 - l_discount)) as promo_revenue
	from
				lineitem,
				part
	where
				l_partkey = p_partkey
				and l_shipdate >= 19950901
				and l_shipdate < 19951001'''
	query2 = db.query().fromTable('lineitem') \
		.join(db.query().fromTable('part'),
							rhsSchema=db.relationSchema('part'),
							method='block-nested-loops',
							expr='L_PARTKEY == P_PARTKEY') \
		.where('L_SHIPDATE >= 19950901') \
		.where('L_SHIPDATE < 19951001') \
		.groupBy(groupSchema=DBSchema('REV', [('dum', 'int')]),
								 aggSchema=DBSchema('SUM', [('SUM', 'double')]),
								 groupExpr=(lambda e: (1)),
								 aggExprs=[(0, lambda acc, e: acc + e.L_EXTENDEDPRICE * (1 - e.L_DISCOUNT), lambda x: x)],
								 groupHashFn=(lambda gbVal: hash(gbVal[0]) % 111)) \
		.select({'PROMO_REVENUE': ('SUM', 'double')}) \
		.finalize()
		
	'''select
				l_orderkey,
				sum(l_extendedprice * (1 - l_discount)) as revenue,
				o_orderdate,
				o_shippriority
	from
				customer,
				orders,
				lineitem
	where
				c_mktsegment = 'BUILDING'
				and c_custkey = o_custkey
				and l_orderkey = o_orderkey
				and o_orderdate < 19950315
				and l_shipdate > 19950315
	group by
		l_orderkey,
				o_orderdate,
				o_shippriority'''
	query3 = db.query().fromTable('customer') \
		.join(db.query().fromTable('orders'),
							rhsSchema=db.relationSchema('orders'),
							method='block-nested-loops',
							expr='C_CUSTKEY == O_CUSTKEY') \
		.join(db.query().fromTable('lineitem'),
							rhsSchema=db.relationSchema('lineitem'),
							method='block-nested-loops',
							expr='L_ORDERKEY == O_ORDERKEY') \
		.where('L_SHIPDATE == \'BUILDING\'') \
		.where('O_ORDERDATE < 19950315') \
		.where('L_SHIPDATE > 19950315') \
		.groupBy(groupSchema=DBSchema('FIRST', [('L_ORDERKEY', 'int'), ('O_ORDERDATE', 'char(10)'), ('O_SHIPPRIORITY', 'int')]),
								 aggSchema=DBSchema('SUM', [('SUM', 'double')]),
								 groupExpr=(lambda e: (e.L_ORDERKEY, e.O_ORDERDATE, e.O_SHIPPRIORITY)),
								 aggExprs=[(0, lambda acc, e: acc + e.L_EXTENDEDPRICE * (1 - e.L_DISCOUNT), lambda x: x)],
								 groupHashFn=(lambda gbVal: hash(gbVal[0]) % 111)) \
		.select({'L_ORDERKEY': ('L_ORDERKEY', 'int'), 'REVENUE': ('SUM', 'double'),'O_ORDERDATE': ('O_ORDERDATE', 'char(10)'),'O_SHIPPRIORITY': ('O_SHIPPRIORITY', 'int')}) \
		.finalize()
		
	'''select
				c_custkey,
				c_name,
				sum(l_extendedprice * (1 - l_discount)) as revenue,
				c_acctbal,
				n_name,
				c_address,
				c_phone,
				c_comment
	from
				customer,
				orders,
				lineitem,
				nation
	where
				c_custkey = o_custkey
				and l_orderkey = o_orderkey
				and o_orderdate >= 19931001
				and o_orderdate < 19940101
				and l_returnflag = 'R'
				and c_nationkey = n_nationkey
	group by
				c_custkey,
				c_name,
				c_acctbal,
				c_phone,
				n_name,
				c_address,
				c_comment'''
	query4 = db.query().fromTable('customer') \
		.join(db.query().fromTable('orders'),
							rhsSchema=db.relationSchema('orders'),
							method='block-nested-loops',
							expr='C_CUSTKEY == O_CUSTKEY') \
		.join(db.query().fromTable('lineitem'),
							rhsSchema=db.relationSchema('lineitem'),
							method='block-nested-loops',
							expr='L_ORDERKEY == O_ORDERKEY') \
		.where('L_SHIPDATE == \'BUILDING\'') \
		.where('O_ORDERDATE >= 19931001') \
		.where('O_ORDERDATE < 19940101') \
		.where('L_RETURNFLAG == \'R\'') \
		.join(db.query().fromTable('nation'),
							rhsSchema=db.relationSchema('nation'),
							method='block-nested-loops',
							expr='C_NATIONKEY == N_NATIONKEY') \
		.groupBy(groupSchema=DBSchema('FIRST', [('C_CUSTKEY', 'int'), ('C_NAME', 'char(25)'), ('C_ACCTBAL', 'double'), ('C_PHONE', 'char(15)'), ('N_NAME', 'char(25)'),('C_ADDRESS', 'char(40)'),('C_COMMENT', 'char(117)')]),
								 aggSchema=DBSchema('SUM', [('SUM', 'double')]),
								 groupExpr=(lambda e: (e.C_CUSTKEY, e.C_NAME, e.C_ACCTBAL, e.C_PHONE, e.N_NAME, e.C_ADDRESS, e.C_PHONE, e.C_COMMENT)),
								 aggExprs=[(0, lambda acc, e: acc + e.L_EXTENDEDPRICE * (1 - e.L_DISCOUNT), lambda x: x)],
								 groupHashFn=(lambda gbVal: hash(gbVal[0]) % 111)) \
		.select({'C_CUSTKEY' : ('C_CUSTKEY', 'int'), 'C_NAME' : ('C_NAME', 'char(25)'),'SUM' : ('SUM', 'double'), 'C_ACCTBAL' : ('C_ACCTBAL', 'double'), 'C_PHONE' : ('C_PHONE', 'char(15)'), 'N_NAME' : ('N_NAME', 'char(25)'),'C_ADDRESS' : ('C_ADDRESS', 'char(40)'),'C_COMMENT' : ('C_COMMENT', 'char(117)')}) \
		.finalize()
		
	'''select
				n_name,
				sum(l_extendedprice * (1 - l_discount)) as revenue
	from
				customer,
				orders,
				lineitem,
				supplier,
				nation,
				region
	where
				c_custkey = o_custkey
				and l_orderkey = o_orderkey
				and l_suppkey = s_suppkey
				and c_nationkey = s_nationkey
				and s_nationkey = n_nationkey
				and n_regionkey = r_regionkey
				and r_name = 'ASIA'
				and o_orderdate >= 19940101
				and o_orderdate < 19950101
	group by
				n_name'''
	query5 = db.query().fromTable('customer') \
		.join(db.query().fromTable('orders'),
							rhsSchema=db.relationSchema('orders'),
							method='block-nested-loops',
							expr='C_CUSTKEY == O_CUSTKEY') \
		.join(db.query().fromTable('lineitem'),
							rhsSchema=db.relationSchema('lineitem'),
							method='block-nested-loops',
							expr='L_ORDERKEY == O_ORDERKEY') \
		.join(db.query().fromTable('supplier'),
							rhsSchema=db.relationSchema('supplier'),
							method='block-nested-loops',
							expr='L_SUPPKEY == S_SUPPKEY and C_NATIONKEY == S_NATIONKEY') \
		.join(db.query().fromTable('nation'),
							rhsSchema=db.relationSchema('nation'),
							method='block-nested-loops',
							expr='S_NATIONKEY == N_NATIONKEY') \
		.join(db.query().fromTable('region'),
							rhsSchema=db.relationSchema('region'),
							method='block-nested-loops',
							expr='N_REGIONKEY == R_REGIONKEY') \
		.where('L_SHIPDATE == \'BUILDING\'') \
		.where('R_NAME == \'ASIA\'') \
		.where('O_ORDERDATE >= 19940101') \
		.where('O_ORDERDATE < 19950101') \
		.groupBy(groupSchema=DBSchema('FIRST', [('N_NAME', 'char(25)')]),
								 aggSchema=DBSchema('SUM', [('SUM', 'double')]),
								 groupExpr=(lambda e: (e.L_ORDERKEY, e.O_ORDERDATE, e.O_SHIPPRIORITY)),
								 aggExprs=[(0, lambda acc, e: acc + e.L_EXTENDEDPRICE * (1 - e.L_DISCOUNT), lambda x: x)],
								 groupHashFn=(lambda gbVal: hash(gbVal[0]) % 111)) \
		.select({'N_NAME': ('N_NAME', 'char(25)'), 'SUM' : ('SUM', 'double')}) \
		.finalize()
	

	queries = [query1, query2, query3, query4, query5]
	names = ['query1', 'query2', 'query3', 'query4', 'query5']
	opt = Optimizer.Optimizer(db)
	for i,q in enumerate(queries):
		querytest(db, printOutput, q, names[i], opt)

def setup():
	db = Database.Database()
	return db

def querytest(db, printOutput, q, qname, opt):
	start = time.time()
	processedQuery = [q.schema().unpack(tup) for page in db.processQuery(q) for tup in page[1]]
	end = time.time()
	duration = end - start
	print('Query: ' + qname)
	if printOutput:
		print('\tResults: ', processedQuery)
	print('\tTime', duration)
	
	start = time.time()
	qopt = opt.pushdownOperators(q)
	qopt = opt.pickJoinOrder(qopt)
	processedQuery = [qopt.schema().unpack(tup) for page in db.processQuery(qopt) for tup in page[1]]
	end = time.time()
	duration = end - start
	print('Query Optimized: ' + qname)
	if printOutput:
		print('\tResults: ', processedQuery)
	print('\tTime', duration)
	
	
	
	

# databases = ['HW2_0.001.db', 'HW2_0.01.db']
# print(sqlite_tests(databases[0]))
# print(sqlite_tests(databases[1]))
tests(True)

