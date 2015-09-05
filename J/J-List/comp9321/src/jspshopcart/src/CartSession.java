package beans;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import beans.DblpBean;

public class CartSession extends HttpServlet
{
    //add to cart
    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException
    {
		DblpBean myDblp = DblpBean.getInstance();

        //1.get paper
        String paperKey = request.getParameter("paper");
		String paper =  myDblp.getCachedPaper(paperKey);

        //2.get cart from session
        HttpSession session=request.getSession();
        Map<String, String> cart= (Map<String, String>)
									  session.getAttribute("cart");

        if(cart==null)
        {
			//cart is created when not found
            cart=new HashMap<String, String>();
        }

        //3.paper already buy or not
        if(cart.containsKey(paperKey))
        {
            cart.put(paperKey, paper);
        } else {
            cart.put(paperKey, paper);
        }

        session.setAttribute("cart", cart);
        response.sendRedirect("shopcart.jsp");//back
    }
    
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException
    {
        doGet(request, response);
    }
}
