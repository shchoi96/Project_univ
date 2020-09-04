using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Drawing;

namespace server
{
    [Serializable]
    public class MyLines
    {
        private Point[] point = new Point[2];
        private int thick;
        private bool isSolid;
        private Color color;
        public int z;

        public MyLines()
        {
            point[0] = new Point();
            point[1] = new Point();
            thick = 1;
            isSolid = true;
            color = Color.Black;
            z = -1;
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
        public static byte[] Serialize(Object o)
        {
            MemoryStream ms = new MemoryStream(1024 * 4);
            BinaryFormatter bf = new BinaryFormatter();
            bf.Serialize(ms, o);
            return ms.ToArray();
        }

        public static Object Deserialize(byte[] bt)
        {
            MemoryStream ms = new MemoryStream(1024 * 4);
            foreach (byte b in bt)
            {
                ms.WriteByte(b);
            }

            ms.Position = 0;
            BinaryFormatter bf = new BinaryFormatter();
            Object obj = bf.Deserialize(ms);
            ms.Close();
            return obj;
        }
    }
}
