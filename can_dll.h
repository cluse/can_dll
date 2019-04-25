#pragma once

extern"C" __declspec(dllexport) bool can_open(int com);

extern"C" __declspec(dllexport) void can_close();

extern"C" __declspec(dllexport) bool can_is_ready();

extern"C" __declspec(dllexport) bool can_reset();

extern"C" __declspec(dllexport) bool can_send_msg(struct CAN_DATA *pCan);

//extern"C" __declspec(dllexport) bool can_del_send_msg(int id);

extern"C" __declspec(dllexport) bool can_del_send_msg();

//extern"C" __declspec(dllexport) bool can_monitor_msg(int id);

extern"C" __declspec(dllexport) bool can_monitor_msg();

extern"C" __declspec(dllexport) bool can_unmonitor_msg();

extern"C" __declspec(dllexport) bool can_output_tx_msg();

extern"C" __declspec(dllexport) bool can_read_msg(struct CAN_DATA *pCan);

