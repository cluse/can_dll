// can_dll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

//#include<stdio.h>
#include<iostream>
#include <thread>
//#include <list>
//#include <mutex>
using namespace std;
#include <string>

#include "can_dll.h"
#include "SerialPort.h"
#include "lib_str.h"


//-------------------------------
//static list<struct CAN_DATA> tx_list;
//static list<struct CAN_DATA> rx_list;
static struct CAN_FIFO rx_list;

static CSerialPort can_com;
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
	can_data_fifo_init(&rx_list);
}

static void request_version()
{
	can_com.WriteData((unsigned char *)"???", 3);
}


//-------------------------------
bool can_open(int com)
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

void can_close()
{
	can_del_all_send_msg();
	can_unmonitor_all_msg();
	Sleep(100);
	can_com.ClosePort();
	flag_thread_finish = true;
	flag_dev_open = false;
	param_init();
}

bool can_is_ready()
{
	if (can_com.IsOpen())
	{
		request_version();
		return flag_dev_exist;
	}
	return false;
}

bool can_reset()
{
	bool flag1 = can_del_all_send_msg();
	bool flag2 = can_unmonitor_all_msg();
	return flag1 & flag2;
}

bool can_send_msg(struct CAN_DATA *pCan)
{
	can_data_to_buf(c_cmd_tmp, pCan, MSG_FULL_SEL);
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can msg %s &,", c_cmd_tmp);
	bool ret = can_com.WriteData((unsigned char *)c_cmd, len_cmd);
	return ret;
}

bool can_del_send_msg(int id)
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can del tx %x &", id);
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can_del_all_send_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can del tx all &");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can_monitor_msg(int id)
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can monitor %x &", id);
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can_monitor_all_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can monitor all &");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can_unmonitor_all_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can unmonitor all &");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can_output_tx_msg()
{
	int len_cmd = sprintf_s(c_cmd, CMD_BUF_LEN, "can output tx &");
	return can_com.WriteData((unsigned char *)c_cmd, len_cmd);
}

bool can_read_msg(struct CAN_DATA *pCan)
{
	return can_data_fifo_get(&rx_list, pCan);
}


//-------------------------------
static void output_debug_info(const char *info)
{
	cout << "1- " << info;
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
					can_data_fifo_add(&rx_list, &tmp);
				}
			}
		}
	}
}

//------------------------------------------
static void com_read_event()
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

