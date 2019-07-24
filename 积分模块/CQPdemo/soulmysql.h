#include <iostream>
#include <string>
#include <mysql.h>
#include "time.h"
using namespace std;
#include <stdlib.h>  
#include <stdio.h> 
#include "soul.h"
#include <winsock2.h>    
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
int maain(int argc, char *argv[]) {
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;

	data = mysql_init(NULL);//��ʼ��  
	if (!data) {
		return EXIT_FAILURE;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "test", 0, NULL, 0);//�������ݿ� 1��success or failed  

	if (data) {
		printf("Connection success\n");
	}
	else {
		printf("Connection failed\n");
	}
	mysql_query(data, "use test");
	mysql_query(data, "insert into id values('20','world!')");
	mysql_query(data, "select * from id");

	result = mysql_store_result(data);
	row = mysql_fetch_row(result);
	cout << row[0] << row[1];
	row = mysql_fetch_row(result);
	cout << row[0] << row[1];
	getchar();
	return EXIT_SUCCESS;
}
//�����ݿ��л�ȡ���ּ�ǩ�����ڣ��ɹ�����1��ʧ�ܷ���0
int getfromsql(int64_t QQID,QDdata *sign)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[100];

	data = mysql_init(NULL);//��ʼ��  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//�������ݿ� 1��success or failed  

	if (data) {
	}
	else {
		return 0;
	}

	_i64toa(QQID, strQQID, 10);
	sprintf_s(operate, "select integral,date from sign where id = %s", strQQID);
	if (mysql_real_query(data, operate, strlen(operate)))
	{
		return 0;
	}
	result = mysql_store_result(data);
	row = mysql_fetch_row(result);
	sign->integral = atoi(row[0]);
	strcpy(sign->date, row[1]);
	return 1;
}
//�����ݿ��л�ȡū�����������id�����������ڣ��ɹ�����1��ʧ�ܷ���0
int getfromsql(int64_t QQID, masterlist *master)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[100];//���ݿ�������

	data = mysql_init(NULL);//��ʼ��  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//�������ݿ� 1��success or failed  

	if (data) {
	}
	else {
		return 0;
	}

	_i64toa(QQID, strQQID, 10);
	sprintf_s(operate, "select ransom,masterid,date from master where id = %s", strQQID);
	mysql_real_query(data, operate, strlen(operate));
	result = mysql_store_result(data);
	row = mysql_fetch_row(result);
	master->QQID = atoll(row[1]);
	master->ransom = atoi(row[0]);
	strcpy(master->date, row[2]);
	return 1;
}
//�����ݿ��л�ȡ����masteridΪQQID��id��¼�����δ��������У��ɹ�����1��ʧ�ܷ���0
int getfromsql(int64_t QQID, int64_t slavelist[10])
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[100];//���ݿ�������
	int i,n;//ѭ����

	data = mysql_init(NULL);//��ʼ��  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//�������ݿ� 1��success or failed  

	if (data) {
	}
	else {
		return 0;
	}

	_i64toa(QQID, strQQID, 10);
	sprintf_s(operate, "select id from master where masterid = %s", strQQID);
	mysql_real_query(data, operate, strlen(operate));
	result = mysql_store_result(data);
	n = 0;
	while (row = mysql_fetch_row(result))
	{
		slavelist[n] = atoll(row[0]);
		n++;
	}
	return 1;
}
//can't use
char* getfromsql(int64_t QQID)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[100];//���ݿ�������
	char tips[200];

	data = mysql_init(NULL);//��ʼ��  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//�������ݿ� 1��success or failed  

	if (data) {
	}
	else {
		return 0;
	}

	_i64toa(QQID, strQQID, 10);
	sprintf_s(operate, "select ");

	return tips;
}
//��ǩ����Ϣ��ǩ�����ڼ����ִ������ݿ⣬�ɹ�����1��ʧ�ܷ���0,����Ƿ���Ϣ�򷵻�-1
int putinsql(int64_t QQID, QDdata *sign)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[150];//���ݿ�������
	struct tm sysTime;
	
	if (sign->integral < 0)
		return -1;
	data = mysql_init(NULL);//��ʼ��  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//�������ݿ� 1��success or failed  

	if (data) {
	}
	else {
		return 0;
	}

	_i64toa(QQID, strQQID, 10);
	localtime_s(&sysTime, &sign->Time);
	memset(operate, 0, sizeof(operate));
	sprintf_s(operate, "select * from sign where id ='%s'", strQQID);
	if (mysql_real_query(data, operate, strlen(operate)))
	{
		return 0;
	}
	result = mysql_store_result(data);
	if (row = mysql_fetch_row(result))
	{
		memset(operate, 0, sizeof(operate));
		sprintf_s(operate, "update sign set date='%s-%s-%s-%s-%s-%s',integra=%i where id ='%s'", sysTime.tm_year, sysTime.tm_mon, sysTime.tm_yday, sysTime.tm_hour, sysTime.tm_min, sysTime.tm_sec, sign->integral, strQQID);
		if (mysql_real_query(data, operate, strlen(operate)))
		{
			return 0;
		}
		return 1;
	}
	else
	{
		memset(operate, 0, sizeof(operate));
		sprintf_s(operate, "insert into sign(id,date,integral) values('%s','%s-%s-%s-%s-%s-%s',%i)", sysTime.tm_year, sysTime.tm_mon, sysTime.tm_yday, sysTime.tm_hour, sysTime.tm_min, sysTime.tm_sec, sign->integral, strQQID);
		if (mysql_real_query(data, operate, strlen(operate)))
		{
			return 0;
		}
		return 1;
	}
}
//��������Ϣ�е�masterid����𼰱�������Ϣ�������ݿ⣬�ɹ�����1��ʧ�ܷ���0,����Ƿ���Ϣ�򷵻�-1
int putinsql(int64_t QQID, masterlist *master)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[150];//���ݿ�������
	struct tm sysTime;

	if (master->QQID == 0 | master->ransom == 0)
	{
		return -1;
	}
	data = mysql_init(NULL);//��ʼ��  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//�������ݿ� 1��success or failed  

	if (data) {
	}
	else {
		return 0;
	}

	_i64toa(QQID, strQQID, 10);
	localtime_s(&sysTime, &master->Time);
	memset(operate, 0, sizeof(operate));
	sprintf_s(operate, "select * from master where id ='%s'", strQQID);
	if (mysql_real_query(data, operate, strlen(operate)))
	{
		return 0;
	}
	result = mysql_store_result(data);
	if (row = mysql_fetch_row(result))
	{
		memset(operate, 0, sizeof(operate));
		sprintf_s(operate, "update master set date='%s-%s-%s-%s-%s-%s',masterid=%lld ransom=%i where id ='%s'", sysTime.tm_year, sysTime.tm_mon, sysTime.tm_yday, sysTime.tm_hour, sysTime.tm_min, sysTime.tm_sec, master->QQID, master->ransom, strQQID);
		if (mysql_real_query(data, operate, strlen(operate)))
		{
			return 0;
		}
		return 1;
	}
	else
	{
		memset(operate, 0, sizeof(operate));
		sprintf_s(operate, "insert into sign(id,date,masterid,ransom) values('%s','%s-%s-%s-%s-%s-%s',%lld,%i)", sysTime.tm_year, sysTime.tm_mon, sysTime.tm_yday, sysTime.tm_hour, sysTime.tm_min, sysTime.tm_sec, master->QQID, master->ransom, strQQID);
		if (mysql_real_query(data, operate, strlen(operate)))
		{
			return 0;
		}
		return 1;
	}
}
//���slaveQQID��Ϊū����ū�����ݣ��ɹ�����1��ʧ�ܷ���0
int deleteslave(int64_t slaveQQID)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[150];//���ݿ�������

	data = mysql_init(NULL);//��ʼ��  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//�������ݿ� 1��success or failed  

	if (data) {
	}
	else {
		return 0;
	}

	_i64toa(slaveQQID, strQQID, 10);
	memset(operate, 0, sizeof(operate));
	sprintf_s(operate, "delete from master where id ='%s'", strQQID);
	if (mysql_real_query(data, operate, strlen(operate)))
	{
		return 0;
	}
	return 1;
}
//����id��sign�ļ����������򽫳������б������Ϣ��ֲ�����ݿ��У��ɹ�������ֲ����1��ʧ�ܷ���0,��ֲ���ɾ��ʧ�ܷ���-1


