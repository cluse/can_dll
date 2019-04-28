// can_dll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

//#include<stdio.h>
#include<iostream>
#include <thread>
#include <list>
#include <mutex>
using namespace std;
#include <string>

#include "can_dll.h"
#include "SerialPort.h"
#include "lib_str.h"

static mutex read_mutex;

//-------------------------------
//static list<struct CAN_DATA> tx_list;
static list<struct CAN_DATA> rx_list;

static CSerialPort can2_com;
static string can_version;

static bool flag_dev_open = false;
static bool flag_dev_exist;

static bool flag_thread_finish = false;
static void Create_Thread();

static bool flag_msg_full = false;
//#define MSG_FULL_SEL flag_msg_full
#define MSG_FULL_SEL true

#define CMD_BUF_LEN 100
static char c_cmd[CMD_BUF_LEN];
static char c_cmd_tmp[CMD_BUF_LEN];

static void param_init()
{
	flag_dev_exist = false;
	rx_list.clear();
}

static void request_version()
{
	can2_com.WriteData((unsigned char *)"???", 3);
}


//-------------------------------
bool can2_open(int com)
{
	if (!flag_dev_open)
	{
		flag_dev_open = can2_com.OpenPort(com);
		if (flag_dev_open)
		{
			param_init();
			Create_Thread();
			request_version();
		}
		else
		{
			can2_com.ClosePort();
		}
	}
	return flag_dev_open;
}

void can2_close()
{
	can2_del_all_send_msg();
	can2_unmonitor_all_msg();
	Sleep(100);
	can2_com.ClosePort();
	flag_thread_finish = true;
	flag_dev_open = false;
	param_init();
}

bool can2_is_ready()
{
	if (can2_com.IsOpen())
	{
		request_version();
		return flag_dev_exist;
	}
	return false;
}

bool can2_reset()
{
	bool flag1 = can2_del_all_send_msg();
	bool flag2 = can2_unmonitor_all_msg();
	return flag1 & flag2;
}

bool can2_send_msg(struct CAN_DATA *pCan)
{
	can_data_to_buf(c_cmd_tmp, pCan, MSG_FULL_SEL);
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can msg %s &,", c_cmd_tmp);
	bool ret = can2_com.WriteData((unsigned char *)c_cmd, len_cmd);
	return ret;
}

bool can2_del_send_msg(int id)
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can del tx %x &", id);
	return can2_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can2_del_all_send_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can del tx all &");
	return can2_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can2_monitor_msg(int id)
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can monitor %x &", id);
	return can2_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can2_monitor_all_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can monitor all &");
	return can2_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can2_unmonitor_all_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can unmonitor all &");
	return can2_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can2_output_tx_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can output tx &");
	return can2_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can2_read_msg(struct CAN_DATA *pCan)
{
	bool ret = false;
	read_mutex.lock();
	if (!rx_list.empty())
	{
		struct CAN_DATA tmp = rx_list.front();
		rx_list.pop_front();
		can_data_copy(&tmp, pCan);
		ret = true;
	}
	read_mutex.unlock();
	return ret;
}



//-------------------------------
static void output_debug_info(const char *info)
{
	cout << "2- " << info;
}

static int tx_buf_limit = 200;
static char tx_buf[300];
static char tx_tmp[64];
static int tx_buf_index = 0;
static void Get_ComRecData(char by)
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
				int ret = buf_to_can_data(tx_buf + 7,&tmp, MSG_FULL_SEL);
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

//------------------------------------------
static void com_read_event()
{
	if (!can2_com.IsOpen())
	{
		return;
	}
	int len = can2_com.GetBytesInCOM();
	for (int i = 0; i < len; i++)
	{
		char ch;
		if (can2_com.ReadChar(ch))
		{
			Get_ComRecData(ch);
		}
	}
}

static UINT thread_process(LPVOID pParam)
{
	while (!flag_thread_finish)
	{
		com_read_event();
		Sleep(10);
	}
	return 0;
}

static void Create_Thread()
{
	//thread readThread(thread_process);
	//readThread.join();
	DWORD dwThreadId;
	HANDLE m_hThread = CreateThread(
		NULL,              // default security attributes
		0,                 // use default stack size  
		(LPTHREAD_START_ROUTINE)thread_process,        // thread function 
		0,             // argument to thread function 
		0,                 // use default creation flags 
		&dwThreadId);
}

