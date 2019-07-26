/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
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
#define goodbye "大家再见，傻馒也要睡觉了，晚安哦！"
#define hi "大家好，人见人爱,花见花开的傻馒回来啦，有人想我了吗！"
#define thanksforweather "感谢订阅武理知音天气预报，每晚九点wuli傻馒准时为您播报明日武汉天气哦！"
#define sorryforweather "您已取订武理知音天气预报，是傻馒哪里做的不好吗?可以向qq:3047964704反馈或者在我的说说下面评论哦！"
#define Error "傻馒出了点小问题，请联系维护人员解决！"
#define wulizhiyin "大家好，这里是武理知音广播电台，我是主持人wuli傻馒，该电台由武理ai社(486712165)承办。\n目前功能有:\n天气预报\n邀请入群\n维护反馈\n菜单\n武理知音\n发送功能关键词了解更多哦(欢迎将我邀请进更多qq群)！"
int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;
//static makeroom room[5];
MMRESULT timer_id;
MMRESULT game_timer_id[5];
HHOOK keyboardHook = 0;
HHOOK g_hMouse = 0;
static game gameroom[5];//游戏房间
//轮询时钟
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
	connection connections;//与服务器连接的实例对象
	connections.init(ac);
	if ((number = atoi(connections.ask("t"))) == 1)//如果处于九点到九点零一分之间
	{
		char *a = NULL;
		char *list = NULL;
		memset(sendmsg,0,sizeof(sendmsg));
		memset(groupid, 0, sizeof(groupid));
		connections.getweatherlist(groupid, 1);
		i = 0;
		while (groupid[i] != 0)
		{
			a = connections.ask("city:武汉\ntype:weather\naction:get\nsendby:soul");
			CQ_sendGroupMsg(ac, groupid[i], a);
			i++;
		}
		connections.getweatherlist(groupid, 0);
		i = 0;
		while (groupid[i] != 0)
		{
			a = connections.ask("city:武汉\ntype:weather\naction:get\nsendby:soul");
			CQ_sendPrivateMsg(ac, groupid[i], a);
			i++;
		}
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

	//不可漏掉消息处理，不然程序会卡死
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
			Sleep(1);    //避免CPU全负载运行
	}
	UnhookWindowsHookEx(keyboardHook);
	CQ_addLog(ac, CQLOG_DEBUG, "hook", "exit");
}
*/

/* 
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
*/
CQEVENT(int32_t, __eventExit, 0)() {
	return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	connection connections;//与服务器连接的实例对象
	connections.init(ac);
	connections.ask("t");
	timeSetEvent(60000, 1, (LPTIMECALLBACK)onTimeFunc, DWORD(1), TIME_PERIODIC);//启动时钟函数，间隔为1分钟
	enabled = true;
	return 0;
}

/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
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
	connection connections;//与服务器连接的实例对象
	if (strstr(msg, "rich") != NULL)
	{
		return EVENT_BLOCK;
	}
	else if (sizeof(msg) > 40)
	{
		return EVENT_BLOCK;
	}
	connections.init(ac);
	//谁是卧底(暂时停用)
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
						CQ_sendPrivateMsg(ac, fromQQ, "你的指令错误！");
						return EVENT_BLOCK;
					}
					if (atoi(msg) > gameroom[sum].number)
					{
						CQ_sendPrivateMsg(ac, fromQQ, "你输入的玩家不存在!");
						return EVENT_BLOCK;
					}
					if (gameroom[sum].hadvote(fromQQ, atoi(msg) - 1))
					{
						CQ_sendPrivateMsg(ac, fromQQ, "投票成功");
					}
					else
					{
						CQ_sendPrivateMsg(ac, fromQQ, "不能重复投票或者修改投票！");
						return EVENT_BLOCK;
					}
					if ((x = gameroom[sum].check(0,ac)) == -1)
					{
						return EVENT_BLOCK;
					}
					return EVENT_BLOCK;								  //进行淘汰通知
				}
				else if (gameroom[sum].rank >= 0)
				{
					char sendmsg[200];
					memset(sendmsg,0,sizeof(sendmsg));
					strcpy(sendmsg, msg);
					if (0 == (x = gameroom[sum].hadsay(fromQQ, sendmsg, ac)))
					{
						CQ_sendPrivateMsg(ac, fromQQ, "还未轮到你发言!");
						return EVENT_BLOCK;
					}
					else if (x == -1)
					{
						CQ_sendPrivateMsg(ac, fromQQ, "你已经发言过了!");
						return EVENT_BLOCK;
					}
					return EVENT_BLOCK;
				}
			}
		}
	}
	if (!strcmp(msg, "开启天气预报"))
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
	else if (!strcmp(msg, "关闭天气预报"))
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
	else if (!strcmp(msg, "查看天气预报"))
	{
		recData = connections.ask("city:武汉\ntype:weather\naction:get\nsendby:soul");
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
	else if (!strcmp(msg, "武理知音"))
	{
		CQ_sendPrivateMsg(ac, fromQQ, wulizhiyin);
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "维护反馈"))
	{
		CQ_sendPrivateMsg(ac, fromQQ, "如果碰到了什么问题或是有什么建议，可以通过以下几个方法进行反馈\n1.维护人员qq:3047964704\n2.发送邮件至3047964704的qq邮箱\n3.在我的说说下面做出评论");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "菜单") || !strcmp(msg, "功能"))
	{
		CQ_sendGroupMsg(ac, fromQQ, "目前的功能有:\n谁是卧底\n好友买卖系统\n签到抽奖系统\n道具系统\n充值\n武理知音\n输入这些词询问我相关内容");
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "邀请入群"))
	{
		strcpy(tips, msg + 8);
		sprintf_s(sendmsg, "user:%s\ntype:validate\naction:add\nsendby:soul", tips);
		if (atoi(connections.ask(sendmsg)))
		{
			CQ_sendPrivateMsg(ac, fromQQ, "邀请确认成功，邀请我进群即可！");
		}
		else
		{
			CQ_sendPrivateMsg(ac, fromQQ, "啊哦，不知为何确认失败了，联系一下维护人员或者重试一下吧！");
			CQ_sendPrivateMsg(ac, fromQQ, "正确的格式是\n邀请入群xxxx\nxxxx为qq群的群号哦！");
		}
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "&#91;QQ红包"))
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "%lld给我发了红包哦，快来领红包啦！", fromQQ);
		CQ_sendPrivateMsg(ac, 3047964704, sendmsg);
		CQ_sendPrivateMsg(ac, fromQQ, "谢谢大佬的浇水，傻馒一定会在大家的支持下越来越棒的！");
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
			CQ_sendPrivateMsg(ac, fromQQ, "武理知音欢迎您！");
			CQ_sendPrivateMsg(ac, fromQQ, sendmsg);
			return EVENT_BLOCK;
		}
	}
	return EVENT_IGNORE;
}


