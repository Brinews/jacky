import java.net.MalformedURLException;
import java.rmi.AlreadyBoundException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;

public class StartService {

	public static void startService() {
		String host ="127.0.0.1";//公开的ip地址
		String port ="8999";//公开的端口

		try {
			IRmiService service = new RmiService();
			// 本地主机上的远程对象注册表Registry的实例，并指定端口为8999
			LocateRegistry.createRegistry(Integer.valueOf(port));

			// 把远程对象注册到RMI注册服务器上，并命名为RmiService
			// 绑定的URL标准格式为：rmi://host:port/name(其中协议名可以省略，下面两种写法都是正确的）
			Naming.bind("rmi://" + host + ":" + port + "/RmiService", service);
			System.out.println(">>>>>INFO:远程RmiService对象绑定成功！");

		} catch (NumberFormatException e) {
			e.printStackTrace();
		} catch (RemoteException e) {
			e.printStackTrace();
		} catch (MalformedURLException e) {
			e.printStackTrace();
		} catch (AlreadyBoundException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		startService();
	}
}

