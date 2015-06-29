import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.ResultSet;
import java.util.Vector;
import java.util.Timer;
import java.util.TimerTask;
import javax.swing.table.DefaultTableModel;
import javax.swing.SwingWorker;

import java.util.List;

import java.rmi.Remote;

class CarLstPanel extends JPanel
{
	private static final Timer time = new Timer("RUNNER");

	Vector<Vector<Object>> colData = new Vector<Vector<Object>>();
	Vector<String> colNames = new Vector<String>();
	private JTable rTable;

	IRmiService rmiService = null;
	String keyCond = "";
	String priceCond = "";

	public CarLstPanel(IRmiService rmiSer)
	{
		rmiService = rmiSer;

		setSize(800, 320);

		refreshData(); /* 加载数据 */

		rTable = new JTable(new DefaultTableModel(colData, colNames));
		add(new JScrollPane(rTable));

		//update every second
		time.schedule(new TimerTask() {
				@Override
				public void run() { update(); }
				}, 0, 1000);
	}

	private void update() {
		new SwingWorker<Object, Object>() {
			protected Object doInBackground() throws Exception {
				colData.clear();
				colNames.clear();

				refreshData();

				return null;
			}

			protected void done() {
				((DefaultTableModel) rTable.getModel()).fireTableDataChanged();
			}
		}.execute();
	}

	public void refreshData() {

		String[] names = {"广告编号", "广告名称", "广告图片", "价位" };

		for (int i = 0; i < names.length; i++)
			colNames.add(names[i]);

		try {

			List<AdsRecord> lst = null;
			
			if (keyCond.equals("") && priceCond.equals(""))
				lst = rmiService.searchByPrice(0, ">");
			else if (!keyCond.equals("")) {
				lst = rmiService.searchByKey(keyCond);
			} else if (!priceCond.equals("")) {
				String[] items = priceCond.split(" ");
				String mode = items[0];
				int price = Integer.parseInt(items[1]);

				lst = rmiService.searchByPrice(price, mode);
			}

			for (AdsRecord ar : lst) {
				Vector<Object> oneCol = new Vector<Object>();

				oneCol.add(ar.adNo);
				oneCol.add(ar.adName);
				oneCol.add(ar.adPics);
				oneCol.add(ar.adPrice);

				colData.add(oneCol);
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void setFilter(String key, String price) {
		keyCond = key;
		priceCond = price;
	}
}