/*
* Type=2 群消息
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
	int k = 0, number = 0, i = 0, price = 0, n = 0;//k、i、n为循环数
	int sum = 0, x = 0, y = 0, len = 0;
	int t = 0;
	static int shutup = 0;
	CQTool fromQQclass;
	CQ_Type_GroupMember fromQQinfo;
	CQ_Type_GroupMember targetQQinfo;
	CQ_Type_GroupMember masterQQinfo;
	connection connections;//与服务器连接的实例对象

	connections.init(ac);
	for (k = 0; k < 10; k++)
	{
		if (list.QQID[k] == fromQQ)//取到了QQID的记录
		{
			if (list.number[k] == 2)//赠送
			{
				if (!strcmp(msg, "1"))
				{
					//if (useprop(fromQQ, "赠送卡") != 1)
					if (connections.changeprop(fromQQ, 1, 0) != 1)
					{
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						list.number[k] = 0;
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					targetQQID = list.targetQQID[k];//取出列中的目标
					connections.getintegral(targetQQID, &targetdata);
					connections.changeintegral(targetQQID, list.integral[k], 1);
					connections.changeintegral(fromQQ, -list.integral[k], 1);
					CQ_sendGroupMsg(ac, fromGroup, "赠送成功!");
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
					CQ_sendGroupMsg(ac, fromGroup, "你取消了赠送");
					return EVENT_BLOCK;
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, "输入1确认，输入0取消！");
					return EVENT_BLOCK;
				}
			}
			else if (list.number[k] == 3)//流放
			{
				targetQQID = list.targetQQID[k];//取出列中的目标
				if (!strcmp(msg, "1"))//指令为1
				{
					if (connections.deleteslave(targetQQID) == 0)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						list.QQID[k] = 0;
						list.targetQQID[k] = 0;
						list.number[k] = 0;
						return EVENT_BLOCK;
					}
					CQ_sendGroupMsg(ac, fromGroup, "流放成功！");
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
					CQ_sendGroupMsg(ac, fromGroup, "你取消了流放");
					return EVENT_BLOCK;
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, "输入1确认，输入0取消！");
					return EVENT_BLOCK;
				}
			}
			else if (list.number[k] == 0)//购买
			{
				if (!strcmp(msg, "1"))//指令为1
				{
					targetQQID = list.targetQQID[k];//取出列中的目标
					t = connections.buyslave(fromQQ, targetQQID);
					if (t == 0)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
					}
					else if (t == 1)
					{
						CQ_sendGroupMsg(ac, fromGroup, "购买成功！");
					}
					else
					{
						CQ_sendGroupMsg(ac, fromGroup, "对方自动使用了保护卡，重新尝试购买吧！");
					}
					list.QQID[k] = 0;
					list.targetQQID[k] = 0;
					return EVENT_BLOCK;
				}
				else if (!strcmp(msg, "0"))
				{
					list.QQID[k] = 0;
					list.targetQQID[k] = 0;
					CQ_sendGroupMsg(ac, fromGroup, "你取消了购买");
					return EVENT_BLOCK;
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, "输入1确认，输入0取消！");
					return EVENT_BLOCK;
				}
			}
			else if (list.number[k] == 1)
			{
				int n = atoi(msg);
				if (n == 1)
				{
					list.number[k] = 4;
					list.targetQQID[k] = fromQQ;
					CQ_sendGroupMsg(ac, fromGroup, "请输入您的名字，请勿含有不适当的词汇及超过十个字！");
				}
				else if (n == 2)
				{
					list.number[k] = 5;
					CQ_sendGroupMsg(ac, fromGroup, "请输入奴隶的qq或艾特对方！");
				}
				else
				{
					list.QQID[k] = 0;
					list.number[k] = 0;
					CQ_sendGroupMsg(ac, fromGroup, "你取消了该指令！");
				}
				return EVENT_BLOCK;
			}
			else if (list.number[k] == 4)
			{
				if (strlen(msg) > 20)
				{
					CQ_sendGroupMsg(ac, fromGroup, "昵称过长！");
				}
				else
				{
					char *recData = NULL;
					int n = connections.slave_putname(fromQQ, list.targetQQID[k], (char*)msg);
					list.QQID[k] = 0;
					list.targetQQID[k] = 0;
					list.number[k] = 0;
					if (n == -1)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
					}
					else if (n == -2)
					{
						CQ_sendGroupMsg(ac, fromGroup, "你不是对方的主人，你无法为对方取名！");
					}
					else if (n == 1)
					{
						CQ_sendGroupMsg(ac, fromGroup, "取名成功！");
					}
					else
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
					}
				}
				return EVENT_BLOCK;
			}
			else if (list.number[k] == 5)
			{
				if (strstr(msg, "qq=") != NULL)
				{
					targetQQID = atoll(strstr(msg, "=") + 1);
				}
				else
				{
					targetQQID = atoll(msg);
				}
				if (targetQQID == 0)
				{
					CQ_sendGroupMsg(ac, fromGroup, "你输入的信息有误，该指令已取消！");
					list.number[k] = 0;
					list.QQID[k] = 0;
				}
				else
				{
					list.targetQQID[k] = targetQQID;
					list.number[k] = 4;
					CQ_sendGroupMsg(ac, fromGroup, "请输入奴隶的名字，请勿含有不适当的词汇及超过十个字！");
				}
				return EVENT_BLOCK;
			}
		}
	}
	if (strlen(msg) > 100)
	{
		return EVENT_BLOCK;
	}
	if ((!strcmp(msg, "签到")) || (strstr(msg, "sign") != NULL))
	{
		CQ_sendGroupMsg(ac, fromGroup, connections.sign(fromQQ, fromGroup));
		return EVENT_BLOCK;
	}
	else if ((strstr(msg, "给我一张")!=NULL)&fromQQ == 3047964704)
	{
		strcpy(tips, strstr(msg, "张") + 2);
		if (connections.changeprop(fromQQ, atoi(tips), 1) == 1)
			CQ_sendGroupMsg(ac, fromGroup, "添加成功！");
		else
		{
			CQ_sendGroupMsg(ac, fromGroup, "添加失败！");
		}
		return EVENT_BLOCK;
	}
	else if ((strstr(msg, "奖励") != NULL)&(fromQQ == 3047964704))
	{
		strcpy(targetstrQQID, strstr(msg, "励") + 2);
		targetQQID = atoll(targetstrQQID);
		strcpy(tips, strstr(msg, "分") + 2);
		if (connections.changeintegral(atoll(targetstrQQID), atoi(tips), 0) != 0)
			CQ_sendGroupMsg(ac, fromGroup, "奖励成功！");
		else
			CQ_sendGroupMsg(ac, fromGroup, Error);
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "开启天气预报"))
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
	else if (!strcmp(msg, "关闭天气预报"))
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
	else if (!strcmp(msg, "查看天气预报"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "武理知音欢迎您！");
		recData = connections.ask("city:武汉\ntype:weather\naction:get\nsendby:soul");
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
	else if (!strcmp(msg, "维护反馈"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "如果碰到了什么问题或是有什么建议，可以通过以下几个方法进行反馈\n1.维护人员qq:3047964704\n2.发送邮件至3047964704的qq邮箱\n3.在我的说说下面做出评论");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "天气预报"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "在qq群或私聊中发送开启天气预报即可订阅武理知音天气预报，发送关闭天气预报即可取消订阅，发送查看天气预报可立即查询明日天气！\n武理知音欢迎您！");
	}
	else if (!strcmp(msg, "邀请入群"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "私聊我发送'邀请入群（QQ群号）'即可，不要带单引号和括号哦！");
	}
	else if (!strcmp(msg, "武理知音"))
	{
		CQ_sendGroupMsg(ac, fromGroup, wulizhiyin);
	}
	else if (strstr(msg, "购买") != NULL)
	{
		int64_t n;
		if (!strcmp(msg, "购买"))
		{
			CQ_sendGroupMsg(ac, fromGroup, "请输入你要购买的好友qq,例如：购买xxxxxxxxxxx");//假装这是第一步
			return EVENT_BLOCK;
		}
		if (strlen(strstr(msg, "买") + 2) <= 20)
		{
			n = connections.getIDbyname_slave(fromQQ, (char*)strstr(msg, "买") + 2);
		}
		else
		{
			n == -1;
		}
		if (n == -1)
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
			memset(targetstrQQID, 0, sizeof(targetstrQQID));
			for (i = 0; k < strlen(msg); k++, i++)//取连续的数字
			{
				if (msg[k] < 48 | msg[k]>57)
				{
					break;
				}
				targetstrQQID[i] = msg[k];
			}
			targetQQID = atoll(targetstrQQID);
		}
		else if (n == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		else
		{
			targetQQID = n;
		}
		if (targetQQID == fromQQ)
		{
			CQ_sendGroupMsg(ac, fromGroup, "你不能购买你自己!");
			return EVENT_BLOCK;
		}
		//fromQQclass.GetGroupMemberInfo(ac, fromGroup, targetQQID, targetQQinfo);//取目标QQ信息(该构件出现代码问题，停用) mark
		recData = connections.buycheck(fromQQ, targetQQID);
		if (recData == NULL)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		if (atoi(recData) == -1)
		{
			CQ_sendGroupMsg(ac, fromGroup, strstr(recData, " ") + 1);
			return EVENT_BLOCK;
		}
		else
		{
			for (k = 0; k < 10; k++)
			{
				if (list.QQID[k] == 0)
				{
					CQ_sendGroupMsg(ac, fromGroup, strstr(recData, " ") + 1);
					list.QQID[k] = fromQQ;
					list.targetQQID[k] = targetQQID;
					return EVENT_BLOCK;
				}
			}
			CQ_sendGroupMsg(ac, fromGroup, "不好意思，服务器繁忙，稍后再试吧！或者联系维护人员哦！");
			return EVENT_BLOCK;
		}
	}
	else if (!strcmp(msg, "赎回"))
	{
		int number = connections.buy_back(fromQQ);
		if (number == 1)
		{
			CQ_sendGroupMsg(ac, fromGroup, "赎回成功，你是自由身了！");
		}
		else if (number == 2)
		{
			CQ_sendGroupMsg(ac, fromGroup, "你是自由身，无需赎回！");
		}
		else if (number == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
		}
		else
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf_s(sendmsg, "%s\n赎回需要%i积分，你的积分不足无法赎回！", connections.getat(fromQQ), -number);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
		}
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "查询") != NULL)
	{
		connections.getintegral(fromQQ, &data);
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf(sendmsg, "你现在的积分为：%i", data.integral);
		CQ_sendGroupMsg(ac,fromGroup,sendmsg);
		CQ_sendGroupMsg(ac, fromGroup, connections.listprop(fromQQ));
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "抽奖"))
	{
		i = connections.changeprop(fromQQ, 4, 0);
		if (i == 1)//成功
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
					sprintf(sendmsg, "%s\n你获得了赠送卡", connections.getat(fromQQ));
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case -2:
					if (connections.changeprop(fromQQ, 2, 1) != 1)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "%s\n你获得了强制购买卡", connections.getat(fromQQ));
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case -3:
					if (connections.changeprop(fromQQ, 3, 1) != 1)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "%s\n你获得了保护卡", connections.getat(fromQQ));
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case -4:
					if (connections.changeprop(fromQQ, 4, 1) != 1)
					{
						CQ_sendGroupMsg(ac, fromGroup, Error);
						return EVENT_BLOCK;
					}
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "%s\n你获得了抽奖卡", connections.getat(fromQQ));
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				case 4:
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "%s\n不好意思，你什么都没抽到，下次再来吧！", connections.getat(fromQQ));
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
					sprintf(sendmsg, "%s\n你获得了%i积分,你现在拥有%i积分", connections.getat(fromQQ), number, data.integral, 1);
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
					sprintf(sendmsg, "%s\n你获得了%i积分,你现在拥有%i积分", connections.getat(fromQQ), number, data.integral, 1);
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
					sprintf(sendmsg, "%s\n你获得了%i积分,你现在拥有%i积分", connections.getat(fromQQ), number, data.integral, 1);
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
					sprintf(sendmsg, "%s\n你获得了%i积分,你现在拥有%i积分", connections.getat(fromQQ), number, data.integral, 1);
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
					sprintf(sendmsg, "%s\n你获得了%i积分,你现在拥有%i积分", connections.getat(fromQQ), number, data.integral, 1);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					break;
				}
				return EVENT_BLOCK;
			}
		}
		else if (i == 0)//失败
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		else if (i == -1)
		{
			CQ_sendGroupMsg(ac, fromGroup, "你没有抽奖卡！请在商店兑换抽奖卡。");
			return EVENT_BLOCK;
		}
		
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "罚") != NULL)
	{
		int64_t n;
		if (!strcmp(msg, "惩罚"))
		{
			CQ_sendGroupMsg(ac, fromGroup, "请输入你要惩罚的好友qq以及操作,例如：惩罚xxxxxxxxxxx跳舞，可以问我惩罚有哪些哦！");//假装这是第一步
			return EVENT_BLOCK;
		}
		if (strstr(msg, "有") != NULL)
		{
			CQ_sendGroupMsg(ac, fromGroup, "目前只有跳舞、唱歌、拖地和穿女装哦！");
			return EVENT_BLOCK;
		}
		char askmsg[50];
		memset(askmsg, 0, sizeof(askmsg));
		if (strstr(msg, "女装") != NULL)
		{
			strcpy(askmsg, strstr(msg, "罚") + 2);
			*(strstr(askmsg, "女")) = '\0';
		}
		else if (strstr(msg, "跳舞") != NULL)
		{
			strcpy(askmsg, strstr(msg, "罚") + 2);
			*(strstr(askmsg, "跳")) = '\0';
		}
		else if (strstr(msg, "唱歌") != NULL)
		{
			strcpy(askmsg, strstr(msg, "罚") + 2);
			*(strstr(askmsg, "唱")) = '\0';
		}
		else if (strstr(msg, "拖地") != NULL)
		{
			strcpy(askmsg, strstr(msg, "罚") + 2);
			*(strstr(askmsg, "拖")) = '\0';
		}
		else
		{
			return EVENT_BLOCK;
		}
		if (strlen(askmsg) <= 20)
		{
			
			n = connections.getIDbyname_slave(fromQQ, askmsg);
		}
		else
		{
			n = -1;
		}
		if (n == -1)
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
			memset(targetstrQQID, 0, sizeof(targetstrQQID));
			for (i = 0; k < strlen(msg); k++, i++)//取连续的数字
			{
				if (msg[k] < 48 | msg[k]>57)
				{
					break;
				}
				targetstrQQID[i] = msg[k];
			}
			targetQQID = atoll(targetstrQQID);
		}
		else if (n == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		else
		{
			targetQQID = n;
		}
		if (connections.getmaster(&master, targetQQID) == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		if (master.QQID == fromQQ)
		{
			if (strstr(msg, "女装") != NULL)
			{
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%lld]在主人的命令下被迫穿上了女装，满脸娇羞的看着围观的众人。", targetQQID);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			}
			else if (strstr(msg, "跳舞") != NULL)
			{
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%lld]在主人的命令下短短的跳了一支舞，真是惊艳了众人，如果鞋子没掉的话那就更好了。", targetQQID);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			}
			else if (strstr(msg, "唱歌") != NULL)
			{
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%lld]在主人的命令下高歌一曲，灵魂歌手一般的歌喉震惊听众，隔壁的邻居都气的跑来敲门啦！", targetQQID);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			}
			else if (strstr(msg, "拖地") != NULL)
			{
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%lld]在主人的命令下把房间的地板全部拖了一遍，光洁的地板闪闪发光！", targetQQID);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			}
			else
			{
				CQ_sendGroupMsg(ac, fromGroup, "请输入正确的惩罚！");
			}			
		}
		else
		{
			CQ_sendGroupMsg(ac, fromGroup, "对方不是你的奴隶或输入错误！");
		}
		return EVENT_BLOCK;
	}
	else if ((strstr(msg, "查看") != NULL)&(strstr(msg, "新版") == NULL))
	{
		int64_t n;
		if (!strcmp(msg, "查看"))
		{
			CQ_sendGroupMsg(ac, fromGroup, "请输入你要购买的查看qq,例如：查看xxxxxxxxxxx");
			return EVENT_BLOCK;
		}
		if (strlen(strstr(msg, "看") + 2) <= 20)
		{
			n = connections.getIDbyname_slave(fromQQ, (char*)strstr(msg, "看") + 2);
		}
		else
		{
			n = -1;
		}
		if (n == -1)
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
			memset(targetstrQQID, 0, sizeof(targetstrQQID));
			for (i = 0; k < strlen(msg); k++, i++)//取连续的数字
			{
				if (msg[k] < 48 | msg[k]>57)
				{
					break;
				}
				targetstrQQID[i] = msg[k];
			}
			targetQQID = atoll(targetstrQQID);
		}
		else if (n == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		else
		{
			targetQQID = n;
		}
		if (targetQQID < 100000)
		{
			return EVENT_BLOCK;
		}
		//fromQQclass.GetGroupMemberInfo(ac, fromGroup, targetQQID, targetQQinfo);//取目标QQ信息
		/*if (targetQQinfo.QQID != targetQQID)
		{
		CQ_sendGroupMsg(ac, fromGroup, "你查看的QQ错误或者不在此群");
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
			CQ_sendGroupMsg(ac, fromGroup, "对方的主人并不在本群，我无法为你提供该信息");
			}*/
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:at,qq=%lld]的主人是[CQ:at,qq=%lld]", targetQQID, master.QQID);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
		}
		connections.getintegral(targetQQID, &targetdata);
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf(sendmsg, "[CQ:at,qq=%lld]的积分为:%i", targetQQID, targetdata.integral);
		CQ_sendGroupMsg(ac, fromGroup, sendmsg);
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
				CQ_sendGroupMsg(ac, fromGroup, "对方的这名奴隶并不在本群，我无法为你提供该信息");
				continue;
				}*/
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf_s(sendmsg, "该奴隶为[CQ:at,qq=%lld]:%lld", slavelist[n], slavelist[n]);
				CQ_sendPrivateMsg(ac, fromQQ, sendmsg);
			}
		}
		return EVENT_BLOCK;
	}
	else if ((strstr(msg, "赠送") != NULL) && (strstr(msg, "积分") != NULL))
	{
		int64_t n;
		int integral;
		char askmsg[50];
		memset(askmsg, 0, sizeof(askmsg));
		if (!strcmp(msg, "赠送"))
		{
			CQ_sendGroupMsg(ac, fromGroup, "请输入你要赠送的qq及积分\n例如：赠送xxxxx积分xxx");
			return EVENT_BLOCK;
		}
		if (strstr(msg, "积") != NULL)
		{
			strcpy(askmsg, strstr(msg, "送") + 2);
			*(strstr(askmsg, "积")) = '\0';
		}
		if (strlen(askmsg) <= 20)
		{
			n = connections.getIDbyname_slave(fromQQ, askmsg);
		}
		else
		{
			n = -1;
		}
		if (n == -1)
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
			memset(targetstrQQID, 0, sizeof(targetstrQQID));
			for (i = 0; k < strlen(msg); k++, i++)//取连续的数字
			{
				if (msg[k] < 48 | msg[k]>57)
				{
					break;
				}
				targetstrQQID[i] = msg[k];
			}
			targetQQID = atoll(targetstrQQID);
		}
		else if (n == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		else
		{
			targetQQID = n;
		}
		if ((integral = atoi(strstr(msg, "分") + 2)) <= 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, "赠送的积分不能小于或等于0");
			return EVENT_BLOCK;
		}
		connections.getintegral(fromQQ, &data);
		if (data.integral < integral)
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "你没有%i积分，你只有%i积分", integral, data.integral);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			return EVENT_BLOCK;
		}
		else
		{
			if (connections.changeprop(fromQQ, 1, 2) == -1)
			{
				CQ_sendGroupMsg(ac, fromGroup, "你没有可以使用的赠送卡！");
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
					CQ_sendGroupMsg(ac, fromGroup, "服务器繁忙，请稍后再试！");
					return EVENT_BLOCK;
				}
			}
			list.QQID[n] = fromQQ;
			list.targetQQID[n] = targetQQID;
			list.number[n] = 2;
			list.integral[n] = integral;
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "你将赠送[CQ:at,qq=%lld] %i积分！", targetQQID, integral);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			CQ_sendGroupMsg(ac, fromGroup, "确认赠送请输1，取消请输0");
		}
	}
	/*else if (!strcmp(msg, "使用保护卡"))
	{ 
		i = connections.changeprop(fromQQ, 3, 0);
		if (i == 1)
		{
			if (connections.getmaster(&master, fromQQ) == 0)
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				CQ_addLog(ac, CQLOG_ERROR, "道具使用", "保护卡已消耗！");
				return EVENT_BLOCK;
			}
			time(&master.Time);
			if (connections.buyslave(fromQQ, master.QQID, master.ransom, master.Time) == 0)
			{
				CQ_sendGroupMsg(ac, fromGroup, Error);
				CQ_addLog(ac, CQLOG_ERROR, "道具使用", "保护卡已消耗！");
				return EVENT_BLOCK;
			}
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:at,qq=%lld]使用保护卡成功!", fromQQ);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
		}
		else if (i == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		else if (i == -1)
		{
			CQ_sendGroupMsg(ac, fromGroup, "你没有保护卡，快去商店兑换一张保护卡吧！");
			return EVENT_BLOCK;
		}
	}*/
	else if (!strcmp(msg, "使用取名卡"))
	{
		i = connections.changeprop(fromQQ, 5, 2);
		if (i == 1)
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:at,qq=%lld]\n输入1为自己取名，输入2为奴隶取名，输入0取消!", fromQQ);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			list.QQID[n] = fromQQ;
			list.number[n] = 1;
			return EVENT_BLOCK;
		}
		else if (i == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		else if (i == -1)
		{
			CQ_sendGroupMsg(ac, fromGroup, "你没有取名卡，快去商店兑换一张取名卡吧！");
			return EVENT_BLOCK;
		}
	}
	else if (strstr(msg, "流放") != NULL)
	{
		int64_t n;
		if (!strcmp(msg, "流放"))
		{
			CQ_sendGroupMsg(ac, fromGroup, "请输入你要流放的qq\n例如：流放xxxxx");
			return EVENT_BLOCK;
		}
		if (strlen(strstr(msg, "放") + 2) <= 20)
		{
			n = connections.getIDbyname_slave(fromQQ, (char*)strstr(msg, "放") + 2);
		}
		else
		{
			n = -1;
		}
		if (n == -1)
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
			memset(targetstrQQID, 0, sizeof(targetstrQQID));
			for (i = 0; k < strlen(msg); k++, i++)//取连续的数字
			{
				if (msg[k] < 48 | msg[k]>57)
				{
					break;
				}
				targetstrQQID[i] = msg[k];
			}
			targetQQID = atoll(targetstrQQID);
		}
		else if (n == 0)
		{
			CQ_sendGroupMsg(ac, fromGroup, Error);
			return EVENT_BLOCK;
		}
		else if (n == 1)
		{
			CQ_sendGroupMsg(ac, fromGroup, "对方不是你的奴隶，你无法进行流放操作！");
		}
		else
		{
			targetQQID = n;
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
				sprintf(sendmsg, "你确定要流放[CQ:at,qq=%lld]吗?\n确认流放请输1，取消请输0！", targetQQID);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				for (n = 0; n < 10; n++)
				{
					if (list.QQID[n] == 0)
						break;
					if (n == 9)
					{
						CQ_sendGroupMsg(ac, fromGroup, "服务器繁忙，请稍后再试！");
						return EVENT_BLOCK;
					}
				}
				list.QQID[n] = fromQQ;
				list.targetQQID[n] = targetQQID;
				list.number[n] = 3;
				return EVENT_BLOCK;
			}
		}
		CQ_sendGroupMsg(ac, fromGroup, "对方不是你的奴隶，你无法进行流放操作!");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "开始"))
	{
		for (sum = 0; sum < 5; sum++)
		{
			if (gameroom[sum].memberinhere(fromQQ) == 0)
			{
				if (gameroom[sum].number >= 6)
				{
					if (gameroom[sum].startgame(ac) == -1)
					{
						CQ_sendGroupMsg(ac, fromGroup, "游戏已开始，请不要重复开始！");
						return EVENT_BLOCK;
					}
					else
					{
						CQ_sendGroupMsg(ac, fromGroup, "游戏开始了，请看我的私聊进行游戏！");
					}
					if (FALSE == gameroom[sum].starttimer())
					{
						CQ_addLog(ac, CQLOG_ERROR, "game", "can't set timer");
					}
					return EVENT_BLOCK;
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, "人数不足六个无法开始游戏！");
					return EVENT_BLOCK;
				}
			}
		}
		CQ_sendGroupMsg(ac, fromGroup, "你不是任何一个房间的房主，你无法开始游戏!");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "退出"))
	{
		for (sum = 0; sum < 5; sum++)
		{
			if (gameroom[sum].memberinhere(fromQQ) > -1)
			{
				if (gameroom[sum].runstate == 1)
				{
					CQ_sendGroupMsg(ac, fromGroup, "该房间已开始游戏，你不能退出");
					return EVENT_BLOCK;
				}
				if (gameroom[sum].number == 1)
				{
					CQ_sendGroupMsg(ac, fromGroup, "这个房间因为无人被摧毁了");
					gameroom[sum].gameover();
					return EVENT_BLOCK;
				}
				if (gameroom[sum].memberinhere(fromQQ) == 0)
				{
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "房间%i的房主退出了房间，房主变更为:%lld", gameroom[sum].roomid, gameroom[sum].memberlist(1)[1]);
					CQ_sendGroupMsg(ac, fromGroup, sendmsg);
					memset(sendmsg, 0, sizeof(sendmsg));
					sprintf(sendmsg, "你成为了房间%i的房主。", gameroom[sum].roomid);
					CQ_sendPrivateMsg(ac, gameroom[sum].memberlist(1)[1], sendmsg);
				}
				if (gameroom[sum].deletemember(fromQQ) == 1)
				{
					CQ_sendGroupMsg(ac, fromGroup, "你退出了房间");
				}
				else
				{
					CQ_sendGroupMsg(ac, fromGroup, Error);
					return EVENT_BLOCK;
				}
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "有一名玩家退出了你所在的房间,现在你的房间有%i名玩家！", gameroom[sum].number);
				CQ_sendPrivateMsg(ac, gameroom[sum].memberlist(1)[0], sendmsg);
				return EVENT_BLOCK;
			}
		}
		CQ_sendGroupMsg(ac, fromGroup, "你不处于任何一个游戏房间中！");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "创建谁是卧底"))
	{
		
		for (sum = 0; sum < 5; sum++)
		{
			if (gameroom[sum].memberinhere(fromQQ) > -1)
			{
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%lld]请先退出你所在的游戏房间！", fromQQ);
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
				sprintf(sendmsg, "你创建的房间号为：%i,在群里发送房间号即可加入房间！输入退出即可离开房间！房主输入开始则开始游戏！", gameroom[sum].roomid);
				gameroom[sum].addmember(fromQQ);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				return EVENT_BLOCK;
			}
		}
		CQ_sendGroupMsg(ac, fromGroup, "抱歉，房间数量已经达到上限，你无法创建房间。");
	}
	else if (!strcmp(msg, "寻找房间"))
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
				sprintf(sendmsg, "%i号房间:%i(正在游戏中)", sum + 1, gameroom[sum].roomid);
			else
				sprintf(sendmsg, "%i号房间:%i(等待中)\n房主为:%lld", sum + 1, gameroom[sum].roomid, gameroom[sum].memberlist(1)[0]);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
		}
		sprintf_s(tips, "%i", x);
		CQ_addLog(ac, CQLOG_DEBUG, "谁是卧底test", tips);
		if (x == 5)
		{
			CQ_sendGroupMsg(ac, fromGroup, "目前没有可以加入的房间！");
		}
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "谁是卧底"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "输入:创建谁是卧底即可创建房间。");
		CQ_sendGroupMsg(ac, fromGroup, "输入:寻找房间即可查看房间列表。");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "菜单") || !strcmp(msg,"功能"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "目前的功能有:\n谁是卧底\n好友买卖系统\n签到抽奖系统\n道具系统\n充值\n武理知音\n输入这些词询问我相关内容");
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "充值"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "为了让傻馒能够茁壮成长，傻馒不得不含泪推出充值功能，给我发整数元红包便能获得积分奖励，一元一百分，首冲额外赠送四百分哦！");
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "好友买卖系统") != NULL)
	{
		CQ_sendGroupMsg(ac, fromGroup, "好友买卖系统中有购买，赎回，查看，查询，流放,惩罚等功能，输入这些词询问我相关内容。");
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "签到抽奖系统") != NULL)
	{
		CQ_sendGroupMsg(ac, fromGroup, "每天可以签到一次哦，签到只能获得积分，在商店兑换抽奖卡然后抽奖可以获得道具或者积分，但也有可能什么都得不到哦！");
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "道具系统") != NULL)
	{
		CQ_sendGroupMsg(ac, fromGroup, "目前只能通过抽奖来获取道具，强制购买卡可以用于购买处于保护期中的玩家，赠送卡可以用于赠送别人积分，保护卡可以让自己进入保护期(在购买时被动使用)\n发送使用取名卡可以取名。查询可以知道自己有哪些道具！");
		return EVENT_BLOCK;
	}
