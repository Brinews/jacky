import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;
import java.io.Serializable;

class AdsRecord implements Serializable{
	public int adNo;
	public String adName;
	public String adPics;
	public int adPrice;

	public AdsRecord() {}
}

public interface IRmiService extends Remote {
	/**
	 * 公开远程调用方法hello
	 * @param userName
	 * @return
	 */
	public String hello(String userName) throws RemoteException;

	/**
	 * 验证远程登陆用户
	 * @param userName string
	 * @param userPwd string
	 * @param userType string
	 * @return boolean true or false
	 */
	public boolean checkLogin(String userName, String userPwd, String userType) throws RemoteException;

	/**
	 * 添加广告
	 * @param adName string
	 * @param adPics string
	 * @param adPrice int
	 * @return int ad number
	 */
	public int addADs(String adName, String adPics, int adPrice) throws RemoteException;

	/**
	 * 删除指定广告
	 * @param adId	the advertisement key number
	 */
	public void delADs(int adId) throws RemoteException;

	/**
	 * 修改指定广告
	 * @param adId ad's key number
	 * @param adName ad's name
	 * @param adPics ad's image
	 * @param adPrice ad's price
	 */
	public void updADs(int adId, String adName, String adPics, int adPrice) throws RemoteException;

	/**
	 * 查询指定广告
	 * @param adId ad's ID
	 * @return AdsRecord class
	 */
	public AdsRecord selADs(int adId) throws RemoteException;

	/**
	 * 按照关键字搜索广告
	 * @param adKey key of String,
	 * @return List<AdsRecord> Ads having name contains the key
	 */
	public List<AdsRecord> searchByKey(String adKey) throws RemoteException;

	/**
	 * 按照价格搜索广告
	 * @param adPrice price of ads
	 * @param mode < for lower, > for higher, = for equals
	 * @return List<AdsRecord> Ads with price compared to search price
	 */
	public List<AdsRecord> searchByPrice(int adPrice, String mode) throws RemoteException;

}
