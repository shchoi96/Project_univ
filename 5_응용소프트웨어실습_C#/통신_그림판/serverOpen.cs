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
using System.Net.Sockets;
using System.Threading;
using System.Net;
using Packet;
using System.Drawing.Drawing2D;
using System.Runtime.Serialization.Formatters.Binary;

namespace server
{
    public partial class serverOpen : Form
    {
        //server client members
        public List<NetworkStream> m_Stream;
        private List<Thread> m_ThReader;

        int port;
        IPAddress IPAddr;
        //server members
        public bool m_bStop = false;        //서버 시작&중단 플래그
        private TcpListener m_listener;     //서버 작동 리스너
        private Thread m_thServer;          //서버 쓰레드
        private List<TcpClient> m_Client;
        
        //client members
        private List<bool> m_Connect;
        Form_server form_Server;
        
        public serverOpen()
        {
            InitializeComponent();
        }
        
        private void serverOpen_Load(object sender, EventArgs e)
        {
            m_Client = new List<TcpClient>();
            m_ThReader = new List<Thread>();
            m_Stream = new List<NetworkStream>();
            m_Connect = new List<bool>();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (button1.Text == "서버열기")
            {
                form_Server = new Form_server();
                this.AddOwnedForm(form_Server);
                form_Server.parent = this;
                form_Server.Show();
                port = int.Parse(txt_port.Text);
                IPAddr = IPAddress.Parse(txt_IP.Text);
                m_thServer = new Thread(new ThreadStart(ServerStart));
                m_thServer.Start();

                button1.Text = "서버끊기";
                button1.ForeColor = Color.Red;
            }
            else
            {
                ServerStop();
                button1.Text = "서버열기";
                button1.ForeColor = Color.Black;
            }
        }

        int cnt = 0;
        public void ServerStart()
        {
            try
            {
                m_listener = new TcpListener(IPAddr, port);
                m_listener.Start();
                m_bStop = true;
                while (m_bStop)
                {
                    TcpClient hClient = m_listener.AcceptTcpClient();
                    m_Client.Add(hClient);
                    
                    if (hClient.Connected)
                    {

                        
                        NetworkStream hStream = hClient.GetStream();
                        hStream.Flush();
                        bool m_bConnect = true;
                        m_Connect.Add(m_bConnect);
                        m_Stream.Add(hStream);
                        Thread hThReader = new Thread(() => Receive(cnt++));
                        m_ThReader.Add(hThReader);
                        hThReader.Start();
                    }
                    else
                    {
                        hClient = m_listener.AcceptTcpClient();
                    }
                }
            }
            catch
            {

            }
        }

        public void ServerStop()
        {
            if (!m_bStop)
                return;
            m_listener.Stop();
            if (m_Stream != null)
                for (int i = 0; i < m_Stream.Count; i++)
                    if(m_Stream[i] != null)
                        m_Stream[i].Close();
            for (int i = 0; i < m_ThReader.Count; i++)
                m_ThReader[i].Abort();
            m_thServer.Abort();
        }

        public void Send(NetworkStream stream, ref byte[] sendBuffer)
        {
            for(int i = 0; i < m_Stream.Count; i++)
            {
                if(m_Stream[i] != stream && m_Stream[i] != null)
                {
                    m_Stream[i].Write(sendBuffer, 0, sendBuffer.Length);
                    m_Stream[i].Flush();

                }
            }

            for (int j = 0; j < 1024 * 4; j++)
            {
                sendBuffer[j] = 0;
            }
        }

        public void Receive(int o)
        {
            byte[] readBuffer = new byte[1024 * 4];
            byte[] sendBuffer = new byte[1024 * 4];
            Array.Clear(readBuffer, 0, readBuffer.Length);
            Array.Clear(sendBuffer, 0, sendBuffer.Length);


            int nRead = 0;
            while (m_Connect[o])
            {
                try
                {
                    nRead = 0;
                    nRead = m_Stream[o].Read(readBuffer, 0, 1024 * 4);

                }
                catch
                {
                    m_Connect[o] = false;
                    m_Stream = null;
                }
                
                PacketC packet = (PacketC)PacketC.Deserialize(readBuffer);
                switch ((int)packet.Type)
                {
                    case (int)PacketType.초기화:
                        {

                            break;
                        }
                    case (int)PacketType.원:
                        {
                            
                            pShape temp = (pShape)PacketC.Deserialize(readBuffer);
                            temp.z = form_Server.z++;
                            form_Server.mycircle[form_Server.ncircle++].setRectC(new Rectangle(temp.rectC[0], temp.rectC[1], temp.rectC[2], temp.rectC[3]), temp.thick, temp.isSolid, Color.FromArgb(temp.line_color), Color.FromArgb(temp.fill_color), temp.isFill, temp.z);
                            PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
                            Send(m_Stream[o], ref sendBuffer);
                            break;
                        }
                    case (int)PacketType.선:
                        {
                            pShape temp = (pShape)PacketC.Deserialize(readBuffer);
                            temp.z = form_Server.z++;
                            form_Server.mylines[form_Server.nline++].setPoint(new Point(temp.rectC[0], temp.rectC[1]), new Point(temp.rectC[2], temp.rectC[3]), temp.thick, temp.isSolid, Color.FromArgb(temp.line_color), temp.z);
                            PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
                            Send(m_Stream[o], ref sendBuffer);
                            break;
                        }
                    case (int)PacketType.연필:
                        {
                            pShape temp = (pShape)PacketC.Deserialize(readBuffer);
                            temp.z = form_Server.z++;
                            form_Server.mypencil[form_Server.npencil].seAttr(temp.thick, temp.isSolid, Color.FromArgb(temp.line_color), temp.z);
                            for (int i = 0; i < temp.startsX.Count; i++)
                            {
                                form_Server.mypencil[form_Server.npencil].pushPoint(new Point(temp.startsX[i], temp.startsY[i]), new Point(temp.finishesX[i], temp.finishesY[i]));
                            }
                                
                            PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
                            Send(m_Stream[o], ref sendBuffer);
                            form_Server.npencil++;
                            break;
                        }
                    case (int)PacketType.사각:
                        {
                            pShape temp = (pShape)PacketC.Deserialize(readBuffer);
                            temp.z = form_Server.z++;
                            form_Server.myrect[form_Server.nrect++].setRect(new Rectangle(temp.rectC[0], temp.rectC[1], temp.rectC[2], temp.rectC[3]), temp.thick, temp.isSolid, Color.FromArgb(temp.line_color), Color.FromArgb(temp.fill_color), temp.isFill, temp.z);

                            PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
                            Send(m_Stream[o], ref sendBuffer);
                            break;
                        }
                    case (int)PacketType.채팅:
                        {
                            form_Server.textBox1.AppendText(packet.name + ": " + packet.tts + "\r\n");
                            readBuffer.CopyTo(sendBuffer, 0);
                            Send(m_Stream[o], ref sendBuffer);
                            break;
                        }
                    case (int)PacketType.종료:
                        {
                            m_Connect[o] = false;
                            m_Stream[o].Close();
                            m_Stream[o] = null;
                            break;
                        }

                }
                form_Server.painter.Invalidate(true);
                form_Server.painter.Update();

            }
            
        }

        private void serverOpen_FormClosing(object sender, FormClosingEventArgs e)
        {
            ServerStop();
        }
    }
}
