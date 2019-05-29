namespace server
{
    partial class serverOpen
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new System.Windows.Forms.Button();
            this.txt_IP = new System.Windows.Forms.TextBox();
            this.txt_port = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 46);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(341, 35);
            this.button1.TabIndex = 0;
            this.button1.Text = "서버열기";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // txt_IP
            // 
            this.txt_IP.Location = new System.Drawing.Point(12, 12);
            this.txt_IP.Name = "txt_IP";
            this.txt_IP.Size = new System.Drawing.Size(235, 28);
            this.txt_IP.TabIndex = 1;
            // 
            // txt_port
            // 
            this.txt_port.Location = new System.Drawing.Point(253, 12);
            this.txt_port.Name = "txt_port";
            this.txt_port.Size = new System.Drawing.Size(100, 28);
            this.txt_port.TabIndex = 2;
            // 
            // serverOpen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(365, 94);
            this.Controls.Add(this.txt_port);
            this.Controls.Add(this.txt_IP);
            this.Controls.Add(this.button1);
            this.Name = "serverOpen";
            this.Text = "세계그림판서버";
            this.Load += new System.EventHandler(this.serverOpen_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox txt_IP;
        private System.Windows.Forms.TextBox txt_port;
    }
}