package beans;

import java.util.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import org.w3c.dom.*;
import org.xml.sax.SAXException;

import javax.xml.parsers.*;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.*;
import javax.xml.xpath.*;

/**
 * @author code
 * used for xml file operation
 */
public class DblpBean {

	private static DblpBean instance = null;
	private static String xmlFileName = "../webapps/jspshopcart/dblp.xml";
	private Document xmlDoc = null;
	private NodeList dblpList = null;
	private static HashMap<String, String> paperCache = null;

	public DblpBean() {
		/* load xml file */
		DocumentBuilderFactory factory
			= DocumentBuilderFactory.newInstance();

		paperCache = new HashMap<String, String> ();

        Element root=null;

        try {
            factory.setIgnoringElementContentWhitespace(true);
            
            DocumentBuilder db=factory.newDocumentBuilder();

			/* load source xml file */
            xmlDoc=db.parse(new File(xmlFileName));

            root=xmlDoc.getDocumentElement();
            
            dblpList = selectNodes("/dblp/*", root);
			
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        } catch (SAXException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
	}

	/*
	 * get node by condition
	 */
	public static NodeList selectNodes(String express, Object source) {
        NodeList result=null;

        XPathFactory xpathFactory=XPathFactory.newInstance();
        XPath xpath=xpathFactory.newXPath();

        try {
            result=(NodeList) xpath.evaluate(express, 
					source, XPathConstants.NODESET);
        } catch (XPathExpressionException e) {
            e.printStackTrace();
        }
        
        return result;
    }

	public static synchronized DblpBean getInstance() {
		if (instance == null) {
			instance = new DblpBean();
		}

		return instance;
	}

	public String getDblpNodeAttr(Node node) {
		StringBuilder sb = new StringBuilder("");

		/* node attribute */
		NamedNodeMap attributes = node.getAttributes();
		for (int j = 0; j < attributes.getLength(); j++) {
			Node attribute = attributes.item(j);
			/*
			System.out.println(attribute.getNodeName() + ":"
					+ attribute.getNodeValue());
			*/
			sb.append("<tr><td>" + attribute.getNodeName()
					+"</td><td>" + 
					attribute.getNodeValue() + "</td></tr>");
		}

		
		return sb.toString();
	}

	public String getDblpNodeKey(Node node) {
		StringBuilder sb = new StringBuilder("");

		/* node attribute */
		NamedNodeMap attributes = node.getAttributes();

		for (int j = 0; j < attributes.getLength(); j++) {
			Node attribute = attributes.item(j);

			if (attribute.getNodeName().equals("key"))
					return attribute.getNodeValue();
		}
		
		return "";
	}


	public String getDblpNodeChild(Node node) {
		StringBuilder sb = new StringBuilder("");

		/* node's childs */
		NodeList childNodes=node.getChildNodes();
		for (int j = 0; j < childNodes.getLength(); j++) {
			Node childNode=childNodes.item(j);
			/*
			System.out.println(childNode.getNodeName()+":"
					+childNode.getNodeValue());
			*/

			sb.append("<tr><td>" + childNode.getNodeName()+"</td><td>" +
					childNode.getFirstChild().getNodeValue() 
					+ "</td></tr>");
		}

		return sb.toString();
	}

	public String getDblpNodeTitle(Node node) {
		StringBuilder sb = new StringBuilder("");

		/* node's childs */
		NodeList childNodes=node.getChildNodes();
		for (int j = 0; j < childNodes.getLength(); j++) {
			Node childNode=childNodes.item(j);

			if (childNode.getNodeName().equals("title")) {
				sb.append("<tr><td>" 
						+ childNode.getNodeName()+"</td><td>" 
						+ childNode.getFirstChild().getNodeValue() 
						+ "</td></tr>");
				break;
			}
		}

		return sb.toString();
	}


	/* get 10 items randomly */
	public String getDblpRandomly(int total) {
		StringBuilder sb = new StringBuilder("");

		int i = 0;
		Random r = new Random();
		int count = total;

		while (count > 0) {

			while (r.nextInt() % (total+1) == count) {
				i++;
			}

			count --;

			sb.append(getDblpNodeAttr(dblpList.item(i)));
			sb.append(getDblpNodeChild(dblpList.item(i)));
			sb.append("<tr><td colspan=2></td></tr>");
		}

		return sb.toString();
	}

	/*
	 * does the node has the given attr and val
	 */
	public boolean queryCondition(Node node, String attr, String val) {

		//System.out.println(attr+":"+val);

		/* node attribute */
		NamedNodeMap attributes = node.getAttributes();
		for (int j = 0; j < attributes.getLength(); j++) {
			Node attribute = attributes.item(j);

			if (attribute.getNodeName().equals(attr)
					&& attribute.getNodeValue().equals(val))
				return true;
		}

		/* node's childs */
		NodeList childNodes=node.getChildNodes();
		for (int j = 0; j < childNodes.getLength(); j++) {
			Node childNode=childNodes.item(j);
			
			if (childNode.getNodeName().equals(attr)
					&& childNode.getFirstChild().getNodeValue().equals(val))
				return true;
		}
			
		return false;
	}

	/*
	 * return paper in cache
	 * @param key string
	 * @return String paper-info
	 */
	public String getCachedPaper(String key) {
		if (paperCache.containsKey(key))
			return paperCache.get(key);

		return "Sorry, no paper found...";
	}

	/*
	 * return query list 
	 * @param keyMap query keys
	 * @param pageno page number
	 * @param isEnd reach the end of list
	 */
	public String getSearchDblp(Map<String, String> keyMap,
			int pageno, int[] isEnd) {

		StringBuilder content = new StringBuilder("");

		int TOTAL = 11, count = 0, i = 0, index = 0;

		isEnd[0] = 1; //assume reach end

		while (i < dblpList.getLength() && count < TOTAL) {

			boolean found = true;

			for (Map.Entry<String, String> entry : keyMap.entrySet()) {
				if (queryCondition(dblpList.item(i), 
						entry.getKey(), entry.getValue()) == false) {
					found = false;
					break;
				}
			}

			if (found) {
				index ++;
				if (index > pageno * (TOTAL - 1)) {
					if (count >= TOTAL - 1) {
						isEnd[0] = 0; // not reach end
					}
					else {
						/* add node to content */
						content.append(
								getDblpNodeTitle(dblpList.item(i)));

						content.append(
								getDblpNodeAttr(dblpList.item(i))); 

						String paperKey = 
								getDblpNodeKey(dblpList.item(i));

						String paperContent = 
								getDblpNodeAttr(dblpList.item(i))+
								getDblpNodeChild(dblpList.item(i));

						if (!paperCache.containsKey(paperKey)) {
							paperCache.put(paperKey, paperContent);
						}

						content.append("<tr><td colspan=2>"+
						"<a href=addToCart?paper="+paperKey
						+">Add To Cart</a> &nbsp&nbsp"
						+ "<a href=paper.jsp?key="
						+ paperKey + ">Details</a>"+"</td></tr>");
					}
					count ++;
				}
			}

			i++;
		}

		return content.toString();
	}
}
