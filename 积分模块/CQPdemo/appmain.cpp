/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����
#include "soul.h"
#include "io.h"
#include "direct.h"
#include <string.h>
#include <stdio.h> 
#include "time.h"
#include "QTool.h"
#include "base64.h"
#include <Windows.h>
#include <strsafe.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#include <iostream>

using namespace std;
#define goodbye "����ټ���ɵ��ҲҪ˯���ˣ���Ŷ��"
#define hi "��Һã��˼��˰�,����������ɵ����������������������"
#define thanksforweather "��л��������֪������Ԥ����ÿ��ŵ�wuliɵ��׼ʱΪ�����������人����Ŷ��"
#define sorryforweather "����ȡ������֪������Ԥ������ɵ���������Ĳ�����?������qq:3047964704�����������ҵ�˵˵��������Ŷ��"
#define Error "ɵ�����˵�С���⣬����ϵά����Ա�����"
#define wulizhiyin "��Һã�����������֪���㲥��̨������������wuliɵ�����õ�̨������ai��(486712165)�а졣\nĿǰ������:\n����Ԥ��\n������Ⱥ\nά������\n����֪��\n���͹��ܹؼ����˽����Ŷ(��ӭ�������������qqȺ)��"
int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;
//static makeroom room[5];
MMRESULT timer_id;
MMRESULT game_timer_id[5];
HHOOK keyboardHook = 0;
HHOOK g_hMouse = 0;
static game gameroom[5];//��Ϸ����
connection connections;//����������ӵ�ʵ������
//��ѯʱ��
void WINAPI onTimeFunc(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	time_t Time;
	struct tm sysTime;
	FILE *fp;
	char sendmsg[100];
	char substring[100];
	char str;
	int i, t, k, number;
	int64_t groupid[100];
	if ((number = atoi(connections.ask("t"))) == 1)//������ھŵ㵽�ŵ���һ��֮��
	{
		char *a = NULL;
		char *list = NULL;
		memset(sendmsg,0,sizeof(sendmsg));
		memset(groupid, 0, sizeof(groupid));
		connections.getweatherlist(groupid, 1);
		i = 0;
		while (groupid[i] != 0)
		{
			a = connections.ask("city:�人\ntype:weather\naction:get\nsendby:soul");
			CQ_sendGroupMsg(ac, groupid[i], a);
			i++;
		}
		connections.getweatherlist(groupid, 0);
		i = 0;
		while (groupid[i] != 0)
		{
			a = connections.ask("city:�人\ntype:weather\naction:get\nsendby:soul");
			CQ_sendPrivateMsg(ac, groupid[i], a);
			i++;
		}
		if ((fp = fopen("c:/clear.txt", "r")) == NULL)
		{
			CQ_addLog(ac, CQLOG_ERROR, "clear", "can't read");
		}
		else
		{
			str = fgetc(fp);
			fclose(fp);
			sprintf(sendmsg, "[CQ:at,qq=all]�����ֵ�%c������࣡��ɺ����ڱ�Ⱥ����������֣�", str);
			CQ_sendGroupMsg(ac, 657557375, sendmsg);
		}
		return;
	}
	else if (number == -1)
	{
		return;
	}
	return;
}
/*
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

LRESULT CALLBACK HookCallback(int code, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT *ks = (KBDLLHOOKSTRUCT*)lParam;
	if (ks->vkCode != 'P')
	{
		CQ_addLog(ac, CQLOG_DEBUG, "hook", "have intercept");
		return 1;
	}
	CQ_addLog(ac, CQLOG_DEBUG, "hook", "get P");
	UnhookWindowsHookEx(keyboardHook);
	UnhookWindowsHookEx(g_hMouse);
	CQ_addLog(ac, CQLOG_DEBUG, "hook", "exit");
	return 1;
}

DWORD WINAPI ThreadProc1(LPVOID lpParam)
{
	keyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, HookCallback, GetModuleHandleA(0), 0);
	g_hMouse = SetWindowsHookExA(WH_MOUSE_LL, MouseProc, GetModuleHandleA(0), 0);
	if (keyboardHook == 0)
	{
		CQ_addLog(ac, CQLOG_ERROR, "hook", "faile");
		return -1;
	}
	CQ_addLog(ac, CQLOG_DEBUG, "hook", "success");

	//����©����Ϣ������Ȼ����Ῠ��
	MSG msg;
	while (1)
	{
		//SetCursorPos(0, 0);
		if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
			Sleep(1);    //����CPUȫ��������
	}
	UnhookWindowsHookEx(keyboardHook);
	CQ_addLog(ac, CQLOG_DEBUG, "hook", "exit");
}
*/

/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {
	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	connections.init(ac);
	connections.ask("t");
	timeSetEvent(60000, 1, (LPTIMECALLBACK)onTimeFunc, DWORD(1), TIME_PERIODIC);//����ʱ�Ӻ��������Ϊ1����
	enabled = true;
	return 0;
}

/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {

	int sum, x, y;
	int info,number;
	char sendmsg[100],tips[100];
	char *recData;
	char strQQID[100];
	QDdata data;
	QDdata masterdata;
	int64_t *IDlist = NULL;
	int64_t groupid;
	if (strstr(msg, "rich") != NULL)
	{
		return EVENT_BLOCK;
	}
	else if (sizeof(msg) > 40)
	{
		return EVENT_BLOCK;
	}
	connections.init(ac);
	//˭���Ե�(��ʱͣ��)
	for (sum = 0; sum < 5; sum++)
	{
		if (gameroom[sum].runstate)
		{			
			if (gameroom[sum].memberinhere(fromQQ) > -1)
			{
				if (gameroom[sum].rank == -1)
				{
					if (msg[0] < 47 | msg[0] > 58)
					{
						CQ_sendPrivateMsg(ac, fromQQ, "���ָ�����");
						return EVENT_BLOCK;
					}
					if (atoi(msg) > gameroom[sum].number)
					{
						CQ_sendPrivateMsg(ac, fromQQ, "���������Ҳ�����!");
						return EVENT_BLOCK;
					}
					if (gameroom[sum].hadvote(fromQQ, atoi(msg) - 1))
					{
						CQ_sendPrivateMsg(ac, fromQQ, "ͶƱ�ɹ�");
					}
					else
					{
						CQ_sendPrivateMsg(ac, fromQQ, "�����ظ�ͶƱ�����޸�ͶƱ��");
						return EVENT_BLOCK;
					}
					if ((x = gameroom[sum].check(0,ac)) == -1)
					{
						return EVENT_BLOCK;
					}
					return EVENT_BLOCK;								  //������̭֪ͨ
				}
				else if (gameroom[sum].rank >= 0)
				{
					char sendmsg[200];
					memset(sendmsg,0,sizeof(sendmsg));
					strcpy(sendmsg, msg);
					if (0 == (x = gameroom[sum].hadsay(fromQQ, sendmsg, ac)))
					{
						CQ_sendPrivateMsg(ac, fromQQ, "��δ�ֵ��㷢��!");
						return EVENT_BLOCK;
					}
					else if (x == -1)
					{
						CQ_sendPrivateMsg(ac, fromQQ, "���Ѿ����Թ���!");
						return EVENT_BLOCK;
					}
					return EVENT_BLOCK;
				}
			}
		}
	}
	if (!strcmp(msg, "��������Ԥ��"))
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "user:%lld\nkind:0\ntype:weather\naction:add\nsendby:soul", fromQQ);
		if (atoi(connections.ask(sendmsg)))
		{
			CQ_sendPrivateMsg(ac, fromQQ, thanksforweather);
			return EVENT_BLOCK;
		}
		else
		{
			CQ_sendPrivateMsg(ac, fromQQ, Error);
			return EVENT_BLOCK;
		}
	}
	else if (!strcmp(msg, "�ر�����Ԥ��"))
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "user:%lld\nkind:0\ntype:weather\naction:delete\nsendby:soul", fromQQ);
		if (atoi(connections.ask(sendmsg)))
		{
			CQ_sendPrivateMsg(ac, fromQQ, sorryforweather);
			return EVENT_BLOCK;
		}
		else
		{
			CQ_sendPrivateMsg(ac, fromQQ, Error);
			return EVENT_BLOCK;
		}
	}
	else if (!strcmp(msg, "�鿴����Ԥ��"))
	{
		recData = connections.ask("city:�人\ntype:weather\naction:get\nsendby:soul");
		if (recData == NULL)
		{
			CQ_sendPrivateMsg(ac, fromQQ, Error);
			return EVENT_BLOCK;
		}
		else
		{
			CQ_sendPrivateMsg(ac, fromQQ, recData);
			return EVENT_BLOCK;
		}
	}
	else if (!strcmp(msg, "����֪��"))
	{
		CQ_sendPrivateMsg(ac, fromQQ, wulizhiyin);
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "ά������"))
	{
		CQ_sendPrivateMsg(ac, fromQQ, "���������ʲô���������ʲô���飬����ͨ�����¼����������з���\n1.ά����Աqq:3047964704\n2.�����ʼ���3047964704��qq����\n3.���ҵ�˵˵������������");
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "������Ⱥ"))
	{
		strcpy(tips, msg + 8);
		sprintf_s(sendmsg, "user:%s\ntype:validate\naction:add\nsendby:soul", tips);
		if (atoi(connections.ask(sendmsg)))
		{
			CQ_sendPrivateMsg(ac, fromQQ, "����ȷ�ϳɹ��������ҽ�Ⱥ���ɣ�");
		}
		else
		{
			CQ_sendPrivateMsg(ac, fromQQ, "��Ŷ����֪Ϊ��ȷ��ʧ���ˣ���ϵһ��ά����Ա��������һ�°ɣ�");
			CQ_sendPrivateMsg(ac, fromQQ, "��ȷ�ĸ�ʽ��\n������Ⱥxxxx\nxxxxΪqqȺ��Ⱥ��Ŷ��");
		}
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "&#91;QQ���"))
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "%lld���ҷ��˺��Ŷ��������������", fromQQ);
		CQ_sendPrivateMsg(ac, 3047964704, sendmsg);
		CQ_sendPrivateMsg(ac, fromQQ, "лл���еĽ�ˮ��ɵ��һ�����ڴ�ҵ�֧����Խ��Խ���ģ�");
		return EVENT_BLOCK;
	}
	else
	{
		strcpy(sendmsg, connections.ask((char *)msg));
		if (strstr(sendmsg, "0") != NULL)
		{
			return EVENT_IGNORE;
		}
		else
		{
			CQ_sendPrivateMsg(ac, fromQQ, "����֪����ӭ����");
			CQ_sendPrivateMsg(ac, fromQQ, sendmsg);
			return EVENT_BLOCK;
		}
	}
	return EVENT_IGNORE;
}


