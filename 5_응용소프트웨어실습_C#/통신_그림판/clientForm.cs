using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using System.Threading;
using System.Net;
using Packet;
using System.IO;
using System.Diagnostics;
using System.Net.Sockets;

namespace client
{
   
    public partial class clientForm : Form
    {
        public NetworkStream m_Stream;
        private byte[] sendBuffer = new byte[1024 * 4];
        private byte[] readBuffer = new byte[1024 * 4];
        public int port;                           //포트번호
        public IPAddress IPAddr;
        public string nickname;
        //client members
        public bool m_bConnect = false;     //서버 접속 플래그
        TcpClient m_Client;
        private Thread m_ThReader;

        private bool hand;
        private bool circle;
        private bool line;
        private Point start;
        private Point finish;
        private Pen pen;
        private SolidBrush brush;
        private bool pencil;
        private bool rect;

        private int z;
        private int ncircle;
        private int nline;
        private int npencil;
        private int nrect;
        private int i;
        private int thick;
        private bool isSolid;
        private MyCircle[] mycircle;
        private MyLines[] mylines;
        private MyPencil[] mypencil;
        private MyRect[] myrect;
        
        public Graphics g;
        public Graphics tempG;
        public float zoom;
        public Point wheelPoint;
        public Point scroll;
        public bool isDrag;

        public serverConnect parent;

        public clientForm(IPAddress IP, int port, string nickname)
        {
            InitializeComponent();
            g = panel1.CreateGraphics();
            SetupVar();
            IPAddr = IP;
            this.port = port;
            this.nickname = nickname;

            Connect();
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
            m_ThReader = new Thread(Receive);
            m_ThReader.Start();
        }

