using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using System.Net.Sockets;
using System.Threading;
using System.Net;

namespace client
{
    public partial class serverConnect : Form
    {
        public serverConnect()
        {
            InitializeComponent();
        }

        clientForm form;
        private void btn_Connect_Click(object sender, EventArgs e)
        {
            if (btn_Connect.Text == "접속")
            {
                btn_Connect.Text = "연결끊기";
                btn_Connect.ForeColor = Color.Red;
                form = new clientForm(IPAddress.Parse(txt_IP.Text), int.Parse(txt_port.Text), txt_name.Text);
                form.parent = this;
                form.Show();
            }
            else
            {
                btn_Connect.Text = "접속";
                btn_Connect.ForeColor = Color.Black;
                form.Disconnect();
            }
        }
    }
}
