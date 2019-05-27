using System;
using System.Collections;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace client
{
    public class MyCircle
    {
        private Rectangle rectC;
        private int thick;
        private bool isSolid;
        private Color color;

        public MyCircle()
        {
            rectC = new Rectangle();
            thick = 1;
            isSolid = true;
            color = Color.Black;
        }

        public void setRectC(Point start, Point finish, int thick, bool isSolid, Color color)
        {
            rectC.X = Math.Min(start.X, finish.X);
            rectC.Y = Math.Min(start.Y, finish.Y);
            rectC.Width = Math.Abs(start.X - finish.X);
            rectC.Height = Math.Abs(start.Y - finish.Y);
            this.thick = thick;
            this.isSolid = isSolid;
            this.color = color;
        }

        public Rectangle getRectC()
        {
            return rectC;
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
