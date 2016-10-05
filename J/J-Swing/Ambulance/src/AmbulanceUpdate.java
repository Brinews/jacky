import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.*;


public class AmbulanceUpdate extends JPanel {
	
	JLabel titleLabel, idLabel, locLabel, statusLabel, ambLabel;
	JTextField idField, xLocField, yLocField;
	JComboBox statusBox, patBox;
	JButton cancelBtn, saveBtn;
	
	AmbulanceList ambList;
	PatientsList patList;
	
	ControlPanel control;
	String ambId;
	
	public AmbulanceUpdate(AmbulanceList amb, PatientsList pat, ControlPanel cp) {
		ambList = amb;
		patList = pat;
		
		control = cp;
		
		setLayout(new BorderLayout(80, 100));
		
		titleLabel = new JLabel("Ambulance:");
		titleLabel.setFont(new Font("Serif", Font.BOLD, 40));
		
		add(titleLabel, BorderLayout.NORTH);
		add(new JLabel(" "), BorderLayout.EAST);
		add(new JLabel(" "), BorderLayout.WEST);
		
		JPanel form = new JPanel();
		form.setLayout(new GridLayout(4,2,3,20));
		
		idLabel = new JLabel("Ambulance ID:");
		idField = new JTextField("");
		
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
		statusBox = new JComboBox(new String[]{"At Station","Responding",
				"At Scene","Transporting","At Destination","Returning"});
		
		form.add(statusLabel);
		form.add(statusBox);
		
		ambLabel = new JLabel("Patient:");
		patBox = new JComboBox();
		
		form.add(ambLabel);
		form.add(patBox);
		
		add(form, BorderLayout.CENTER);
		
		ActionListener clkAction = new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				JButton src = (JButton) event.getSource();
				if (src == cancelBtn) {
					control.changeLayout("1");
				} else if (src == saveBtn) {
					addAmbulance();
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
	public void selectAmb(String id) {
		ambId = id;
		
		patBox.removeAllItems();
		Vector<Object> pat = patList.getAllPatsByOrder();
		patBox.addItem("None");
		for (int i = 0; i < pat.size(); i++) {
			patBox.addItem(pat.get(i));
		}
		
		Vector<Object> amb = ambList.getAmbById(id);
		if (amb != null && amb.size() > 0) {
			idField.setText(id);
			xLocField.setText((String)amb.get(1));
			yLocField.setText((String)amb.get(2));
			statusBox.setSelectedItem(amb.get(3));
			patBox.setSelectedItem((String)amb.get(4));
		} else {
			idField.setText(id);
			xLocField.setText("");
			yLocField.setText("");
			statusBox.setSelectedIndex(0);
			patBox.setSelectedIndex(0);
		}
		
		this.invalidate();
	}
	
	public void addAmbulance() {
		ambId = idField.getText().trim();
		String x = xLocField.getText().trim();
		String y = yLocField.getText().trim();
		
		if (!"".equals(ambId) && !"".equals(x) && !"".equals(y)
				&& Utils.validLocation(x) && Utils.validLocation(y)) {
			Vector<Object> ambOne = new Vector<Object>();
			ambOne.add(ambId);
			ambOne.add(x);
			ambOne.add(y);
			ambOne.add(statusBox.getSelectedItem());
			ambOne.add(patBox.getSelectedItem());
			ambList.addAmbulance(ambOne);
		} else {
			JOptionPane.showMessageDialog(null, "Wrong Format", "alert", JOptionPane.ERROR_MESSAGE);
		}
	}
}
