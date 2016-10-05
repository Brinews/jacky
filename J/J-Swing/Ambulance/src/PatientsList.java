import javax.swing.*;
import javax.swing.table.*;

import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Scanner;
import java.util.Vector;

public class PatientsList extends JPanel {
	Vector<Vector<Object>> colData = new Vector<Vector<Object>>();
	Vector<Object> colNames = new Vector<Object>();
	JTable rTable;
	final String filename = "patients.csv";
	JButton backBtn;
	JButton addBtn;
	ControlPanel control;
	PatientUpdate patAdd;
	
	/*
	 * constructor
	 */
	public PatientsList(ControlPanel cp) {
		control = cp;
		setLayout(new BorderLayout());
		JLabel titleLabel = new JLabel("Patients");
		titleLabel.setFont(new Font("Serif", Font.BOLD, 40));
		
		add(titleLabel, BorderLayout.NORTH);
		
		listPatients(filename);
		rTable = new JTable(new DefaultTableModel(colData, colNames));
		add(new JScrollPane(rTable), BorderLayout.CENTER);
		
		JPanel botPane = new JPanel();
		
		backBtn = new JButton("Back");
		addBtn = new JButton("Add New");
		
		ActionListener actListener = new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				JButton src = (JButton) event.getSource();
				if (src == backBtn) {
					control.changeLayout("0");
				} else if (src == addBtn) {
					if (rTable.getSelectedRowCount() > 0) {
						String id = (String) colData.get(rTable.getSelectedRow()).get(0);
						rTable.clearSelection();
						patAdd.selectPat(id);
					} else {
						patAdd.selectPat(getMaxId());
					}
					control.changeLayout("3");
				}
			}
		};
		
		backBtn.addActionListener(actListener);
		addBtn.addActionListener(actListener);
		
		botPane.add(backBtn);
		botPane.add(addBtn);
		
		add(botPane, BorderLayout.SOUTH);
	}
	
	public void setAddPane(PatientUpdate add) {
		patAdd = add;
	}
	
	/*
	 * get next id
	 */
	public String getMaxId() {
		Vector<Object> list = getAllPatsByOrder();
		String lastId = (String) list.get(list.size()-1);
		Integer maxid = Integer.parseInt(lastId) + 1;
		
		return maxid.toString();
	}
	
	/* update data in table display
	 */
	void refreshList() {
		listPatients(filename);
		((DefaultTableModel) rTable.getModel()).fireTableDataChanged();
		this.invalidate();
	}
	
	/* Read data from files
	 * @param filename ambulance data file
	 */
	void listPatients(String filename) {
		colNames.clear();
		colData.clear();
		InputStream stream = AmbulanceList.class.getResourceAsStream(filename);
		Scanner in = new Scanner(stream);
		String header = in.nextLine();
		
		String []items = header.split(",");
		
		for (int i = 0; i < items.length; i++) {
			colNames.add(items[i]);
		}
		
		while (in.hasNext()) {
			String data = in.nextLine();
			String []vals = data.split(",");
			Vector<Object> oneCol = new Vector<Object>();

			for (int i = 0; i < vals.length; i++) {
				oneCol.add(Utils.decodeString(vals[i]));
			}
			colData.add(oneCol);
		}
	}
	
	class MyCompare implements Comparator {
		public int compare(Object o1, Object o2) {
			
			Integer e1 = Integer.parseInt(o1.toString());
			Integer e2 = Integer.parseInt(o2.toString());
			
			return e1-e2;
		}
	}
	
	/** get all patients
	 * 
	 * @return ordered patIds list
	 */
	public Vector<Object> getAllPatsByOrder() {
		Vector<Object> patIds = new Vector<Object>();
		
		for (int i = 0; i < colData.size(); i++) {
			patIds.add(colData.get(i).get(0));
		}
		
		Collections.sort(patIds, new MyCompare());
		
		return patIds;
	}
	
	/** return the ambulance in list
	 * 
	 * @param id ambulance ID
	 * @return ambulance information vector
	 */
	public Vector<Object> getPatById(String id) {
		
		for (int i = 0; i < colData.size(); i++) {
			if (colData.get(i).get(0).equals(id)){
				return colData.get(i);
			}
		}
		
		return null;
	}
	
	public void addPatient(Vector<Object> ambOne) {
		boolean found = false;
		for (int i = 0; i < colData.size(); i++) {
			if (colData.get(i).get(0).equals(ambOne.get(0))) {
				found = true;
				
				colData.get(i).set(1, ambOne.get(1));
				colData.get(i).set(2, ambOne.get(2));
				colData.get(i).set(3, ambOne.get(3));
				colData.get(i).set(4, ambOne.get(4));
			}
		}
		
		if (found == false) {
			colData.add(ambOne);
		}
		
		/* update file */
		updatePatFile(filename);
	}
	
	/**
	 * change ambulance information into a string line
	 * @param vec information vector
	 * @return line 
	 */
	public String patLine(Vector<Object> vec) {
		String line = "";
		
		for (int i = 0; i < vec.size(); i++) {
			if (vec.get(i) != null) {
				line += vec.get(i);
			}
			if (i != vec.size() - 1) {
				line += ",";
			}
		}
		
		line += "\n";
		
		return line;
	}
	
	/**
	 * ambulance file write
	 * @param filename the file name
	 */
	public void updatePatFile(String filename) {
		try {
			File file = new File(AmbulanceList.class.getResource(filename).toURI());
			FileOutputStream out = new FileOutputStream(file);
			
			out.write(patLine(colNames).getBytes());
			for (int i = 0; i < colData.size(); i++) {
				String line = patLine(colData.get(i));
				out.write(line.getBytes());
			}
			
			out.flush();
			out.close();
		} catch (Exception e) {
			
		}
	}
}
