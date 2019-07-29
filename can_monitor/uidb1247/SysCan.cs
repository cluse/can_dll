using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace uidb1247
{
    public enum MSG_SORT
    {
        NONE = 0,
        ID,
        PERIOD,
        MAX
    }

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
        
        private MSG_SORT flag_sort_type;
        private bool flag_rx_msg;

        private Can_Data can_tmp;
        private readonly int can_buf_len = 20;
        private Can_Data[] can_buf;
        
        public SysCan()
        {
            version = null;
            flag_monitor = false;
            flag_overflow = false;
            flag_rx_msg = false;
            flag_sort_type = MSG_SORT.NONE;

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

        public void list_sort(MSG_SORT type)
        {
            flag_sort_type = type;
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
                    flag_rx_msg = true;
                    sort_process();
                }
                
            }
            return info;
        }

        public bool get_rx_msg_flag()
        {
            bool flag = flag_rx_msg;
            flag_rx_msg = false;
            return flag;
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

        private void sort_process()
        {
            if (flag_sort_type == MSG_SORT.NONE)
            {
                return;
            }
            int len = 0;
            for (int i=0; i< GetCanBufLen();i++)
            {
                if (GetCanBufData(i).id > 0)
                    len = i;
            }
            for (int i = 0; i < len; i++)
            {
                for (int j = 0; j < len; j++)
                {
                    if (flag_sort_type == MSG_SORT.ID)
                    {
                        if (can_buf[j].id > can_buf[j + 1].id)
                            Can_Data_Change(ref can_buf[j], ref can_buf[j + 1]);
                    }
                    else if (flag_sort_type == MSG_SORT.PERIOD)
                    {
                        if (can_buf[j + 1].period > can_buf[j].period * 1.2)
                            Can_Data_Change(ref can_buf[j], ref can_buf[j + 1]);
                        else if (can_buf[j + 1].period > can_buf[j].period * 0.8)
                        {
                            if (can_buf[j].id > can_buf[j + 1].id)
                                Can_Data_Change(ref can_buf[j], ref can_buf[j + 1]);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            flag_sort_type = MSG_SORT.NONE;
        }

        private void Can_Data_Change(ref Can_Data c1, ref Can_Data c2)
        {
            Can_Data tmp = new Can_Data();
            tmp.data = new byte[8];
            Can_Data_Copy(c1, ref tmp);
            Can_Data_Copy(c2, ref c1);
            Can_Data_Copy(tmp, ref c2);
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
        private string CanDataToTxCmd(Can_Data can)
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

        private Can_Data getRxData(string info)
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




