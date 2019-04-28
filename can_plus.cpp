

#include "stdafx.h"

//#include<stdio.h>
#include<iostream>
#include <thread>

#include "can_plus.h"
#include "lib_str.h"

bool CanPort::open(int com)
{
	if (!flag_dev_open)
	{
		flag_dev_open = can_com.OpenPort(com);
		if (flag_dev_open)
		{
			param_init();
			Create_Thread();
			request_version();
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
	flag_thread_finish = true;
	flag_dev_open = false;
	param_deinit();
}

bool CanPort::is_ready()
{
	if (can_com.IsOpen())
	{
		request_version();
		return flag_dev_exist;
	}
	return false;
}

bool CanPort::reset()
{
	bool flag1 = del_send_msg();
	bool flag2 = unmonitor_msg();
	return flag1 & flag2;
}

bool CanPort::send_msg(struct CAN_DATA *pCan)
{
	can_data_to_buf(c_cmd_tmp, pCan, MSG_FULL_SEL);
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can msg %s &,", c_cmd_tmp);
	bool ret = can_com.WriteData((unsigned char *)c_cmd, len_cmd);
	return ret;
}

bool CanPort::del_send_msg(int id)
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can del tx %x &", id);
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::del_send_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can del tx all &");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::monitor_msg(int id)
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can monitor %x &", id);
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::monitor_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can monitor all &");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::unmonitor_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can unmonitor all &");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::output_tx_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can output tx &");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool CanPort::read_msg(struct CAN_DATA *pCan)
{
	if (!rx_list.empty())
	{
		struct CAN_DATA tmp = rx_list.front();
		rx_list.pop_front();
		can_data_copy(&tmp, pCan);
		return true;
	}
	return false;
}


//-------------------------------------------
void CanPort::param_init()
{
	flag_dev_exist = false;
	rx_list.clear();
}

void CanPort::param_deinit()
{
	flag_dev_exist = false;
	rx_list.clear();
}

void CanPort::request_version()
{
	can_com.WriteData((unsigned char *)"???", 3);
}

void CanPort::output_debug_info(const char *info)
{
	cout << info;
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
					rx_list.push_back(tmp);
				}
			}
		}
	}
}

void CanPort::com_read_event()
{
	if (!can_com.IsOpen())
	{
		return;
	}
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

void CanPort::thread_process()
{
	while (!flag_thread_finish)
	{
		com_read_event();
		Sleep(10);
	}
}

void CanPort::Create_Thread()
{
	//thread readThread(thread_process);
	//readThread.join();
	/*
	DWORD dwThreadId;
	HANDLE m_hThread = CreateThread(
		NULL,              // default security attributes
		0,                 // use default stack size  
		(LPTHREAD_START_ROUTINE)thread_process,        // thread function 
		0,             // argument to thread function 
		0,                 // use default creation flags 
		&dwThreadId);
	*/

	std::thread th = std::thread(&CanPort::thread_process, this);
	th.join();
}

