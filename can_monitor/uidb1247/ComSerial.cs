using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace uidb1247
{

    class ComSerial
    {
        private SerialPort port = null;
        private List<string> list_com_str = new List<string>();

        public bool flag_exist;

        public ComSerial()
        {
            port = new SerialPort();
            flag_exist = false;
        }

        public string[] Get_PortList()
        {
            return SerialPort.GetPortNames();
        }

        public void Open(string str_port, string str_Speed= "115200")
        {
            list_com_str.Clear();
            port.PortName = str_port;
            port.BaudRate = Convert.ToInt32(str_Speed);
            port.DataBits = 8;
            port.ReadTimeout = 10;
            port.StopBits = StopBits.One;
            port.Parity = Parity.None;
            try
            {
                port.Open();
            }
            catch (Exception ee)
            {
                MessageBox.Show(ee.ToString(), "Error Open");
            }
        }

        public void Close()
        {
            if (port.IsOpen)
            {
                port.Close();
            }
            flag_exist = false;
        }

        public void ClearRxbuf()
        {
            list_com_str.Clear();
            if (port.IsOpen)
            {
                port.DiscardInBuffer();
            }
        }

        private void Write(byte[] buf)
        {
            try
            {
                port.Write(buf, 0, buf.Length);
            }
            catch (Exception ee)
            {
                MessageBox.Show(ee.ToString(), "Error write");
            }
        }
        
        public void WriteString(string str)
        {
            if (port.IsOpen)
            {
                byte[] info = StrFormat.String_ToAscii(str);
                byte[] send = new byte[info.Length + 1];
                Array.Copy(info, send, info.Length);
                send[info.Length] = (byte)'\n';
                Write(send);
            }
        }

        public int Read(byte[] buf)
        {
            int ret = 0;
            try
            {
                int len = buf.Length - 2;
                if (len > port.BytesToRead)
                {
                    len = port.BytesToRead;
                }
                if (len > 0)
                {
                    ret = port.Read(buf, 0, len);
                }
            }
            catch (Exception ee)
            {
                //MessageBox.Show(ee.ToString(), "Error read");
                Console.WriteLine(ee.ToString());
            }
            return ret;
        }

        public bool Is_Open()
        {
            return port.IsOpen;
        }

        public bool Is_Exist()
        {
            return flag_exist;
        }
        
        public string ReadStringEvent()
        {
            string info = null;
            if (port.IsOpen)
            {
                Get_ComRecData();
                if (list_com_str.Count > 0)
                {
                    info = list_com_str[0];
                    list_com_str.RemoveAt(0);
                }
            }
            return info;
        }

        //------------------------------------------
        private readonly int tx_buf_limit = 200;
        private byte[] tx_buf = new byte[300];
        private byte[] tx_tmp = new byte[64];
        private int tx_buf_index = 0;

        private void Get_ComRecData()
        {
            int len = Read(tx_tmp);
            if (len <= 0)
            {
                return;
            }
            for (int i = 0; i < len; i++)
            {
                if (tx_tmp[i] != 0)
                {
                    tx_buf[tx_buf_index++] = tx_tmp[i];
                    if (tx_tmp[i] == (byte)'\n' || tx_buf_index >= tx_buf_limit)
                    {
                        int dsp_len = tx_buf_index - 1;
                        byte[] dsp = new byte[dsp_len];
                        for (int j = 0; j < dsp_len; j++)
                        {
                            dsp[j] = tx_buf[j];
                        }
                        string info = StrFormat.Ascii_ToString(dsp);
                        info = info.Trim();
                        list_com_str.Add(info);
                        tx_buf_index = 0;
                    }
                }
            }
        }
    }
}