/*
* Type=2 Ⱥ��Ϣ
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	char strQQID[20],targetstrQQID[20],sendmsg[200],strintegral[20];
	char *recData;
	char nick[20],where[50],tips[50];
	int64_t targetQQID=0;
	int64_t slavelist[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int64_t slaveQQID = 0;
	FILE *fp;
	QDdata data;
	QDdata targetdata;
	time_t Time;
	char date[20];
	struct makelist
	{
		int64_t QQID[10] = { 0,0,0,0,0,0,0,0,0,0 };
		int64_t targetQQID[10] = { 0,0,0,0,0,0,0,0,0,0 };
		int number[10] = { 0,0,0,0,0,0,0,0,0,0 };
		int integral[10] = { 0,0,0,0,0,0,0,0,0,0 };
	}static list;
	struct makeprop
	{
		char name[25];
		int type;
	}prop;
	//static gamelist gameroom;
	struct tm sysTime1;
	struct tm sysTime2;
	masterlist master;
	int k = 0, number = 0, i = 0, price = 0, n = 0;//k��i��nΪѭ����
	int sum = 0, x = 0, y = 0, len = 0;
	int t = 0;
	static int shutup = 0;
	CQTool fromQQclass;
	CQ_Type_GroupMember fromQQinfo;
	CQ_Type_GroupMember targetQQinfo;
	CQ_Type_GroupMember masterQQinfo;
	
	connections.init(ac);
	for (k = 0; k < 10; k++)
	{
		if (list.QQID[k] == fromQQ)//ȡ����QQID�ļ�¼
		{
			if (list.number[k] == 2)//����
			{
				if (!strcmp(msg, "1"))
				{
					//if (useprop(fromQQ, "���Ϳ�") != 1)
					if (connections.changeprop(fromQQ, 1, 0) != 1)
					{
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						list.number[k] = 0;
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					targetQQID = list.targetQQID[k];//ȡ�����е�Ŀ��
					connections.getintegral(targetQQID, &targetdata);
					connections.changeintegral(targetQQID, list.integral[k], 1);
					connections.changeintegral(fromQQ, -list.integral[k], 1);
					CQ_sendGroupMsg(ac, fromGroup, "���ͳɹ�!");
					list.QQID[k] = 0;
					list.targetQQID[k] = 0;
					list.number[k] = 0;
					return EVENT_BLOCK;
				}
				else if (!strcmp(msg, "0"))
				{
					list.QQID[k] = 0;
					list.targetQQID[k] = 0;
					list.number[k] = 0;
					list.integral[k] = 0;
					CQ_sendGroupMsg(ac, fromGroup, "��ȡ��������");
					return EVENT_BLOCK;
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, "����1ȷ�ϣ�����0ȡ����");
					return EVENT_BLOCK;
				}
			}
			else if (list.number[k] == 3)//����
			{
				targetQQID = list.targetQQID[k];//ȡ�����е�Ŀ��
				if (!strcmp(msg, "1"))//ָ��Ϊ1
				{
					if (connections.getmaster(&master, targetQQID) == 0)
					{
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						list.number[k] = 0;
						list.integral[k] = 0;
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					if (connections.buyslave(targetQQID, 0, 0, master.Time) == 0)
					{
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						list.number[k] = 0;
						list.integral[k] = 0;
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					CQ_sendGroupMsg(ac, fromGroup, "���ųɹ���");
					list.QQID[k] = 0;
					list.targetQQID[k] = 0;
					list.number[k] = 0;
					list.integral[k] = 0;
					return EVENT_BLOCK;
				}
				else if (!strcmp(msg, "0"))
				{
					list.QQID[k] = 0;
					list.targetQQID[k] = 0;
					list.number[k] = 0;
					list.integral[k] = 0;
					CQ_sendGroupMsg(ac, fromGroup, "��ȡ��������");
					return EVENT_BLOCK;
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, "����1ȷ�ϣ�����0ȡ����");
					return EVENT_BLOCK;
				}
			}
			else if (list.number[k] == 1)//ǿ�ƹ���
			{
				if (!strcmp(msg, "1"))//ָ��Ϊ1
				{
					targetQQID = list.targetQQID[k];//ȡ�����е�Ŀ��
					connections.getintegral(targetQQID, &targetdata);
					if (connections.getmaster(&master, targetQQID) == 0)//��ȡ�Է���master
					{
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						list.number[k] = 0;
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					if (master.QQID != 0)//����Է���master
					{				
						if (connections.changeprop(fromQQ, 2, 0) != 1)
						{
							list.QQID[k] = 0;
							list.targetQQID[k] = 0;
							list.number[k] = 0;
							CQ_sendGroupMsg(ac, fromGroup, Error);
							return EVENT_BLOCK;
						}
						connections.getintegral(master.QQID, &data);
						connections.changeintegral(master.QQID, master.ransom*0.8, 1);
						connections.getintegral(fromQQ, &data);
						connections.changeintegral(fromQQ, -master.ransom, 1);
						master.QQID = fromQQ;
						master.ransom = master.ransom + 100;
						time(&master.Time);
						if (connections.buyslave(targetQQID, fromQQ, master.ransom, master.Time) == 0)//����Ŀ���master
						{
							list.QQID[k] = 0;
							list.targetQQID[k] = 0;
							list.number[k] = 0;
							CQ_sendGroupMsg(ac, fromGroup, Error);
							return EVENT_BLOCK;
						}
						memset(sendmsg, 0, sizeof(sendmsg));
						sprintf(sendmsg, "����[CQ:at,qq=%lld]�ɹ�!", targetQQID);
						CQ_sendGroupMsg(ac, fromGroup, sendmsg);
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						list.number[k] = 0;
						return EVENT_BLOCK;
					}
					else//�Է�û��master
					{
						if (connections.changeprop(fromQQ, 2, 0) != 1)
						{
							list.QQID[k] = 0;
							list.targetQQID[k] = 0;
							list.number[k] = 0;
							CQ_sendGroupMsg(ac, fromGroup, Error);
							return EVENT_BLOCK;
						}
						price = targetdata.integral + 100;//������Ҫ�ļ۸�
						connections.getintegral(fromQQ, &data);
						connections.changeintegral(fromQQ, -price, 1);
						connections.changeintegral(targetQQID, 0.5*price, 1);
						master.QQID = fromQQ;
						master.ransom = price + 100;
						time(&master.Time);
						if (connections.buyslave(targetQQID, fromQQ, master.ransom, master.Time) == 0)//���ǶԷ���master
						{
							list.QQID[k] = 0;
							list.targetQQID[k] = 0;
							list.number[k] = 0;
							CQ_sendGroupMsg(ac, fromGroup, Error);
							return EVENT_BLOCK;
						}
						memset(sendmsg, 0, sizeof(sendmsg));
						sprintf(sendmsg, "����[CQ:at,qq=%lld]�ɹ�!", list.targetQQID[k] = 0);
						CQ_sendGroupMsg(ac, fromGroup, sendmsg);
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						list.number[k] = 0;
						return EVENT_BLOCK;
					}
				}
				else if (!strcmp(msg, "0"))
				{
					list.QQID[k] = 0;
					list.targetQQID[k] = 0;
					list.number[k] = 0;
					list.integral[k] = 0;
					CQ_sendGroupMsg(ac, fromGroup, "��ȡ���˹���");
					return EVENT_BLOCK;
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, "����1ȷ�ϣ�����0ȡ����");
					return EVENT_BLOCK;
				}
			}
			else if (list.number[k] == 0)//����
			{
				if (!strcmp(msg, "1"))//ָ��Ϊ1
				{
					targetQQID = list.targetQQID[k];//ȡ�����е�Ŀ��
					connections.getintegral(targetQQID, &targetdata);
					if (connections.getmaster(&master, targetQQID) == 0)//��ȡ�Է���master
					{
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					if (master.QQID != 0)//����Է���master
					{
						connections.changeintegral(master.QQID, master.ransom*0.8, 1);
						connections.changeintegral(fromQQ, -master.ransom, 1);
						master.QQID = fromQQ;
						master.ransom = master.ransom + 100;
						time(&master.Time);
						if (connections.buyslave(targetQQID, fromQQ, master.ransom, master.Time) == 0)//����Ŀ���master
						{
							list.QQID[k] = 0;
							list.targetQQID[k] = 0;
							CQ_sendGroupMsg(ac, fromGroup, Error);
							return EVENT_BLOCK;
						}
						memset(sendmsg, 0, sizeof(sendmsg));
						sprintf(sendmsg, "����[CQ:at,qq=%lld]�ɹ�!", targetQQID);
						CQ_sendGroupMsg(ac, fromGroup, sendmsg);
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						return EVENT_BLOCK;
					}
					else//�Է�û��master
					{
						price = targetdata.integral + 100;//������Ҫ�ļ۸�
						connections.changeintegral(fromQQ, -price, 1);
						connections.changeintegral(targetQQID, 0.5*price, 1);
						master.QQID = fromQQ;
						master.ransom = price + 100;
						time(&master.Time);
						if (connections.buyslave(targetQQID, fromQQ, master.ransom, master.Time) == 0)//���ǶԷ���master
						{
							list.QQID[k] = 0;
							list.targetQQID[k] = 0;
							CQ_sendGroupMsg(ac, fromGroup, Error);
							return EVENT_BLOCK;
						}
						memset(sendmsg, 0, sizeof(sendmsg));
						sprintf(sendmsg, "����[CQ:at,qq=%lld]�ɹ�!", list.targetQQID[k] = 0);
						CQ_sendGroupMsg(ac, fromGroup, sendmsg);
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						return EVENT_BLOCK;
					}
				}
				else if (!strcmp(msg, "0"))
				{
					list.QQID[k] = 0;
					list.targetQQID[k] = 0;
					list.number[k] = 0;
					list.integral[k] = 0;
					CQ_sendGroupMsg(ac, fromGroup, "��ȡ���˹���");
					return EVENT_BLOCK;
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, "����1ȷ�ϣ�����0ȡ����");
					return EVENT_BLOCK;
				}
			}
		}
	}
	if (strlen(msg) > 100)
	{
		return EVENT_BLOCK;
	}
	if ((!strcmp(msg, "ǩ��")) || (strstr(msg, "sign") != NULL))
	{
		//_i64toa(fromQQ, strQQID, 10);
		/*if (connections.getintegral(fromQQ, &data) == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		time(&Time);
		localtime_s(&sysTime1, &Time);
		sprintf_s(date, "%i-%i-%i", sysTime1.tm_year + 1900, sysTime1.tm_mon + 1, sysTime1.tm_mday);
		if (!strcmp(date,data.date))
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg,"[CQ:at,qq=%s]\n������Ѿ�ǩ����齱���ˣ��벻Ҫ�ظ�ǩ��!",strQQID );
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			return EVENT_BLOCK;
		}
		else
		{
			n = connections.listsign(fromQQ, fromGroup, date);
			if (n == 0)
			{
				number = connections.changeintegral(fromQQ, 80, 1);
				connections.getintegral(fromQQ, &data);
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%s]\nǩ���ɹ�\n���Ǳ�Ⱥ��%i��ǩ�����ˣ���Ļ���+%i�������ڵĻ���Ϊ��%i", strQQID, n + 1, number, data.integral);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				connections.changedate(fromQQ, fromGroup, date);
				return EVENT_BLOCK;
			}
			else if (n == 1)
			{
				number = connections.changeintegral(fromQQ, 70, 1);
				connections.getintegral(fromQQ, &data);
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%s]\nǩ���ɹ�\n���Ǳ�Ⱥ��%i��ǩ�����ˣ���Ļ���+%i�������ڵĻ���Ϊ��%i", strQQID, n + 1, number, data.integral);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				connections.changedate(fromQQ, fromGroup, date);
				return EVENT_BLOCK;
			}
			else if (n == 2)
			{
				number = connections.changeintegral(fromQQ, 60, 1);
				connections.getintegral(fromQQ, &data);
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%s]\nǩ���ɹ�\n���Ǳ�Ⱥ��%i��ǩ�����ˣ���Ļ���+%i�������ڵĻ���Ϊ��%i", strQQID, n + 1, number, data.integral);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				connections.changedate(fromQQ, fromGroup, date);
				return EVENT_BLOCK;
			}
			number = connections.changeintegral(fromQQ, 50, 1);
			connections.getintegral(fromQQ, &data);
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:at,qq=%s]\nǩ���ɹ�\n���Ǳ�Ⱥ��%i��ǩ�����ˣ���Ļ���+%i�������ڵĻ���Ϊ��%i", strQQID, n + 1, number, data.integral);
			CQ_sendGroupMsg(ac,fromGroup,sendmsg);
			connections.changedate(fromQQ, fromGroup, date);*/
		CQ_sendGroupMsg(ac, fromGroup, connections.sign(fromQQ, fromGroup));
		return EVENT_BLOCK;
	}
	else if ((strstr(msg, "����һ��")!=NULL)&fromQQ == 3047964704)
	{
		strcpy(tips, strstr(msg, "��") + 2);
		if (connections.changeprop(fromQQ, atoi(tips), 1) == 1)
			CQ_sendGroupMsg(ac, fromGroup, "��ӳɹ���");
		else
		{
			CQ_sendGroupMsg(ac, fromGroup, "���ʧ�ܣ�");
		}
		return EVENT_BLOCK;
	}
	else if ((strstr(msg, "����") != NULL)&(fromQQ == 3047964704))
	{
		strcpy(targetstrQQID, strstr(msg, "��") + 2);
		targetQQID = atoll(targetstrQQID);
		strcpy(tips, strstr(msg, "��") + 2);
		if (connections.changeintegral(atoll(targetstrQQID), atoi(tips), 0) != 0)
			CQ_sendGroupMsg(ac, fromGroup, "�����ɹ���");
		else
			CQ_sendGroupMsg(ac, fromGroup, Error);
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "��������Ԥ��"))
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "user:%lld\nkind:1\ntype:weather\naction:add\nsendby:soul", fromGroup);
		if (atoi(connections.ask(sendmsg)))
		{
			CQ_sendGroupMsg(ac, fromGroup, thanksforweather);
			return EVENT_BLOCK;
		}
		else
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
	}
	else if (!strcmp(msg, "�ر�����Ԥ��"))
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "user:%lld\nkind:1\ntype:weather\naction:delete\nsendby:soul", fromGroup);
		if (atoi(connections.ask(sendmsg)))
		{
			CQ_sendGroupMsg(ac, fromGroup, sorryforweather);
			return EVENT_BLOCK;
		}
		else
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
	}
	else if (!strcmp(msg, "�鿴����Ԥ��"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "����֪����ӭ����");
		recData = connections.ask("city:�人\ntype:weather\naction:get\nsendby:soul");
		if (recData == NULL)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
		}
		else
		{
			CQ_sendGroupMsg(ac, fromGroup, recData);
		}
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "ά������"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "���������ʲô���������ʲô���飬����ͨ�����¼����������з���\n1.ά����Աqq:3047964704\n2.�����ʼ���3047964704��qq����\n3.���ҵ�˵˵������������");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "���"))
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "user:%lld\ntype:school_card\naction:delete\nsendby:soul", fromQQ);
		CQ_sendGroupMsg(ac, fromGroup, connections.ask(sendmsg));
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "����Ԥ��"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "��qqȺ��˽���з��Ϳ�������Ԥ�����ɶ�������֪������Ԥ�������͹ر�����Ԥ������ȡ�����ģ����Ͳ鿴����Ԥ����������ѯ����������\n����֪����ӭ����");
	}
	else if (!strcmp(msg, "������Ⱥ"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "˽���ҷ���'������Ⱥ��QQȺ�ţ�'���ɣ���Ҫ�������ź�����Ŷ��");
	}
	else if (!strcmp(msg, "����֪��"))
	{
		CQ_sendGroupMsg(ac, fromGroup, wulizhiyin);
	}
	else if (strstr(msg, "����") != NULL)
	{
		for (k = 0; k < strlen(msg); k++)
		{
			if (msg[k] > 47 & msg[k] < 58)
				break;
		}
		if (k == strlen(msg) && !strcmp(msg, "����"))
		{
			CQ_sendGroupMsg(ac, fromGroup, "��������Ҫ����ĺ���qq,���磺����xxxxxxxxxxx");//��װ���ǵ�һ��
			return EVENT_BLOCK;
		}
		else if (k == strlen(msg))
		{
			return EVENT_BLOCK;
		}
		else
		{
			memset(targetstrQQID, 0, sizeof(targetstrQQID));
			for (i = 0; k < strlen(msg); k++, i++)//ȡ����������
			{
				if (msg[k] < 48 | msg[k]>57)
				{
					break;
				}
				targetstrQQID[i] = msg[k];
			}
			targetQQID = atoll(targetstrQQID);
			if (targetQQID == fromQQ)
			{
				CQ_sendGroupMsg(ac, fromGroup, "�㲻�ܹ������Լ�!");
				return EVENT_BLOCK;
			}
			//fromQQclass.GetGroupMemberInfo(ac, fromGroup, targetQQID, targetQQinfo);//ȡĿ��QQ��Ϣ(�ù������ִ������⣬ͣ��) mark
			if (connections.getmaster(&master, fromQQ) == 0)//��ȡ�Լ���master
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				return EVENT_BLOCK;
			}
			else
			{
				if (master.QQID == targetQQID)
				{
					CQ_sendGroupMsg(ac, fromGroup, "���޷������������!����Ҫ������Լ���");
					return EVENT_BLOCK;
				}
			}
			connections.getintegral(fromQQ, &data);
			connections.getintegral(targetQQID, &targetdata);
			if (connections.getmaster(&master, targetQQID) == 0)//��ȡĿ���master
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				return EVENT_BLOCK;
			}
			if (master.QQID == fromQQ)
			{
				CQ_sendGroupMsg(ac, fromGroup, "�벻Ҫ�ظ������Լ���ū����");
				return EVENT_BLOCK;
			}
			time(&Time);
			localtime_s(&sysTime1, &Time);
			localtime_s(&sysTime2, &master.Time);
			//sprintf_s(date, "%i-%i-%i", sysTime1.tm_year + 1900, sysTime1.tm_mon + 1, sysTime1.tm_mday);
			if (sysTime1.tm_mday == sysTime2.tm_mday)
			{
				if (connections.changeprop(fromQQ, 2, 2) == 1)
				{
					CQ_sendGroupMsg(ac, fromGroup, "�Է����ڱ����ڣ���������㽫ʹ��һ��ǿ�ƹ��򿨣�");
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, "�Է������ڱ����ڣ����޷�����");
					return EVENT_BLOCK;
				}
			}
			else
			{
				if (master.QQID >= 10000000)
				{
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "%lld", master.QQID);
					CQ_addLog(ac, CQLOG_DEBUG, "����", sendmsg);
					memset(sendmsg, 0, sizeof(sendmsg));
					if (data.integral < master.ransom)
					{
						sprintf(sendmsg, "����[CQ:at,qq=%lld]��Ҫ֧��%i��Ϊ��𣡵�����û����ô��Ļ��֣�", targetQQID, master.ransom);
						CQ_sendGroupMsg(ac, fromGroup, sendmsg);
						return EVENT_BLOCK;
					}
					sprintf(sendmsg, "����[CQ:at,qq=%lld]��Ҫ֧��%i��Ϊ���", targetQQID, master.ransom);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					CQ_sendGroupMsg(ac, fromGroup, "ȷ�Ϲ���������1��ȡ������������0");
					for (k = 0; k < 10; k++)
					{
						if (list.QQID[k] == 0)
							break;
					}
					list.QQID[k] = fromQQ;
					list.targetQQID[k] = targetQQID;
					return EVENT_BLOCK;
				}
				else
				{
					memset(sendmsg, 0, sizeof(sendmsg));
					price = int(targetdata.integral + 100);
					if (data.integral < price)
					{
						sprintf(sendmsg, "����[CQ:at,qq=%lld]��Ҫ֧��%i���֣�������û����ô��Ļ��֣�", targetQQID, price);
						CQ_sendGroupMsg(ac, fromGroup, sendmsg);
						return EVENT_BLOCK;
					}
					sprintf(sendmsg, "����[CQ:at,qq=%lld]��Ҫ֧��%i����!", targetQQID, price);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					CQ_sendGroupMsg(ac, fromGroup, "ȷ�Ϲ���������1��ȡ������������0");
					for (k = 0; k < 10; k++)
					{
						if (list.QQID[k] == 0)
							break;
					}
					list.QQID[k] = fromQQ;
					list.targetQQID[k] = targetQQID;
					return EVENT_BLOCK;
				}
			}
			if (master.QQID != 0)
			{
				if (master.ransom == 0)
				{
					CQ_sendGroupMsg(ac, fromGroup, Error);
					return EVENT_BLOCK;
				}
				memset(sendmsg, 0, sizeof(sendmsg));
				if (data.integral < master.ransom)
				{
					sprintf(sendmsg, "����[CQ:at,qq=%lld]��Ҫ֧��%i��Ϊ��𣡵�����û����ô��Ļ��֣�", targetQQID, master.ransom);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					return EVENT_BLOCK;
				}
				sprintf(sendmsg, "����[CQ:at,qq=%lld]��Ҫ֧��%i��Ϊ���", targetQQID, master.ransom);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				CQ_sendGroupMsg(ac, fromGroup, "ȷ�Ϲ���������1��ȡ������������0");
				for (k = 0; k < 10; k++)
				{
					if (list.QQID[k] == 0)
						break;
				}
				list.QQID[k] = fromQQ;
				list.targetQQID[k] = targetQQID;
				list.number[k] = 1;
				return EVENT_BLOCK;
			}
			else
			{
				memset(sendmsg, 0, sizeof(sendmsg));
				price = int(targetdata.integral + 100);
				if (data.integral < price)
				{
					sprintf(sendmsg, "����[CQ:at,qq=%lld]��Ҫ֧��%i���֣�������û����ô��Ļ��֣�", targetQQID, price);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					return EVENT_BLOCK;
				}
				sprintf(sendmsg, "����[CQ:at,qq=%lld]��Ҫ֧��%i����!", targetQQID, price);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				CQ_sendGroupMsg(ac, fromGroup, "ȷ�Ϲ���������1��ȡ������������0");
				for (k = 0; k < 10; k++)
				{
					if (list.QQID[k] == 0)
						break;
				}
				list.QQID[k] = fromQQ;
				list.targetQQID[k] = targetQQID;
				list.number[k] = 1;
				return EVENT_BLOCK;
			}
		}
	}
	else if (!strcmp(msg, "���"))
	{
		if (connections.getmaster(&master, fromQQ) == 0)//��ȡ�Լ���master
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		if (master.QQID != 0)//���������
		{
			if (master.ransom == 0)
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				return EVENT_BLOCK;
			}
			connections.getintegral(fromQQ, &data);
			if (data.integral < master.ransom)
			{
				sprintf(sendmsg, "����Ҫ֧��%i������Ϊ���,�����㲢û����ô����֣�", master.ransom);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				return EVENT_BLOCK;
			}
			else
			{
				sprintf(sendmsg, "��֧����%i������Ϊ���", master.ransom);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				connections.changeintegral(master.QQID, master.ransom, 1);
				time(&master.Time);
				if (connections.buyslave(fromQQ, 0, 0, master.Time) == 0)//ɾ���Լ������˼�¼
				{
					CQ_sendGroupMsg(ac, fromGroup, Error);
					return EVENT_BLOCK;
				}
				connections.changeintegral(fromQQ, -master.ransom, 1);	
				CQ_sendGroupMsg(ac, fromGroup, "��سɹ���");
				return EVENT_BLOCK;
			}
		}
		else
		{
			CQ_sendGroupMsg(ac, fromGroup, "��������������Ҫ����");
			return EVENT_BLOCK;
		}
	}
	else if (!strcmp(msg, "��ѯ") != NULL)
	{
		connections.getintegral(fromQQ, &data);
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf(sendmsg, "�����ڵĻ���Ϊ��%i", data.integral);
		CQ_sendGroupMsg(ac,fromGroup,sendmsg);
		CQ_sendGroupMsg(ac, fromGroup, connections.listprop(fromQQ));
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "�齱"))
	{
		i = connections.changeprop(fromQQ, 4, 0);
		if (i == 1)//�ɹ�
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:raffle\nsendby:soul", fromQQ);
			recData = connections.ask(sendmsg);
			if (recData == NULL)
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				return EVENT_BLOCK;
			}
			else
			{
				switch (atoi(recData))
				{
				case -1:
					if (connections.changeprop(fromQQ, 1, 1) != 1)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "[CQ:at,qq=%lld]���������Ϳ�", fromQQ);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case -2:
					if (connections.changeprop(fromQQ, 2, 1) != 1)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "[CQ:at,qq=%lld]������ǿ�ƹ���", fromQQ);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case -3:
					if (connections.changeprop(fromQQ, 3, 1) != 1)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "[CQ:at,qq=%lld]�����˱�����", fromQQ);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case -4:
					if (connections.changeprop(fromQQ, 4, 1) != 1)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "[CQ:at,qq=%lld]�����˳齱��", fromQQ);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case 4:
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "[CQ:at,qq=%lld]������˼����ʲô��û�鵽���´������ɣ�", fromQQ);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case 5:
					number = connections.changeintegral(fromQQ, 70, 1);
					if (number == 0)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					connections.getintegral(fromQQ, &data);
					sprintf(sendmsg, "[CQ:at,qq=%lld]������%i����,������ӵ��%i����", fromQQ, number, data.integral, 1);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case 6:
					number = connections.changeintegral(fromQQ, 100, 1);
					if (number == 0)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					connections.getintegral(fromQQ, &data);
					sprintf(sendmsg, "[CQ:at,qq=%lld]������%i����,������ӵ��%i����", fromQQ, number, data.integral, 1);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case 7:
					number = connections.changeintegral(fromQQ, 120, 1);
					if (number == 0)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					connections.getintegral(fromQQ, &data);
					sprintf(sendmsg, "[CQ:at,qq=%lld]������%i����,������ӵ��%i����", fromQQ, number, data.integral, 1);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case 8:
					number = connections.changeintegral(fromQQ, 150, 1);
					if (number == 0)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					connections.getintegral(fromQQ, &data);
					sprintf(sendmsg, "[CQ:at,qq=%lld]������%i����,������ӵ��%i����", fromQQ, number, data.integral, 1);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case 9:
					number = connections.changeintegral(fromQQ, 50, 1);
					if (number == 0)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					connections.getintegral(fromQQ, &data);
					sprintf(sendmsg, "[CQ:at,qq=%lld]������%i����,������ӵ��%i����", fromQQ, number, data.integral, 1);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				}
				return EVENT_BLOCK;
			}
		}
		else if (i == 0)//ʧ��
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		else if (i == -1)
		{
			CQ_sendGroupMsg(ac, fromGroup, "��û�г齱���������̵�һ��齱����");
			return EVENT_BLOCK;
		}
		
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "��") != NULL)
	{
		if (!strcmp(msg, "�ͷ�"))
		{
			CQ_sendGroupMsg(ac, fromGroup, "��������Ҫ�ͷ��ĺ���qq�Լ�����,���磺�ͷ�xxxxxxxxxxx���裬�������ҳͷ�����ЩŶ��");//��װ���ǵ�һ��
			return EVENT_BLOCK;
		}
		if (strstr(msg, "��") != NULL)
		{
			CQ_sendGroupMsg(ac, fromGroup, "Ŀǰֻ�����衢���衢�ϵغʹ�ŮװŶ��");
			return EVENT_BLOCK;
		}
		for (k = 0; k < strlen(msg); k++)
		{
			if (msg[k] > 47 & msg[k] < 58)
				break;
		}
		if (k == strlen(msg))
		{
			return EVENT_IGNORE;
		}
		else
		{
			for (i = 0; k < strlen(msg); k++, i++)//ȡ����������
			{
				if (msg[k] < 48 | msg[k]>57)
				{
					break;
				}
				targetstrQQID[i] = msg[k];
			}
			targetQQID=atoll(targetstrQQID);
			if (connections.getmaster(&master, targetQQID) == 0)
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				return EVENT_BLOCK;
			}
			if (master.QQID == fromQQ)
			{
				if (strstr(msg, "Ůװ") != NULL)
				{
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "[CQ:at,qq=%lld]�����˵������±��ȴ�����Ůװ���������ߵĿ���Χ�۵����ˡ�", targetQQID);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				}
				else if (strstr(msg, "����") != NULL)
				{
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "[CQ:at,qq=%lld]�����˵������¶̶̵�����һ֧�裬���Ǿ��������ˣ����Ь��û���Ļ��Ǿ͸����ˡ�", targetQQID);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				}
				else if (strstr(msg, "����") != NULL)
				{
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "[CQ:at,qq=%lld]�����˵������¸߸�һ����������һ��ĸ�������ڣ����ڵ��ھӶ�����������������", targetQQID);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				}
				else if (strstr(msg, "�ϵ�") != NULL)
				{
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "[CQ:at,qq=%lld]�����˵������°ѷ���ĵذ�ȫ������һ�飬���ĵذ��������⣡", targetQQID);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				}
			}
			else
			{
				CQ_sendGroupMsg(ac, fromGroup, "�Է��������ū�����������");
			}
			return EVENT_BLOCK;
		}
	}
	else if ((strstr(msg, "�鿴") != NULL)&(strstr(msg, "�°�") == NULL))
	{
		if (!strcmp(msg, "�鿴"))
		{
			CQ_sendGroupMsg(ac, fromGroup, "��������Ҫ����Ĳ鿴qq,���磺�鿴xxxxxxxxxxx");
			return EVENT_BLOCK;
		}
		for (k = 0; k < strlen(msg); k++)
		{
			if (msg[k] > 47 & msg[k] < 58)
				break;
		}
		if (k == strlen(msg))
		{
			return EVENT_BLOCK;
		}
		else
		{
			memset(targetstrQQID, 0, sizeof(targetstrQQID));
			for (i = 0; k < strlen(msg); k++, i++)//ȡ����������
			{
				if (msg[k] < 48 | msg[k]>57)
				{
					break;
				}
				targetstrQQID[i] = msg[k];
			}
			targetQQID = atoll(targetstrQQID);
			if (targetQQID < 100000)
			{
				return EVENT_BLOCK;
			}
			//fromQQclass.GetGroupMemberInfo(ac, fromGroup, targetQQID, targetQQinfo);//ȡĿ��QQ��Ϣ
			/*if (targetQQinfo.QQID != targetQQID)
			{
				CQ_sendGroupMsg(ac, fromGroup, "��鿴��QQ������߲��ڴ�Ⱥ");
				return EVENT_BLOCK;
			}*/
			if (connections.getmaster(&master, targetQQID) == 0)
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				return EVENT_BLOCK;
			}
			if (master.QQID != 0)
			{
				//fromQQclass.GetGroupMemberInfo(ac, fromGroup, master.QQID, masterQQinfo);
				/*if (masterQQinfo.QQID != master.QQID)
				{
					CQ_sendGroupMsg(ac, fromGroup, "�Է������˲����ڱ�Ⱥ�����޷�Ϊ���ṩ����Ϣ");
				}*/
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%lld]��������[CQ:at,qq=%lld]", targetQQID, master.QQID);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			}

			connections.getintegral(targetQQID, &targetdata);
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:at,qq=%lld]�Ļ���Ϊ:%i", targetQQID, targetdata.integral);
			CQ_sendGroupMsg(ac,fromGroup,sendmsg);
			if (connections.listslave(slavelist, targetQQID) == 0)
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				return EVENT_BLOCK;
			}
			for (n = 0; n < 10; n++)
			{
				if (slavelist[n] != 0)
				{
					//fromQQclass.GetGroupMemberInfo(ac, fromGroup, slavelist[n], targetQQinfo);
					/*if (targetQQinfo.QQID != slavelist[n])
					{
						CQ_sendGroupMsg(ac, fromGroup, "�Է�������ū�������ڱ�Ⱥ�����޷�Ϊ���ṩ����Ϣ");
						continue;
					}*/
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "[CQ:at,qq=%lld]��ū����[CQ:at,qq=%lld]:%lld", targetQQID, slavelist[n],slavelist[n]);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);			
				}
			}
			return EVENT_BLOCK;
		}
	}
	else if (strstr(msg, "����") != NULL)
	{
		for (k = 0; k < strlen(msg); k++)
		{
			if (msg[k] > 47 & msg[k] < 58)
				break;
		}
		if (k == strlen(msg))
		{
			return EVENT_BLOCK;
		}
		else
		{
			for (i = 0; k < strlen(msg); k++, i++)//ȡ����������
			{
				if (msg[k] < 48 | msg[k]>57)
				{
					break;
				}
				targetstrQQID[i] = msg[k];
			}
			targetQQID = atoll(targetstrQQID);
		}
		for (; k < strlen(msg); k++)
		{
			if (msg[k] > 47 & msg[k] < 58)
				break;
		}
		for (i = 0; k < strlen(msg); k++, i++)//ȡ����������
		{
			if (msg[k] < 48 | msg[k]>57)
			{
				break;
			}
			strintegral[i] = msg[k];
		}
		if (atoi(strintegral) <= 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, "���͵Ļ��ֲ���С�ڻ����0");
			return EVENT_BLOCK;
		}
		//readwhere(fromQQ, &data, sizeof(data), "sign");
		connections.getintegral(fromQQ, &data);
		if (data.integral < atoi(strintegral))
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "��û��%i���֣���ֻ��%i����", atoi(strintegral), data.integral);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			return EVENT_BLOCK;
		}
		else
		{
			if (connections.changeprop(fromQQ, 1, 2) == -1)
			{
				CQ_sendGroupMsg(ac, fromGroup, "��û�п���ʹ�õ����Ϳ���");
				return EVENT_BLOCK;
			}
			else if (connections.changeprop(fromQQ, 1,2) == 0)
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				return EVENT_BLOCK;
			}
			for (n = 0; n < 10; n++)
			{
				if (list.QQID[n] == 0)
					break;
				if (n == 9)
				{
					CQ_sendGroupMsg(ac, fromGroup, "��������æ�����Ժ����ԣ�");
					return EVENT_BLOCK;
				}
			}
			list.QQID[n] = fromQQ;
			list.targetQQID[n] = targetQQID;
			list.number[n] = 2;
			list.integral[n] = atoi(strintegral);
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "�㽫����[CQ:at,qq=%lld] %i���֣�", targetQQID, atoi(strintegral));
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			CQ_sendGroupMsg(ac, fromGroup, "ȷ����������1��ȡ������0");
		}
	}
	else if (!strcmp(msg, "ʹ�ñ�����"))
	{ 
		i = connections.changeprop(fromQQ, 3, 0);
		if (i == 1)
		{
			if (connections.getmaster(&master, fromQQ) == 0)
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				CQ_addLog(ac, CQLOG_ERROR, "����ʹ��", "�����������ģ�");
				return EVENT_BLOCK;
			}
			time(&master.Time);
			if (connections.buyslave(fromQQ, master.QQID, master.ransom, master.Time) == 0)
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				CQ_addLog(ac, CQLOG_ERROR, "����ʹ��", "�����������ģ�");
				return EVENT_BLOCK;
			}
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:at,qq=%lld]ʹ�ñ������ɹ�!", fromQQ);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
		}
		else if (i == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		else if (i == -1)
		{
			CQ_sendGroupMsg(ac, fromGroup, "��û�б���������ȥ�̵�һ�һ�ű������ɣ�");
			return EVENT_BLOCK;
		}
	}
	else if (strstr(msg, "����") != NULL)
	{
		for (k = 0; k < strlen(msg); k++)
		{
			if (msg[k] > 47 & msg[k] < 58)
				break;
		}
		if (k == strlen(msg))
		{
			CQ_sendGroupMsg(ac, fromGroup, "��������Ҫ���ŵ�ū��qq,���磺����xxxxxxxxxxx");//��װ���ǵ�һ��
			return EVENT_BLOCK;
		}
		else
		{
			for (i = 0; k < strlen(msg); k++, i++)//ȡ����������
			{
				if (msg[k] < 48 | msg[k]>57)
				{
					break;
				}
				targetstrQQID[i] = msg[k];
			}
			targetQQID = atoll(targetstrQQID);
		}
		if (targetQQID == 0)
		{
			return EVENT_BLOCK;
		}
		if (connections.listslave(slavelist, fromQQ) == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		for (n = 0; n < 10; n++)
		{
			if (slavelist[n] == targetQQID)
			{
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "��ȷ��Ҫ����[CQ:at,qq=%lld]��?\nȷ����������1��ȡ������0��", targetQQID);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				for (n = 0; n < 10; n++)
				{
					if (list.QQID[n] == 0)
						break;
					if (n == 9)
					{
						CQ_sendGroupMsg(ac, fromGroup, "��������æ�����Ժ����ԣ�");
						return EVENT_BLOCK;
					}
				}
				list.QQID[n] = fromQQ;
				list.targetQQID[n] = targetQQID;
				list.number[n] = 3;
				return EVENT_BLOCK;
			}
		}
		CQ_sendGroupMsg(ac, fromGroup, "�Է��������ū��!");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "��ʼ"))
	{
		for (sum = 0; sum < 5; sum++)
		{
			if (gameroom[sum].memberinhere(fromQQ) == 0)
			{
				if (gameroom[sum].number >= 6)
				{
					if (gameroom[sum].startgame(ac) == -1)
					{
						CQ_sendGroupMsg(ac, fromGroup, "��Ϸ�ѿ�ʼ���벻Ҫ�ظ���ʼ��");
						return EVENT_BLOCK;
					}
					else
					{
						CQ_sendGroupMsg(ac, fromGroup, "��Ϸ��ʼ�ˣ��뿴�ҵ�˽�Ľ�����Ϸ��");
					}
					if (FALSE == gameroom[sum].starttimer())
					{
						CQ_addLog(ac, CQLOG_ERROR, "game", "can't set timer");
					}
					return EVENT_BLOCK;
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, "�������������޷���ʼ��Ϸ��");
					return EVENT_BLOCK;
				}
			}
		}
		CQ_sendGroupMsg(ac, fromGroup, "�㲻���κ�һ������ķ��������޷���ʼ��Ϸ!");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "�˳�"))
	{
		for (sum = 0; sum < 5; sum++)
		{
			if (gameroom[sum].memberinhere(fromQQ) > -1)
			{
				if (gameroom[sum].runstate == 1)
				{
					CQ_sendGroupMsg(ac, fromGroup, "�÷����ѿ�ʼ��Ϸ���㲻���˳�");
					return EVENT_BLOCK;
				}
				if (gameroom[sum].number == 1)
				{
					CQ_sendGroupMsg(ac, fromGroup, "���������Ϊ���˱��ݻ���");
					gameroom[sum].gameover();
					return EVENT_BLOCK;
				}
				if (gameroom[sum].memberinhere(fromQQ) == 0)
				{
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "����%i�ķ����˳��˷��䣬�������Ϊ:%lld", gameroom[sum].roomid, gameroom[sum].memberlist(1)[1]);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "���Ϊ�˷���%i�ķ�����", gameroom[sum].roomid);
					CQ_sendPrivateMsg(ac, gameroom[sum].memberlist(1)[1], sendmsg);
				}
				if (gameroom[sum].deletemember(fromQQ) == 1)
				{
					CQ_sendGroupMsg(ac, fromGroup, "���˳��˷���");
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, Error);
					return EVENT_BLOCK;
				}
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "��һ������˳��������ڵķ���,������ķ�����%i����ң�", gameroom[sum].number);
				CQ_sendPrivateMsg(ac, gameroom[sum].memberlist(1)[0], sendmsg);
				return EVENT_BLOCK;
			}
		}
		CQ_sendGroupMsg(ac, fromGroup, "�㲻�����κ�һ����Ϸ�����У�");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "����˭���Ե�"))
	{
		
		for (sum = 0; sum < 5; sum++)
		{
			if (gameroom[sum].memberinhere(fromQQ) > -1)
			{
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%lld]�����˳������ڵ���Ϸ���䣡", fromQQ);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				return EVENT_BLOCK;
			}
		}
		for (sum = 0; sum < 5; sum++)
		{
			if (gameroom[sum].roomid == 0)
			{
				gameroom[sum].gameover();
				gameroom[sum].putac(ac);
				srand(time(NULL));
				gameroom[sum].roomid = 10000 + rand() % 90000;
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "�㴴���ķ����Ϊ��%i,��Ⱥ�﷢�ͷ���ż��ɼ��뷿�䣡�����˳������뿪���䣡�������뿪ʼ��ʼ��Ϸ��", gameroom[sum].roomid);
				gameroom[sum].addmember(fromQQ);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				return EVENT_BLOCK;
			}
		}
		CQ_sendGroupMsg(ac, fromGroup, "��Ǹ�����������Ѿ��ﵽ���ޣ����޷��������䡣");
	}
	else if (!strcmp(msg, "Ѱ�ҷ���"))
	{
		x = 0;
		for (sum = 0; sum < 5; sum++)
		{
			if (gameroom[sum].roomid == 0)
			{
				x++;
				continue;
			}
			memset(sendmsg, 0, sizeof(sendmsg));
			if (gameroom[sum].runstate)
				sprintf(sendmsg, "%i�ŷ���:%i(������Ϸ��)", sum + 1, gameroom[sum].roomid);
			else
				sprintf(sendmsg, "%i�ŷ���:%i(�ȴ���)\n����Ϊ:%lld", sum + 1, gameroom[sum].roomid, gameroom[sum].memberlist(1)[0]);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
		}
		sprintf_s(tips, "%i", x);
		CQ_addLog(ac, CQLOG_DEBUG, "˭���Ե�test", tips);
		if (x == 5)
		{
			CQ_sendGroupMsg(ac, fromGroup, "Ŀǰû�п��Լ���ķ��䣡");
		}
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "˭���Ե�"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "����:����˭���Ե׼��ɴ������䡣");
		CQ_sendGroupMsg(ac, fromGroup, "����:Ѱ�ҷ��伴�ɲ鿴�����б�");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "�˵�"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "Ŀǰ�Ĺ�����:\n˭���Ե�\n��������ϵͳ\nǩ���齱ϵͳ\n����ϵͳ\n��ֵ\n����֪��\n������Щ��ѯ�����������");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "��ֵ"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "Ϊ����ɵ���ܹ���׳�ɳ���ɵ�����ò������Ƴ���ֵ���ܣ����ҷ�����Ԫ������ܻ�û��ֽ�����һԪһ�ٷ֣��׳���������İٷ�Ŷ��");
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "��������ϵͳ") != NULL)
	{
		CQ_sendGroupMsg(ac, fromGroup, "��������ϵͳ���й�����أ��鿴����ѯ������,�ͷ��ȹ��ܣ�������Щ��ѯ����������ݡ�");
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "ǩ���齱ϵͳ") != NULL)
	{
		CQ_sendGroupMsg(ac, fromGroup, "ÿ�����ǩ��һ��Ŷ��ǩ��ֻ�ܻ�û��֣����̵�һ��齱��Ȼ��齱���Ի�õ��߻��߻��֣���Ҳ�п���ʲô���ò���Ŷ��");
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "����ϵͳ") != NULL)
	{
		CQ_sendGroupMsg(ac, fromGroup, "Ŀǰֻ��ͨ���齱����ȡ���ߣ�ǿ�ƹ��򿨿������ڹ����ڱ������е���ң����Ϳ������������ͱ��˻��֣��������������Լ����뱣���ڡ�����ʹ�ñ���������ʹ�ã���ѯ����֪���Լ�����Щ���ߣ�");
		return EVENT_BLOCK;
	}
