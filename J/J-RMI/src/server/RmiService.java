import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.List;
import java.util.ArrayList;
import java.sql.*;


/**
 * service implementation
 */
public class RmiService extends UnicastRemoteObject implements IRmiService {
	private static final long serialVersionUID = -8306441060593704819L;
	MySqlProxy mySql = new MySqlProxy();

	protected RmiService() throws RemoteException {
		super();

	}

	@Override
	public String hello(String userName) throws RemoteException{
		return "ÄãºÃ~"+userName+"!";
	}

	@Override
	public boolean checkLogin(String userName, String userPwd, String userType) throws RemoteException {
		boolean login = true;
		String pwd = "", type = "";

		try {
			mySql.spyConnectDB();

			ResultSet myRs = mySql.spyDoQuery("select pwd, type from user where uid='" + userName + "'");

			while (myRs.next()) {
				pwd = myRs.getString(1);
				type = myRs.getString(2);
			}

			myRs.close();
			mySql.spyCloseConnection();
		} catch (Exception e) {
			e.printStackTrace();
		}

		if (pwd.equals(userPwd) && type.equals(userType)) 
			return true;

		return false;
	}

	@Override
	public int addADs(String adName, String adPics, int adPrice) throws RemoteException {
		int adId = -1;

		try {
			mySql.spyConnectDB();
			ResultSet myRs = mySql.spyDoQuery("select max(aId) from advers");
			while (myRs.next()) {
				adId = Integer.parseInt(myRs.getString(1));
			}

			adId = adId + 1;

			myRs.close();

			mySql.spyDoUpdate("insert into advers values("+adId+",'"+adName+"','"+adPics+"',"+adPrice+")");

			mySql.spyCloseConnection();

		} catch (Exception e) {
			e.printStackTrace();
		}

		return adId;
	}

	@Override
	public void delADs(int adId) throws RemoteException {
		try {
			mySql.spyConnectDB();

			mySql.spyDoUpdate("delete from advers where aId = "+ adId);

			mySql.spyCloseConnection();

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public void updADs(int adId, String adName, String adPics, int adPrice) throws RemoteException {
		try {
			mySql.spyConnectDB();

			mySql.spyDoUpdate("update advers set aName='"+adName+"', aPic='"+adPics+"', aPrice="+adPrice+" where aId = " + adId);

			mySql.spyCloseConnection();

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public AdsRecord selADs(int adId) throws RemoteException {
		AdsRecord ar = new AdsRecord();

		try {
			mySql.spyConnectDB();

			ResultSet myRs = mySql.spyDoQuery("select aName, aPic, aPrice from advers where aId = " + adId);

			while (myRs.next()) {
				ar.adName = myRs.getString(1);
				ar.adPics = myRs.getString(2);
				ar.adPrice = Integer.parseInt(myRs.getString(3));
			}

			ar.adNo = adId;

			myRs.close();
			mySql.spyCloseConnection();

		} catch (Exception e) {
			e.printStackTrace();
		}

		return ar;
	}

	@Override
	public List<AdsRecord> searchByKey(String adKey) throws RemoteException {
		List<AdsRecord> myArLst = new ArrayList<AdsRecord>();

		try {
			mySql.spyConnectDB();
			ResultSet myRs = mySql.spyDoQuery("select aId, aName, aPic, aPrice from advers where aName like '%"+adKey+"%'");
			while (myRs.next()) {
				AdsRecord ar = new AdsRecord();

				ar.adNo = Integer.parseInt(myRs.getString(1));
				ar.adName = myRs.getString(2);
				ar.adPics = myRs.getString(3);
				ar.adPrice = Integer.parseInt(myRs.getString(4));

				myArLst.add(ar);
			}

			mySql.spyCloseConnection();
		} catch (Exception e) {
			e.printStackTrace();
		}

		return myArLst;
	}

	@Override
	public List<AdsRecord> searchByPrice(int adPrice, String mode) throws RemoteException {
		List<AdsRecord> myArLst = new ArrayList<AdsRecord>();

		try {
			mySql.spyConnectDB();
			String sql = "select aId, aName, aPic, aPrice from advers where aPrice " + mode + " ";
			ResultSet myRs = mySql.spyDoQuery(sql+adPrice);

			while (myRs.next()) {
				AdsRecord ar = new AdsRecord();

				ar.adNo = Integer.parseInt(myRs.getString(1));
				ar.adName = myRs.getString(2);
				ar.adPics = myRs.getString(3);
				ar.adPrice = Integer.parseInt(myRs.getString(4));

				myArLst.add(ar);
			}

			mySql.spyCloseConnection();
		} catch (Exception e) {
			e.printStackTrace();
		}

		return myArLst;
	}
}
