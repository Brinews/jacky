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

    bool alreadyAssign(string stuid, string asgid)
    {
        myControl.OpenConnection();
        int r = myControl.CalculateRecord("[Distribution]", "[asgn_id]='"+asgid+"' and [stu_id]='"+stuid+"'");
        myControl.CloseReader();
        myControl.CloseConnection();

        return r > 0;
    }

    void BindGrid()
    {
        if (asgnId == null) return;

        string tableName1 = "User";
        myControl.OpenConnection();
        string objComm = "SELECT * From [" + tableName1 + "] where [role]='student'";
        DataTable myTable = myControl.GetTable(objComm);
        DataGrid1.DataSource = myTable;
        DataGrid1.DataBind();
        myControl.CloseConnection();

        for (int i = 0; i < DataGrid1.Items.Count; i++)
        {
            CheckBox cb = (CheckBox)DataGrid1.Items[i].FindControl("ck_asgn");
            if (alreadyAssign(((Label)DataGrid1.Items[i].FindControl("Label3")).Text, asgnId))
            {
                cb.Checked = true;
            }
        }
    }

    protected void ItemsGrid_Command(object source, DataGridCommandEventArgs e)
    {
        if (e.CommandName == "DisAssign" && asgnId != null)
        {
            myControl.OpenConnection();

            string id = asgnId;

            myControl.UpdateDataBase("delete from [Distribution] where [asgn_id]='"+id+"'");
            myControl.UpdateDataBase("delete from [Answers] where [question_id] in (select [id] from [Questions] where [asgn_id]='"+id+"')");

            for (int i = 0; i < DataGrid1.Items.Count; i++)
            {
                CheckBox cb = (CheckBox)DataGrid1.Items[i].FindControl("ck_asgn");
                Label userid = (Label)DataGrid1.Items[i].FindControl("Label3");
                
                if (cb.Checked == true)
                {
                    myControl.UpdateDataBase("insert into [Distribution](asgn_id, stu_id, is_submit, myscore, review) values('"+id+"', '"+userid.Text+"', 'N', 0, '')");
                    myControl.UpdateDataBase("insert into [Answers] select [id], '"+userid.Text+"', '', '' from [Questions] where [asgn_id] = '"+id+"'");
                }
            }

            myControl.CloseConnection();

            BindGrid();
        }

        if (e.CommandName == "SelectAll")
        {
            for (int i = 0; i < DataGrid1.Items.Count; i++)
            {
                CheckBox cb = (CheckBox)DataGrid1.Items[i].FindControl("ck_asgn");
                cb.Checked = true;
            }
        }
    }
}