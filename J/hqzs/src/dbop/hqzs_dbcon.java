import java.io.*;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.ResultSet;
//import com.mysql.jdbc.Driver; /* No Need */

class hqzs_dbcon{ 

	/**
	 * Connection String For Mysql DB
	 */
	private String confFileName = "./conf/db.conf";
	private String dbName = null;
	private String userName = null;
	private String password = null;


	private Connection mConnection = null;
	private Statement mStatement = null;

	private int status = 0;

	public hqzs_dbcon()
	{
		ReadConfFile();
		MakeConnection();
	}

	/**
	 * read conf file
	 */
	public void ReadConfFile()
	{
		File fd = null;
		Reader in = null;

		try {
			fd = new File(confFileName);
			in = new FileReader(fd);
		} catch (FileNotFoundException e) {
			//throw;
			System.out.println(e.getMessage());
		}

		String line = "";
		int c;

		try {

			while ((c = in.read()) > 0) {

				line += (char) c;

				if (c == '\n') {

					if (line.indexOf("DBName") != -1) {
						String[] strs = line.split("=");
						/*
						   for (int i = 0; i < strs.length; i++) 
						   System.out.println("Str:" + i + strs[i]+ ".");
						 */

						if (strs.length > 1) {
							dbName = strs[1].trim();
						}
					}

					if (line.indexOf("UserName") != -1) {
						String[] strs = line.split("=");
						if (strs.length > 1) {
							userName = strs[1].trim();
						}
					}

					if (line.indexOf("Password") != -1) {
						String[] strs = line.split("=");
						if (strs.length > 1) {
							password = strs[1].trim();
						} else
							password = "";
					}

					/* reset */
					line = "";
				}
			}
			in.close();
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}


		if (dbName == null || userName == null || password == null) {
			System.out.println("[" + dbName + "][" + userName + "][" + password + "]\nDB connection string set failed.");
			System.exit(0);
		}
	}

	public void MakeConnection()
	{
		String dbDriver = "com.mysql.jdbc.Driver";
		String url = "jdbc:mysql://localhost/" + dbName;

		try {

			try {
				Class.forName(dbDriver).newInstance();
			} catch (Exception ex) {
				System.out.println(ex.getMessage());
			}

			mConnection = DriverManager.getConnection(url, userName, password);
			mStatement = mConnection.createStatement();
		} catch (SQLException e) {
			System.out.println("Link DB error.\n" + url + "\n" + e.getMessage());

			status = -1;
			return;
		}

		status = 0;
	}

	public int GetStatus()
	{
		return status;
	}

	public void CloseConnection() throws SQLException
	{
		if (mConnection != null)
			mConnection.close();
	}

	/**
	 * Read Database 
	 */
	public ResultSet ReadDataSet(String sql)
	{
		ResultSet mResultSet = null;

		if (mStatement != null) {
			try {
				mResultSet = mStatement.executeQuery(sql);
			} catch (Exception e) {
				System.out.println(e.getMessage());
			}
		}

		return mResultSet;
	}


	/*
	public void MySqlConnect()
	{

		Statement mStatement = null;
		ResultSet mResultSet = null;
		Connection mConnection = null;

		String sql = "select * from employee";

		try {

			try {
				Class.forName(dbDriver).newInstance();
			}catch (Exception ex)
			{
			}

			mConnection = DriverManager.getConnection(url, username, password);
			mStatement = mConnection.createStatement();
			mResultSet = mStatement.executeQuery(sql);

			try {
				while (mResultSet.next()) {
					System.out.print(mResultSet.getString(1) + "\n");
				}
			}
			catch (Exception e) {
				System.out.println("Reading DB error.\n" + e.getMessage());
			}
		} catch(SQLException e) {
			System.out.println("Link DB error.\n" + url + "\n" + e.getMessage());
		} finally {
			//mStatement.close();
			//mConnection.close();
		}
	}
	*/
}
