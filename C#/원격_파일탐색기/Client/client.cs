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
using Packet;


namespace Client
{
    public partial class client : Form
    {
        //server client members
        public NetworkStream m_Stream;      //네트워크 스트림
        //public StreamReader m_Read;         //읽기
        //public StreamWriter m_Write;        //쓰기
        private byte[] sendBuffer = new byte[1024 * 4];
        private byte[] readBuffer = new byte[1024 * 4];
//private Thread m_ThReader;          //읽기 쓰레드
        int port;                           //포트번호
        IPAddress IPAddr;

        //client members
        public bool m_bConnect = false;     //서버 접속 플래그
        TcpClient m_Client;

        TreeViewCancelEventArgs ee;
        info mi;

        public client()
        {
            InitializeComponent();
        }

        private void client_Load(object sender, EventArgs e)
        {
            
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (txtDownload.Text == "")
            {
                MessageBox.Show("경로가 설정되어있지 않습니다.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                if (btnConnect.Text == "서버연결")
                {
                    Connect();
                    btnConnect.Text = "서버끊기";
                    btnConnect.ForeColor = Color.Red;

                    pDirInfo dir = new pDirInfo();
                    dir.Type = (int)PacketType.초기화;

                    PacketC.Serialize(dir).CopyTo(sendBuffer, 0);
                    Send();
                    Receive();
                }
                else
                {
                    Disconnect();
                    btnConnect.Text = "서버연결";
                    btnConnect.ForeColor = Color.Black;
                }
            }

        }

        public void Send()
        {
            m_Stream.Write(sendBuffer, 0, sendBuffer.Length);
            m_Stream.Flush();

            for (int i = 0; i < 1024 * 4; i++)
            {
                sendBuffer[i] = 0;
            }
        }

        public void Connect()
        {
            m_Client = new TcpClient();
            port = int.Parse(txtPort.Text);
            IPAddr = IPAddress.Parse(txtIP.Text);
            try
            {
                m_Client.Connect(IPAddr, port);
            }
            catch
            {
                m_bConnect = false;
                return;
            }
            m_bConnect = true;

            m_Stream = m_Client.GetStream();

        }

        public void Disconnect()
        {
            if (!m_bConnect)
                return;
            m_bConnect = false;
            pDirInfo temp = new pDirInfo();
            temp.Type = (int)PacketType.종료;
            PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
            Send();
            m_Stream.Close();

        }

        public void Receive()
        {
            int nRead = 0;
            while (m_bConnect)
            {
                try
                {
                    Thread.Sleep(10); 
                    Application.DoEvents();


                    nRead = 0;
                    nRead = m_Stream.Read(readBuffer, 0, 1024 * 4);
                }
                catch
                {
                    m_bConnect = false;
                    m_Stream = null;
                }

                PacketC packet = (PacketC)PacketC.Deserialize(readBuffer);

                switch ((int)packet.Type)
                {
                    case (int)PacketType.초기화:
                        {
                            lvwFiles.Items.Clear();    
                            trvDir.Nodes.Clear();
                            pDirInfo dir = (pDirInfo)PacketC.Deserialize(readBuffer);
                            TreeNode root;
                            root = trvDir.Nodes.Add(dir.name);
                            root.ImageIndex = 1;
                            
                            if (trvDir.SelectedNode == null)
                                trvDir.SelectedNode = root;
                            root.SelectedImageIndex = root.ImageIndex;
                            if (dir.Length > 0)
                                root.Nodes.Add("");

                            m_bConnect = false;
                            break;
                        }
                    case (int)PacketType.Expand:
                        {
                            pDirInfo dir = (pDirInfo)PacketC.Deserialize(readBuffer);   
                            while (true)
                            {
                                try
                                {
                                    nRead = 0;
                                    nRead = m_Stream.Read(readBuffer, 0, 1024 * 4);
                                    break;
                                }
                                catch
                                {
                                    m_bConnect = false;
                                    m_Stream = null;
                                    break;
                                }
                                
                            }

                            pDirInfo[] di = (pDirInfo[])PacketC.Deserialize(readBuffer);
                            TreeNode node;
                            if (di[0].name != "")
                            {
                                foreach (pDirInfo dirs in di)
                                {
                                    node = ee.Node.Nodes.Add(dirs.name);

                                    if (dirs.Length > 0)
                                        node.Nodes.Add("");
                                }
                            }
                            
                            m_bConnect = false;
                            break;
                        }
                    case (int)PacketType.Select:
                        {
                            pDirInfo dir = (pDirInfo)PacketC.Deserialize(readBuffer);
                            while (true)
                            {
                                try
                                {
                                    nRead = 0;
                                    nRead = m_Stream.Read(readBuffer, 0, 1024 * 4);
                                    break;
                                }
                                catch
                                {
                                    m_bConnect = false;
                                    m_Stream = null;
                                    break;
                                }

                            }

                            pDirInfo[] di = (pDirInfo[])PacketC.Deserialize(readBuffer);
                            ListViewItem item;
                            
                            foreach (pDirInfo tdis in di)
                            {
                                item = lvwFiles.Items.Add(tdis.name);
                                if (tdis.ext == "folder")
                                    item.SubItems.Add("");
                                else
                                    item.SubItems.Add(tdis.size.ToString());
                                item.SubItems.Add(tdis.modifyDate);
                                switch (tdis.ext)
                                {
                                    case "folder":
                                        {
                                            item.Tag = "D";
                                            item.ImageIndex = 1;
                                            break;
                                        }
                                    case ".txt":
                                        {
                                            item.Tag = "F";
                                            item.ImageIndex = 5;
                                            break;
                                        }
                                    case ".mp3":
                                        {
                                            item.Tag = "F";
                                            item.ImageIndex = 3;
                                            break;
                                        }
                                    case ".png":
                                        {
                                            item.Tag = "F";
                                            item.ImageIndex = 2;
                                            break;
                                        }
                                    case ".avi":
                                        {
                                            item.Tag = "F";
                                            item.ImageIndex = 0;
                                            break;
                                        }
                                    default:
                                        {
                                            item.Tag = "F";
                                            item.ImageIndex = 4;
                                            break;
                                        }
                                }
                            }
                            
                            m_bConnect = false;
                            break;
                        }
                    case (int)PacketType.moreInfo:
                        {
                            pDirInfo dir = (pDirInfo)PacketC.Deserialize(readBuffer);
                            mi.lblType.Text = dir.ext.Substring(1);
                            mi.lblCreateDate.Text = dir.createDate;
                            mi.lblModifyDate.Text = dir.modifyDate;
                            mi.lblAccessDate.Text = dir.accessDate;
                            switch (mi.lblType.Text)
                            {
                                case "folder":
                                    {
                                        mi.pictureBox1.Image = imageList2.Images[1];
                                        break;
                                    }
                                case "txt":
                                    {
                                        mi.pictureBox1.Image = imageList2.Images[5];
                                        break;
                                    }
                                case "mp3":
                                    {
                                        mi.pictureBox1.Image = imageList2.Images[3];
                                        break;
                                    }
                                case "png":
                                    {
                                        mi.pictureBox1.Image = imageList2.Images[2];
                                        break;
                                    }
                                case "avi":
                                    {
                                        mi.pictureBox1.Image = imageList2.Images[0];
                                        break;
                                    }
                                default:
                                    {
                                        mi.pictureBox1.Image = imageList2.Images[4];
                                        break;
                                    }
                            }


                            mi.Show();

                            m_bConnect = false;
                            break;
                        }
                    case (int)PacketType.파일전송:
                        {

                            pDirInfo dir = (pDirInfo)PacketC.Deserialize(readBuffer);
                            int total = 0;

                            FileStream fileStream = new FileStream(txtDownload.Text + "\\" + dir.name, FileMode.Create, FileAccess.Write);
                            BinaryWriter writer = new BinaryWriter(fileStream);

                            int receiveLength = 0;
                            while (total < dir.Length)        //전체 파일을 받는다.
                            {
                                while (true)
                                {
                                    try
                                    {
                                        receiveLength = m_Stream.Read(readBuffer, 0, 1024 * 4);
                                        break;
                                    }
                                    catch
                                    {
                                        m_bConnect = false;
                                        m_Stream = null;
                                        break;
                                    }

                                }

                                writer.Write(readBuffer, 0, receiveLength);
                                total += receiveLength;


                            }
                            writer.Close();
                            fileStream.Close();



                            m_bConnect = false;
                            break;
                        }
                }
                
            }
            m_bConnect = true;
        }

        public void setPlus(TreeNode node, pDirInfo dir)
        {
            if (dir.Length > 0)
                node.Nodes.Add("");

        }

        public void OpenFiles()
        {
            ListView.SelectedListViewItemCollection siList;
            siList = lvwFiles.SelectedItems;

            //아이템이 여러개이기 때문에 다음과 같이 설정
            foreach(ListViewItem item in siList)
            {
                OpenItem(item);
            }
        }

        public void OpenItem(ListViewItem item)
        {
            TreeNode node;
            TreeNode child;

            if(item.Tag.ToString() == "D")
            {
                //현재 선택된 트리뷰의 노드를 확장
                node = trvDir.SelectedNode;
                node.Expand();

                //확장된 노드 중 현재 리스트뷰에서 선택한 같은 노드를 선택
                child = node.FirstNode;

                while(child != null)
                {
                    if(child.Text == item.Text)
                    {
                        trvDir.SelectedNode = child;
                        trvDir.Focus();
                        break;
                    }
                    child = child.NextNode;
                }
            }
            else
            {
                //파일 더블 클릭 시 상세정보 출력
                moreInfo(item);
            }
        }

        public void moreInfo(ListViewItem item)
        {
            mi = new info();
            mi.Owner = this;

            mi.textBox1.Text = item.Text;
            mi.lblRoute.Text = trvDir.SelectedNode.FullPath;
            if (Path.GetExtension(item.Text) == "")
            {
                mi.lblSize.Text = "0 바이트";
            }
            else
            {
                mi.lblSize.Text = item.SubItems[1].Text + "바이트";
            }
            

            pDirInfo temp = new pDirInfo();
            temp.Type = (int)PacketType.moreInfo;
            temp.name = trvDir.SelectedNode.FullPath + "\\" + item.Text;
            PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
            Send();
            Receive();

        }
        
        private void btnRoute_Click(object sender, EventArgs e)
        {
            if (folderBrowserDlg.ShowDialog() == DialogResult.OK)
            {
                txtDownload.Text = folderBrowserDlg.SelectedPath;
            }
        }

        private void btnFolder_Click(object sender, EventArgs e)
        {
            if (txtDownload.Text != "")
                Process.Start(txtDownload.Text);
            else
                MessageBox.Show("경로가 설정되어있지 않습니다.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void trvDir_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            e.Node.Nodes.Clear();
            ee = e;
            pDirInfo dir = new pDirInfo();
            dir.Type = (int)PacketType.Expand;
            dir.name = e.Node.FullPath;
            PacketC.Serialize(dir).CopyTo(sendBuffer, 0);
            Send();
            Receive();

        }

        private void trvDir_BeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            lvwFiles.Items.Clear();     //기존 정보 초기화
            ee = e;
            pDirInfo dir = new pDirInfo();
            dir.Type = (int)PacketType.Select;
            dir.name = e.Node.FullPath;
            PacketC.Serialize(dir).CopyTo(sendBuffer, 0);
            Send();
            Receive();

        }

        private void lvwFiles_DoubleClick(object sender, EventArgs e)
        {
            OpenFiles();
        }

        private void mnuInfo_Click(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection siList;
            siList = lvwFiles.SelectedItems;
            foreach (ListViewItem item in siList)
            {
                moreInfo(item);
            }
            
        }

        private void mnuDetail_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = (ToolStripMenuItem)sender;

            mnuDetail.Checked = false;
            mnuList.Checked = false;
            mnuSmall.Checked = false;
            mnuLarge.Checked = false;

            switch (item.Text)
            {
                case "자세히":
                    mnuDetail.Checked = true;
                    lvwFiles.View = View.Details;
                    break;
                case "간단히":
                    mnuList.Checked = true;
                    lvwFiles.View = View.List;
                    break;
                case "작은아이콘":
                    mnuSmall.Checked = true;
                    lvwFiles.View = View.SmallIcon;
                    break;
                case "큰아이콘":
                    mnuLarge.Checked = true;
                    lvwFiles.View = View.LargeIcon;
                    break;
            }
        }

        private void mnuDownload_Click(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection siList;
            siList = lvwFiles.SelectedItems;
            foreach (ListViewItem item in siList)
            {
                if (Path.GetExtension(item.Text) == "")
                {
                    MessageBox.Show("폴더는 다운로드를 지원하지 않습니다", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else
                {
                    pDirInfo dir = new pDirInfo();
                    dir.Type = (int)PacketType.파일전송;
                    dir.name = trvDir.SelectedNode.FullPath + "\\" + item.Text;
                    PacketC.Serialize(dir).CopyTo(sendBuffer, 0);
                    Send();
                    Receive();
                }
            }
        }

        private void client_FormClosing(object sender, FormClosingEventArgs e)
        {
            Disconnect();
        }
    }
}
