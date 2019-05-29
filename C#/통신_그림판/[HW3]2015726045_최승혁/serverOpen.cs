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

        private byte[] sendBuffer = new byte[1024 * 4];
        private byte[] readBuffer = new byte[1024 * 4];
        private List<Thread> m_ThReader;

        int port;
        IPAddress IPAddr;
        //server members
        public bool m_bStop = false;        //서버 시작&중단 플래그
        private TcpListener m_listener;     //서버 작동 리스너
        private Thread m_thServer;          //서버 쓰레드

        private List<TcpClient> m_Client;

        //client members
        public bool m_bConnect = false;     //서버 접속 플래그

        public serverOpen()
        {
            InitializeComponent();
            
        }
        
        private void serverOpen_Load(object sender, EventArgs e)
        {
            m_Client = new List<TcpClient>();
            m_ThReader = new List<Thread>();
            m_Stream = new List<NetworkStream>();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (button1.Text == "서버켜기")
            {
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
                button1.Text = "서버켜기";
                button1.ForeColor = Color.Black;
            }
        }

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
                        m_bConnect = true;
                        NetworkStream hStream = hClient.GetStream();
                        m_Stream.Add(hStream);
                        Thread hThReader = new Thread(() => Receive(m_ThReader.Count));
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
                return;
            }
        }

        public void ServerStop()
        {
            if (!m_bStop)
                return;
            m_listener.Stop();
            if (m_Stream != null)
                for (int i = 0; i < m_Stream.Count; i++)
                    m_Stream[i].Close();
            for (int i = 0; i < m_ThReader.Count; i++)
                m_ThReader[i].Abort();
            m_thServer.Abort();
        }

        public void Receive(int o)
        {
            byte[] readBuffer = new byte[1024 * 4];
            int nRead = 0;
            while (m_bConnect)
            {
                try
                {
                    nRead = 0;
                    nRead = m_Stream[o].Read(readBuffer, 0, 1024 * 4);
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

                            break;
                        }
                    case (int)PacketType.원:
                        {
                            nRead = m_Stream[o].Read(readBuffer, 0, 1024 * 4);
                            MyCircle temp = (MyCircle)PacketC.Deserialize(readBuffer);

                            break;
                        }
                    case (int)PacketType.선:
                        {

                            break;
                        }
                    case (int)PacketType.연필:
                        {

                            break;
                        }
                    case (int)PacketType.사각:
                        {

                            break;
                        }
                }


            }
        }
    }
}
