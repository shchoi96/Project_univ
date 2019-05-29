using System;
using System.Collections;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace client
{
    public class MyLines
    {
        private Point[] point = new Point[2];
        private int thick;
        private bool isSolid;
        private Color color;
        private int z;

        public MyLines()
        {
            point[0] = new Point();
            point[1] = new Point();
            thick = 1;
            isSolid = true;
            color = Color.Black;
            z = 0;
        }

        public void setPoint(Point start, Point finish, int thick, bool isSolid, Color color, int z)
        {
            point[0] = start;
            point[1] = finish;
            this.thick = thick;
            this.isSolid = isSolid;
            this.color = color;
            this.z = z;
        }

        public Point GetPoint1()
        {
            return point[0];
        }

        public Point GetPoint2()
        {
            return point[1];
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

        public int getz()
        {
            return z;
        }
    }
}
