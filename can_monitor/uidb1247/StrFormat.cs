using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace uidb1247
{
    class StrFormat
    {
        public static string Ascii_ToString(byte[] buf)
        {
            string str = System.Text.Encoding.ASCII.GetString(buf);
            return str;
        }

        public static byte[] String_ToAscii(string str)
        {
            byte[] buf = System.Text.Encoding.ASCII.GetBytes(str);
            return buf;
        }

        public static bool IsNumberic(string oText)
        {
            try
            {
                int var1 = Convert.ToInt32(oText);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static int GetNumberic(string str)
        {
            //return Convert.ToInt32(str);
            //return int.Parse(str);
            return AsciiDecToInt(str);
        }

        private static int Ascii16to10(string str)
        {
            int val = 0;
            for (int i=0;i<str.Length;i++)
            {
                char c = str[i];
                int tmp = 0;
                if (c >= '0' && c <= '9')
                    tmp = c - '0';
                else if (c >= 'a' && c <= 'f')
                    tmp = c - 'a' + 10;
                else if (c >= 'A' && c <= 'F')
                    tmp = c - 'A' + 10;
                else
                    break;
                val *= 16;
                val += tmp;
            }
            return val;
        }

        private static ulong Ascii16to10_ulong(string str)
        {
            ulong val = 0;
            for (int i = 0; i < str.Length; i++)
            {
                char c = str[i];
                int tmp = 0;
                if (c >= '0' && c <= '9')
                    tmp = c - '0';
                else if (c >= 'a' && c <= 'f')
                    tmp = c - 'a' + 10;
                else if (c >= 'A' && c <= 'F')
                    tmp = c - 'A' + 10;
                else
                    break;
                val *= 16;
                val += (ulong)tmp;
            }
            return val;
        }

        public static int AsciiDecToInt(string str)
        {
            int val = 0;
            for (int i = 0; i < str.Length; i++)
            {
                char c = str[i];
                int tmp = 0;
                if (c >= '0' && c <= '9')
                    tmp = c - '0';
                else
                    break;
                val *= 10;
                val += tmp;
            }
            return val;
        }

        public static int AsciiHexToInt(string str)
        {
            string info = str;
            int index = info.IndexOf("0x");
            if (index >= 0)
            {
                info = info.Substring(index + 2);
            }
            return Ascii16to10(info);
        }

        public static ulong AsciiHexToULong(string str)
        {
            string info = str;
            int index = info.IndexOf("0x");
            if (index >= 0)
            {
                info = info.Substring(index + 2);
            }
            return Ascii16to10_ulong(info);
        }

        public static int GetCanId(string[] arr)
        {
            return AsciiHexToInt(arr[0].Trim());
        }

        public static int GetCanCyc(string[] arr)
        {
            return AsciiHexToInt(arr[1].Trim());
        }

        public static int GetCanLen(string[] arr)
        {
            if (arr.Length > 2)
            {
                return AsciiHexToInt(arr[2].Trim());
            }
            return 0;
        }

        public static byte[] GetCanData(string[] arr, int len)
        {
            if (arr.Length >= 3 + len)
            {
                byte[] data = new byte[len];
                for (int i = 0; i < len; i++)
                {
                    data[i] = (byte)StrFormat.AsciiHexToInt(arr[3 + i].Trim());
                }
                return data;
            }
            return null;
        }

        public static string GetCanString(int id,int cyc,byte[] data)
        {
            StringBuilder str = new StringBuilder();
            str.Clear();
            str.Append(id.ToString("X"));
            str.Append(",");
            str.Append(cyc.ToString("D"));
            str.Append(",");
            str.Append(data.Length.ToString("D"));
            str.Append(",");
            for (int i=0;i< data.Length; i++)
            {
                str.Append(data[i].ToString("X"));
                str.Append(",");
            }
            return str.ToString();
        }

        public static int ByteToInt(byte[] buf,int index,int num)
        {
            return (int)ByteToLong(buf, index, num);
        }

        public static long ByteToLong(byte[] buf, int index, int num)
        {
            long val = 0;
            int start_byte = index / 8;
            int start_bit = index % 8;
            int end_byte = (index + num) / 8;
            int end_bit = (index + num) % 8;
            int tmp;
            if (start_byte != end_byte)
            {
                for (int i = start_byte; i < end_byte; i++)
                {
                    val <<= 8;
                    tmp = ((int)buf[i] & 0xff);
                    tmp >>= start_bit;
                    val += tmp;
                    start_bit = 0;
                }
                if (end_bit > 0)
                {
                    val <<= end_bit;
                    tmp = ((int)buf[end_byte + 1] & 0xff);
                    tmp &= (1 << end_bit) - 1;
                    val += tmp;
                }
            }
            else
            {
                tmp = ((int)buf[start_byte] & 0xff);
                tmp &= (1 << end_bit) - 1;
                tmp >>= start_bit;
                val += tmp;
            }
            return val;
        }

        public static System.DateTime GetSystemTime(string[] sArray, int index)
        {
            System.DateTime time = new DateTime(
                StrFormat.Ascii16to10(sArray[index + 3]) + 2000,
                StrFormat.Ascii16to10(sArray[index + 4]),
                StrFormat.Ascii16to10(sArray[index + 5]),
                StrFormat.Ascii16to10(sArray[index + 0]),
                StrFormat.Ascii16to10(sArray[index + 1]),
                StrFormat.Ascii16to10(sArray[index + 2]));
            return time;
        }
        
    }
}



