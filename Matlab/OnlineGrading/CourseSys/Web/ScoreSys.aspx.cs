using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using System.Collections;
using System.Data;
using System.Data.SqlClient;

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
        string sid = Request["id"];
        string aid = Request["aid"];

        string objComm = "SELECT * From [Distribution] left join [Assignment] on [asgn_id] = [id]";

        if (null != sid && !sid.Equals(""))
        {
            objComm += " where [stu_id]='"+sid+"'";
        }

        if (null != aid && !aid.Equals(""))
        {
            if (null == sid || sid.Equals("")) objComm += " where ";
            else objComm += " and";
            objComm += " [asgn_id]='"+aid+"'";
        }

        myControl.OpenConnection();
        DataTable myTable = myControl.GetTable(objComm);
        DataGrid1.DataSource = myTable;
        DataGrid1.DataBind();
        myControl.CloseConnection();

        for (int i = 0; i < DataGrid1.Items.Count; i++)
        {
            LinkButton cb = (LinkButton)(DataGrid1.Items[i].Cells[10].Controls[0]);

            if (!myTable.Rows[i][2].Equals("Y"))
            {
                (DataGrid1.Items[i].Cells[9].Controls[0]).Visible = false;
                cb.Enabled = false;
            }
        }
    }

    protected void DataGrid1_EditCommand(object source, DataGridCommandEventArgs e)
    {
        DataGrid1.EditItemIndex = (int)e.Item.ItemIndex;
        BindGrid();
    }

    void updateAverageScore(string asgnid)
    {
        myControl.OpenConnection();

        SqlDataReader dr = myControl.GetReader("select AVG(myscore) as [aver] from [Distribution] where [asgn_id]='"+asgnid+"'");

        int aver = 0;

        while (dr.Read())
        {
            aver = int.Parse(dr["aver"].ToString());
        }

        myControl.CloseReader();

        myControl.UpdateDataBase("update [Assignment] set [average_score]=" + aver + " where [id]='"+asgnid+"'");

        myControl.CloseConnection();
    }

    void updateGpa(string stuid)
    {
        myControl.OpenConnection();

        SqlDataReader dr = myControl.GetReader("select AVG(myscore) as [aver] from [Distribution] where [stu_id]='"+stuid+"'");

        int aver = 0;

        while (dr.Read())
        {
            aver = int.Parse(dr["aver"].ToString());
        }

        myControl.CloseReader();

        char Level = 'B';
        if (aver > 10) Level = 'A';

        myControl.UpdateDataBase("update [Gpa] set [gpa]=" + aver + ", [level]='"+Level+"' where [stu_id]='"+stuid+"'");

        myControl.CloseConnection();
    }

    protected void DataGrid1_UpdateCommand(object source, DataGridCommandEventArgs e)
    {
        myControl.OpenConnection();

        string asgnid = ((Label)e.Item.FindControl("Label1")).Text;
        string stuid = ((Label)e.Item.FindControl("Label13")).Text;

        int score = Int32.Parse(((TextBox)e.Item.FindControl("textbox3")).Text);

        myControl.UpdateDataBase("update [Distribution] set [myscore]=" + score + " where [asgn_id]='" + asgnid + "' and [stu_id] = '"+stuid+"'");

        myControl.CloseConnection();

        updateAverageScore(asgnid);
        updateGpa(stuid);

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
        if (e.CommandName == "Review")
        {
            string asgnid = ((Label)e.Item.FindControl("Label10")).Text;
            string stuid = ((Label)e.Item.FindControl("Label12")).Text;
            
            Response.Redirect("CommentSys.aspx?aid="+ asgnid +"&sid="+stuid);
        }
    }
}