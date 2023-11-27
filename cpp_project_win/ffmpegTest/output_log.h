#pragma once
/*
*���ܣ���־��¼
*
*��ע��
*���ߣ�
*/
extern int g_log_debug_flag;
extern int g_log_info_flag;
extern int g_log_warnning_flag;
extern int g_log_error_flag;

enum LOG_LEVEL
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

//����������
void set_log_flag(int log_debug_flag, int log_info_flag, int log_warnning_flag, int log_error_flag);

//�����־
void output_log(LOG_LEVEL log_level, const char* fmt, ...);