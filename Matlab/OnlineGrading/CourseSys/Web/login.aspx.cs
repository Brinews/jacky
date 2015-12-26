using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using System.Data;
using System.Data.SqlClient;

public partial class login : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
    }

    protected void Button2_Click(object sender, EventArgs e)
    {
        this.TextBox1.Text = "";
        this.TextBox2.Text = "";
    }

    protected void Button1_Click(object sender, EventArgs e)
    {
        string connString = "Server=127.0.0.1;Database=Grading;uid=sa;pwd=123;";

        string id = this.TextBox1.Text;
        string pwd = this.TextBox2.Text;
        string type = this.DropDownList1.SelectedValue;

        if (!"".Equals(id) && !"".Equals(pwd))
        {
            DataControl myDataControl = new DataControl(connString);

            bool valid = false;

            myDataControl.OpenConnection();

            int r = myDataControl.CalculateRecord("[User]", "[userid]='"+id+"' and [password]='"+pwd+"'");

            if (r > 0) valid = true;

            myDataControl.CloseReader();
            myDataControl.CloseConnection();

            if (valid == true)
            {
                Session["uid"] = id;
                Response.Redirect("MainFrame.aspx?type="+type);
            }
        }
    }
}