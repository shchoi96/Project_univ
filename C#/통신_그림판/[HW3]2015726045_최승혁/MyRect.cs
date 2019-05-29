﻿using System;
using System.Collections;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace server
{
    public class MyRect
    {
        private Rectangle rect;
        private int thick;
        private bool isSolid;
        private Color line_color;
        private Color fill_color;
        private bool isFill;
        private int z;

        public MyRect()
        {
            rect = new Rectangle();
            thick = 1;
            isSolid = true;
            isFill = false;
            line_color = Color.Black;
            fill_color = Color.White;
            z = -1;
        }

        public void setRect(Point start, Point finish, int thick, bool isSolid, Color line_color, Color fill_color, bool isFill, int z)
        {
            rect.X = Math.Min(start.X, finish.X);
            rect.Y = Math.Min(start.Y, finish.Y);
            rect.Width = Math.Abs(start.X - finish.X);
            rect.Height = Math.Abs(start.Y - finish.Y);
            this.thick = thick;
            this.isSolid = isSolid;
            this.line_color = line_color;
            this.fill_color = fill_color;
            this.isFill = isFill;
            this.z = z;
        }

        public Rectangle getRect()
        {
            return rect;
        }

        public int getThick()
        {
            return thick;
        }

        public bool getSolid()
        {
            return isSolid;
        }

        public Color getLineColor()
        {
            return line_color;
        }

        public Color getFillColor()
        {
            return fill_color;
        }

        public bool getFill()
        {
            return isFill;
        }

        public int getz()
        {
            return z;
        }
    }
}
