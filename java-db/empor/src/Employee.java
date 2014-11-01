package beans;

import java.sql.*;
import java.util.*;
import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

public class Employee 
{
	public EmpInfo oneEmp = null;

	public Employee()
	{
		super();
	}

	/* read employee information by emp_id */
	public EmpInfo getEmployeeInfoById(String id)
	{
		oneEmp = new EmpInfo();

		MySqlProxy mySql = new MySqlProxy();

		try {
			mySql.spyConnectDB();
			ResultSet myRS = mySql.spyDoQuery("select emp_id,emp_name,status,job_title,site_no from employee where emp_id=" + id);

			while (myRS.next()) {
				oneEmp.empId = myRS.getString(1);
				oneEmp.empName = myRS.getString(2);
				oneEmp.empStatus = myRS.getString(3);
				oneEmp.empTitle = myRS.getString(4);
				oneEmp.empSite = myRS.getString(5);
			}

			myRS.close();
			mySql.spyCloseConnection();
		}
		catch (Exception e)
		{
			System.out.println(e.getMessage());
		}

		return oneEmp;
	}

	/* update employee infomation */
	public static boolean setEmployeeInfo(EmpInfo emp)
	{
		String ret = "";

		MySqlProxy mySql = new MySqlProxy();

		/* update employee info SQL*/
		String updSql = "update employee set status='"+emp.empStatus+"', "+"job_title='"+emp.empTitle+"',"+"site_no='"+emp.empSite+"' where emp_id='"+emp.empId+"'";

		try {
			mySql.spyConnectDB();

			System.out.println(updSql);

			ret = mySql.spyDoUpdate(updSql);
			mySql.spyCloseConnection();
		}
		catch (Exception e)
		{
			System.out.print(e.getMessage());
		}

		if ("ACK".equals(ret))
		{
			return true;
		}

		return false;
	}

	/* add course-employee finish infomation */
	public static boolean setTrainingInfo(TrainingInfo tinf)
	{
		String ret = "";

		Employee E = new Employee();
		EmpInfo emp = E.getEmployeeInfoById(tinf.empId);
		if ("".equals(emp.empName))
		{
			/* not exist employee */
			return false;
		}

		MySqlProxy mySql = new MySqlProxy();

		String updSql = "insert into training values('"+tinf.empId+"','"+tinf.finDate+"','"+tinf.cosName+"')";

		try {
			mySql.spyConnectDB();

			System.out.println(updSql);

			ret = mySql.spyDoUpdate(updSql);
			mySql.spyCloseConnection();
		}
		catch (Exception e)
		{
			System.out.print(e.getMessage());
		}

		if ("ACK".equals(ret))
		{
			return true;
		}

		return false;
	}

	/* read employee to arraylist */
	public ArrayList getEmployeeList()
	{
		ArrayList<String> tlist = new ArrayList<String>();
		MySqlProxy mySql = new MySqlProxy();

		try {
			//1. open db
			mySql.spyConnectDB();

			//2. do query
			ResultSet myRS = mySql.spyDoQuery("select emp_id,emp_name,status,job_title,site_no from employee");
			int colNum = 5;

			while (myRS.next()) {
				String rcd = "";
				for (int i = 1; i <= colNum; i++) {

					rcd += myRS.getString(i) + ",";
					//System.out.printf(myRS.getString(i) + ",");
				}

				tlist.add(rcd);
			}

			myRS.close();

			//3. close db
			mySql.spyCloseConnection();

		}
		catch (Exception e)
		{
			System.out.println(e.getMessage());
		}

		return tlist;
	}

	/* read training to arraylist */
	public ArrayList getTrainingList(String qrySql, int qryCols)
	{
		ArrayList<String> tlist = new ArrayList<String>();
		MySqlProxy mySql = new MySqlProxy();

		try {
			//1. open db
			mySql.spyConnectDB();

			//2. do query
			ResultSet myRS = mySql.spyDoQuery(qrySql);
			int colNum = qryCols;

			while (myRS.next()) {
				String rcd = "";
				for (int i = 1; i <= colNum; i++) {

					rcd += myRS.getString(i) + ",";
					//System.out.printf(myRS.getString(i) + ",");
				}

				tlist.add(rcd);
			}

			myRS.close();

			//3. close db
			mySql.spyCloseConnection();

		}
		catch (Exception e)
		{
			System.out.println(e.getMessage());
		}

		return tlist;
	}

	/* date functions */
	public String subYear(String y)
	{
		DateFormat format = new SimpleDateFormat("yyyy-MM-dd");

		//current date
		String now = format.format(new Date());
		String ret = "";

		try {
			int year = Integer.parseInt(y);

			Date d = format.parse(now);
			Calendar c = Calendar.getInstance();
			c.setTime(d);
			c.add(c.YEAR, -year);
			ret = format.format(c.getTime());
		} catch(Exception e) {
			e.printStackTrace();
		}

		return ret;
	}
}
