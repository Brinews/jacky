import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;
import javax.swing.*;

public class LinkService extends JFrame {

	IRmiService rmiService = null;

	SellerPanel selPanel = null;
	BuyerPanel buyPanel = null; 
	LoginPanel logPanel = null; 

	public LinkService () {

		try {
			linkHello(""); /* rmi init */
		} catch (RemoteException e) {
			e.printStackTrace();
		}

		selPanel = new SellerPanel(rmiService);
		buyPanel = new BuyerPanel(rmiService);
		logPanel = new LoginPanel(rmiService, this);

		setTitle("汽车广告平台");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		setSize(800, 480);

		add(logPanel);
		logPanel.setVisible(true);

		add(selPanel);
		selPanel.setVisible(false);

		add(buyPanel);
		buyPanel.setVisible(false);

		setVisible(true);
	}

	public void changePanel(int selc) {
		logPanel.setVisible(false);
		selPanel.setVisible(false);
		buyPanel.setVisible(false);

		if (selc == 1) {
			selPanel.setVisible(true);
		} else if (selc == 2) {
			buyPanel.setVisible(true);
		}
	}

	/**
	 * 调用远程Hello方法
	 * @param userName
	 * @throws RemoteException
	 */
	public void linkHello(String userName) throws RemoteException {
		String host = "127.0.0.1";
		String port = "8999";

		try {
			Remote remote = Naming.lookup("rmi://" + host + ":" + port + "/RmiService");
			if (remote instanceof IRmiService) {
				rmiService = (IRmiService) remote;

				if (userName.equals("lion")) {
					String result = rmiService.hello(userName);
					System.out.println(result);

					rmiService.addADs("BcCar", "bc.png", 150000);
					AdsRecord ar = rmiService.selADs(1);
					System.out.println("Name="+ar.adName+",Price="+ar.adPrice);

					rmiService.delADs(1);
					rmiService.addADs("BcCar1", "bc.png", 150001);
					rmiService.addADs("BcCar2", "bc.png", 150002);
					rmiService.addADs("1BcCar2", "bc.png", 150003);

					List<AdsRecord> lst = rmiService.searchByKey("BcCar");

					for (AdsRecord ad : lst) {

						System.out.println("Name="+ad.adName+",Price="+ad.adPrice);
					}
				}
			}
		} catch (NotBoundException e) {
			e.printStackTrace();
		} catch (MalformedURLException e) {
			e.printStackTrace();
		} catch (RemoteException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {

		LinkService ls = new LinkService();

	}
}

