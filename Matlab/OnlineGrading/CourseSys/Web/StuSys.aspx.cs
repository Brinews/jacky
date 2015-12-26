using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using System.Collections;
using System.Data;

public partial class StuSys : System.Web.UI.Page
{
    static string constr = "Server=127.0.0.1;Database=Grading;uid=sa;pwd=123;";
    DataControl myControl = new DataControl(constr);

    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack)
            BindGrid();
    }

    void BindGrid()
    {
        string tableName1 = "User";
        myControl.OpenConnection();
        string objComm = "SELECT * From [" + tableName1 + "]";
        DataTable myTable = myControl.GetTable(objComm);
        DataGrid1.DataSource = myTable;
        DataGrid1.DataBind();
        myControl.CloseConnection();
    }

    protected void DataGrid1_EditCommand(object source, DataGridCommandEventArgs e)
    {
        DataGrid1.EditItemIndex = (int)e.Item.ItemIndex;
        BindGrid();
    }
    protected void DataGrid1_UpdateCommand(object source, DataGridCommandEventArgs e)
    {
        myControl.OpenConnection();

        string id = ((Label)e.Item.FindControl("Label5")).Text;

        TextBox name = (TextBox)e.Item.FindControl("textbox1");
        TextBox pwd = (TextBox)e.Item.FindControl("textbox3");
        TextBox email = (TextBox)e.Item.FindControl("textbox7");

        myControl.UpdateDataBase("update [User] set [username]='"+name.Text+"' , [password]='"+pwd.Text+"',[useremail]='"+email.Text+"' where [userid]='"+id+"'");
        myControl.CloseConnection();

        DataGrid1.EditItemIndex = -1;
        BindGrid();
    }

    protected void DataGrid1_CancelCommand(object source, DataGridCommandEventArgs e)
    {
        DataGrid1.EditItemIndex = -1;
        BindGrid();
    }

    protected void DataGrid1_DeleteCommand(object source, DataGridCommandEventArgs e)
    {
        string id = ((Label)e.Item.Cells[1].Controls[1]).Text;
        string sql = "delete from [User] where [userid]='"+id+"'";

        myControl.OpenConnection();
        myControl.UpdateDataBase(sql);
        myControl.CloseConnection();

        BindGrid();
    }
    protected void ItemsGrid_Command(object source, DataGridCommandEventArgs e)
    {
        if (e.CommandName == "Insert")
        {
            myControl.OpenConnection();

            TextBox t4 = (TextBox)e.Item.FindControl("textbox6");
            TextBox t1 = (TextBox)e.Item.FindControl("textbox2");
            TextBox t2 = (TextBox)e.Item.FindControl("textbox4");
            TextBox t3 = (TextBox)e.Item.FindControl("textbox8");

            myControl.UpdateDataBase("insert into [User] (userid,username,password,useremail,role) values('" + t4.Text + "','" + t1.Text + "','" + t2.Text + "','" + t3.Text + "', 'student')");

            myControl.UpdateDataBase("insert into [Gpa] (stu_id, gpa, level) values ('"+t4.Text+"',0,'')");

            myControl.CloseConnection();

            BindGrid();
        }

        if (e.CommandName == "ViewScore")
        {
            string id = ((Label)e.Item.Cells[1].Controls[1]).Text;
            Response.Redirect("ScoreSys.aspx?id="+ id);
        }
    }
}