        public void Disconnect()
        {
            if (!m_bConnect)
                return;

            PacketC temp = new PacketC();
            temp.Type = (int)PacketType.종료;
            PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
            Send();
            m_bConnect = false;
            m_Stream.Close();
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
                            temp.z = z++;
                            mycircle[ncircle++].setRectC(new Rectangle(temp.rectC[0], temp.rectC[1], temp.rectC[2], temp.rectC[3]), temp.thick, temp.isSolid, Color.FromArgb(temp.line_color), Color.FromArgb(temp.fill_color), temp.isFill, temp.z);

                            break;
                        }
                    case (int)PacketType.선:
                        {
                            pShape temp = (pShape)PacketC.Deserialize(readBuffer);
                            temp.z = z++;
                            mylines[nline++].setPoint(new Point(temp.rectC[0], temp.rectC[1]), new Point(temp.rectC[2], temp.rectC[3]), temp.thick, temp.isSolid, Color.FromArgb(temp.line_color), temp.z);

                            break;
                        }
                    case (int)PacketType.연필:
                        {
                            pShape temp = (pShape)PacketC.Deserialize(readBuffer);
                            temp.z = z++;
                            mypencil[npencil].seAttr(temp.thick, temp.isSolid, Color.FromArgb(temp.line_color), temp.z);
                            for (int i = 0; i < temp.startsX.Count; i++)
                            {
                                mypencil[npencil].pushPoint(new Point(temp.startsX[i], temp.startsY[i]), new Point(temp.finishesX[i], temp.finishesY[i]));
                            }

                            npencil++;
                            break;
                        }
                    case (int)PacketType.사각:
                        {
                            pShape temp = (pShape)PacketC.Deserialize(readBuffer);
                            temp.z = z++;
                            myrect[nrect++].setRect(new Rectangle(temp.rectC[0], temp.rectC[1], temp.rectC[2], temp.rectC[3]), temp.thick, temp.isSolid, Color.FromArgb(temp.line_color), Color.FromArgb(temp.fill_color), temp.isFill, temp.z);

                            break;
                        }
                    case (int)PacketType.채팅:
                        {
                            textBox2.AppendText(packet.name + ": " + packet.tts + "\r\n");
                            
                            break;
                        }
                    case (int)PacketType.종료:
                        {

                            break;
                        }

                }
                panel1.Invalidate(true);
                panel1.Update();
            }
        }

        private void SetupVar()
        {
            i = 0;
            thick = 1;
            isSolid = true;
            hand = true;
            circle = false;
            line = false;
            pencil = false;
            rect = false;
            start = new Point(0, 0);
            finish = new Point(0, 0);
            pen = new Pen(Color.Black);
            brush = new SolidBrush(Color.White);
            mycircle = new MyCircle[100];
            mylines = new MyLines[100];
            mypencil = new MyPencil[100];
            myrect = new MyRect[100];
            ncircle = 0;
            nline = 0;
            npencil = 0;
            nrect = 0;
            z = 0;

            zoom = 1;
            wheelPoint = new Point(0, 0);
            panel1.MouseWheel += new MouseEventHandler(panel1_MouseWheel);
            isDrag = false;

            scroll = new Point(0, 0);
            SetupMine();
        }

        private void SetupMine()
        {
            for (i = 0; i < 100; i++)
            {
                mycircle[i] = new MyCircle();
                mylines[i] = new MyLines();
                mypencil[i] = new MyPencil();
                myrect[i] = new MyRect();
            }
        }

        public class DoubleBufferPanel : Panel
        {
            public DoubleBufferPanel()
            {
                this.SetStyle(ControlStyles.DoubleBuffer, true);
                this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
                this.SetStyle(ControlStyles.UserPaint, true);
                this.UpdateStyles();
            }
        }

        private void btn_Shape_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = (ToolStripMenuItem)sender;
            btn_tsdd1.Image = item.Image;
            if (item == btn_Hand)
            {
                hand = true;
                circle = false;
                line = false;
                pencil = false;
                rect = false;

                btn_Hand.Checked = true;
                btn_Circle.Checked = false;
                btn_Line.Checked = false;
                btn_Pencil.Checked = false;
                btn_Rect.Checked = false;

            }
            if (item == btn_Circle)
            {
                hand = false;
                circle = true;
                line = false;
                pencil = false;
                rect = false;

                btn_Hand.Checked = false;
                btn_Circle.Checked = true;
                btn_Line.Checked = false;
                btn_Pencil.Checked = false;
                btn_Rect.Checked = false;


            }
            if (item == btn_Line)
            {
                hand = false;
                circle = false;
                line = true;
                pencil = false;
                rect = false;

                btn_Hand.Checked = false;
                btn_Circle.Checked = false;
                btn_Line.Checked = true;
                btn_Pencil.Checked = false;
                btn_Rect.Checked = false;
            }
            if (item == btn_Pencil)
            {
                hand = false;
                circle = false;
                line = false;
                pencil = true;
                rect = false;

                btn_Hand.Checked = false;
                btn_Circle.Checked = false;
                btn_Line.Checked = false;
                btn_Pencil.Checked = true;
                btn_Rect.Checked = false;
            }
            if (item == btn_Rect)
            {
                hand = false;
                circle = false;
                line = false;
                pencil = false;
                rect = true;

                btn_Hand.Checked = false;
                btn_Circle.Checked = false;
                btn_Line.Checked = false;
                btn_Pencil.Checked = false;
                btn_Rect.Checked = true;
            }

        }

        private void btn_Line1_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = (ToolStripMenuItem)sender;
            btn_tsdd2.Image = item.Image;
            if (item == btn_Line1)
            {
                thick = 1;
                isSolid = true;

                btn_Line1.Checked = true;
                btn_Line2.Checked = false;
                btn_Line3.Checked = false;
                btn_Line4.Checked = false;
                btn_Line5.Checked = false;
            }
            if (item == btn_Line2)
            {
                thick = 2;
                isSolid = true;

                btn_Line1.Checked = false;
                btn_Line2.Checked = true;
                btn_Line3.Checked = false;
                btn_Line4.Checked = false;
                btn_Line5.Checked = false;
            }
            if (item == btn_Line3)
            {
                thick = 3;
                isSolid = true;

                btn_Line1.Checked = false;
                btn_Line2.Checked = false;
                btn_Line3.Checked = true;
                btn_Line4.Checked = false;
                btn_Line5.Checked = false;
            }
            if (item == btn_Line4)
            {
                thick = 4;
                isSolid = true;

                btn_Line1.Checked = false;
                btn_Line2.Checked = false;
                btn_Line3.Checked = false;
                btn_Line4.Checked = true;
                btn_Line5.Checked = false;
            }
            if (item == btn_Line5)
            {
                thick = 5;
                isSolid = true;

                btn_Line1.Checked = false;
                btn_Line2.Checked = false;
                btn_Line3.Checked = false;
                btn_Line4.Checked = false;
                btn_Line5.Checked = true;
            }

        }

        private void btn_Color1_Click(object sender, EventArgs e)
        {
            ToolStripButton btn = (ToolStripButton)sender; 
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                btn.BackColor = colorDialog1.Color;
            }
        }

        private void panel1_MouseWheel(object sender, MouseEventArgs e)
        {
            float prevZoom = zoom;
            if (e.Delta/120 > 0)
            {//확대
                if(zoom < 5)
                {
                    zoom += (float)0.1;
                }
                else
                {
                    zoom = 5;
                }
            }
            else
            {//축소
                if (zoom >0.5)
                {
                    zoom -= (float)0.1;
                }
                else
                {
                    zoom = (float)0.5;
                }

            }
            int w = (int)(panel1.Width * zoom);
            int h = (int)(panel1.Height * zoom);

            wheelPoint.X = e.X;
            wheelPoint.Y = e.Y;

            scroll.X = (int)((scroll.X + e.X) * (zoom / prevZoom)) - e.X;
            scroll.Y = (int)((scroll.Y + e.Y) * (zoom / prevZoom)) - e.Y;


            panel1.Invalidate(true);
            panel1.Update();
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            start.X = e.X;
            start.Y = e.Y;

        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            if (start.X == 0 && start.Y == 0)
            {
                return;
            }
            isDrag = true;
            
            finish.X = e.X;
            finish.Y = e.Y;

            Point newStart = new Point((int)((scroll.X + start.X) / zoom), (int)((scroll.Y + start.Y) / zoom));
            Point newFinish = new Point((int)((scroll.X + finish.X) / zoom), (int)((scroll.Y + finish.Y) / zoom));

            if (hand)
            {
                int moveX = finish.X - start.X;
                int moveY = finish.Y - start.Y;
                scroll.X = scroll.X - moveX;
                scroll.Y = scroll.Y - moveY;
                start = finish;
            }
            else if (circle)
            {
                mycircle[ncircle].setRectC(newStart, newFinish, thick, isSolid, btn_Color1.BackColor, btn_Color2.BackColor, btn_Fill.Checked, z);
            }
            else if (line)
            {
                mylines[nline].setPoint(newStart, newFinish, thick, isSolid, btn_Color1.BackColor, z);
            }
            else if (rect)
            {
                myrect[nrect].setRect(newStart, newFinish, thick, isSolid, btn_Color1.BackColor, btn_Color2.BackColor, btn_Fill.Checked, z);
            }
            else if (pencil)
            {
                mypencil[npencil].seAttr(thick, isSolid, btn_Color1.BackColor, z);
                mypencil[npencil].pushPoint(newStart, newFinish);
                
                start = finish;
            }
            panel1.Invalidate(true);
            panel1.Update();
        }

        private void panel1_MouseUp(object sender, MouseEventArgs e)
        {
            if(start != e.Location || isDrag)
            {
                if (circle == true)
                {
                    pShape temp = new pShape();
                    MyCircle ctemp = mycircle[ncircle];
                    Rectangle rtemp = mycircle[ncircle].getRectC();
                    temp.setShape(rtemp.X, rtemp.Y, rtemp.Width, rtemp.Height, ctemp.getThick(), ctemp.getSolid(), ctemp.getLineColor().ToArgb(), ctemp.getFillColor().ToArgb(), ctemp.getFill(), ctemp.getz());
                    temp.Type = (int)PacketType.원;
                    PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
                    Send();

                    ncircle++;
                }
                if (line == true)
                {
                    pShape temp = new pShape();
                    MyLines ctemp = mylines[nline];
                    temp.setShape(ctemp.GetPoint1().X, ctemp.GetPoint1().Y, ctemp.GetPoint2().X, ctemp.GetPoint2().Y, ctemp.getThick(), ctemp.getSolid(), ctemp.getColor().ToArgb(), 0, false, ctemp.getz());
                    temp.Type = (int)PacketType.선;
                    PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
                    Send();
                    nline++;

                }
                if (pencil == true)
                {
                    pShape temp = new pShape();
                    MyPencil ctemp = mypencil[npencil];
                    temp.setShape(0, 0, 0, 0, ctemp.getThick(), true, ctemp.getColor().ToArgb(), 0, false, ctemp.getz());
                    for (int i = 0; i < ctemp.GetPoint1().Count; i++)
                    {
                        temp.pushPoint(ctemp.GetPoint1()[i].X, ctemp.GetPoint1()[i].Y, ctemp.GetPoint2()[i].X, ctemp.GetPoint2()[i].Y);
                    }
                    temp.Type = (int)PacketType.연필;
                    PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
                    Send();
                    npencil++;

                }
                if (rect == true)
                {
                    pShape temp = new pShape();
                    MyRect ctemp = myrect[nrect];
                    Rectangle rtemp = myrect[nrect].getRect();
                    temp.setShape(rtemp.X, rtemp.Y, rtemp.Width, rtemp.Height, ctemp.getThick(), ctemp.getSolid(), ctemp.getLineColor().ToArgb(), ctemp.getFillColor().ToArgb(), ctemp.getFill(), ctemp.getz());
                    temp.Type = (int)PacketType.사각;
                    PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
                    Send();
                    nrect++;
                }
                z++;
            }

            isDrag = false;
            start.X = 0;
            start.Y = 0;
            finish.X = 0;
            finish.Y = 0;

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            int indexC = 0, indexL = 0, indexP = 0, indexR = 0;
            for (i = 0; i <= z; i++)
            {
                if (indexC <= ncircle && mycircle[indexC].getz() == i)
                {
                    Rectangle temp = mycircle[indexC].getRectC();
                    temp.X = (int)(temp.X * zoom) - scroll.X;
                    temp.Y = (int)(temp.Y * zoom) - scroll.Y;
                    temp.Width = (int)(temp.Width * zoom);
                    temp.Height = (int)(temp.Height * zoom);

                    pen.Color = mycircle[indexC].getLineColor();
                    pen.Width = mycircle[indexC].getThick();
                    pen.DashStyle = DashStyle.Solid;
                    brush.Color = mycircle[indexC].getFillColor();


                    e.Graphics.DrawEllipse(pen, temp);
                    if (mycircle[indexC].getFill())
                    {
                        e.Graphics.FillEllipse(brush, temp);
                    }
                    indexC++;
                }
                if (indexL <= nline && mylines[indexL].getz() == i)
                {
                    Point temp1 = new Point((int)(mylines[indexL].GetPoint1().X * zoom) - scroll.X, (int)(mylines[indexL].GetPoint1().Y * zoom) - scroll.Y);
                    Point temp2 = new Point((int)(mylines[indexL].GetPoint2().X * zoom) - scroll.X, (int)(mylines[indexL].GetPoint2().Y * zoom) - scroll.Y);
                    pen.Color = mylines[indexL].getColor();
                    pen.Width = mylines[indexL].getThick();
                    pen.DashStyle = DashStyle.Solid;

                    e.Graphics.DrawLine(pen, temp1, temp2);
                    indexL++;
                }
                if (indexP <= npencil && mypencil[indexP].getz() == i)
                {
                    List<Point> temp1 = mypencil[indexP].GetPoint1();
                    List<Point> temp2 = mypencil[indexP].GetPoint2();
                    Point newStart;
                    Point newFinish;

                    pen.Color = mypencil[indexP].getColor();
                    pen.Width = mypencil[indexP].getThick();
                    pen.DashStyle = DashStyle.Solid;

                    for (int j = 0; j < temp1.Count; j++)
                    {
                        newStart = new Point((int)(temp1[j].X * zoom) - scroll.X, (int)(temp1[j].Y * zoom) - scroll.Y);
                        newFinish = new Point((int)(temp2[j].X * zoom) - scroll.X, (int)(temp2[j].Y * zoom) - scroll.Y);
                        e.Graphics.DrawLine(pen, newStart, newFinish);
                    }
                    indexP++;
                }
                if (indexR <= nrect && myrect[indexR].getz() == i)
                {
                    Rectangle temp = myrect[indexR].getRect();
                    temp.X = (int)(temp.X * zoom) - scroll.X;
                    temp.Y = (int)(temp.Y * zoom) - scroll.Y;
                    temp.Width = (int)(temp.Width * zoom);
                    temp.Height = (int)(temp.Height * zoom);

                    pen.Color = myrect[indexR].getLineColor();
                    pen.Width = myrect[indexR].getThick();
                    pen.DashStyle = DashStyle.Solid;
                    brush.Color = myrect[indexR].getFillColor();

                    e.Graphics.DrawRectangle(pen, temp);
                    if (myrect[indexR].getFill())
                    {
                        e.Graphics.FillRectangle(brush, temp);
                    }
                    indexR++;
                }
            }
            pen.Width = thick;
            pen.DashStyle = DashStyle.Solid;

        }

        private void btn_Fill_Click(object sender, EventArgs e)
        {
            if (btn_Fill.Checked)
            {
                btn_Fill.Checked = false;
            }
            else
            {
                btn_Fill.Checked = true;
            }
        }

        private void panel1_Click(object sender, EventArgs e)
        {
            panel1.Focus();
        }

        private void clientForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Disconnect();
            parent.btn_Connect.Text = "접속";
            parent.btn_Connect.ForeColor = Color.Black;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string tts = textBox1.Text.Trim();
            if(string.IsNullOrEmpty(tts))
            {
                MessageBox.Show("텍스트가 입력되지 않았습니다!");
                textBox1.Focus();
                return;
            }

            PacketC temp = new PacketC();
            temp.Type = (int)PacketType.채팅;
            temp.name = nickname;
            temp.tts = tts;
            textBox2.AppendText(temp.name + ": " + temp.tts + "\r\n");
            PacketC.Serialize(temp).CopyTo(sendBuffer, 0);
            Send();

            textBox1.Text = "";
            textBox1.Focus();

        }
    }
}
