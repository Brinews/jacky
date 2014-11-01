<%@ page import ="beans.Employee" %>
<%@ page import ="java.util.ArrayList" %>

<head>
<link href="./css/table.css" rel="stylesheet" type="text/css"></link>
</head>

<body>
  <table>
   <tr>
    <td>EmpID</td><td>EmpName</td><td>EmpStatus</td><td>EmpTitle</td><td>EmpSite</td><td>-</td>
   </tr>

	<%
	Employee emp = new Employee();

	ArrayList<String> empList = null;
	empList = emp.getEmployeeList();

	for (int i = 0; i < empList.size(); i++) {
		String inf = (String)empList.get(i);
		String[] items = inf.split(",");

		out.write("<tr>");
		for (int j = 0; j < items.length; j++)
		{
			out.write("<td>");
			out.write(items[j]);
			out.write("</td>");
		}
		out.write("<td><a href=empUpdate.jsp?empid="+items[0]+">Update</a></td>");
		out.write("</tr>");
	}

	%>
 </table>
</body>
