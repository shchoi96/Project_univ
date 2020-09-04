using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Drawing;

namespace Packet
{
    public enum PacketType
    {
        초기화 = 0,
        원,
        선,
        연필,
        사각,
        채팅,
        종료
    }
    [Serializable]
    public class PacketC
    {
        public int Type;
        public string name;
        public string tts;

        public PacketC()
        {
            this.Type = 0;
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

    [Serializable]
    public class pShape : PacketC
    {
        public int[] rectC;
        public int thick;
        public bool isSolid;
        public int line_color;//argb 이용 toArgb fromArgb
        public int fill_color;
        public bool isFill;
        public int z;
        public List<int> startsX;
        public List<int> startsY;
        public List<int> finishesX;
        public List<int> finishesY;



        public pShape()
        {
            rectC = new int[4];
            thick = 1;
            isSolid = true;
            isFill = false;
            line_color = 0;
            fill_color = 0;
            isFill = false;
            z = -1;
            startsX = new List<int>();
            startsY = new List<int>();
            finishesX = new List<int>();
            finishesY = new List<int>();
        }

        public void setShape(int x1, int y1, int x2, int y2, int thick, bool isSolid, int line_color, int fill_color, bool isFill, int z)
        {
            rectC[0] = x1;
            rectC[1] = y1;
            rectC[2] = x2;
            rectC[3] = y2;
            this.thick = thick;
            this.isSolid = isSolid;
            this.line_color = line_color;
            this.fill_color = fill_color;
            this.isFill = isFill;
            this.z = z;
        }

        public void pushPoint(int sx, int sy, int fx, int fy)
        {
            startsX.Add(sx);
            startsY.Add(sy);
            finishesX.Add(fx);
            finishesY.Add(fy);
        }
    }
}