/*else if ((strstr(msg, "����ͼ") != NULL)&(strstr(msg, "1839538956") != NULL))
	{
		if (gameroom.state)
		{
			CQ_sendGroupMsg(ac, fromGroup, "ɵ����æ���أ����Ժ�������");
			return EVENT_BLOCK;
		}
		else
		{
			CQ_sendGroupMsg(ac, fromGroup, "�������ͼģʽ���ش���Ŀ�ǵð�����Ŷ��һ�����ֻ����15�֡�");
			CQ_sendGroupMsg(ac, fromGroup, "ʵ�ڲ²��������԰�����˵��ʾ������һ��Ŷ�������ǿɾ�û�л��ֽ�����");
			srand(time(NULL));
			gameroom.number = rand() % 401;
			memset(where, 0, sizeof(where));
			sprintf(where, "f:/mingxing1/%i.txt",gameroom.number);
			fopen_s(&fp, where, "r");
			if (fp == NULL)
			{
				CQ_sendGroupMsg(ac, fromGroup, "����ͼ�����𻵣�����ϵά����Ա");
				return EVENT_BLOCK;
			}
			else
			{
				n = 0;
				while (EOF != fgetc(fp))
				{
					n++;
				}
				rewind(fp);
				fgets(gameroom.name, n + 2, fp);
				fclose(fp);
			}
			gameroom.difficulty = 7;
			gameroom.gametype = 1;
			gameroom.groupID = fromGroup;
			gameroom.state = 1;
			gameroom.rank = 1;
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "����С��ʾŶ����һ������Ϊ%i���ֵ�����", int(n / 2));
			CQ_sendGroupMsg(ac, fromGroup,sendmsg);
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:image,file=%s%i.jpg]", gameroom.name,gameroom.difficulty);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			return EVENT_BLOCK;
		}
	}
	else if ((strstr(msg, "1839538956") != NULL)&(fromGroup == gameroom.groupID))
	{
		if (strstr(msg, gameroom.name) != NULL)
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:at,qq=%lld]�ش���ȷ�����5����֣�", fromQQ);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			gameroom.difficulty = 7;
			if (changeintegral(fromQQ, 5) == 0)
			{
				memset(tips, 0, sizeof(tips));
				sprintf(tips, "Ϊ%lld����%i����ʧ�ܣ�", fromQQ, 5);
				CQ_addLog(ac, CQLOG_ERROR, "����ͼ", tips);
			}
			gameroom.rank++;
			if (gameroom.rank > 15)
			{
				CQ_sendGroupMsg(ac, fromGroup, "����ͼ��Ϸ������");
				memset(&gameroom, 0, sizeof(gameroom));
				return EVENT_BLOCK;
			}
			else
			{
				srand(time(NULL));
				gameroom.number = rand() % 401;
				memset(where, 0, sizeof(where));
				sprintf(where, "f:/mingxing1/%i.txt", gameroom.number);
				fopen_s(&fp, where, "r");
				if (fp == NULL)
				{
					CQ_sendGroupMsg(ac, fromGroup, "����ͼ�����𻵣�����ϵά����Ա");
					memset(&gameroom, 0, sizeof(gameroom));
					return EVENT_BLOCK;
				}
				else
				{
					n = 0;
					while (EOF != fgetc(fp))
					{
						n++;
					}
					rewind(fp);
					fgets(gameroom.name, n + 2, fp);
					fclose(fp);
				}
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "����С��ʾŶ����һ��%i���ֵ�����", n / 2);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:image,file=%s%i.jpg]", gameroom.name, gameroom.difficulty);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				return EVENT_BLOCK;
			}
		}
		else if (strstr(msg, "��һ��") != NULL)
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "�ðɣ���λ������%s!", gameroom.name);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			gameroom.rank++;
			if (gameroom.rank > 15)
			{
				CQ_sendGroupMsg(ac, fromGroup, "����ͼ��Ϸ������");
				memset(&gameroom, 0, sizeof(gameroom));
				return EVENT_BLOCK;
			}
			else
			{
				srand(time(NULL));
				gameroom.number = rand() % 401;
				memset(where, 0, sizeof(where));
				sprintf(where, "f:/mingxing1/%i.txt", gameroom.number);
				fopen_s(&fp, where, "r");
				if (fp == NULL)
				{
					CQ_sendGroupMsg(ac, fromGroup, "����ͼ�����𻵣�����ϵά����Ա");
					memset(&gameroom, 0, sizeof(gameroom));
					return EVENT_BLOCK;
				}
				else
				{
					n = 0;
					while (EOF != fgetc(fp))
					{
						n++;
					}
					rewind(fp);
					fgets(gameroom.name, n + 2, fp);
					fclose(fp);
				}
				gameroom.difficulty = 7;
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "����С��ʾŶ����һ��%i���ֵ�����", n / 2);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:image,file=%s%i.jpg]", gameroom.name, gameroom.difficulty);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				return EVENT_BLOCK;
			}
		}
		else if (strstr(msg, "��ʾ") != NULL)
		{
			if (gameroom.difficulty == 10)
			{
				gameroom.difficulty -= 1;
			}
			else if (gameroom.difficulty == 7)
			{
			}
			else if (gameroom.difficulty == 5)
			{
			}
			else if (gameroom.difficulty == 3)
			{
				CQ_sendGroupMsg(ac, fromGroup, "���Ѿ�������Ѷ��ˣ�Ҫ�Ǵ𲻳������ǸϿ���һ���ɣ�");
				return EVENT_BLOCK;
			}
			CQ_sendGroupMsg(ac, fromGroup, "�ðɣ��Ҿ����Խ��͵��ѶȰɣ�");
			gameroom.difficulty -= 2;
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:image,file=%s%i.jpg]", gameroom.name, gameroom.difficulty);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			return EVENT_BLOCK;
		}
		else
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:at,qq=%lld]�ش����", fromQQ);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			return EVENT_BLOCK;
		}
	}*/
	else if (!strcmp(msg, "�̵�"))
	{
		CQ_sendGroupMsg(ac, fromGroup, connections.ask("user:1839538956\ntype:prop\naction:shop\nsendby:soul"));
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "�һ�"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "���Ͷһ�xx������ʹ�û��ֶһ�����Ŷ��");
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "�һ�") != NULL)
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:exchange\nname:%s\nsendby:soul", fromQQ, strstr(msg, "��") + 2);
		CQ_sendGroupMsg(ac, fromGroup, connections.ask(sendmsg));
		return EVENT_BLOCK;
	}
	for (len = 0; len < strlen(msg); len++)
	{
		if (msg[len] < 47 | msg[len] > 58)
			goto aim;
	}
	if (atoi(msg) == 0)
		goto aim;
	//˭���Ե�
	for (sum = 0; sum < 5; sum++)
	{
		if (gameroom[sum].roomid == atoi(msg))
		{
			if (gameroom[sum].runstate == 1)
			{
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%lld]�÷����ѿ�ʼ��Ϸ���㲻�ܼ��룡",fromQQ);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				return EVENT_BLOCK;
			}
			for (int k = 0; k < 5; k++)
			{
				if (gameroom[k].memberinhere(fromQQ) > -1)
				{
					CQ_sendGroupMsg(ac, fromGroup, "�����˳������ڵķ���");
					return EVENT_BLOCK;
				}
			}
			gameroom[sum].addmember(fromQQ);
			if (gameroom[sum].memberinhere(fromQQ) > -1)
			{
				
			}
			else
			{
				CQ_sendGroupMsg(ac, fromGroup, "����ʧ�ܣ������Ի���ϵά����Ա����");
				return EVENT_BLOCK;
			}
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf_s(sendmsg, "����%i�ɹ�,�����˳������뿪���䣡", gameroom[sum].roomid);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "һ����Ҽ�������ķ��䣬������ķ�����%i�����", gameroom[sum].number);
			CQ_sendPrivateMsg(ac, gameroom[sum].memberlist(1)[0], sendmsg);
			return EVENT_BLOCK;
		}
	}

