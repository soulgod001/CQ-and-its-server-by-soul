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

	data = mysql_init(NULL);//初始化  
	if (!data) {
		return EXIT_FAILURE;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "test", 0, NULL, 0);//连接数据库 1则success or failed  

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
//从数据库中获取积分及签到日期，成功返回1，失败返回0
int getfromsql(int64_t QQID,QDdata *sign)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[100];

	data = mysql_init(NULL);//初始化  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//连接数据库 1则success or failed  

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
//从数据库中获取奴隶的赎金，主人id及被购买日期，成功返回1，失败返回0
int getfromsql(int64_t QQID, masterlist *master)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[100];//数据库操作语句

	data = mysql_init(NULL);//初始化  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//连接数据库 1则success or failed  

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
//从数据库中获取所有masterid为QQID的id记录并依次存入数组中，成功返回1，失败返回0
int getfromsql(int64_t QQID, int64_t slavelist[10])
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[100];//数据库操作语句
	int i,n;//循环数

	data = mysql_init(NULL);//初始化  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//连接数据库 1则success or failed  

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
	char operate[100];//数据库操作语句
	char tips[200];

	data = mysql_init(NULL);//初始化  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//连接数据库 1则success or failed  

	if (data) {
	}
	else {
		return 0;
	}

	_i64toa(QQID, strQQID, 10);
	sprintf_s(operate, "select ");

	return tips;
}
//将签到信息的签到日期及积分存入数据库，成功返回1，失败返回0,存入非法信息则返回-1
int putinsql(int64_t QQID, QDdata *sign)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[150];//数据库操作语句
	struct tm sysTime;
	
	if (sign->integral < 0)
		return -1;
	data = mysql_init(NULL);//初始化  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//连接数据库 1则success or failed  

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
//将主人信息中的masterid，赎金及被购买信息存入数据库，成功返回1，失败返回0,存入非法信息则返回-1
int putinsql(int64_t QQID, masterlist *master)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[150];//数据库操作语句
	struct tm sysTime;

	if (master->QQID == 0 | master->ransom == 0)
	{
		return -1;
	}
	data = mysql_init(NULL);//初始化  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//连接数据库 1则success or failed  

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
//清除slaveQQID作为奴隶的奴隶数据，成功返回1，失败返回0
int deleteslave(int64_t slaveQQID)
{
	MYSQL *data;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row = NULL;
	char strQQID[20];
	char operate[150];//数据库操作语句

	data = mysql_init(NULL);//初始化  
	mysql_options(data, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!data) {
		return 0;
	}

	data = mysql_real_connect(data, "localhost", "root", "zheng123", "qq", 0, NULL, 0);//连接数据库 1则success or failed  

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
//检测该id的sign文件，若存在则将除道具列表外的信息移植至数据库中，成功或已移植返回1，失败返回0,移植完成删除失败返回-1


