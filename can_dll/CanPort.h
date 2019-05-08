#pragma once

#include <string>
using namespace std;

#include "SerialPort.h"
#include "lib_str.h"

#define MSG_FULL_SEL true
#define CMD_BUF_LEN 100


class CanPort
{
public:
	CanPort();
	~CanPort();

private:
	struct CAN_FIFO rx_list;

	CSerialPort can_com;
	string can_version;

	bool flag_dev_open = false;
	bool flag_dev_exist;

	char c_cmd[CMD_BUF_LEN];
	char c_cmd_tmp[CMD_BUF_LEN];

	void param_init();
	void request_version();
	void output_debug_info(const char *info);

	int tx_buf_limit = 200;
	char tx_buf[300];
	char tx_tmp[64];
	int tx_buf_index = 0;

	void Get_ComRecData(char by);

public:
	bool open(int com);
	void close();
	bool is_ready();
	bool reset(int com);
	bool send_msg(struct CAN_DATA *pCan);
	bool del_send_msg(int id);
	bool del_send_msg();
	bool monitor_msg(int id);
	bool monitor_msg();
	bool unmonitor_msg();
	bool output_tx_msg();
	bool read_msg(struct CAN_DATA *pCan);

	void read_event();

};