aim:

	if (strstr(msg, "ɵ��") != NULL)
	{
		if (strstr(msg, "����") != NULL)
		{
			shutup = 1;
			return EVENT_BLOCK;
		}
		else if (strstr(msg, "����") != NULL)
		{
			shutup = 0;
			return EVENT_IGNORE;
		}
		if (shutup == 1)
		{
			return EVENT_BLOCK;
		}
		else
		{
			return EVENT_IGNORE;
		}
	}
	else if ((strstr(msg, "at") != NULL) & (strstr(msg, "1839538956") != NULL))
	{
		if (shutup == 1)
		{
			return EVENT_BLOCK;
		}
		else
		{
			return EVENT_IGNORE;
		}
	}
	return EVENT_BLOCK;
}

/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {
	char sendmsg[1000];
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf(sendmsg, wulizhiyin);
	CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");
	CQ_sendPrivateMsg(ac, fromQQ, sendmsg);
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {
	char sendmsg[100];
	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_DENY, "");
	//}
	if (subType == 1)
	{
		CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	}
	else if (subType == 2)
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "user:%lld\ntype:validate\naction:confirm\nsendby:soul",fromGroup);
		if (atoi(connections.ask(sendmsg)))
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf_s(sendmsg, "groupQQ:%lld��������Ⱥͨ����", fromGroup);
			CQ_sendPrivateMsg(ac, 3047964704, sendmsg);
			CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
			CQ_sendGroupMsg(ac, fromGroup, wulizhiyin);
		}
		else
		{
			CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_DENY, "");
			sprintf_s(sendmsg, "groupQQ:%lld��������Ⱥ�ܾ���", fromGroup);
		}
	}
	return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "����menuA�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "����menuB�����������봰�ڣ����߽�������������", "", 0);
	return 0;
}
