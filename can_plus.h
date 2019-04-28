#pragma once


#ifndef CAN_PLUS_H_  
#define CAN_PLUS_H_  

#include <Windows.h>

#include <string>
#include <list>
using namespace std;


struct CAN_DATA {
	int id;
	char len;
	char buf[8];
	unsigned long tm;
};

#include "SerialPort.h"
//#include "lib_str.h"

#define MSG_FULL_SEL true

#define CMD_BUF_LEN 100

class CanPort
{
public:
	bool open(int com);
	
	void close();
	
	bool is_ready();
	
	bool reset();
	
	bool send_msg(struct CAN_DATA *pCan);
	
	bool del_send_msg(int id);
	
	bool del_send_msg();
	
	bool monitor_msg(int id);
	
	bool monitor_msg();
	
	bool unmonitor_msg();
	
	bool output_tx_msg();
	
	bool read_msg(struct CAN_DATA *pCan);

private:

	list<struct CAN_DATA> tx_list;
	list<struct CAN_DATA> rx_list;

	CSerialPort can_com;
	string can_version;

	bool flag_dev_open = false;
	bool flag_dev_exist;

	bool flag_thread_finish = false;

	bool flag_msg_full = false;
	//#define MSG_FULL_SEL flag_msg_full
	char c_cmd[CMD_BUF_LEN];
	char c_cmd_tmp[CMD_BUF_LEN];

	void param_init();

	void param_deinit();

	void request_version();

	void output_debug_info(const char *info);

	int tx_buf_limit = 200;
	char tx_buf[300];
	char tx_tmp[64];
	int tx_buf_index = 0;

	void Get_ComRecData(char by);

	void com_read_event();

	void thread_process();

	void Create_Thread();
};

#endif //CAN_PLUS_H_ 


