namespace client
{
    partial class clientForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(clientForm));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.btn_tsdd1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.btn_Hand = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_Circle = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_Line = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_Pencil = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_Rect = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_tsdd2 = new System.Windows.Forms.ToolStripDropDownButton();
            this.btn_Line1 = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_Line2 = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_Line3 = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_Line4 = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_Line5 = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_Fill = new System.Windows.Forms.ToolStripButton();
            this.btn_Color1 = new System.Windows.Forms.ToolStripButton();
            this.btn_Color2 = new System.Windows.Forms.ToolStripButton();
            this.panel1 = new client.clientForm.DoubleBufferPanel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.panel3 = new System.Windows.Forms.Panel();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.toolStrip1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "line1Button.JPG");
            this.imageList1.Images.SetKeyName(1, "line2Button.JPG");
            this.imageList1.Images.SetKeyName(2, "line3Button.JPG");
            this.imageList1.Images.SetKeyName(3, "line4Button.JPG");
            this.imageList1.Images.SetKeyName(4, "line5Button.JPG");
            this.imageList1.Images.SetKeyName(5, "꽉찬 사각형.jpg");
            this.imageList1.Images.SetKeyName(6, "꽉찬 원.jpg");
            this.imageList1.Images.SetKeyName(7, "사각형.jpg");
            this.imageList1.Images.SetKeyName(8, "손.png");
            this.imageList1.Images.SetKeyName(9, "연필.jpg");
            this.imageList1.Images.SetKeyName(10, "원.jpg");
            this.imageList1.Images.SetKeyName(11, "직선.jpg");
            // 
            // toolStrip1
            // 
            this.toolStrip1.AutoSize = false;
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(35, 35);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_tsdd1,
            this.btn_tsdd2,
            this.btn_Fill,
            this.btn_Color1,
            this.btn_Color2});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(769, 50);
            this.toolStrip1.TabIndex = 2;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // btn_tsdd1
            // 
            this.btn_tsdd1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btn_tsdd1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_Hand,
            this.btn_Circle,
            this.btn_Line,
            this.btn_Pencil,
            this.btn_Rect});
            this.btn_tsdd1.Image = ((System.Drawing.Image)(resources.GetObject("btn_tsdd1.Image")));
            this.btn_tsdd1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btn_tsdd1.Name = "btn_tsdd1";
            this.btn_tsdd1.Size = new System.Drawing.Size(49, 47);
            this.btn_tsdd1.Text = "toolStripDropDownButton1";
            // 
            // btn_Hand
            // 
            this.btn_Hand.Checked = true;
            this.btn_Hand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btn_Hand.Image = ((System.Drawing.Image)(resources.GetObject("btn_Hand.Image")));
            this.btn_Hand.Name = "btn_Hand";
            this.btn_Hand.Size = new System.Drawing.Size(125, 26);
            this.btn_Hand.Text = "Hand";
            this.btn_Hand.Click += new System.EventHandler(this.btn_Shape_Click);
            // 
            // btn_Circle
            // 
            this.btn_Circle.Image = ((System.Drawing.Image)(resources.GetObject("btn_Circle.Image")));
            this.btn_Circle.Name = "btn_Circle";
            this.btn_Circle.Size = new System.Drawing.Size(125, 26);
            this.btn_Circle.Text = "Circle";
            this.btn_Circle.Click += new System.EventHandler(this.btn_Shape_Click);
            // 
            // btn_Line
            // 
            this.btn_Line.Image = ((System.Drawing.Image)(resources.GetObject("btn_Line.Image")));
            this.btn_Line.Name = "btn_Line";
            this.btn_Line.Size = new System.Drawing.Size(125, 26);
            this.btn_Line.Text = "Line";
            this.btn_Line.Click += new System.EventHandler(this.btn_Shape_Click);
            // 
            // btn_Pencil
            // 
            this.btn_Pencil.Image = ((System.Drawing.Image)(resources.GetObject("btn_Pencil.Image")));
            this.btn_Pencil.Name = "btn_Pencil";
            this.btn_Pencil.Size = new System.Drawing.Size(125, 26);
            this.btn_Pencil.Text = "Pencil";
            this.btn_Pencil.Click += new System.EventHandler(this.btn_Shape_Click);
            // 
            // btn_Rect
            // 
            this.btn_Rect.Image = ((System.Drawing.Image)(resources.GetObject("btn_Rect.Image")));
            this.btn_Rect.Name = "btn_Rect";
            this.btn_Rect.Size = new System.Drawing.Size(125, 26);
            this.btn_Rect.Text = "Rect";
            this.btn_Rect.Click += new System.EventHandler(this.btn_Shape_Click);
            // 
            // btn_tsdd2
            // 
            this.btn_tsdd2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btn_tsdd2.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btn_Line1,
            this.btn_Line2,
            this.btn_Line3,
            this.btn_Line4,
            this.btn_Line5});
            this.btn_tsdd2.Image = ((System.Drawing.Image)(resources.GetObject("btn_tsdd2.Image")));
            this.btn_tsdd2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btn_tsdd2.Name = "btn_tsdd2";
            this.btn_tsdd2.Size = new System.Drawing.Size(49, 47);
            this.btn_tsdd2.Text = "toolStripDropDownButton2";
            // 
            // btn_Line1
            // 
            this.btn_Line1.Checked = true;
            this.btn_Line1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btn_Line1.Image = ((System.Drawing.Image)(resources.GetObject("btn_Line1.Image")));
            this.btn_Line1.Name = "btn_Line1";
            this.btn_Line1.Size = new System.Drawing.Size(92, 26);
            this.btn_Line1.Text = "1";
            this.btn_Line1.Click += new System.EventHandler(this.btn_Line1_Click);
            // 
            // btn_Line2
            // 
            this.btn_Line2.Image = ((System.Drawing.Image)(resources.GetObject("btn_Line2.Image")));
            this.btn_Line2.Name = "btn_Line2";
            this.btn_Line2.Size = new System.Drawing.Size(92, 26);
            this.btn_Line2.Text = "2";
            this.btn_Line2.Click += new System.EventHandler(this.btn_Line1_Click);
            // 
            // btn_Line3
            // 
            this.btn_Line3.Image = ((System.Drawing.Image)(resources.GetObject("btn_Line3.Image")));
            this.btn_Line3.Name = "btn_Line3";
            this.btn_Line3.Size = new System.Drawing.Size(92, 26);
            this.btn_Line3.Text = "3";
            this.btn_Line3.Click += new System.EventHandler(this.btn_Line1_Click);
            // 
            // btn_Line4
            // 
            this.btn_Line4.Image = ((System.Drawing.Image)(resources.GetObject("btn_Line4.Image")));
            this.btn_Line4.Name = "btn_Line4";
            this.btn_Line4.Size = new System.Drawing.Size(92, 26);
            this.btn_Line4.Text = "4";
            this.btn_Line4.Click += new System.EventHandler(this.btn_Line1_Click);
            // 
            // btn_Line5
            // 
            this.btn_Line5.Image = ((System.Drawing.Image)(resources.GetObject("btn_Line5.Image")));
            this.btn_Line5.Name = "btn_Line5";
            this.btn_Line5.Size = new System.Drawing.Size(92, 26);
            this.btn_Line5.Text = "5";
            this.btn_Line5.Click += new System.EventHandler(this.btn_Line1_Click);
            // 
            // btn_Fill
            // 
            this.btn_Fill.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btn_Fill.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btn_Fill.Name = "btn_Fill";
            this.btn_Fill.Size = new System.Drawing.Size(58, 47);
            this.btn_Fill.Text = "채우기";
            this.btn_Fill.Click += new System.EventHandler(this.btn_Fill_Click);
            // 
            // btn_Color1
            // 
            this.btn_Color1.AutoSize = false;
            this.btn_Color1.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.btn_Color1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.None;
            this.btn_Color1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btn_Color1.Name = "btn_Color1";
            this.btn_Color1.Size = new System.Drawing.Size(37, 37);
            this.btn_Color1.Text = "toolStripButton2";
            this.btn_Color1.Click += new System.EventHandler(this.btn_Color1_Click);
            // 
            // btn_Color2
            // 
            this.btn_Color2.AutoSize = false;
            this.btn_Color2.BackColor = System.Drawing.Color.White;
            this.btn_Color2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.None;
            this.btn_Color2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btn_Color2.Name = "btn_Color2";
            this.btn_Color2.Size = new System.Drawing.Size(37, 37);
            this.btn_Color2.Text = "toolStripButton3";
            this.btn_Color2.Click += new System.EventHandler(this.btn_Color1_Click);
            // 
            // panel1
            // 
            this.panel1.AutoScroll = true;
            this.panel1.BackColor = System.Drawing.Color.White;
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 50);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(769, 400);
            this.panel1.TabIndex = 3;
            this.panel1.Click += new System.EventHandler(this.panel1_Click);
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            this.panel1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseDown);
            this.panel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseMove);
            this.panel1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseUp);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.textBox2);
            this.panel2.Controls.Add(this.panel3);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel2.Location = new System.Drawing.Point(0, 318);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(769, 132);
            this.panel2.TabIndex = 4;
            // 
            // textBox2
            // 
            this.textBox2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox2.Location = new System.Drawing.Point(0, 0);
            this.textBox2.Multiline = true;
            this.textBox2.Name = "textBox2";
            this.textBox2.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox2.Size = new System.Drawing.Size(769, 105);
            this.textBox2.TabIndex = 3;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.textBox1);
            this.panel3.Controls.Add(this.button1);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel3.Location = new System.Drawing.Point(0, 105);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(769, 27);
            this.panel3.TabIndex = 2;
            // 
            // textBox1
            // 
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Location = new System.Drawing.Point(0, 0);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(694, 25);
            this.textBox1.TabIndex = 2;
            // 
            // button1
            // 
            this.button1.Dock = System.Windows.Forms.DockStyle.Right;
            this.button1.Location = new System.Drawing.Point(694, 0);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 27);
            this.button1.TabIndex = 1;
            this.button1.Text = "Say";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // clientForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(769, 450);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.toolStrip1);
            this.Name = "clientForm";
            this.Text = "세계그림판";
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ToolStripDropDownButton btn_tsdd1;
        private System.Windows.Forms.ToolStripMenuItem btn_Pencil;
        private System.Windows.Forms.ToolStripMenuItem btn_Line;
        private System.Windows.Forms.ToolStripMenuItem btn_Circle;
        private System.Windows.Forms.ToolStripMenuItem btn_Rect;
        private System.Windows.Forms.ToolStripDropDownButton btn_tsdd2;
        private System.Windows.Forms.ToolStripMenuItem btn_Hand;
        private System.Windows.Forms.ToolStripMenuItem btn_Line1;
        private System.Windows.Forms.ToolStripMenuItem btn_Line2;
        private System.Windows.Forms.ToolStripMenuItem btn_Line3;
        private System.Windows.Forms.ToolStripMenuItem btn_Line4;
        private System.Windows.Forms.ToolStripMenuItem btn_Line5;
        private System.Windows.Forms.ToolStripButton btn_Fill;
        private System.Windows.Forms.ToolStripButton btn_Color1;
        private System.Windows.Forms.ToolStripButton btn_Color2;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private DoubleBufferPanel panel1;
    }
}

