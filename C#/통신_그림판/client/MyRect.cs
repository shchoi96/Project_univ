using System;
using System.Collections;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace client
{
    public class MyRect
    {
        private Rectangle rect;
        private int thick;
        private bool isSolid;
        private Color color;

        public MyRect()
        {
            rect = new Rectangle();
            thick = 1;
            isSolid = true;
            Color color = Color.Black;
        }

        public void setRect(Point start, Point finish, int thick, bool isSolid, Color color)
        {
            rect.X = Math.Min(start.X, finish.X);
            rect.Y = Math.Min(start.Y, finish.Y);
            rect.Width = Math.Abs(start.X - finish.X);
            rect.Height = Math.Abs(start.Y - finish.Y);
            this.thick = thick;
            this.isSolid = isSolid;
            this.color = color;
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

        public Color getColor()
        {
            return color;
        }
    }
}
