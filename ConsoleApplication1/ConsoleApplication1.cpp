// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
//#include "stdafx.h"
#include <windows.h>
#include <string>
using namespace std;

#include <atlbase.h>
using namespace ATL;

#include <time.h>
#include <vector>
#include <list>

#include "lib_str.h"

#define TWO_BOARD_TIME 1000

#define MSG_IN_ONE_TEST 20

#define ONE_TEST_TIME 1000

struct CAN_DATA msg_tx;
struct CAN_DATA msg_rx;

//static list<struct CAN_DATA> msg_list;
struct CAN_FIFO fifo_tx;
//struct CAN_FIFO fifo_rx;

struct CYC_FIFO fifo_cyc_rx;

typedef bool(*fnFun_Open)(int);
typedef bool(*fnFun_IsReady)();
typedef bool(*fnFun_Close)();
typedef bool(*fnFun_Reset)(int);
typedef bool(*fnFun_SendMsg)(struct CAN_DATA *);
typedef bool(*fnFun_DelMsg)(int);
typedef bool(*fnFun_DelALLMsg)();
typedef bool(*fnFun_MonitorMsg)(int);
typedef bool(*fnFun_MonitorALLMsg)();
typedef bool(*fnFun_UnMonitorALLMsg)();
typedef bool(*fnFun_ReadMsg)(struct CAN_DATA *);

void output_com_list()
{
	cout << "---------------------------------\n";
	HKEY   hkey;
	LONG   lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), NULL, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_READ, &hkey);
	if (lRes == ERROR_SUCCESS)
	{
		TCHAR   tchKey[MAX_PATH];
		TCHAR   tchValue[20];
		DWORD   dwIndex = 0;
		DWORD   dwType = REG_SZ;
		while (lRes == ERROR_SUCCESS)
		{
			DWORD   dwCount = MAX_PATH;
			DWORD   dwVCount = 20;
			lRes = RegEnumValue(hkey, dwIndex++, tchKey, &dwCount, NULL, &dwType, (LPBYTE)tchValue, &dwVCount);
			if (lRes == ERROR_SUCCESS)
			{
				wcout << tchValue << "\n";
			}
		}
	}
	RegCloseKey(hkey);
	cout << "---------------------------------\n";
}

void error_exit(string info)
{
	int tmp;
	cout << info;
	cin >> tmp;
	exit(-1);
}

int test_type;
int com1_num;
int com2_num;
void select_test_type()
{
	cout << "select testing type \n";
	cout << "0 - monitor \n";
	cout << "1 - send and read (one board) \n";
	cout << "2 - send and read (two board) \n";
	cin >> test_type;

	cout << "input com1 num \n";
	cin >> com1_num;

	if (test_type >= 2)
	{
		test_type = 2;
		cout << "input com2 num \n";
		cin >> com2_num;
	}
	else if (test_type < 0)
	{
		test_type = 0;
	}

	cout << "-----------------------------\n";
	cout << "testing type = " << test_type << "\n";
	cout << "-----------------------------\n";
}

void random_can_tx_msg()
{
	int id = (rand() & 0x3ff);
	msg_tx.id = id;
	msg_tx.tm = 0;
	msg_tx.len = rand() % 8 + 1;
	for (int i = 0; i < 8; i++)
	{
		msg_tx.buf[i] = rand() % 0x100;
	}
}

bool is_can_msg_same(CAN_DATA *m1, CAN_DATA *m2)
{
	if (m1->id != m2->id || m1->len != m2->len)
	{
		return false;
	}
	for (int i = 0; i < m1->len; i++)
	{
		if (m1->buf[i] != m2->buf[i])
		{
			return false;
		}
	}
	return true;
}

clock_t tag_ms;
void timer_start()
{
	tag_ms = clock();
}

int timer_get_ms()
{
	clock_t nw = clock();
	return (nw - tag_ms);
}

