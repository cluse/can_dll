using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using uidb1247;

namespace can_monitor
{
    public partial class Form1 : Form
    {
        private ComSerial com = new ComSerial(); //only for com list
        private SysCan my_can = new SysCan();
        private long app_start_time;
        private bool flag_update_display;
        //private int list_len;
        //private ListViewItem[] list_display;

        public Form1()
        {
            InitializeComponent();

            Init_ComList();
            app_start_time = getUtcNowMs();

            //list_len = my_can.GetCanBufLen();
            //list_display = new ListViewItem[list_len];
            Init_Display();
        }

        private void Init_Display()
        {
            for (int i = 0; i < my_can.GetCanBufLen(); i++)
            {
                ListViewItem item = new ListViewItem();
                item.SubItems.Add("");
                item.SubItems.Add("");
                item.SubItems.Add("");
                item.SubItems.Add("");
                listView_Can.Items.Add(item);
            }
            flag_update_display = true;
        }

        private long getUtcNowMs()
        {
            return (long)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1)).TotalMilliseconds);
        }

        private int get_app_run_time()
        {
            return (int)((getUtcNowMs() - app_start_time) / 1000);
        }
        
        private void Init_ComList()
        {
            string[] info = com.Get_PortList();
            comboBox_CanCom.Items.Clear();
            for (int i = 0; i < info.Length; i++)
            {
                comboBox_CanCom.Items.Add(info[i]);
            }
        }

        private uint timer_calc;
        private readonly int read_time_in_once = 20;
        private void timer10ms_Tick(object sender, EventArgs e)
        {
            timer_calc += 10;

            for (int i = 0; i < read_time_in_once; i++)
            {
                string info = my_can.EventReadString();
                if (info == null)
                {
                    break;
                }
            }

            if (timer_calc % 500 == 0)
            {
                update_status();
                my_can.EventDetectDev();
                Update_Display();
            }
        }
        
        private void update_status()
        {
            if (my_can.Is_Exist())
            {
                ProgressBar_Run(true);
                toolStripStatusLabel3.ForeColor = Color.Red;
                toolStripStatusLabel3.Text = my_can.GetVersion();
            }
            else
            {
                ProgressBar_Run(false);
                toolStripStatusLabel3.ForeColor = Color.Black;
                toolStripStatusLabel3.Text = "Can Dev";
            }
            int sec = get_app_run_time();
            toolStripStatusLabel4.Text = "Sec " + sec.ToString();
            if (my_can.GetOverFlow())
            {
                toolStripStatusLabel5.ForeColor = Color.Red;
                toolStripStatusLabel5.Text = "Over flow";
            }
            else
            {
                toolStripStatusLabel5.ForeColor = Color.Black;
                toolStripStatusLabel5.Text = "Status";
            }
        }

        private void ProgressBar_Run(bool active)
        {
            if (active)
            {
                int tmp = toolStripProgressBar1.Value;
                tmp += 5;
                if (tmp > 100)
                    tmp = 0;
                toolStripProgressBar1.Value = tmp;
            }
            else
            {
                toolStripProgressBar1.Value = 0;
            }
        }

        private void button_CanCom_Click(object sender, EventArgs e)
        {
            ComboBox box = comboBox_CanCom;
            ComSerial com = my_can;
            if (box.Enabled)
            {
                if (box.SelectedIndex >= 0)
                {
                    string name = box.SelectedItem.ToString();
                    com.Open(name);
                    if (com.Is_Open())
                    {
                        box.Enabled = false;
                        return;
                    }
                }
                MessageBox.Show("请选择COM","Err",
                    MessageBoxButtons.OK,MessageBoxIcon.Question);
            }
            else
            {
                com.Close();
                box.Enabled = true;
            }
        }

        private void button_Clear_Click(object sender, EventArgs e)
        {
            Reset_Display();
            my_can.Clear();
        }

        private void Reset_Display()
        {
            listView_Can.BeginUpdate();
            for (int i = 0; i < my_can.GetCanBufLen(); i++)
            {
                ListViewItem item;
                item = listView_Can.Items[i];
                item.SubItems[0].Text = "";
                item.SubItems[1].Text = "";
                item.SubItems[2].Text = "";
                item.SubItems[3].Text = "";
                item.SubItems[4].Text = "";
            }
            listView_Can.EndUpdate();
            flag_update_display = true;
        }
        
        private void Update_Display()
        {
            if (!flag_update_display && !my_can.get_rx_msg_flag())
            {
                return;
            }
            flag_update_display = false;

            listView_Can.BeginUpdate();
            int len = my_can.GetCanBufLen();
            for (int i=0;i< len; i++)
            {
                ListViewItem item;
                item = listView_Can.Items[i];
                item.SubItems[0].Text = i.ToString();

                Can_Data can = my_can.GetCanBufData(i);
                if (can.id > 0)
                {
                    fill_item_can(ref item, ref can);
                }
            }
            listView_Can.EndUpdate();
        }

        private void fill_item_can(ref ListViewItem item,ref Can_Data can)
        {
            item.SubItems[1].Text = can.id.ToString("X");
            item.SubItems[2].Text = can.period.ToString("D");
            string info = "";
            for (int j = 0; j < can.len; j++)
            {
                int val = can.data[j];
                info += val.ToString("X");
                info += ",";
            }
            info = info.Substring(0, info.Length - 1);
            item.SubItems[3].Text = info;
        }

        private void button_Sort_Click(object sender, EventArgs e)
        {
            MessageBox.Show("按ID排序");
            my_can.list_sort(MSG_SORT.ID);
        }

        private void button_SortTm_Click(object sender, EventArgs e)
        {
            MessageBox.Show("按周期排序");
            my_can.list_sort(MSG_SORT.PERIOD);
        }
    }
}
