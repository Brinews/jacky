using System;
using System.Web.UI.WebControls;
using System.Data;
using System.Data.SqlClient;

namespace WestGarden.Web
{
    public partial class Products : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string connString = "Server=127.0.0.1;Database=Grading;uid=sa;pwd=123;";
            string cmdText = "SELECT * FROM FunctionModular where [role] = '"+Request["type"]+"'";

            labelW.Text = "Welcome,<font color=red> " + Session["uid"]+ "</font>";
            dat.Text = "<br>Today:<br>" + DateTime.Now.ToShortDateString();

            SqlConnection conn = new SqlConnection();
            conn.ConnectionString = connString;

            SqlCommand cmd = new SqlCommand();
            cmd.Connection = conn;
            cmd.CommandType = CommandType.Text;
            cmd.CommandText = cmdText;

            conn.Open();

            SqlDataReader dr = cmd.ExecuteReader();

            repCategories.DataSource = dr;
            repCategories.DataBind();

            dr.Close();
            conn.Close();


            string categoryId = Request.QueryString["id"];
            if (!string.IsNullOrEmpty(categoryId))
            {
                SelectCategory(categoryId);
            }
        }

        private void SelectCategory(string categoryId)
        {
            foreach (RepeaterItem item in repCategories.Items)
            {
                HiddenField hidCategoryId = (HiddenField)item.FindControl("hidCategoryId");
                if (hidCategoryId.Value.ToLower() == categoryId.ToLower())
                {
                    HyperLink lnkCategory = (HyperLink)item.FindControl("lnkCategory");
                    lnkCategory.Target = "mainFrame";
                    lnkCategory.NavigateUrl = categoryId + ".aspx";

                    this.mainFrame.Attributes.Add("src", lnkCategory.NavigateUrl);

                    if(string.IsNullOrEmpty(lnkCategory.CssClass))
                        lnkCategory.CssClass = lnkCategory.CssClass + "a";
                    else
                        lnkCategory.CssClass = lnkCategory.CssClass + " a";

                    break;
                }
            }
        }

        protected void logout_Click(object sender, EventArgs e)
        {
            Session["uid"] = "";
            Response.Redirect("login.aspx");
        }
    }
}
