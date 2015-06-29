
import java.sql.*;    
import java.util.*;
import java.lang.*;

public class MySqlProxy {
	Connection myCon;

	public MySqlProxy()
	{
	}

	public void spyConnectDB() throws Exception
	{
		try {
			Class.forName("com.mysql.jdbc.Driver");
			myCon = DriverManager.getConnection("jdbc:mysql://localhost/myrmi", "root", "");

			//Class.forName("oracle.jdbc.driver.OracleDriver");
			//myCon = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:XE", "root", "123456");
			//myCon = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:orcl", "c##scott", "tiger");
		}catch(Exception e) 
		{
			System.out.println("Exception: ConnectDB()-" + e.getMessage());

			e.printStackTrace();
		}
	}

	public Connection spyGetConnection() throws Exception
	{
		return myCon;
	}

	public void spyCloseConnection() throws Exception
	{
		try {
			myCon.close();
		}
		catch (Exception e) {
			System.out.println("Close Exception :" + e.getMessage());
		}
	}

	/***
	 * Attendtion: myResult.close() is need when used complete
	 */
	public ResultSet spyDoQuery(String sql) throws Exception
	{
		ResultSet myResult = null;

		try {
			Statement myStat = myCon.createStatement();

			myResult = myStat.executeQuery(sql);
		}
		catch (Exception e)
		{
			System.out.println("Query Exception :" + e.getMessage());
			System.out.println(sql);
		}

		return myResult;
	}

	/***
	 * Update Including: insert, delete, update
	 */
	public String spyDoUpdate(String sql) throws Exception
	{
		try {
			Statement myStat = myCon.createStatement();

			String[] esql = sql.split(";");

			for (int i = 0; i < esql.length; i++)
			{
				myStat.executeUpdate(esql[i]);
			}
		}
		catch (Exception e) {
			System.out.println("Update Exception: "+ e.getMessage());
			System.out.println(sql);

			return "failed reason: " + e.getMessage();
		}

		return "ACK";
	}

	/***
	 * Test Case
	 */
	public static void main(String[] args)
	{
		MySqlProxy mySql = new MySqlProxy();

		int a = 100;

		String p = "test" + a;

		System.out.println(p);
		
		try {
			//1. open db
			mySql.spyConnectDB();

			//2. do sql update
			mySql.spyDoUpdate("insert into mtest values(5, 'easy')");

			//3. do query
			ResultSet myRS = mySql.spyDoQuery("select emp_id from training");
			int colNum = 1;

			while (myRS.next()) {
				for (int i = 1; i <= colNum; i++) {
					System.out.printf(myRS.getString(i) + ",");
				}

				System.out.println("");
			}

			myRS.close();

			//4. close db
			mySql.spyCloseConnection();
		}
		catch (Exception e)
		{
			System.out.println("Exception: "+ e.getMessage());
		}
	}
}

