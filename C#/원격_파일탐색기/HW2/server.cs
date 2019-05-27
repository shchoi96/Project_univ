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

namespace HW2
{
    public partial class server : Form
    {
        //server client members
        public NetworkStream m_Stream;      //네트워크 스트림

        private byte[] sendBuffer = new byte[1024 * 4];
        private byte[] readBuffer = new byte[1024 * 4];
        private Thread m_ThReader;          //읽기 쓰레드
        int port;                           //포트번호
        IPAddress IPAddr;

        //server members
        public bool m_bStop = false;        //서버 시작&중단 플래그
        private TcpListener m_listener;     //서버 작동 리스너
        private Thread m_thServer;          //서버 쓰레드

        //client members
        public bool m_bConnect = false;     //서버 접속 플래그
        TcpClient m_Client;

        public server()
        {
            InitializeComponent();
        }

        private void btnServer_Click(object sender, EventArgs e)
        {
            if (txtPATH.Text == "")
            {
                MessageBox.Show("경로를 선택해주세요", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                if (btnServer.Text == "서버켜기")
                {
                    m_thServer = new Thread(new ThreadStart(ServerStart));
                    m_thServer.Start();

                    btnServer.Text = "서버끊기";
                    btnServer.ForeColor = Color.Red;
                }
                else
                {
                    ServerStop();
                    btnServer.Text = "서버켜기";
                    btnServer.ForeColor = Color.Black;
                }
            }
        }

        private void btnRoute_Click(object sender, EventArgs e)
        {
            if (folderBrowserDlg.ShowDialog() == DialogResult.OK)
            {
                txtPATH.Text = folderBrowserDlg.SelectedPath;
                Invoke(new MethodInvoker(delegate ()
                {
                    txtLog.AppendText(txtPATH.Text + "로 경로가 수정되었습니다.\r\n");
                }));
            }
        }

        public void Send()
        {
            m_Stream.Write(sendBuffer, 0, sendBuffer.Length);
            m_Stream.Flush();

            for(int i = 0; i < 1024 * 4; i++)
            {
                sendBuffer[i] = 0;
            }
        }

        public void ServerStart()
        {
            port = int.Parse(txtPORT.Text);
            IPAddr = IPAddress.Parse(txtIP.Text);
            try
            {
                m_listener = new TcpListener(IPAddr, port);
                m_listener.Start();

                m_bStop = true;

                Invoke(new MethodInvoker(delegate ()
                {
                    txtLog.AppendText("클라이언트 접속 대기중...\r\n");
                }));
                while (m_bStop)
                {
                    TcpClient hClient = m_listener.AcceptTcpClient();

                    if (hClient.Connected)
                    {
                        m_bConnect = true;

                        Invoke(new MethodInvoker(delegate ()
                        {
                            txtLog.AppendText("클라이언트 접속\r\n");
                        }));
                        m_Stream = hClient.GetStream();

                        m_ThReader = new Thread(new ThreadStart(Receive));
                        m_ThReader.Start();
                    }
                    else
                    {
                        hClient = m_listener.AcceptTcpClient();
                    }
                }
            }
            catch
            {
                Invoke(new MethodInvoker(delegate ()
                {
                    txtLog.AppendText("시작 도중에 오류 발생\r\n");
                }));
                return;
            }
        }

        public void ServerStop()
        {
            if (!m_bStop)
                return;
            m_listener.Stop();      //서버 소켓 작동 중지

            if (m_Stream != null)
                m_Stream.Close();
            m_ThReader.Abort();     //서버 소켓 스레드 종료
            m_thServer.Abort();     //스레드 종료
            Invoke(new MethodInvoker(delegate ()
            {
                txtLog.AppendText("서비스 종료\r\n");
            }));
     
        }

        public void Receive()
        {
            int nRead = 0;
            while (m_bConnect)
            {
                try
                {
                    nRead = 0;
                    nRead = m_Stream.Read(readBuffer, 0, 1024 * 4);
                }
                catch
                {
                    m_bConnect = false;
                    m_Stream = null;
                    Invoke(new MethodInvoker(delegate ()
                    {
                        txtLog.AppendText("데이터를 읽는 과정에서 오류가 발생\r\n");
                    }));

                }

                PacketC packet = (PacketC)PacketC.Deserialize(readBuffer);
                switch ((int)packet.Type)
                {
                    case (int)PacketType.초기화:
                        {
                            pDirInfo dir = new pDirInfo();
                            dir.Type = (int)PacketType.초기화;
                            dir.name = txtPATH.Text;
                            DirectoryInfo dirTemp = new DirectoryInfo(dir.name);
                            DirectoryInfo[] diTemp = dirTemp.GetDirectories();
                            dir.Length = diTemp.Length;

                            PacketC.Serialize(dir).CopyTo(sendBuffer, 0);
                            Send();
                            Invoke(new MethodInvoker(delegate ()
                            {
                                txtLog.AppendText("초기화 데이터 요청..\r\n");
                            }));

                            break;
                        }
                    case (int)PacketType.Expand:
                        {
                            try
                            {
                                pDirInfo temp = (pDirInfo)PacketC.Deserialize(readBuffer);
                                pDirInfo dir = new pDirInfo();
                                pDirInfo[] di;

                                DirectoryInfo dirTemp = new DirectoryInfo(temp.name);
                                DirectoryInfo[] diTemp = dirTemp.GetDirectories();

                                dir.Type = (int)PacketType.Expand;
                                dir.Length = diTemp.Length;

                                PacketC.Serialize(dir).CopyTo(sendBuffer, 0);
                                Send();

                                if (diTemp.Length != 0)
                                {
                                    di = new pDirInfo[diTemp.Length];
                                    for (int i = 0; i < diTemp.Length; i++)
                                    {
                                        di[i] = new pDirInfo();
                                        di[i].name = diTemp[i].Name;
                                        DirectoryInfo[] didiTemp = diTemp[i].GetDirectories();
                                        di[i].Length = didiTemp.Length;
                                    }
                                    PacketC.Serialize(di).CopyTo(sendBuffer, 0);
                                    Send();
                                }
                                else
                                {
                                    di = new pDirInfo[1];
                                    di[0] = new pDirInfo();
                                    di[0].Length = 0;
                                    di[0].name = "";
                                    PacketC.Serialize(di).CopyTo(sendBuffer, 0);
                                    Send();
                                }
                                
                                Invoke(new MethodInvoker(delegate ()
                                {
                                    txtLog.AppendText("beforeExpand 데이터 요청..\r\n");
                                }));
                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show(ex.Message);
                            }
                            
                            break;
                        }
                    case (int)PacketType.Select:
                        {
                            try
                            {
                                pDirInfo temp = (pDirInfo)PacketC.Deserialize(readBuffer);
                                pDirInfo dir = new pDirInfo();
                                pDirInfo[] di;

                                DirectoryInfo dirTemp = new DirectoryInfo(temp.name);
                                DirectoryInfo[] diTemp = dirTemp.GetDirectories();
                                FileInfo[] fiTemp = dirTemp.GetFiles();

                                dir.Type = (int)PacketType.Select;
                                dir.Length = diTemp.Length + fiTemp.Length;

                                di = new pDirInfo[diTemp.Length + fiTemp.Length];

                                for (int i = 0; i < diTemp.Length; i++)
                                {
                                    di[i] = new pDirInfo();
                                    di[i].name = diTemp[i].Name;
                                    di[i].modifyDate = diTemp[i].LastWriteTime.ToString();
                                    di[i].ext = "folder";
                                }
                                for (int i = 0; i < fiTemp.Length; i++)
                                {
                                    di[i + diTemp.Length] = new pDirInfo();
                                    di[i + diTemp.Length].name = fiTemp[i].Name;
                                    di[i + diTemp.Length].size = fiTemp[i].Length.ToString();
                                    di[i + diTemp.Length].ext = Path.GetExtension(fiTemp[i].Name);
                                    di[i + diTemp.Length].modifyDate = fiTemp[i].LastWriteTime.ToString();
                                }

                                PacketC.Serialize(dir).CopyTo(sendBuffer, 0);
                                Send();
                                PacketC.Serialize(di).CopyTo(sendBuffer, 0);
                                Send();
                                Invoke(new MethodInvoker(delegate ()
                                {
                                    txtLog.AppendText("beforeSelect 데이터 요청..\r\n");
                                }));
                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show(ex.Message);
                            }
                            

                            break;
                        }
                    case (int)PacketType.moreInfo:
                        {
                            try
                            {
                                pDirInfo temp = (pDirInfo)PacketC.Deserialize(readBuffer);
                                pDirInfo dir = new pDirInfo();
                                FileInfo fiTemp = new FileInfo(temp.name);
                                


                                if (Path.GetExtension(temp.name) == "")
                                {
                                    dir.ext = ".folder";
                                }
                                else
                                {
                                    dir.ext = Path.GetExtension(temp.name);
                                }

                                dir.createDate = fiTemp.CreationTime.ToString();
                                dir.modifyDate = fiTemp.LastWriteTime.ToString();
                                dir.accessDate = fiTemp.LastAccessTime.ToString();
                                dir.Type = (int)PacketType.moreInfo;
                                PacketC.Serialize(dir).CopyTo(sendBuffer, 0);
                                Send();
                                Invoke(new MethodInvoker(delegate ()
                                {
                                    txtLog.AppendText("상세정보 데이터 요청..\r\n");
                                }));

                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show(ex.Message);
                            }
                            break;
                        }
                    case (int)PacketType.파일전송:
                        {
  
                            {
                                pDirInfo temp = (pDirInfo)PacketC.Deserialize(readBuffer);
                                pDirInfo dir = new pDirInfo();
                                FileInfo fiTemp = new FileInfo(temp.name);
                                FileStream fileStream = new FileStream(temp.name, FileMode.Open, FileAccess.Read);

                                dir.Length = (int)fileStream.Length;
                                dir.Type = (int)PacketType.파일전송;
                                dir.name = fiTemp.Name;
                                PacketC.Serialize(dir).CopyTo(sendBuffer, 0);
                                Send();

                                int count = dir.Length / 4096 + 1;  // 전송할 파일 분할
                                BinaryReader reader = new BinaryReader(fileStream);

                                for (int i = 0; i < count - 1; i++)
                                {
                                    sendBuffer = reader.ReadBytes(1024 * 4);
                                    Send();
                                }
                                sendBuffer = reader.ReadBytes(1024 * 4);
                                m_Stream.Write(sendBuffer, 0, sendBuffer.Length);
                                m_Stream.Flush();

                                sendBuffer = new byte[1024 * 4];

                                Invoke(new MethodInvoker(delegate ()
                                {
                                    txtLog.AppendText(temp.name + " 데이터 다운로드 완료..\r\n");
                                }));
                                reader.Close();
                                fileStream.Close();
                            }

                           
                            break;
                        }
                    case (int)PacketType.종료:
                        {
                            m_bConnect = false;
                            m_Stream.Close();
                            Invoke(new MethodInvoker(delegate ()
                            {
                                txtLog.AppendText("클라이언트에서 연결 끊음\r\n");
                            }));
                            break;
                        }
                }
            }
            
        }

        private void server_FormClosing(object sender, FormClosingEventArgs e)
        {
            ServerStop();
        }
    }
}
