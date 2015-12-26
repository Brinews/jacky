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
    string stuId = null;

    protected void Page_Load(object sender, EventArgs e)
    {
        asgnId = Request["aid"];
        stuId = Request["sid"];

        if (!IsPostBack)
            BindGrid();
    }

    void BindGrid()
    {
        if (asgnId == null || stuId == null) return;

        myControl.OpenConnection();

        string objComm = "SELECT * From [Questions] left join [Answers] on [id] = [question_id]   where [asgn_id]='"+asgnId+"' and [stu_id]='"+stuId+"'";

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

        myControl.UpdateDataBase("update [Answers] set [answer]='"+content.Text+"' where [question_id]='"+id+"' and [stu_id] = '"+stuId+"'");
        myControl.CloseConnection();

        DataGrid1.EditItemIndex = -1;
        BindGrid();
    }

    protected void DataGrid1_CancelCommand(object source, DataGridCommandEventArgs e)
    {
        DataGrid1.EditItemIndex = -1;
        BindGrid();
    }

    protected void ItemsGrid_Command(object source, DataGridCommandEventArgs e)
    {
        if (e.CommandName == "CommitAll")
        {
            myControl.OpenConnection();

            myControl.UpdateDataBase("update [Distribution] set [is_submit]='Y' where [asgn_id]='"+asgnId+"' and [stu_id]='"+stuId+"'");

            myControl.CloseConnection();
        }
    }
}