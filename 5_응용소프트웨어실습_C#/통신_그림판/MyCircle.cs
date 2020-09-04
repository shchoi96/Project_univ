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
    public class MyCircle
    {
        private Rectangle rectC;
        private int thick;
        private bool isSolid;
        private Color line_color;
        private Color fill_color;
        private bool isFill;
        public int z;

        public MyCircle()
        {
            rectC = new Rectangle();
            thick = 1;
            isSolid = true;
            isFill = false;
            line_color = Color.Black;
            fill_color = Color.White;
            z = -1;
        }

        public void setRectC(Point start, Point finish, int thick, bool isSolid, Color line_color, Color fill_color, bool isFill, int z)
        {
            rectC.X = Math.Min(start.X, finish.X);
            rectC.Y = Math.Min(start.Y, finish.Y);
            rectC.Width = Math.Abs(start.X - finish.X);
            rectC.Height = Math.Abs(start.Y - finish.Y);
            this.thick = thick;
            this.isSolid = isSolid;
            this.line_color = line_color;
            this.fill_color = fill_color;
            this.isFill = isFill;
            this.z = z;
        }

        public void setRectC(Rectangle rect, int thick, bool isSolid, Color line_color, Color fill_color, bool isFill, int z)
        {
            rectC = rect;
            this.thick = thick;
            this.isSolid = isSolid;
            this.line_color = line_color;
            this.fill_color = fill_color;
            this.isFill = isFill;
            this.z = z;
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
