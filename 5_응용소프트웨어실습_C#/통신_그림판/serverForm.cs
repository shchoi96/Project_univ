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
using System.Drawing.Drawing2D;
using System.Runtime.Serialization.Formatters.Binary;


namespace server
{
    public partial class Form_server : Form
    {
        //그림그리기
        public bool hand;
        public bool circle;
        public bool line;
        public Point start;
        public Point finish;
        public Pen pen;
        public SolidBrush brush;
        public bool pencil;
        public bool rect;

        public int z;
        public int ncircle;
        public int nline;
        public int npencil;
        public int nrect;
        public int i;
        public int thick;
        public bool isSolid;
        public MyCircle[] mycircle;
        public MyLines[] mylines;
        public MyPencil[] mypencil;
        public MyRect[] myrect;

        public Graphics g;
        public Graphics tempG;
        public float zoom;
        public Point wheelPoint;
        public Point scroll;
        public bool isDrag;
        //저장
        private string fileName;
        public serverOpen parent;

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
        public Form_server()
        {          
            InitializeComponent();
            SetupVar();

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

        public void panel1_Paint(object sender, PaintEventArgs e)
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

        private void Form_server_FormClosing(object sender, FormClosingEventArgs e)
        {
            parent.ServerStop();
            parent.button1.Text = "서버열기";
            parent.button1.ForeColor = Color.Black;
        }
    }
}
