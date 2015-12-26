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
        string tableName1 = "Assignment";
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

        string id = ((Label)e.Item.FindControl("Label11")).Text;
        TextBox name = (TextBox)e.Item.FindControl("textbox5");
        TextBox desc = (TextBox)e.Item.FindControl("textbox1");
        TextBox obj = (TextBox)e.Item.FindControl("textbox3");
        TextBox critical = (TextBox)e.Item.FindControl("textbox7");

        int score = Int32.Parse(((TextBox)e.Item.FindControl("textbox9")).Text);
        int weight = Int32.Parse(((TextBox)e.Item.FindControl("textbox11")).Text);

        myControl.UpdateDataBase("update [Assignment] set [name]='"+name.Text+"', [score]="+score+", [weight]="+weight+", [desc]='"+desc.Text+"',[objective]='"+obj.Text+"' where [id]='"+id+"'");
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
        string sql = "delete from [Assignment] where [id]='"+id+"'";

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
            TextBox t5 = (TextBox)e.Item.FindControl("textbox10");
            TextBox t6 = (TextBox)e.Item.FindControl("textbox12");

            try
            {
                myControl.UpdateDataBase("insert into [Assignment] (name,[desc], objective,critical, score, weight ) values('" + t4.Text + "','" + t1.Text + "','" + t2.Text + "','" + t3.Text + "', " + t5.Text + ", " + t6.Text + ")");
            } catch(Exception ei) {}

            myControl.CloseConnection();

            BindGrid();
        }

        if (e.CommandName == "AddQuestion")
        {
            string id = ((Label)e.Item.Cells[1].Controls[1]).Text;
            Response.Redirect("QuestionSys.aspx?id="+ id);
        }

        if (e.CommandName == "Assign")
        {
            string id = ((Label)e.Item.Cells[1].Controls[1]).Text;
            Response.Redirect("DisSys.aspx?id="+ id);
        }
    }
}