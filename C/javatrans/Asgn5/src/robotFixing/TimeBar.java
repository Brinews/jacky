package robotFixing;

import java.awt.Color;

import javax.swing.JPanel;

public class TimeBar extends JPanel {
	private static final long serialVersionUID = 1L;
	
	private JPanel HP;
	
	public TimeBar(){
		this.setBounds(125,680,1000,50);
		this.setBackground(Color.black);
		this.setOpaque(true);
		this.setLayout(null);
		this.HP = new JPanel();
		this.HP.setBounds(0, 0, this.getWidth(), this.getHeight());
		this.HP.setBackground(Color.green);
		this.HP.setOpaque(true);
		
		this.add(HP);
	}
	
	public void minusTime(){
		this.HP.setBounds(0, 0, (int)(this.HP.getWidth()-this.getWidth()*0.025), this.HP.getHeight());
	}
	
	public void refill(){
		this.HP.setBounds(0, 0, this.getWidth(), this.getHeight());
	}
}
