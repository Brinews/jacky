/****
 * Author: 2975653
 * Date: 2014-05-17
 * Function: Stack usage
 */

import java.lang.*;

public class Calculator
{
	/***
	 * Helper container
	 */
	QueueReferenceBased infix = new QueueReferenceBased();
	QueueReferenceBased postfix = new QueueReferenceBased();
	StackReferenceBased stack = new StackReferenceBased();

	public Calculator()
	{
	}

	public void calTokenProcess(String exp)
	{
		String token;
		char c;

		if ("".equals(exp)) return;

		for (int i = 0; i < exp.length(); i++)
		{
			c = exp.charAt(i);
			token = "";

			if (c >= '0' && c <= '9')
			{
				do {
					token += c;
					i++;
				} while (i < exp.length() && (c = exp.charAt(i)) >= '0'
						&& (c >= '0' && c <= '9'));

				if (i != exp.length())
					i--;
			}
			else if (c == '(' || c == ')' 
				|| c == '+' || c == '-'
				|| c == '*' || c == '/')
			{
				token = "" + c;
			}
			else if (c == ' ')
			{
				do {
					i++;
				} while (i < exp.length() &&(c = exp.charAt(i)) == ' ');

				if (i != exp.length()) i--;
			}
			else 
			{
				System.out.println("Invalid Character in Expression.");

				infix.dequeueAll();
				return;
			}

			try 
			{
				if (!"".equals(token))
					infix.enqueue(token);
			}
			catch (QueueException qe)
			{
				System.out.println(qe.toString());
			}
		}
	}

	boolean isOperator(String s)
	{
		char c = s.charAt(0);

		if (c == '+' || c == '-' ||
				c == '*' || c == '/')
			return true;

		return false;
	}

	boolean isPrior(String ps, String qs)
	{
		char p = ps.charAt(0);
		char q = qs.charAt(0);

		if (p == '*' || p == '/')
			return true;

		if ((p == '+' || p == '-')
				&& (q == '+' || q == '-'))
			return true;

		return false;
	}

	/***
	 * infix expesssion to postfix 
	 */
	public void calTransform()
	{
		if (infix.isEmpty()) return;

		StackReferenceBased optor = new StackReferenceBased();

		String token, top;

		try {
			while (!infix.isEmpty())
			{
				token = infix.dequeue().toString();

				char c = token.charAt(0);
				if (c >= '0' && c <= '9')
				{
					postfix.enqueue(token);
				}
				else if(isOperator(token))
				{
					while (!optor.isEmpty() 
							&& isOperator(optor.peek().toString())
							&& isPrior(optor.peek().toString(), token))
					{
						postfix.enqueue(optor.peek());
						optor.pop();
					}

					optor.push(token);
				}
				else if ( c == '(')
				{
					optor.push(token);
				}
				else if (c == ')')
				{
					while(!optor.isEmpty() && !optor.peek().equals("("))
					{
						postfix.enqueue(optor.peek());
						optor.pop();
					}

					if (!optor.peek().equals("("))
					{
						System.out.println("Syntax error: brackets mismatch -> too many open brackets");

						postfix.dequeueAll();
						return ;
					}

					optor.pop();
				}
			}

			while (!optor.isEmpty())
			{
				if (optor.peek().equals("("))
				{
					System.out.println("Syntax error: brackets mismatch -> too many open brackets");

					postfix.dequeueAll();
					return ;
				}

				postfix.enqueue(optor.peek());
				optor.pop();
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	/***
	 * print out the postfix
	 */
	public void calPrintPostfix()
	{
		if (!postfix.isEmpty())
		{
			System.out.print("Postfix form :");
		}

		QueueReferenceBased qfb = new QueueReferenceBased();

		while (!postfix.isEmpty())
		{
			qfb.enqueue(postfix.peek());
			System.out.print(postfix.dequeue() + " ");
		}

		postfix = qfb;
	}

	/****
	 * calculate by postfix expression
	 */
	public double calEvaluate()
	{
		if (postfix.isEmpty()) return Double.NaN;

		String token;
		double a, b;
		char c;

		stack.popAll();

		while (!postfix.isEmpty())
		{
			token = postfix.dequeue().toString();

			c = token.charAt(0);

			if (c >= '0' && c <= '9')
			{
				stack.push(token);
			}
			else if (c == '+')
			{
				a = Double.parseDouble(stack.pop().toString());
				b = Double.parseDouble(stack.pop().toString());

				a = a + b;
				stack.push(a);
			}
			else if (c == '-')
			{
				a = Double.parseDouble(stack.pop().toString());
				b = Double.parseDouble(stack.pop().toString());

				a = b - a;
				stack.push(a);
			}
			else if (c == '*')
			{
				a = Double.parseDouble(stack.pop().toString());
				b = Double.parseDouble(stack.pop().toString());

				a = a * b;
				stack.push(a);
			}
			else if (c == '/')
			{
				a = Double.parseDouble(stack.pop().toString());
				b = Double.parseDouble(stack.pop().toString());

				a = b / a;
				stack.push(a);
			}
		}

		if (stack.isEmpty())
		{
			return Double.NaN;
		}

		return Double.parseDouble(stack.pop().toString());
	}

	public static void main(String[] args)
	{
		Calculator cal = new Calculator();

		if (args.length < 1)
		{
			System.out.println("Usage Calculator <expression>");
			return;
		}

		//System.out.println(args[0]);
		System.out.println("A simple calculator by 2975653 :");
		System.out.println("Evaluating ... ");

		cal.calTokenProcess(args[0]);
		cal.calTransform();
		
		/* get result */

		cal.calPrintPostfix();
		double ret = cal.calEvaluate();

		if (!Double.isNaN(ret)) {
			System.out.println("\nResult : " + ret);
		}
	}
}
