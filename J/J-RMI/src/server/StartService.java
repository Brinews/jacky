import java.net.MalformedURLException;
import java.rmi.AlreadyBoundException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;

public class StartService {

	public static void startService() {
		String host ="127.0.0.1";//������ip��ַ
		String port ="8999";//�����Ķ˿�

		try {
			IRmiService service = new RmiService();
			// ���������ϵ�Զ�̶���ע���Registry��ʵ������ָ���˿�Ϊ8999
			LocateRegistry.createRegistry(Integer.valueOf(port));

			// ��Զ�̶���ע�ᵽRMIע��������ϣ�������ΪRmiService
			// �󶨵�URL��׼��ʽΪ��rmi://host:port/name(����Э��������ʡ�ԣ���������д��������ȷ�ģ�
			Naming.bind("rmi://" + host + ":" + port + "/RmiService", service);
			System.out.println(">>>>>INFO:Զ��RmiService����󶨳ɹ���");

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