/*else if ((strstr(msg, "猜码图") != NULL)&(strstr(msg, "1839538956") != NULL))
	{
		if (gameroom.state)
		{
			CQ_sendGroupMsg(ac, fromGroup, "傻馒正忙着呢，请稍后再来！");
			return EVENT_BLOCK;
		}
		else
		{
			CQ_sendGroupMsg(ac, fromGroup, "进入猜码图模式，回答题目记得艾特我哦，一次最多只能玩15轮。");
			CQ_sendGroupMsg(ac, fromGroup, "实在猜不出来可以艾特我说提示或者下一个哦，不过那可就没有积分奖励了");
			srand(time(NULL));
			gameroom.number = rand() % 401;
			memset(where, 0, sizeof(where));
			sprintf(where, "f:/mingxing1/%i.txt",gameroom.number);
			fopen_s(&fp, where, "r");
			if (fp == NULL)
			{
				CQ_sendGroupMsg(ac, fromGroup, "猜码图数据损坏，请联系维护人员");
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
			sprintf(sendmsg, "给个小提示哦，是一个名字为%i个字的明星", int(n / 2));
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
			sprintf(sendmsg, "[CQ:at,qq=%lld]回答正确，获得5点积分！", fromQQ);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			gameroom.difficulty = 7;
			if (changeintegral(fromQQ, 5) == 0)
			{
				memset(tips, 0, sizeof(tips));
				sprintf(tips, "为%lld增加%i积分失败！", fromQQ, 5);
				CQ_addLog(ac, CQLOG_ERROR, "猜码图", tips);
			}
			gameroom.rank++;
			if (gameroom.rank > 15)
			{
				CQ_sendGroupMsg(ac, fromGroup, "猜码图游戏结束了");
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
					CQ_sendGroupMsg(ac, fromGroup, "猜码图数据损坏，请联系维护人员");
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
				sprintf(sendmsg, "给个小提示哦，是一个%i个字的明星", n / 2);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:image,file=%s%i.jpg]", gameroom.name, gameroom.difficulty);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				return EVENT_BLOCK;
			}
		}
		else if (strstr(msg, "下一个") != NULL)
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "好吧，这位明星是%s!", gameroom.name);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			gameroom.rank++;
			if (gameroom.rank > 15)
			{
				CQ_sendGroupMsg(ac, fromGroup, "猜码图游戏结束了");
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
					CQ_sendGroupMsg(ac, fromGroup, "猜码图数据损坏，请联系维护人员");
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
				sprintf(sendmsg, "给个小提示哦，是一个%i个字的明星", n / 2);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:image,file=%s%i.jpg]", gameroom.name, gameroom.difficulty);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				return EVENT_BLOCK;
			}
		}
		else if (strstr(msg, "提示") != NULL)
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
				CQ_sendGroupMsg(ac, fromGroup, "这已经是最低难度了！要是答不出来还是赶快下一个吧！");
				return EVENT_BLOCK;
			}
			CQ_sendGroupMsg(ac, fromGroup, "好吧，我就稍稍降低点难度吧！");
			gameroom.difficulty -= 2;
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:image,file=%s%i.jpg]", gameroom.name, gameroom.difficulty);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			return EVENT_BLOCK;
		}
		else
		{
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "[CQ:at,qq=%lld]回答错误！", fromQQ);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			return EVENT_BLOCK;
		}
	}*/
	else if (!strcmp(msg, "商店"))
	{
		CQ_sendGroupMsg(ac, fromGroup, connections.ask("user:1839538956\ntype:prop\naction:shop\nsendby:soul"));
		return EVENT_BLOCK;
	}
	else if (!strcmp(msg, "兑换"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "发送兑换xx卡即可使用积分兑换道具哦！");
		return EVENT_BLOCK;
	}
	else if (strstr(msg, "兑换") != NULL)
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:exchange\nname:%s\nsendby:soul", fromQQ, strstr(msg, "换") + 2);
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
	//谁是卧底
	for (sum = 0; sum < 5; sum++)
	{
		if (gameroom[sum].roomid == atoi(msg))
		{
			if (gameroom[sum].runstate == 1)
			{
				memset(sendmsg, 0, sizeof(sendmsg));
				sprintf(sendmsg, "[CQ:at,qq=%lld]该房间已开始游戏，你不能加入！",fromQQ);
				CQ_sendGroupMsg(ac, fromGroup, sendmsg);
				return EVENT_BLOCK;
			}
			for (int k = 0; k < 5; k++)
			{
				if (gameroom[k].memberinhere(fromQQ) > -1)
				{
					CQ_sendGroupMsg(ac, fromGroup, "请先退出你所在的房间");
					return EVENT_BLOCK;
				}
			}
			gameroom[sum].addmember(fromQQ);
			if (gameroom[sum].memberinhere(fromQQ) > -1)
			{
				
			}
			else
			{
				CQ_sendGroupMsg(ac, fromGroup, "加入失败，请重试或联系维护人员反馈");
				return EVENT_BLOCK;
			}
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf_s(sendmsg, "加入%i成功,输入退出即可离开房间！", gameroom[sum].roomid);
			CQ_sendGroupMsg(ac, fromGroup, sendmsg);
			memset(sendmsg, 0, sizeof(sendmsg));
			sprintf(sendmsg, "一名玩家加入了你的房间，现在你的房间有%i名玩家", gameroom[sum].number);
			CQ_sendPrivateMsg(ac, gameroom[sum].memberlist(1)[0], sendmsg);
			return EVENT_BLOCK;
		}
	}

