using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;
using System.Xml.Linq;
using System.IO;

using System.Data;

using System.Collections;

namespace FinalDegree
{
    public class XmlOpter
    {
        private XmlDocument xd;
        private string xmlFileName;

        public XmlOpter(string fileName)
        {
            xd = new XmlDocument();
            xmlFileName = fileName;
            readXmlFile();
        }

        public void readXmlFile()
        {
            XmlTextReader xmlFile = new XmlTextReader(xmlFileName);
            xmlFile.Read();

            xd.Load(xmlFile);
        }

        public ArrayList getCourseList(string levelName)
        {
            ArrayList clist = new ArrayList();

            XmlNode level = xd.SelectSingleNode("level");
            XmlNodeList lst = level.ChildNodes;
            XmlNode testsNode = null;

            int idx = 0;

            foreach (XmlNode xn2 in lst)
            {
                XmlElement xe = (XmlElement)xn2;

                if (xe.Name.Equals(levelName))
                {
                    XmlNodeList lst2 = xn2.SelectNodes("course");

                    foreach (XmlNode xn20 in lst2)
                    {
                        pfd_course pc = new pfd_course();

                        // course list
                        foreach (XmlNode xn21 in xn20.ChildNodes)
                        {

                            XmlElement xe21 = (XmlElement)xn21;

                            if (xe21.Name.Equals("coursename"))
                                pc.courseName = xe21.InnerText;

                            if (xe21.Name.Equals("coursecredit"))
                                pc.courseCredit = Int32.Parse(xe21.InnerText);

                            if (xe21.Name.Equals("coursetotal"))
                                pc.courseTotal = Double.Parse(xe21.InnerText);

                            if (xe21.Name.Equals("tests")) testsNode = xn21;
                        }

                        if (null != testsNode)
                        {
                            XmlNodeList testsList = testsNode.ChildNodes;

                            int i = 0;

                            foreach (XmlNode xn3 in testsList)
                            {
                                XmlNodeList xn3lst = xn3.ChildNodes;

                                foreach (XmlNode xn4 in xn3lst)
                                {
                                    if (i == 0)
                                    {
                                        if (xn4.Name.Equals("planName"))
                                            pc.planName1 = xn4.InnerText;

                                        if (xn4.Name.Equals("planWeight"))
                                            pc.planWeight1 = Int32.Parse(xn4.InnerText);
                                    }

                                    if (i == 1)
                                    {
                                        if (xn4.Name.Equals("planName"))
                                            pc.planName2 = xn4.InnerText;

                                        if (xn4.Name.Equals("planWeight"))
                                            pc.planWeight2 = Int32.Parse(xn4.InnerText);
                                    }

                                    if (i == 2)
                                    {
                                        if (xn4.Name.Equals("planName"))
                                            pc.planName3 = xn4.InnerText;

                                        if (xn4.Name.Equals("planWeight"))
                                            pc.planWeight3 = Int32.Parse(xn4.InnerText);
                                    }

                                    if (i == 3)
                                    {
                                        if (xn4.Name.Equals("planName"))
                                            pc.planName4 = xn4.InnerText;

                                        if (xn4.Name.Equals("planWeight"))
                                            pc.planWeight4 = Int32.Parse(xn4.InnerText);
                                    }

                                }

                                i++;
                            }
                        }
                        clist.Insert(idx++, pc);
                    }// end for
                }// end if
            }

            return clist;
        }

        /**
         * write xml to file
         * @param lv4-lv6
         */
        public void saveXmlFile(ArrayList lv4, ArrayList lv5, ArrayList lv6, string fileName)
        {
            XmlDocument xmlDoc = new XmlDocument();

            xmlDoc.CreateXmlDeclaration("1.0", "utf-8", "yes");

            XmlNode rootNode = xmlDoc.CreateElement("level");

            XmlNode lv4Node = setCourseList(xmlDoc, lv4, "level4");
            XmlNode lv5Node = setCourseList(xmlDoc, lv5, "level5");
            XmlNode lv6Node = setCourseList(xmlDoc, lv6, "level6");

            rootNode.AppendChild(lv4Node);
            rootNode.AppendChild(lv5Node);
            rootNode.AppendChild(lv6Node);

            xmlDoc.AppendChild(rootNode);

            xmlDoc.Save(fileName);
        }

        XmlNode setCourseList(XmlDocument xmlDoc, ArrayList lv, string name)
        {
            XmlNode root = xmlDoc.CreateElement(name);

            for (int i = 0; i < lv.Count; i++)
            {
                XmlNode course = xmlDoc.CreateElement("course");

                pfd_course pc = (pfd_course) lv[i];

                XmlNode courseName = xmlDoc.CreateElement("coursename");
                courseName.InnerText = pc.courseName;
                course.AppendChild(courseName);

                XmlNode courseCredit = xmlDoc.CreateElement("coursecredit");
                courseCredit.InnerText = pc.courseCredit.ToString();
                course.AppendChild(courseCredit);

                XmlNode tests = xmlDoc.CreateElement("tests");
                if (!pc.planName1.Equals(""))
                {
                    XmlNode test = xmlDoc.CreateElement("test");
                    XmlNode pname = xmlDoc.CreateElement("planName");
                    pname.InnerText = pc.planName1;
                    XmlNode pweight = xmlDoc.CreateElement("planWeight");
                    pweight.InnerText = pc.planWeight1.ToString();

                    test.AppendChild(pname);
                    test.AppendChild(pweight);

                    tests.AppendChild(test);
                }
                if (!pc.planName2.Equals(""))
                {
                    XmlNode test = xmlDoc.CreateElement("test");
                    XmlNode pname = xmlDoc.CreateElement("planName");
                    pname.InnerText = pc.planName2;
                    XmlNode pweight = xmlDoc.CreateElement("planWeight");
                    pweight.InnerText = pc.planWeight2.ToString();

                    test.AppendChild(pname);
                    test.AppendChild(pweight);

                    tests.AppendChild(test);
                }
                if (!pc.planName3.Equals(""))
                {
                    XmlNode test = xmlDoc.CreateElement("test");
                    XmlNode pname = xmlDoc.CreateElement("planName");
                    pname.InnerText = pc.planName3;
                    XmlNode pweight = xmlDoc.CreateElement("planWeight");
                    pweight.InnerText = pc.planWeight3.ToString();

                    test.AppendChild(pname);
                    test.AppendChild(pweight);

                    tests.AppendChild(test);
                }
                if (!pc.planName4.Equals(""))
                {
                    XmlNode test = xmlDoc.CreateElement("test");
                    XmlNode pname = xmlDoc.CreateElement("planName");
                    pname.InnerText = pc.planName4;
                    XmlNode pweight = xmlDoc.CreateElement("planWeight");
                    pweight.InnerText = pc.planWeight4.ToString();

                    test.AppendChild(pname);
                    test.AppendChild(pweight);

                    tests.AppendChild(test);
                }

                course.AppendChild(tests);

                XmlNode courseTotal = xmlDoc.CreateElement("coursetotal");
                courseTotal.InnerText = pc.courseTotal.ToString();
                course.AppendChild(courseTotal);

                root.AppendChild(course);
            }

            return root;
        }
    }
}
