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
    public class MyPencil
    {
        private int thick;
        private bool isSolid;
        private Color color;
        private List<Point> starts;
        private List<Point> finishes;
        public int z;

        public MyPencil()
        {
            starts = new List<Point>();
            finishes = new List<Point>();
            thick = 1;
            isSolid = true;
            color = Color.Black;
            z = -1;
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
