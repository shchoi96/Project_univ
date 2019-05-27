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
    public partial class Form1 : Form
    {
        private bool hand;
        private bool circle;
        private bool line;
        private Point start;
        private Point finish;
        private Pen pen;
        private bool pencil;
        private bool rect;

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
        



        public Form1()
        {
            InitializeComponent();

            SetupVar();
        }

        private void SetupVar()
        {
            i = 0;
            thick = 1;
            isSolid = true;
            circle = false;
            line = false;
            pencil = false;
            rect = false;
            start = new Point(0, 0);
            finish = new Point(0, 0);
            pen = new Pen(Color.Black);
            mycircle = new MyCircle[100];
            mylines = new MyLines[100];
            mypencil = new MyPencil[100];
            myrect = new MyRect[100];
            ncircle = 0;
            nline = 0;
            npencil = 0;
            nrect = 0;

            SetupMine();
        }

        private void SetupMine()
        {
            for(i = 0; i < 100; i++)
            {
                mycircle[i] = new MyCircle();
                mylines[i] = new MyLines();
                mypencil[i] = new MyPencil();
                myrect[i] = new MyRect();
            }
        }
        
        private void btn_Shape_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = (ToolStripMenuItem)sender;
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
                btn_Line.Checked = true;
                btn_Pencil.Checked = false;
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
            //btn_tsdd 눌렀을 때 이벤트 처리

        }

        private void btn_Line1_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = (ToolStripMenuItem)sender;

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

            finish.X = e.X;
            finish.Y = e.Y;

            if (hand)
            {

            }
            else if (circle)
            {
                mycircle[ncircle].setRectC(start, finish, thick, isSolid, btn_Color1.BackColor);
            }
            else if (line)
            {
                mylines[nline].setPoint(start, finish, thick, isSolid, btn_Color1.BackColor);
            }
            else if (rect)
            {
                myrect[nrect].setRect(start, finish, thick, isSolid, btn_Color1.BackColor);
            }
            else if (pencil)
            {

            }

            panel1.Invalidate(true);
            panel1.Update();
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            
            for (i = 0; i <= ncircle; i++)
            {
                pen.Color = mycircle[i].getColor();
                pen.Width = mycircle[i].getThick();
                pen.DashStyle = DashStyle.Solid;

                e.Graphics.DrawEllipse(pen, mycircle[i].getRectC());
            }
            for (i = 0; i <= nline; i++)
            {
                pen.Color = mylines[i].getColor();
                pen.Width = mylines[i].getThick();
                pen.DashStyle = DashStyle.Solid;

                e.Graphics.DrawLine(pen, mylines[i].GetPoint1(), mylines[i].GetPoint2());
            }
            for (i = 0; i <= nrect; i++)
            {
                pen.Color = myrect[i].getColor();
                pen.Width = myrect[i].getThick();
                pen.DashStyle = DashStyle.Solid;

                e.Graphics.DrawRectangle(pen, myrect[i].getRect());
                e.Graphics.FillRectangle(pen, myrect[i].getRect());
            }
            

            pen.Width = thick;
            pen.DashStyle = DashStyle.Solid;

        }

        private void panel1_MouseUp(object sender, MouseEventArgs e)
        {

            if (circle == true)
                ncircle++;
            if (line == true)
                nline++;
            if (pencil == true)
                npencil++;
            if (rect == true)
                nrect++;

            start.X = 0;
            start.Y = 0;
            finish.X = 0;
            finish.Y = 0;
        }
    }
}
