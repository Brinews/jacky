using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Collections;
using System.Threading;

namespace FinalDegree
{
    public partial class pfd_main : Form
    {
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.TabPage tabPage5;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBox7;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.TextBox textBox18;
        private System.Windows.Forms.Label label39;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label label52;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Label label53;
        private System.Windows.Forms.TextBox textBox24;
        private System.Windows.Forms.Label label54;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.Button button8;

        public pfd_main()
        {
            // 1. Dynamically build views
            pfd_build_tabs();

            // 2. Load XML file
            loadXmlCourses();

            // 3. Dynamically build module
            dynAddModule();
        }

        /**
         * build 5 tabs as requirements
         */
        void pfd_build_tabs()
        {
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button3 = new System.Windows.Forms.Button();
            this.label12 = new System.Windows.Forms.Label();
            this.textBox7 = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label37 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.button4 = new System.Windows.Forms.Button();
            this.label38 = new System.Windows.Forms.Label();
            this.textBox18 = new System.Windows.Forms.TextBox();
            this.label39 = new System.Windows.Forms.Label();
            this.button5 = new System.Windows.Forms.Button();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.label52 = new System.Windows.Forms.Label();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.button6 = new System.Windows.Forms.Button();
            this.label53 = new System.Windows.Forms.Label();
            this.textBox24 = new System.Windows.Forms.TextBox();
            this.label54 = new System.Windows.Forms.Label();
            this.button7 = new System.Windows.Forms.Button();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.button8 = new System.Windows.Forms.Button();
            this.label9 = new System.Windows.Forms.Label();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tabControl.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.tabPage5.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabPage1);
            this.tabControl.Controls.Add(this.tabPage2);
            this.tabControl.Controls.Add(this.tabPage3);
            this.tabControl.Controls.Add(this.tabPage4);
            this.tabControl.Controls.Add(this.tabPage5);
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(718, 529);
            this.tabControl.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.button1);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(710, 503);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Start";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(593, 399);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(84, 26);
            this.button1.TabIndex = 1;
            this.button1.Text = "Go";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("微软雅黑", 26.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(241, 389);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(346, 46);
            this.label1.TabIndex = 0;
            this.label1.Text = "Build Your Course.";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.groupBox2);
            this.tabPage2.Controls.Add(this.groupBox1);
            this.tabPage2.Controls.Add(this.label10);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(710, 503);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Level4";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Location = new System.Drawing.Point(5, 83);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(702, 414);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button3);
            this.groupBox1.Controls.Add(this.label12);
            this.groupBox1.Controls.Add(this.textBox7);
            this.groupBox1.Controls.Add(this.label11);
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Location = new System.Drawing.Point(5, 27);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(700, 50);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            // 
            // button3
            // 
            this.button3.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button3.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.button3.Location = new System.Drawing.Point(592, 14);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(102, 30);
            this.button3.TabIndex = 3;
            this.button3.Text = "Calculate";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label12.Location = new System.Drawing.Point(359, 20);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(25, 22);
            this.label12.TabIndex = 2;
            this.label12.Text = "%";
            // 
            // textBox7
            // 
            this.textBox7.Location = new System.Drawing.Point(315, 21);
            this.textBox7.Name = "textBox7";
            this.textBox7.Size = new System.Drawing.Size(38, 21);
            this.textBox7.TabIndex = 0;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label11.Location = new System.Drawing.Point(242, 20);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(65, 22);
            this.label11.TabIndex = 1;
            this.label11.Text = "Year 1:";
            // 
            // button2
            // 
            this.button2.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button2.ForeColor = System.Drawing.Color.OrangeRed;
            this.button2.Location = new System.Drawing.Point(6, 14);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(36, 30);
            this.button2.TabIndex = 0;
            this.button2.Text = "+";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label10.Location = new System.Drawing.Point(201, 3);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(245, 19);
            this.label10.TabIndex = 0;
            this.label10.Text = "BEng Mobile and Web Computing";
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.groupBox3);
            this.tabPage3.Controls.Add(this.label37);
            this.tabPage3.Controls.Add(this.groupBox4);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(710, 503);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Level5";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Location = new System.Drawing.Point(5, 83);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(702, 417);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            // 
            // label37
            // 
            this.label37.AutoSize = true;
            this.label37.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label37.Location = new System.Drawing.Point(201, 3);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(245, 19);
            this.label37.TabIndex = 3;
            this.label37.Text = "BEng Mobile and Web Computing";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.button4);
            this.groupBox4.Controls.Add(this.label38);
            this.groupBox4.Controls.Add(this.textBox18);
            this.groupBox4.Controls.Add(this.label39);
            this.groupBox4.Controls.Add(this.button5);
            this.groupBox4.Location = new System.Drawing.Point(5, 27);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(700, 50);
            this.groupBox4.TabIndex = 4;
            this.groupBox4.TabStop = false;
            // 
            // button4
            // 
            this.button4.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button4.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.button4.Location = new System.Drawing.Point(592, 14);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(102, 30);
            this.button4.TabIndex = 3;
            this.button4.Text = "Calculate";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // label38
            // 
            this.label38.AutoSize = true;
            this.label38.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label38.Location = new System.Drawing.Point(359, 20);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(25, 22);
            this.label38.TabIndex = 2;
            this.label38.Text = "%";
            // 
            // textBox18
            // 
            this.textBox18.Location = new System.Drawing.Point(315, 21);
            this.textBox18.Name = "textBox18";
            this.textBox18.Size = new System.Drawing.Size(38, 21);
            this.textBox18.TabIndex = 0;
            // 
            // label39
            // 
            this.label39.AutoSize = true;
            this.label39.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label39.Location = new System.Drawing.Point(242, 20);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(65, 22);
            this.label39.TabIndex = 1;
            this.label39.Text = "Year 2:";
            // 
            // button5
            // 
            this.button5.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button5.ForeColor = System.Drawing.Color.OrangeRed;
            this.button5.Location = new System.Drawing.Point(6, 14);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(36, 30);
            this.button5.TabIndex = 0;
            this.button5.Text = "+";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.groupBox5);
            this.tabPage4.Controls.Add(this.label52);
            this.tabPage4.Controls.Add(this.groupBox6);
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(710, 503);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "Level6";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // groupBox5
            // 
            this.groupBox5.Location = new System.Drawing.Point(5, 83);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(702, 415);
            this.groupBox5.TabIndex = 5;
            this.groupBox5.TabStop = false;
            // 
            // label52
            // 
            this.label52.AutoSize = true;
            this.label52.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label52.Location = new System.Drawing.Point(201, 3);
            this.label52.Name = "label52";
            this.label52.Size = new System.Drawing.Size(245, 19);
            this.label52.TabIndex = 3;
            this.label52.Text = "BEng Mobile and Web Computing";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.button6);
            this.groupBox6.Controls.Add(this.label53);
            this.groupBox6.Controls.Add(this.textBox24);
            this.groupBox6.Controls.Add(this.label54);
            this.groupBox6.Controls.Add(this.button7);
            this.groupBox6.Location = new System.Drawing.Point(5, 27);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(700, 50);
            this.groupBox6.TabIndex = 4;
            this.groupBox6.TabStop = false;
            // 
            // button6
            // 
            this.button6.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button6.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.button6.Location = new System.Drawing.Point(592, 14);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(102, 30);
            this.button6.TabIndex = 3;
            this.button6.Text = "Calculate";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // label53
            // 
            this.label53.AutoSize = true;
            this.label53.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label53.Location = new System.Drawing.Point(359, 20);
            this.label53.Name = "label53";
            this.label53.Size = new System.Drawing.Size(25, 22);
            this.label53.TabIndex = 2;
            this.label53.Text = "%";
            // 
            // textBox24
            // 
            this.textBox24.Location = new System.Drawing.Point(315, 21);
            this.textBox24.Name = "textBox24";
            this.textBox24.Size = new System.Drawing.Size(38, 21);
            this.textBox24.TabIndex = 0;
            // 
            // label54
            // 
            this.label54.AutoSize = true;
            this.label54.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label54.Location = new System.Drawing.Point(242, 20);
            this.label54.Name = "label54";
            this.label54.Size = new System.Drawing.Size(65, 22);
            this.label54.TabIndex = 1;
            this.label54.Text = "Year 3:";
            // 
            // button7
            // 
            this.button7.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button7.ForeColor = System.Drawing.Color.OrangeRed;
            this.button7.Location = new System.Drawing.Point(6, 14);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(36, 30);
            this.button7.TabIndex = 0;
            this.button7.Text = "+";
            this.button7.UseVisualStyleBackColor = true;
            this.button7.Click += new System.EventHandler(this.button7_Click);
            // 
            // tabPage5
            // 
            this.tabPage5.Controls.Add(this.button8);
            this.tabPage5.Controls.Add(this.label9);
            this.tabPage5.Controls.Add(this.textBox6);
            this.tabPage5.Controls.Add(this.textBox5);
            this.tabPage5.Controls.Add(this.textBox4);
            this.tabPage5.Controls.Add(this.label8);
            this.tabPage5.Controls.Add(this.label7);
            this.tabPage5.Controls.Add(this.label6);
            this.tabPage5.Controls.Add(this.textBox3);
            this.tabPage5.Controls.Add(this.textBox2);
            this.tabPage5.Controls.Add(this.label5);
            this.tabPage5.Controls.Add(this.label4);
            this.tabPage5.Controls.Add(this.textBox1);
            this.tabPage5.Controls.Add(this.label3);
            this.tabPage5.Controls.Add(this.label2);
            this.tabPage5.Location = new System.Drawing.Point(4, 22);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage5.Size = new System.Drawing.Size(710, 503);
            this.tabPage5.TabIndex = 4;
            this.tabPage5.Text = "Summary";
            this.tabPage5.UseVisualStyleBackColor = true;
            // 
            // button8
            // 
            this.button8.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button8.Location = new System.Drawing.Point(525, 433);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(149, 45);
            this.button8.TabIndex = 14;
            this.button8.Text = "Save To XML";
            this.button8.UseVisualStyleBackColor = true;
            this.button8.Click += new System.EventHandler(this.button8_Click);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Bell MT", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(572, 93);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(31, 24);
            this.label9.TabIndex = 13;
            this.label9.Text = "%:";
            // 
            // textBox6
            // 
            this.textBox6.Location = new System.Drawing.Point(544, 227);
            this.textBox6.Name = "textBox6";
            this.textBox6.Size = new System.Drawing.Size(100, 21);
            this.textBox6.TabIndex = 12;
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(544, 177);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(100, 21);
            this.textBox5.TabIndex = 11;
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(544, 130);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(100, 21);
            this.textBox4.TabIndex = 10;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Bell MT", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(416, 222);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(122, 24);
            this.label8.TabIndex = 9;
            this.label8.Text = "Level-6 Avg :";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Bell MT", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(416, 172);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(122, 24);
            this.label7.TabIndex = 8;
            this.label7.Text = "Level-5 Avg :";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Bell MT", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(416, 125);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(122, 24);
            this.label6.TabIndex = 7;
            this.label6.Text = "Level-4 Avg :";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(240, 207);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(100, 21);
            this.textBox3.TabIndex = 6;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(240, 155);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(100, 21);
            this.textBox2.TabIndex = 5;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Bell MT", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(71, 202);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(166, 24);
            this.label5.TabIndex = 4;
            this.label5.Text = "Best At Level 4--5:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Bell MT", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(71, 150);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(167, 24);
            this.label4.TabIndex = 3;
            this.label4.Text = "Best At Level 6.....:";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(494, 370);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(150, 21);
            this.textBox1.TabIndex = 2;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("微软雅黑", 18F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(170, 362);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(318, 31);
            this.label3.TabIndex = 1;
            this.label3.Text = "You are expected to get :";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("微软雅黑", 18F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(8, 63);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(231, 31);
            this.label2.TabIndex = 0;
            this.label2.Text = "Summary Degree:";
            // 
            // pfd_main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(719, 532);
            this.Controls.Add(this.tabControl);
            this.Name = "pfd_main";
            this.Text = "Final Degree Predict";
            this.tabControl.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.tabPage4.ResumeLayout(false);
            this.tabPage4.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.tabPage5.ResumeLayout(false);
            this.tabPage5.PerformLayout();
            this.ResumeLayout(false);
        }

        /**
         * go to level-4 tab
         */
        private void button1_Click(object sender, EventArgs e)
        {
            this.tabControl.SelectedIndex = 1;
        }

        /**
         * course name label(x = 6, y = 67, w = 191, h=12)
         */
        Label createCourseLabel(string name, int x, int y, int w, int h)
        {
            Label lbl_course = new Label();
            lbl_course.AutoSize = true;
            lbl_course.Location = new System.Drawing.Point(x, y);
            lbl_course.Name = name;
            lbl_course.Size = new System.Drawing.Size(w, h);
            lbl_course.TabIndex = 0;
            lbl_course.Text = name;

            return lbl_course;
        }

        TextBox createCourseTextBox(string text, int x, int y, int w, int h)
        {    
            TextBox tb_course = new TextBox();
            tb_course.Location = new System.Drawing.Point(x, y);
            tb_course.Size = new System.Drawing.Size(w, h);
            //tb_course.Text = text;
            tb_course.Name = text;
            tb_course.Text = "0";

            return tb_course;
        }

        

        int addModule(GroupBox gb, pfd_course pc, int lv4_posY)
        {
            TextBox tb;

            if (!pc.courseName.Equals(""))
            {
                Label lbl = createCourseLabel(pc.courseName, 6, lv4_posY + lv_dist, 191, 12);
                gb.Controls.Add(lbl);

                lbl = createCourseLabel("Credits: " + pc.courseCredit, 6, lv4_posY + lv_dist + 17, 191, 12);
                gb.Controls.Add(lbl);

                if (!pc.planName1.Equals(""))
                {
                    lbl = createCourseLabel(pc.planName1, 216, lv4_posY + lv_dist, 72, 12);
                    gb.Controls.Add(lbl);
                    lbl = createCourseLabel("Weight:" + pc.planWeight1+"%", 216, lv4_posY + lv_dist + 17, 72, 12);
                    gb.Controls.Add(lbl);
                    tb = createCourseTextBox(pc.courseName +"1", 216, lv4_posY + lv_dist + 17 + 17, 39, 21);
                    gb.Controls.Add(tb);
                }
                if (!pc.planName2.Equals(""))
                {
                    lbl = createCourseLabel(pc.planName2, 313, lv4_posY + lv_dist, 72, 12);
                    gb.Controls.Add(lbl);
                    lbl = createCourseLabel("Weight:" + pc.planWeight2+"%", 313, lv4_posY + lv_dist + 17, 72, 12);
                    gb.Controls.Add(lbl);
                    tb = createCourseTextBox(pc.courseName+"2", 313, lv4_posY + lv_dist + 17 + 17, 39, 21);
                    gb.Controls.Add(tb);
                }
                if (!pc.planName3.Equals(""))
                {
                    lbl = createCourseLabel(pc.planName3, 409, lv4_posY + lv_dist, 72, 12);
                    gb.Controls.Add(lbl);
                    lbl = createCourseLabel("Weight:" + pc.planWeight3+"%", 409, lv4_posY + lv_dist + 17, 72, 12);
                    gb.Controls.Add(lbl);
                    tb = createCourseTextBox(pc.courseName+"3", 409, lv4_posY + lv_dist + 17 + 17, 39, 21);
                    gb.Controls.Add(tb);
                }
                if (!pc.planName4.Equals(""))
                {
                    lbl = createCourseLabel(pc.planName4, 503, lv4_posY + lv_dist, 72, 12);
                    gb.Controls.Add(lbl);
                    lbl = createCourseLabel("Weight:" + pc.planWeight4 +"%", 503, lv4_posY + lv_dist + 17, 72, 12);
                    gb.Controls.Add(lbl);
                    tb = createCourseTextBox(pc.courseName+"4", 503, lv4_posY + lv_dist + 17 + 17, 39, 21);
                    gb.Controls.Add(tb);
                }

                lbl = createCourseLabel("Total", 616, lv4_posY + lv_dist, 72, 12);
                gb.Controls.Add(lbl);
                lbl = createCourseLabel("Weight:100%", 616, lv4_posY + lv_dist + 17, 72, 12);
                gb.Controls.Add(lbl);
                tb = createCourseTextBox(pc.courseName+"T", 616, lv4_posY + lv_dist + 17 + 17, 39, 21);
                gb.Controls.Add(tb);

                lv4_posY += lv_dist;
            }

            return lv4_posY;
        }

        /**
         * add module 
         * each class a line, which will store into XML file
         */
        private void button2_Click(object sender, EventArgs e)
        {
            pfd_module pm = new pfd_module();
            pm.ShowDialog();

            pfd_course pc = pm.newCourse;

            if (pc.courseName.Equals("")) return;

            lv4_PosY = addModule(this.groupBox2, pc, lv4_PosY);

            if (lv4Courses.Count < COURSEMAX)
                lv4Courses.Add(pc);
        }

        /**
         * level-5 add module
         */
        private void button5_Click(object sender, EventArgs e)
        {
            pfd_module pm = new pfd_module();
            pm.ShowDialog();
            pfd_course pc = pm.newCourse;

            if (pc.courseName.Equals("")) return;

            lv5_PosY = addModule(this.groupBox3, pc, lv5_PosY);

            if (lv5Courses.Count < COURSEMAX)
                lv5Courses.Add(pc);
        }

        /**
         * level-6 add module
         */
        private void button7_Click(object sender, EventArgs e)
        {
            pfd_module pm = new pfd_module();
            pm.ShowDialog();
            pfd_course pc = pm.newCourse;

            if (pc.courseName.Equals("")) return;

            lv6_PosY = addModule(this.groupBox5, pc, lv6_PosY);

            if (lv6Courses.Count < COURSEMAX)
                lv6Courses.Add(pc);
        }

        double updateWeights(ArrayList courses, int level)
        {
            double total = 0.0, allTotal = 0.0, maxTotal = -1.0;
            TextBox tb;
            int w;

            for (int i = 0; i < courses.Count; i++)
            {
                total = 0.0;

                pfd_course pc = (pfd_course)courses[i];

                Control[] c = this.Controls.Find(pc.courseName + "1", true);
                if (c != null && c.Count() > 0)
                {
                    tb = (TextBox)c[0];

                    w = Int32.Parse(tb.Text);
                    total += (w * pc.planWeight1 / 100.0);
                }

                c = this.Controls.Find(pc.courseName + "2", true);
                if (c != null && c.Count() > 0)
                {
                    tb = (TextBox)c[0];
                    w = Int32.Parse(tb.Text);
                    total += (w * pc.planWeight2 / 100.0);
                }

                c = this.Controls.Find(pc.courseName + "3", true);
                if (c != null && c.Count() > 0)
                {
                    tb = (TextBox)c[0];
                    w = Int32.Parse(tb.Text);
                    total += (w * pc.planWeight3 / 100.0);
                }

                c = this.Controls.Find(pc.courseName + "4", true);
                if (c != null && c.Count() > 0)
                {
                    tb = (TextBox)c[0];
                    w = Int32.Parse(tb.Text);
                    total += (w * pc.planWeight4 / 100.0);
                }

                pc.courseTotal = total;
                c = this.Controls.Find(pc.courseName + "T", true);
                if (c != null && c.Count() > 0)
                {
                    tb = (TextBox)c[0];
                    tb.Text = total.ToString();
                }

                allTotal += total;
                if (maxTotal < total) maxTotal = total;
            }

            if (level == 4)
            {
                maxLv4 = maxTotal;
                avgLv4 = (allTotal / courses.Count);
            }

            if (level == 5)
            {
                maxLv5 = maxTotal;
                avgLv5 = (allTotal / courses.Count);
            }

            if (level == 6)
            {
                maxLv6 = maxTotal;
                avgLv6 = (allTotal / courses.Count);
            }

            return allTotal;
        }

        /**
         * caculate the marks of lv4
         */
        private void button3_Click(object sender, EventArgs e)
        {
            updateWeights(lv4Courses, 4);
            this.textBox7.Text = avgLv4.ToString();
            this.textBox4.Text = avgLv4.ToString();
            this.textBox3.Text = (maxLv4 > maxLv5 ? maxLv4 : maxLv5).ToString();
        }

        /**
         * caculate the marks of lv5
         */
        private void button4_Click(object sender, EventArgs e)
        {
            updateWeights(lv5Courses, 5);
            this.textBox18.Text = avgLv5.ToString();
            this.textBox5.Text = avgLv5.ToString();
            this.textBox3.Text = (maxLv4 > maxLv5 ? maxLv4 : maxLv5).ToString();
        }

        /**
         * caculate the marks of lv6
         */
        private void button6_Click(object sender, EventArgs e)
        {
            updateWeights(lv6Courses, 6);
            this.textBox24.Text = avgLv6.ToString();
            this.textBox2.Text = maxLv6.ToString();
            this.textBox6.Text = avgLv6.ToString();

            this.textBox1.Text = pfd_judge.getNextLevel(
                maxLv6,
                maxLv4>maxLv5?maxLv4:maxLv5);
        }
        
        /**
         * Load Xml to Arralist
         */
        public void loadXmlCourses()
        {
            lv4Courses = xmlOpt.getCourseList("level4");
            lv5Courses = xmlOpt.getCourseList("level5");
            lv6Courses = xmlOpt.getCourseList("level6");
        }

        /**
         * add course to groupbox
         */
        public void dynAddModule()
        {
            for (int i = 0; i < lv4Courses.Count; i++)
                lv4_PosY = addModule(this.groupBox2, (pfd_course)lv4Courses[i], lv4_PosY);
            for (int i = 0; i < lv5Courses.Count; i++)
                lv5_PosY = addModule(this.groupBox3, (pfd_course)lv5Courses[i], lv5_PosY);
            for (int i = 0; i < lv6Courses.Count; i++)
                lv6_PosY = addModule(this.groupBox5, (pfd_course)lv6Courses[i], lv6_PosY);
        }

        /**
         * save course information into XML file
         */
        private void button8_Click(object sender, EventArgs e)
        {
            ThreadStart myStart = new ThreadStart(myMethod);
            Thread myThread = new Thread(myStart);
            myThread.Start();
        }

        void myMethod()
        {
            xmlOpt.saveXmlFile(lv4Courses, lv5Courses, lv6Courses, "course.xml");
        }

        /**
         * control the postion
         */
        int lv_dist = 54;
        int lv4_PosY = -37; // start pos
        int lv5_PosY = -37;
        int lv6_PosY = -37;

        /**
         * course list
         */
        private const int COURSEMAX = 7;
        private ArrayList lv4Courses;
        private ArrayList lv5Courses;
        private ArrayList lv6Courses;

        /**
         * caculate the score
         */
        private double maxLv4, maxLv5, maxLv6;
        private double avgLv4, avgLv5, avgLv6;

        private XmlOpter xmlOpt = new XmlOpter("course.xml");
    }
}
