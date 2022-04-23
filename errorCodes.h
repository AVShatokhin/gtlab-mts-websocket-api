#pragma once

#define NO_ERROR 0
#define NOT_ENOUGH_PARAMETERS 1001
#define BAD_REQUEST_FORMAT 1002
#define PARSER_FAILED 1003
#define DUPLICATE_REQUEST_ID 1004
#define UNKNOWN_METHOD 1005

#define RECORDER_NOT_FOUND 2001
#define PLOTTER_NOT_FOUND 4001

#define ADC_FAIL 5001

// ADC status
#define ADC_OK 0 // ��� ��������
#define ADC_NOT_INITED 1 // ��������� ��������� �������������
#define ADC_PLUGIN_NOT_FOUND 2 //
#define ADC_ERROR_CREATING_DEVICE 3 // ��� ������� ��� ������ � ��������� ����� ���
#define ADC_START_FAILED 4 // ������ ������������� ����������
#define ADC_CRITICAL_FAULT 5 // ������� ������ ��-�� ����
