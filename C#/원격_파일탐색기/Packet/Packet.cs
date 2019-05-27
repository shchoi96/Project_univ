using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace Packet
{
    public enum PacketType
    {
        초기화 = 0,
        Select,
        Expand,
        moreInfo,
        파일전송,
        종료
    }


    [Serializable]
    public class PacketC
    {
        public int Length;
        public int Type;

        public PacketC()
        {
            this.Length = 0;
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
            foreach(byte b in bt)
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
    public class pDirInfo : PacketC
    {
        public string name;
        public string path;
        public string ext;
        public string size;
        public string createDate;
        public string modifyDate;
        public string accessDate;
        
        public pDirInfo()
        {
            name = null;
            path = null;
            ext = null;
            size = null;
            createDate = null;
            modifyDate = null;
            accessDate = null;
            
        }

    }
}
