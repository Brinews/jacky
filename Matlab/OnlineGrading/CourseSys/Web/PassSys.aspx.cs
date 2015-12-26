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
        this.TextBox3.Text = "";
    }

    protected void Button1_Click(object sender, EventArgs e)
    {
        string id = (string) Session["uid"];
        string connString = "Server=127.0.0.1;Database=Grading;uid=sa;pwd=123;";

        string pwd1 = this.TextBox1.Text;
        string pwd2 = this.TextBox2.Text;
        string pwd3 = this.TextBox3.Text;

        if (!"".Equals(pwd2) && pwd2 == pwd3)
        {
            DataControl myDataControl = new DataControl(connString);

            bool valid = false;

            myDataControl.OpenConnection();

            int r = myDataControl.CalculateRecord("[User]", "[userid]='"+id+"' and [password]='"+pwd1+"'");
            myDataControl.CloseReader();

            if (r > 0)
            {
                myDataControl.UpdateDataBase("update [User] set [password]='"+pwd2+"' where [userid]='"+id+"'");
            }

            myDataControl.CloseConnection();

        }
    }
}