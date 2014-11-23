package model;

public class SentenceElement {
	int valueField;
	String engField;
	String chiField;
	
	public SentenceElement(String valueField, String engField, String chiField) {
		this.valueField = Integer.parseInt(valueField);
		this.engField = engField;
		this.chiField = chiField;
	}
	
	public String getEng(){ return engField;}
	public String getChi(){ return chiField;}
	public int getValue(){ return valueField;}
}
