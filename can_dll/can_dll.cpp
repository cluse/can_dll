// can_dll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

//#include<stdio.h>
//#include<iostream>
//#include <thread>
//#include <list>
//#include <mutex>
//using namespace std;
//#include <string>

#include "CanPort.h"
#include "can_dll.h"

CanPort can1;
CanPort can2;


//------------------------------------------
const char* get_version()
{
	return "1.2";
}


//------------------------------------------
bool can_open(int com)
{
	return can1.open(com);
}

void can_close()
{
	can1.close();
}

bool can_is_ready()
{
	return can1.is_ready();
}

bool can_reset(int com)
{
	return can1.reset(com);
}

bool can_send_msg(struct CAN_DATA *pCan)
{
	return can1.send_msg(pCan);
}

bool can_del_send_msg(int id)
{
	return can1.del_send_msg(id);
}

bool can_del_all_send_msg()
{
	return can1.del_send_msg();
}

bool can_monitor_msg(int id)
{
	return can1.monitor_msg(id);
}

bool can_monitor_all_msg()
{
	return can1.monitor_msg();
}

bool can_unmonitor_all_msg()
{
	return can1.unmonitor_msg();
}

bool can_output_tx_msg()
{
	return can1.output_tx_msg();
}

bool can_read_msg(struct CAN_DATA *pCan)
{
	return can1.read_msg(pCan);
}


//------------------------------------------
bool can2_open(int com)
{
	return can2.open(com);
}

void can2_close()
{
	can2.close();
}

bool can2_is_ready()
{
	return can2.is_ready();
}

bool can2_reset(int com)
{
	return can2.reset(com);
}

bool can2_send_msg(struct CAN_DATA *pCan)
{
	return can2.send_msg(pCan);
}

bool can2_del_send_msg(int id)
{
	return can2.del_send_msg(id);
}

bool can2_del_all_send_msg()
{
	return can2.del_send_msg();
}

bool can2_monitor_msg(int id)
{
	return can2.monitor_msg(id);
}

bool can2_monitor_all_msg()
{
	return can2.monitor_msg();
}

bool can2_unmonitor_all_msg()
{
	return can2.unmonitor_msg();
}

bool can2_output_tx_msg()
{
	return can2.output_tx_msg();
}

bool can2_read_msg(struct CAN_DATA *pCan)
{
	return can2.read_msg(pCan);
}


//------------------------------------------
static HANDLE m_hThread = 0;
static DWORD dwThreadId;
static bool flag_thread_finish;
static UINT timer_calc = 0;
static UINT thread_process(LPVOID pParam)
{
	while (!flag_thread_finish)
	{
		can1.read_event();
		can2.read_event();
		Sleep(10);

		timer_calc++;
		if (timer_calc > 100)
		{
			timer_calc = 0;
			can1.detect_event();
			can2.detect_event();
		}
	}
	return 0;
}

void Create_Thread()
{
	if (!m_hThread)
	{
		flag_thread_finish = false;
		//thread readThread(thread_process);
		//readThread.join();
		m_hThread = CreateThread(
			NULL,              // default security attributes
			0,                 // use default stack size  
			(LPTHREAD_START_ROUTINE)thread_process,        // thread function 
			0,             // argument to thread function 
			0,                 // use default creation flags 
			&dwThreadId);
	}
}

void Destroy_Thread()
{
	flag_thread_finish = true;
}
