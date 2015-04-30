using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace FinalDegree
{
    public partial class pfd_module : Form
    {
        public pfd_course newCourse = new pfd_course();

        public pfd_module()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            newCourse.pfd_clear();

            newCourse.courseName = this.textBox1.Text;
            newCourse.planName1 = this.textBox2.Text;
            newCourse.planName2 = this.textBox5.Text;
            newCourse.planName3 = this.textBox7.Text;
            newCourse.planName4 = this.textBox9.Text;
            try
            {
                newCourse.courseCredit = int.Parse(this.textBox10.Text);
                newCourse.planWeight1 = int.Parse(this.textBox3.Text);
                newCourse.planWeight2 = int.Parse(this.textBox4.Text);
                newCourse.planWeight3 = int.Parse(this.textBox6.Text);
                newCourse.planWeight4 = int.Parse(this.textBox8.Text);
            }
            catch
            {
            }

            int total = 0;

            if (!newCourse.planName1.Equals("")) total += newCourse.planWeight1;
            if (!newCourse.planName2.Equals("")) total += newCourse.planWeight2;
            if (!newCourse.planName3.Equals("")) total += newCourse.planWeight3;
            if (!newCourse.planName4.Equals("")) total += newCourse.planWeight4;

            if (total != 100)
            {
                newCourse.pfd_clear();
                MessageBox.Show("The Course Total Weight Not Equal 100.");

                return;
            }

            if (newCourse.courseCredit < 0)
            {
                newCourse.pfd_clear();
                MessageBox.Show("The Course Credit must > 0.");

                return;
            }

            this.Hide();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            newCourse.pfd_clear();
            this.Hide();
        }
    }
}
