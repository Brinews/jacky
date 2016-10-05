import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.*;


public class PatientUpdate extends JPanel {
	
	JLabel titleLabel, idLabel, locLabel, statusLabel, ambLabel;
	JTextField idField, xLocField, yLocField;
	JComboBox statusBox, ambBox;
	JButton cancelBtn, saveBtn;
	
	String patId;
	AmbulanceList ambList;
	PatientsList patList;
	ControlPanel control;
	
	public PatientUpdate(AmbulanceList amb, PatientsList pat, ControlPanel cp) {
		ambList = amb;
		patList = pat;
		
		control = cp;
		
		setLayout(new BorderLayout(80, 100));
		
		titleLabel = new JLabel("Patient:");
		titleLabel.setFont(new Font("Serif", Font.BOLD, 40));
		
		add(titleLabel, BorderLayout.NORTH);
		add(new JLabel(" "), BorderLayout.EAST);
		add(new JLabel(" "), BorderLayout.WEST);
		
		JPanel form = new JPanel();
		form.setLayout(new GridLayout(4,2,3,20));
		
		idLabel = new JLabel("ID:");
		idField = new JTextField("");
		idField.setEditable(false);
		
		form.add(idLabel);
		form.add(idField);
		
		JPanel locPane = new JPanel();
		locPane.setLayout(new GridLayout(1,2));
		
		locLabel = new JLabel("Location:");
		xLocField = new JTextField("");
		yLocField = new JTextField("");
		
		locPane.add(xLocField);
		locPane.add(yLocField);
		
		form.add(locLabel);
		form.add(locPane);
		
		statusLabel = new JLabel("Status:");
		statusBox = new JComboBox(new String[]{"Pending","Assigned",
				"Transporting","Completed"});
		
		form.add(statusLabel);
		form.add(statusBox);
		
		ambLabel = new JLabel("Ambulance");
		ambBox = new JComboBox();
		
		form.add(ambLabel);
		form.add(ambBox);
		
		add(form, BorderLayout.CENTER);
		
		ActionListener clkAction = new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				JButton src = (JButton) event.getSource();
				if (src == cancelBtn) {
					control.changeLayout("2");
				} else if (src == saveBtn) {
					addPatient();
				}
			}
		};
		
		JPanel botPane = new JPanel();
		
		cancelBtn = new JButton("Back");
		cancelBtn.addActionListener(clkAction);
		
		saveBtn = new JButton("Save");
		saveBtn.addActionListener(clkAction);
		
		botPane.add(cancelBtn);
		botPane.add(saveBtn);
		add(botPane, BorderLayout.SOUTH);
	}
	
	/** change ambulance information
	 * 
	 * @param id ambulance id
	 */
	public void selectPat(String id) {
		patId = id;
		
		ambBox.removeAllItems();
		Vector<Object> amb = ambList.getAllAmbsByOrder();
		ambBox.addItem("None");
		for (int i = 0; i < amb.size(); i++) {
			ambBox.addItem(amb.get(i));
		}
		
		Vector<Object> pat = patList.getPatById(id);
		if (pat != null && pat.size() > 0) {
			idField.setText(id);
			xLocField.setText((String)pat.get(1));
			yLocField.setText((String)pat.get(2));
			statusBox.setSelectedItem(pat.get(3));
			ambBox.setSelectedItem((String)pat.get(4));
		} else {
			idField.setText(id);
			xLocField.setText("");
			yLocField.setText("");
			statusBox.setSelectedIndex(0);
			ambBox.setSelectedIndex(0);
		}
		
		this.invalidate();
	}
	
	public void addPatient() {
		patId = idField.getText().trim();
		String x = xLocField.getText().trim();
		String y = yLocField.getText().trim();
		
		if (!"".equals(patId) && !"".equals(x) && !"".equals(y)
				&& Utils.validLocation(x) && Utils.validLocation(y)) {
			Vector<Object> ambOne = new Vector<Object>();
			ambOne.add(patId);
			ambOne.add(x);
			ambOne.add(y);
			ambOne.add(statusBox.getSelectedItem());
			ambOne.add(ambBox.getSelectedItem());
			patList.addPatient(ambOne);
		} else {
			JOptionPane.showMessageDialog(null,"Wrong Format","alert", JOptionPane.ERROR_MESSAGE);
		}
		
	}

}
