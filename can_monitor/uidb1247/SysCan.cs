using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace uidb1247
{
    public struct Can_Data
    {
        public int id;
        public int len;
        public byte[] data;
        public ulong period;
    }

    class SysCan: ComSerial
    {
        private string version;
        private bool flag_monitor;
        private int monitor_delay;
        private bool flag_overflow;

        private Can_Data can_tmp;
        private readonly int can_buf_len = 20;
        private Can_Data[] can_buf;
        
        public SysCan()
        {
            version = null;
            flag_monitor = false;
            flag_overflow = false;

            can_tmp.data = new byte[8];
            can_buf = new Can_Data[can_buf_len];
            for (int i=0;i< can_buf_len;i++)
            {
                can_buf[i].id = -1;
                can_buf[i].data = new byte[8];
            }
        }

        public void Clear()
        {
            flag_overflow = false;
            for (int i = 0; i < can_buf_len; i++)
            {
                can_buf[i].id = -1;
            }
        }

        public int GetCanBufLen()
        {
            return can_buf_len;
        }

        public Can_Data GetCanBufData(int index)
        {
            return can_buf[index];
        }

        public string GetVersion()
        {
            return version;
        }

        public bool GetOverFlow()
        {
            return flag_overflow;
        }

        private void SendCmd(string cmd)
        {
            string info = " can " + cmd + " &";
            base.WriteString(info);
            //Console.WriteLine("c -> " + info);
        }

        public string EventReadString()
        {
            string info = base.ReadStringEvent();
            if (info != null)
            {
                //Console.WriteLine("c <- " + info);
                if (!flag_exist && info.Contains("CAN_VER"))
                {
                    flag_exist = true;
                    version = info;
                    CanDelTx();
                    CanUnMonitor();
                }
                
                //if (info.Contains("can_rx"))
                if (flag_monitor && info.Contains("can_ss"))
                {
                    info = info.Substring(7);
                    process_can_data(getRxData(info));
                }
                
            }
            return info;
        }

        public void EventDetectDev()
        {
            if (flag_exist)
            {
                if (!flag_monitor)
                {
                    monitor_delay++;
                    if (monitor_delay > 1)
                    {
                        flag_monitor = true;
                        CanMonitor();
                    }
                }
            }
            else
            {
                flag_monitor = false;
                monitor_delay = 0;
                if (Is_Open())
                {
                    SendCmd("??? ");
                }
            }
        }

        private void process_can_data(Can_Data can)
        {
            for (int i=0;i< can_buf_len;i++)
            {
                if (can_buf[i].id < 0)
                {
                    can_buf[i].id = can.id;
                    can_buf[i].period = can.period;
                    can_buf[i].len = can.len;
                    for (int j = 0; j < can.len; j++)
                    {
                        can_buf[i].data[j] = can.data[j];
                    }
                    return;
                }
                else if (can_buf[i].id == can.id)
                {
                    can_buf[i].id = can.id;
                    can_buf[i].period += can.period;
                    can_buf[i].len = can.len;
                    for (int j = 0; j < can.len; j++)
                    {
                        can_buf[i].data[j] = can.data[j];
                    }
                    return;
                }
            }
            flag_overflow = true;
        }

        public void can_list_sort(bool fid)
        {
            int start;
            int index;
            int id;
            ulong tm;

            Can_Data tmp = new Can_Data();
            tmp.data = new byte[8];

            for (start=0; start< can_buf_len;start++)
            {
                index = start;
                id = can_buf[index].id;
                tm = can_buf[index].period;
                if (id <= 0)
                {
                    break;
                }
                for (int i = index + 1; i < can_buf_len; i++)
                {
                    if (can_buf[i].id <= 0)
                    {
                        break;
                    }
                    if (fid)
                    {
                        if (can_buf[i].id < id)
                        {
                            id = can_buf[i].id;
                            index = i;
                        }
                    }
                    else
                    {
                        if (can_buf[i].period > tm /10 *12)
                        {
                            id = can_buf[i].id;
                            tm = can_buf[i].period;
                            index = i;
                        }
                        else if (can_buf[i].period > tm /10 * 8)
                        {
                            if (can_buf[i].id < id)
                            {
                                id = can_buf[i].id;
                                tm = can_buf[i].period;
                                index = i;
                            }
                        }
                    }
                }
                if (index != start)
                {
                    Can_Data_Copy(can_buf[start], ref tmp);
                    Can_Data_Copy(can_buf[index], ref can_buf[start]);
                    Can_Data_Copy(tmp, ref can_buf[index]);
                }
            }
        }

        private void Can_Data_Copy(Can_Data src,ref Can_Data des)
        {
            des.id = src.id;
            des.period = src.period;
            des.len = src.len;
            for (int i=0;i< src.len;i++)
            {
                des.data[i] = src.data[i];
            }
        }

        //----------------------------  
        private void CanDelTx()
        {
            SendCmd("del tx all");
        }

        private void CanUnMonitor()
        {
            SendCmd("unmonitor all");
        }

        private void CanMonitor()
        {
            SendCmd("monitor ss");
        }
        

        //----------------------------   
        public static string CanDataToTxCmd(Can_Data can)
        {
            StringBuilder buffer = new StringBuilder();
            buffer.Append("msg ");
            buffer.Append(can.id.ToString("X"));
            buffer.Append(",");
            buffer.Append(can.period.ToString("D"));
            buffer.Append(",");
            buffer.Append(can.len.ToString("D"));
            buffer.Append(",");
            for (int i = 0; i < can.len; i++)
            {
                buffer.Append(can.data[i].ToString("X"));
                buffer.Append(",");
            }
            return buffer.ToString();
        }
        
        public Can_Data getRxData(string info)
        {
            Can_Data tmp = can_tmp;
            string[] arr = info.Split(',');
            tmp.id = StrFormat.AsciiHexToInt(arr[0]);
            tmp.period = StrFormat.AsciiHexToULong(arr[1]);
            tmp.len = StrFormat.AsciiHexToInt(arr[2]);
            for (int i=0;i< tmp.len;i++)
            {
                tmp.data[i] = (byte)StrFormat.AsciiHexToInt(arr[3 + i]);
            }
            return tmp;
        }
    }
}




