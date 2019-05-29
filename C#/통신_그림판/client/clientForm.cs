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

namespace client
{
   
    public partial class clientForm : Form
    {
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

        public clientForm()
        {
            InitializeComponent();
            g = panel1.CreateGraphics();
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
                    ncircle++;
                if (line == true)
                    nline++;
                if (pencil == true)
                    npencil++;
                if (rect == true)
                    nrect++;
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
    }
}
