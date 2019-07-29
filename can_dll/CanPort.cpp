#include "stdafx.h"
#include<iostream>
#include "CanPort.h"


CanPort::CanPort()
{
}


CanPort::~CanPort()
{
}



//-------------------------------
void CanPort::param_init()
{
	flag_dev_exist = false;
	can_data_fifo_init(&rx_list);
}

void CanPort::request_version()
{
	can_com.WriteData((unsigned char *)"???", 3);
}


//-------------------------------
bool CanPort::open(int com)
{
	if (!flag_dev_open)
	{
		flag_dev_open = can_com.OpenPort(com);
		flag_dev_exist = false;
		if (flag_dev_open)
		{
			param_init();
		}
		else
		{
			can_com.ClosePort();
		}
	}
	return flag_dev_open;
}

void CanPort::close()
{
	del_send_msg();
	unmonitor_msg();
	Sleep(100);
	can_com.ClosePort();
	flag_dev_open = false;
	flag_dev_exist = false;
	param_init();
}

bool CanPort::is_ready()
{
	if (can_com.IsOpen())
	{
		return flag_dev_exist;
	}
	return false;
}

bool CanPort::reset(int com)
{
	can_com.ClosePort();
	param_init();
	flag_dev_open = can_com.OpenPort(com);
	if (!flag_dev_open)
	{
		return false;
	}
	request_version();
	for (int i = 0; i < 100; i++)
	{
		Sleep(20);
		if (flag_dev_exist)
			break;
	}
	if (!flag_dev_exist)
	{
		can_com.ClosePort();
		return false;
	}
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can reset &\n");
	can_com.WriteData((unsigned char *)c_cmd, len_cmd);
	Sleep(100);
	can_data_fifo_init(&rx_list);
	return true;
}

bool CanPort::send_msg(struct CAN_DATA *pCan)
{
	can_data_to_buf(c_cmd_tmp, pCan, MSG_FULL_SEL);
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can msg %s &\n", c_cmd_tmp);
	bool ret = can_com.WriteData((unsigned char *)c_cmd, len_cmd);
	return ret;
}

bool CanPort::del_send_msg(int id)
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can del tx %x &\n", id);
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::del_send_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can del tx all &\n");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::monitor_msg(int id)
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can monitor %x &\n", id);
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::monitor_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can monitor all &\n");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::unmonitor_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can unmonitor all &\n");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::output_tx_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can output tx &\n");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::read_msg(struct CAN_DATA *pCan)
{
	return can_data_fifo_get(&rx_list, pCan);
}


//-------------------------------
void CanPort::output_debug_info(const char *info)
{
	cout << "1- " << info;
}

void CanPort::Get_ComRecData(char by)
{
	char tmp = by;
	if (tmp != 0)
	{
		tx_buf[tx_buf_index++] = tmp;
		if (tmp == '\n' || tx_buf_index >= tx_buf_limit)
		{
			tx_buf[tx_buf_index] = 0;
			tx_buf_index = 0;
			//output_debug_info(tx_buf);

			if (index_of_str(tx_buf, "CAN_VER") >= 0)
			{
				flag_dev_exist = true;
				can_version = string(tx_buf);
			}
			if (index_of_str(tx_buf, "can_rx") >= 0)
			{
				struct CAN_DATA tmp;
				int ret = buf_to_can_data(tx_buf + 7, &tmp, MSG_FULL_SEL);
				if (ret < 0)
				{
					output_debug_info("err-> can_rx id");
				}
				else
				{
					can_data_fifo_add(&rx_list, &tmp);
				}
			}
		}
	}
}

//------------------------------------------
void CanPort::read_event()
{
	if (can_com.IsOpen())
	{
		int len = can_com.GetBytesInCOM();
		for (int i = 0; i < len; i++)
		{
			char ch;
			if (can_com.ReadChar(ch))
			{
				Get_ComRecData(ch);
			}
		}
	}
}

void CanPort::detect_event()
{
	if (can_com.IsOpen())
	{
		if (!flag_dev_exist)
			request_version();
	}
}