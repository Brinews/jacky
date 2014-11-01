package beans;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

public class CosAdd extends HttpServlet {

 @Override
 protected void doPost(HttpServletRequest req, HttpServletResponse resp)
   throws ServletException, IOException {
  doGet(req,resp);
 }

 @Override
 public void doGet(HttpServletRequest req, HttpServletResponse resp)
   throws ServletException, IOException {
  HttpSession session = req.getSession();

  TrainingInfo tinf = new TrainingInfo();

  tinf.empId = req.getParameter("empid");
  tinf.cosName = req.getParameter("cosname");
  tinf.finDate = req.getParameter("findate");

  boolean ret = Employee.setTrainingInfo(tinf);
  String redirect = "fail.jsp";

  if (ret == true)
  {
	  redirect = "cosList.jsp?cosname="+tinf.cosName;
  }

  System.out.println(redirect);

  resp.sendRedirect(redirect);

  return;
 }
 
}

