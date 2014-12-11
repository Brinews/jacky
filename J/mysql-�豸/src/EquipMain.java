import java.sql.*;
import java.util.*;
import java.util.Date;
import java.text.*;

public class EquipMain
{
	MySqlProxy mySql = new MySqlProxy();

	public EquipMain()
	{
	}

	public void getEquipUseFrequency(String startDate, String endDate)
	{
		try {
			SimpleDateFormat fmt = new SimpleDateFormat("yyyy-MM-dd");
			Date a = fmt.parse(startDate);
			a = fmt.parse(endDate);
		}
		catch (Exception e) {
			System.out.println("Exception: "+ e.getMessage());
		}

		try {

			/* 统计设备使用频度 */
			String str = "select devicename, count(*) as a from history, deviceinfo where date>='"+startDate+"' and date <='"+endDate+"' and deviceid=deviceinfo.id group by deviceid order by a desc ";
			mySql.spyConnectDB();

			ResultSet myRS = mySql.spyDoQuery(str);

			System.out.println("---Equipment Name-- --UsedTimes--");
			while (myRS.next())
			{
				System.out.printf("%15s%10s\n", 
						myRS.getString(1),
						myRS.getString(2));
			}

			myRS.close();

			mySql.spyCloseConnection();
		}
		catch (Exception e)
		{
			System.out.println("Exception: "+ e.getMessage());
		}
	}

	public void getStudentUseFrequency(String stuNumber)
	{
		try {
			/* 统计学生使用设备情况 */
			String str = "select name,  devicename, count(*) as a from history, userinfo, deviceinfo where jobnumber= '"+stuNumber+"' and userid=userinfo.id and userinfo.isstudent='1' and deviceid=deviceinfo.id group by deviceid order by a desc";
			mySql.spyConnectDB();

			ResultSet myRS = mySql.spyDoQuery(str);
			System.out.println("---Student Name-- --Equipment Name-- --UseTimes--");
			while (myRS.next())
			{
				System.out.printf("%15s%20s%10s\n", 
						myRS.getString(1),
						myRS.getString(2),
						myRS.getString(3));
			}

			myRS.close();

			mySql.spyCloseConnection();
		}
		catch (Exception e)
		{
			System.out.println("Exception: "+ e.getMessage());
		}
	}

	public static void main(String[] args)
	{
		Scanner in = new Scanner(System.in);

		EquipMain em = new EquipMain();

		/* 控制流程 */
		while (true)
		{
			System.out.println("Experiment Equipment Mangement:");
			System.out.println("1. Query Equipment used frequency at given time.");
			System.out.println("2. Query Given Student's use infomation.");
			System.out.println("3. Quit.");
			int option = in.nextInt();

			if (option == 1)
			{
				System.out.println("Input start date (YYYY-MM-DD) : ");
				String sDate = in.next();

				System.out.println("Input end date (YYYY-MM-DD) : ");
				String eDate = in.next();

				em.getEquipUseFrequency(sDate, eDate);
			}
			else if (option == 2)
			{
				System.out.println("Input Student JobNumber:");
				String stuNumber = in.next();

				em.getStudentUseFrequency(stuNumber);
			}

			if (option == 3) break;
		}
	}
}
