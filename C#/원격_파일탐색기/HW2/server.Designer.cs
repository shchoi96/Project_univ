namespace HW2
{
    partial class server
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
            this.ipLabel = new System.Windows.Forms.Label();
            this.portLabel = new System.Windows.Forms.Label();
            this.pathLabel = new System.Windows.Forms.Label();
            this.txtIP = new System.Windows.Forms.TextBox();
            this.txtPORT = new System.Windows.Forms.TextBox();
            this.txtPATH = new System.Windows.Forms.TextBox();
            this.btnServer = new System.Windows.Forms.Button();
            this.btnRoute = new System.Windows.Forms.Button();
            this.txtLog = new System.Windows.Forms.TextBox();
            this.labelLog = new System.Windows.Forms.Label();
            this.folderBrowserDlg = new System.Windows.Forms.FolderBrowserDialog();
            this.SuspendLayout();
            // 
            // ipLabel
            // 
            this.ipLabel.AutoSize = true;
            this.ipLabel.Location = new System.Drawing.Point(23, 28);
            this.ipLabel.Name = "ipLabel";
            this.ipLabel.Size = new System.Drawing.Size(20, 15);
            this.ipLabel.TabIndex = 0;
            this.ipLabel.Text = "IP";
            // 
            // portLabel
            // 
            this.portLabel.AutoSize = true;
            this.portLabel.Location = new System.Drawing.Point(11, 75);
            this.portLabel.Name = "portLabel";
            this.portLabel.Size = new System.Drawing.Size(47, 15);
            this.portLabel.TabIndex = 1;
            this.portLabel.Text = "PORT";
            // 
            // pathLabel
            // 
            this.pathLabel.AutoSize = true;
            this.pathLabel.Location = new System.Drawing.Point(11, 121);
            this.pathLabel.Name = "pathLabel";
            this.pathLabel.Size = new System.Drawing.Size(44, 15);
            this.pathLabel.TabIndex = 2;
            this.pathLabel.Text = "PATH";
            // 
            // txtIP
            // 
            this.txtIP.Location = new System.Drawing.Point(69, 18);
            this.txtIP.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtIP.Name = "txtIP";
            this.txtIP.Size = new System.Drawing.Size(452, 25);
            this.txtIP.TabIndex = 3;
            // 
            // txtPORT
            // 
            this.txtPORT.Location = new System.Drawing.Point(69, 65);
            this.txtPORT.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtPORT.Name = "txtPORT";
            this.txtPORT.Size = new System.Drawing.Size(452, 25);
            this.txtPORT.TabIndex = 4;
            // 
            // txtPATH
            // 
            this.txtPATH.Location = new System.Drawing.Point(69, 111);
            this.txtPATH.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtPATH.Name = "txtPATH";
            this.txtPATH.ReadOnly = true;
            this.txtPATH.Size = new System.Drawing.Size(452, 25);
            this.txtPATH.TabIndex = 5;
            // 
            // btnServer
            // 
            this.btnServer.Location = new System.Drawing.Point(544, 28);
            this.btnServer.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnServer.Name = "btnServer";
            this.btnServer.Size = new System.Drawing.Size(103, 49);
            this.btnServer.TabIndex = 6;
            this.btnServer.Text = "서버켜기";
            this.btnServer.UseVisualStyleBackColor = true;
            this.btnServer.Click += new System.EventHandler(this.btnServer_Click);
            // 
            // btnRoute
            // 
            this.btnRoute.Location = new System.Drawing.Point(544, 111);
            this.btnRoute.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnRoute.Name = "btnRoute";
            this.btnRoute.Size = new System.Drawing.Size(103, 28);
            this.btnRoute.TabIndex = 7;
            this.btnRoute.Text = "경로선택";
            this.btnRoute.UseVisualStyleBackColor = true;
            this.btnRoute.Click += new System.EventHandler(this.btnRoute_Click);
            // 
            // txtLog
            // 
            this.txtLog.Location = new System.Drawing.Point(11, 180);
            this.txtLog.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtLog.Multiline = true;
            this.txtLog.Name = "txtLog";
            this.txtLog.Size = new System.Drawing.Size(635, 408);
            this.txtLog.TabIndex = 8;
            // 
            // labelLog
            // 
            this.labelLog.AutoSize = true;
            this.labelLog.Location = new System.Drawing.Point(11, 159);
            this.labelLog.Name = "labelLog";
            this.labelLog.Size = new System.Drawing.Size(27, 15);
            this.labelLog.TabIndex = 9;
            this.labelLog.Text = "log";
            // 
            // server
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(659, 600);
            this.Controls.Add(this.labelLog);
            this.Controls.Add(this.txtLog);
            this.Controls.Add(this.btnRoute);
            this.Controls.Add(this.btnServer);
            this.Controls.Add(this.txtPATH);
            this.Controls.Add(this.txtPORT);
            this.Controls.Add(this.txtIP);
            this.Controls.Add(this.pathLabel);
            this.Controls.Add(this.portLabel);
            this.Controls.Add(this.ipLabel);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "server";
            this.Text = "File Manager - Server";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.server_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label ipLabel;
        private System.Windows.Forms.Label portLabel;
        private System.Windows.Forms.Label pathLabel;
        private System.Windows.Forms.TextBox txtIP;
        private System.Windows.Forms.TextBox txtPORT;
        private System.Windows.Forms.TextBox txtPATH;
        private System.Windows.Forms.Button btnServer;
        private System.Windows.Forms.Button btnRoute;
        private System.Windows.Forms.TextBox txtLog;
        private System.Windows.Forms.Label labelLog;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDlg;
    }
}

