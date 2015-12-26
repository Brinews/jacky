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
    string asgnId = null;

    protected void Page_Load(object sender, EventArgs e)
    {
        asgnId = Request["id"];
        if (!IsPostBack)
            BindGrid();
    }

    void BindGrid()
    {
        string tableName1 = "Questions";
        myControl.OpenConnection();
        string objComm = "SELECT * From [" + tableName1 + "]";
        if (asgnId != null)
        {
            objComm += " where [asgn_id] = '"+asgnId+"'";
        }
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

        TextBox content = (TextBox)e.Item.FindControl("textbox1");

        myControl.UpdateDataBase("update [Questions] set [content]='"+content.Text+"' where [id]='"+id+"'");
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
        string sql = "delete from [Questions] where [id]='"+id+"'";

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

            //Label asgn = (Label)e.Item.FindControl("Label3");
            string id = asgnId;
            TextBox content = (TextBox)e.Item.FindControl("textbox2");

            if (myControl.CalculateRecord("[Questions]", "asgn_id='" + id + "'") < 5)
            {
                myControl.CloseReader();
                myControl.UpdateDataBase("insert into [Questions] (asgn_id, content) values('" + id + "','" + content.Text + "')");
            }
            else
                myControl.CloseReader();

            myControl.CloseConnection();

            BindGrid();
        }

        if (e.CommandName == "ViewScore")
        {
            string id = ((Label)e.Item.Cells[2].Controls[1]).Text;
            Response.Redirect("ScoreSys.aspx?aid="+ id);
        }
    }
}