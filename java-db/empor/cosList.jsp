<%@ page import ="beans.Employee" %>
<%@ page import ="java.util.ArrayList" %>

<head>
<link href="./css/table.css" rel="stylesheet" type="text/css"></link>
</head>

<body>
  <form>
  	<table class="query">
		<tr>
		<td>Employees need retraining in subject: </td>
		<td><input type="text" name="cosname"/></td>
		<td>,course training cycle year(N):</td>
		<td><input type="text" name="coscycle"/></td>
		<td><input type="submit" value="Query"/></td>
		</tr>
	</table>
  </form>

  <table>
   <tr>
    <td>EmpID</td><td>EmpName</td><td>CourseName</td><td>Course Completed Date</td>
   </tr>

	<%
	Employee emp = new Employee();

	String cosName = request.getParameter("cosname");
	String cosCycle = request.getParameter("coscycle");

	String qrySql = "select training.emp_id, emp_name, course, qual_date from training, employee where training.emp_id=employee.emp_id order by course asc, qual_date desc";
	if ((null == cosCycle || "".equals(cosCycle))
			&& (null != cosName && !"".equals(cosName))) {
		qrySql = "select training.emp_id,emp_name,course,qual_date from training, employee where course='"+cosName+"' and training.emp_id=employee.emp_id order by qual_date desc";
	}

	if ((null != cosCycle && !"".equals(cosCycle))
			&& (null != cosName && !"".equals(cosName))) {
		String expdate = emp.subYear(cosCycle);
		qrySql = "select emp_id, emp_name, '', '' from employee where emp_id not in (select emp_id from training where qual_date>'"+expdate+"' and course='"+cosName+"')";
	}

	System.out.println(qrySql);

	ArrayList<String> empList = null;
	empList = emp.getTrainingList(qrySql, 4);

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
		out.write("</tr>");
	}

	%>
 </table>
</body>
