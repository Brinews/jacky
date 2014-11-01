package beans;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

public class EmpUpdate extends HttpServlet {

 @Override
 protected void doPost(HttpServletRequest req, HttpServletResponse resp)
   throws ServletException, IOException {
  doGet(req,resp);
 }

 @Override
 public void doGet(HttpServletRequest req, HttpServletResponse resp)
   throws ServletException, IOException {
  HttpSession session = req.getSession();

  EmpInfo emp = new EmpInfo();

  emp.empId = req.getParameter("empid");
  emp.empName = req.getParameter("empname");
  emp.empStatus = req.getParameter("empstatus");
  emp.empTitle = req.getParameter("emptitle");
  emp.empSite = req.getParameter("empsite");

  boolean ret = Employee.setEmployeeInfo(emp);
  String redirect = "fail.jsp";

  if (ret == true)
  {
	  redirect = "empList.jsp";
  }

  System.out.println(redirect);

  resp.sendRedirect(redirect);

  return;
 }
 
}