aim:

	if (strstr(msg, "傻馒") != NULL)
	{
		if (strstr(msg, "闭嘴") != NULL)
		{
			shutup = 1;
			return EVENT_BLOCK;
		}
		else if (strstr(msg, "出来") != NULL)
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
* Type=4 讨论组消息
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=101 群事件-管理员变动
* subType 子类型，1/被取消管理员 2/被设置管理员
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=201 好友事件-好友已添加
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {
	char sendmsg[1000];
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf(sendmsg, wulizhiyin);
	CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");
	CQ_sendPrivateMsg(ac, fromQQ, sendmsg);
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {
	char sendmsg[100];
	connection connections;//与服务器连接的实例对象
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
			sprintf_s(sendmsg, "fromQQ:%lld,groupQQ:%lld邀请我入群通过！", fromQQ, fromGroup);
			CQ_sendPrivateMsg(ac, 3047964704, sendmsg);
			CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
			CQ_sendGroupMsg(ac, fromGroup, wulizhiyin);
		}
		else
		{
			CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_DENY, "");
			sprintf_s(sendmsg, "groupQQ:%lld邀请我入群拒绝！", fromGroup);
			CQ_sendPrivateMsg(ac, 3047964704, sendmsg);
		}
	}
	return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
}

/*
* 菜单，可在 .json 文件中设置菜单数目、函数名
* 如果不使用菜单，请在 .json 及此处删除无用菜单
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "这是menuA，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "这是menuB，在这里载入窗口，或者进行其他工作。", "", 0);
	return 0;
}