int main()
{
	can_data_fifo_init(&fifo_tx);
	//can_data_fifo_init(&fifo_rx);
	cyc_fifo_init(&fifo_cyc_rx);

	output_com_list();

	select_test_type();

    cout << "CAN DLL TEST\n"; 
	HMODULE hDll = LoadLibrary(L"can_dll.dll");
	
	fnFun_Open can_open = (fnFun_Open)GetProcAddress(hDll, "can_open");
	fnFun_IsReady can_is_ready = (fnFun_IsReady)GetProcAddress(hDll, "can_is_ready");
	fnFun_Close can_close = (fnFun_Close)GetProcAddress(hDll, "can_close");
	fnFun_Reset can_reset = (fnFun_Reset)GetProcAddress(hDll, "can_reset");
	fnFun_SendMsg can_send_msg = (fnFun_SendMsg)GetProcAddress(hDll, "can_send_msg");

	fnFun_DelMsg can_del_send_msg = (fnFun_DelMsg)GetProcAddress(hDll, "can_del_send_msg");
	fnFun_DelALLMsg can_del_all_send_msg = (fnFun_DelALLMsg)GetProcAddress(hDll, "can_del_all_send_msg");
	fnFun_MonitorMsg can_monitor_msg = (fnFun_MonitorMsg)GetProcAddress(hDll, "can_monitor_msg");
	fnFun_MonitorALLMsg can_monitor_all_msg = (fnFun_MonitorALLMsg)GetProcAddress(hDll, "can_monitor_all_msg");
	fnFun_UnMonitorALLMsg can_unmonitor_all_msg = (fnFun_UnMonitorALLMsg)GetProcAddress(hDll, "can_unmonitor_all_msg");
	fnFun_ReadMsg can_read_msg = (fnFun_ReadMsg)GetProcAddress(hDll, "can_read_msg");

	fnFun_Open can2_open = (fnFun_Open)GetProcAddress(hDll, "can2_open");
	fnFun_IsReady can2_is_ready = (fnFun_IsReady)GetProcAddress(hDll, "can2_is_ready");
	fnFun_Close can2_close = (fnFun_Close)GetProcAddress(hDll, "can2_close");
	fnFun_Reset can2_reset = (fnFun_Reset)GetProcAddress(hDll, "can2_reset");
	fnFun_SendMsg can2_send_msg = (fnFun_SendMsg)GetProcAddress(hDll, "can2_send_msg");

	fnFun_DelMsg can2_del_send_msg = (fnFun_DelMsg)GetProcAddress(hDll, "can2_del_send_msg");
	fnFun_DelALLMsg can2_del_all_send_msg = (fnFun_DelALLMsg)GetProcAddress(hDll, "can2_del_all_send_msg");
	fnFun_MonitorMsg can2_monitor_msg = (fnFun_MonitorMsg)GetProcAddress(hDll, "can2_monitor_msg");
	fnFun_MonitorALLMsg can2_monitor_all_msg = (fnFun_MonitorALLMsg)GetProcAddress(hDll, "can2_monitor_all_msg");
	fnFun_UnMonitorALLMsg can2_unmonitor_all_msg = (fnFun_UnMonitorALLMsg)GetProcAddress(hDll, "can2_unmonitor_all_msg");
	fnFun_ReadMsg can2_read_msg = (fnFun_ReadMsg)GetProcAddress(hDll, "can2_read_msg");

	timer_start();

	if (test_type == 0)
	{
		bool ret = can_reset(com1_num);
		if (!ret)
		{
			error_exit("can't open ");
		}

		can_monitor_all_msg();

		int calc_time = 0;;
		while (1)
		{
			Sleep(10);
			if (can_read_msg(&msg_rx)) {
				cyc_fifo_add(&fifo_cyc_rx, &msg_rx);
			}

			calc_time++;
			if (calc_time > 200)
			{
				calc_time = 0;
				cout << "-------------------------- \n";
				for (int i=0;i< CYC_FIFO_LEN;i++)
				{
					if (cyc_fifo_read(&fifo_cyc_rx, i, &msg_rx))
					{
						//std::cout << "id " + tmp.id;
						char c_cmd[100];
						can_data_to_buf(c_cmd, &msg_rx, true);
						int len_cmd = sprintf_s(c_cmd, 100, "%s\n", c_cmd);
						//printf(c_cmd);
						cout << c_cmd;
					}
					break;
				}
			}
		}
	}

	if (test_type == 1)
	{
		bool ret = can_reset(com1_num);
		if (!ret)
		{
			error_exit("can't open ");
		}

		can_monitor_all_msg();

		struct CAN_DATA tmp;
		int id_buf[] = {
			0x3aa,  
			0x0c0,
			0x513,
			0x2a1,
			0x3be,
			0x215,
			0x225, 0x235,
			0x323, 0x354, 0x303,
			0x105, 0x295, 0x125,
		};
		tmp.id = 0x320;
		tmp.len = 8;
		for (int i = 0; i < 8; i++)
		{
			tmp.buf[i] = 0x80 + i;
		}
		tmp.tm = 10;

		for (int i = 0; i < 12; i++)
		{
			tmp.id += 0x1;
			tmp.id = id_buf[i];
			ret = can_send_msg(&tmp);
		}

		while (1)
		{
			Sleep(10);
			if (can_read_msg(&msg_rx)) {
				//std::cout << "id " + tmp.id;
				char c_cmd[100];
				can_data_to_buf(c_cmd, &msg_rx, true);
				int len_cmd = sprintf_s(c_cmd, 100, "%s\n", c_cmd);
				//printf(c_cmd);
				cout << c_cmd;
			}
		}
	}

	if (test_type == 2)
	{
		bool ret = can_reset(com1_num);
		if (!ret)
		{
			error_exit("can't open 1 " );
		}

		ret = can2_reset(com2_num);
		if (!ret)
		{
			error_exit("can't open 2");
		}

		can_monitor_all_msg();
		can2_monitor_all_msg();

		int calc_step = 0;
		int calc_delay = 0;
		int calc_cyc = 0;

		int calc_time = 0;
		while (calc_time < TWO_BOARD_TIME)
		{
			switch (calc_step)
			{
			case 0:
				for (int j = 0; j < MSG_IN_ONE_TEST; j++)
				{
					random_can_tx_msg();
					can_send_msg(&msg_tx);
					can_data_fifo_add(&fifo_tx,&msg_tx);
				}
				Sleep(100);
				calc_step++;
				break;
			case 1:
				while (can_data_fifo_get(&fifo_tx,&msg_tx))
				{
					for (calc_cyc = 0; calc_cyc < ONE_TEST_TIME / 2; calc_cyc++)
					{
						Sleep(2);
						if (can2_read_msg(&msg_rx))
						{
							if (is_can_msg_same(&msg_rx, &msg_tx))
							{
								break;
							}
						}
					}
					if (calc_cyc >= ONE_TEST_TIME / 2)
					{
						error_exit("can't read com time");
					}
				}
				calc_step++;
				break;
			case 2:
				for (int j = 0; j < MSG_IN_ONE_TEST; j++)
				{
					random_can_tx_msg();
					can2_send_msg(&msg_tx);
					can_data_fifo_add(&fifo_tx, &msg_tx);
				}
				Sleep(100);
				calc_step++;
				break;
			case 3:
				while (can_data_fifo_get(&fifo_tx, &msg_tx))
				{
					for (calc_cyc = 0; calc_cyc < ONE_TEST_TIME / 2; calc_cyc++)
					{
						Sleep(2);
						if (can_read_msg(&msg_rx))
						{
							if (is_can_msg_same(&msg_rx, &msg_tx))
							{
								break;
							}
						}
					}
					if (calc_cyc >= ONE_TEST_TIME / 2)
					{
						error_exit("can't read com2 time ");
					}
				}
				calc_step++;
				break;
			case 4:
				calc_time++;
				cout << "testing time " << calc_time << "\n";
				calc_step = 0;
				break;
			default:
				error_exit("unknow status");
				break;
			}
		}

		cout << "testing cost " << timer_get_ms() << " ms";

		error_exit("testing finish\n");
	}

	FreeLibrary(hDll);
	return 0;
}







// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
