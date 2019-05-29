using System;
using System.Collections;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace client
{
    public class MyPencil
    {
        private int thick;
        private bool isSolid;
        private Color color;
        private List<Point> starts;
        private List<Point> finishes;
        private int z;

        public MyPencil()
        {
            starts = new List<Point>();
            finishes = new List<Point>();
            thick = 1;
            isSolid = true;
            color = Color.Black;
            z = 0;
        }

        public void seAttr(int thick, bool isSolid, Color color, int z)
        {
            this.thick = thick;
            this.isSolid = isSolid;
            this.color = color;
            this.z = z;
        }

        public void pushPoint(Point start, Point finish)
        {
            starts.Add(start);
            finishes.Add(finish);
        }

        public List<Point> GetPoint1()
        {
            return starts;
        }

        public List<Point> GetPoint2()
        {
            return finishes;
        }

        public int getThick()
        {
            return thick;
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
