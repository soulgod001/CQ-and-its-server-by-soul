#include "stdafx.h"
#include "string"
#include "stdio.h"
#include "cqp.h"
#include "appmain.h"
#include "iostream"
#include "fstream"
#include "io.h"
#include <direct.h>
#include <winsock2.h>    
#include <WS2tcpip.h>
#include "time.h"
#include <Windows.h>
#include <strsafe.h>
#include <Mmsystem.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "Winmm.lib")
using namespace std;
int ChineseConvert(const std::string& one_chinese, std::string& two_chinese, int type);

//结构体存储master信息
struct masterlist
{
	int64_t QQID = 0;
	time_t Time = 0;
	int ransom = 0;
};
struct QDdata
{
	time_t Time = 0;
	int integral = 0;
	char date[20] = "";
};
//用于与服务器进行交互的对象
class connection
{
	WORD sockVersion = NULL;
	WSADATA data;
	SOCKET sclient = NULL;
	sockaddr_in serAddr;
	int ac = 0;
public:
	bool init(int ac);
	int changeprop(int64_t QQID, int rank, int ype);
	char *listprop(int64_t QQID);
	int saleprop(int64_t QQID, int n, char *name);
	char *exchangeprop(int64_t QQID, int n, char *name);
	char *raffle(int64_t QQID);
	int getmaster(masterlist *master, int64_t QQID);
	char *listslave( int64_t QQID); //mark 将返回数据列表改为直接返回可发送的消息，发送消息格式为QQID（name）（近日签到状态）
	int deleteslave(int64_t slave);
	int buy_back(int64_t QQID);
	int buyslave(int64_t masterID, int64_t slaveID);
	int slave_putname(int64_t masterID, int64_t slaveID, char *name);
	int64_t getIDbyname_slave(int64_t user, char *name);
	char *buycheck(int64_t masterID, int64_t slaveID);
	bool put_Administorforgroup(int64_t QQID, int64_t GroupID, int grade);
	int get_Administorforgroup(int64_t QQID, int64_t GroupID);
	int getuse_fromserver(int64_t GroupID);
	int putuse_fromserver(int64_t GroupID,int state);
	char *sign(int64_t QQID, int64_t groupid);
	int changeintegral(int64_t QQID, int number,int type);
	int getintegral(int64_t QQID,QDdata *sign);
	char *getat(int64_t QQID);
	char *ask(char *content);
	char *askagain(char *content);
	int getweatherlist(int64_t *grouplist, int kind);
};
//获取对应Kind的qqlist，用以向对应qq发送天气预报，成功将数据填入grouplist并返回1，失败返回0，kind0为个人qq，kind1为群qq
int connection::getweatherlist(int64_t *grouplist, int kind)
{
	char *recData;
	char sendmsg[100];
	int i = 0;
	int t = 0;
	int k = 0;
	char substring[50];
	memset(grouplist, 0, sizeof(grouplist));
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:1839538956\ntype:weather\naction:list\nkind:%i\nsendby:soul",kind);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	memset(substring, 0, sizeof(substring));
	while (recData[i] != ' ')
	{
		substring[t] = recData[i];
		i++;
		t++;
	}
	k = atoi(substring);
	for (int n = 0; n < k; n++)
	{
		i++;
		t = 0;
		memset(substring, 0, sizeof(substring));
		while (recData[i] != ' ' && i < strlen(recData))
		{
			substring[t] = recData[i];
			i++;
			t++;
		}
		grouplist[n] = atoll(substring);
	}
	return 1;
}

//
char *connection::getat(int64_t QQID)
{
	char sendmsg[200];
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:slave\naction:getat\nsendby:soul", QQID);
	char *recData = ask(sendmsg);
	if (recData == NULL)
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "[CQ:at,qq=%lld]", QQID);
		return sendmsg;
	}
	return recData;
}
//失败返回NULL
char *connection::ask(char *content)
{
	char recData[2550];
	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		return NULL;
	}
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		closesocket(sclient);
		if (!strcmp(content, "t"))
		{
			system("taskkill /f /t /im python.exe");
			system("python c:/workstation/server.py");
			Sleep(1000);//挂起1s以留出重启时间
			return askagain(content);
		}
		else
		{
			return NULL;
		}
		
	}
	if (-1 == send(sclient, content, strlen(content), 0))
	{
		closesocket(sclient);
		if (!strcmp(content, "t"))
		{
			system("taskkill /f /t /im python.exe");
			system("python c:/workstation/server.py");
			Sleep(1000);//挂起1s以留出重启时间
			return askagain(content);
		}
		else
		{
			return NULL;
		}
	}
	memset(recData, 0, sizeof(recData));
	int ret = recv(sclient, recData, 2550 * sizeof(char), 0);
	if (ret > 0)
	{
		recData[ret] = 0x00;
	}
	else
	{
		closesocket(sclient);
		return NULL;//接收数据有误
	}
	closesocket(sclient);
	return recData;
}
char *connection::askagain(char *content)
{
	char recData[2550];
	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		return NULL;
	}
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		closesocket(sclient);
		return NULL;//服务器连接失败
	}
	if (-1 == send(sclient, content, strlen(content), 0))
	{
		closesocket(sclient);
		return NULL;//发送数据失败
	}
	memset(recData, 0, sizeof(recData));
	int ret = recv(sclient, recData, 2550 * sizeof(char), 0);
	if (ret > 0)
	{
		recData[ret] = 0x00;
	}
	else
	{
		closesocket(sclient);
		return NULL;//接收数据有误
	}
	closesocket(sclient);
	return recData;
}

char *connection::sign(int64_t QQID, int64_t groupid)
{
	char sendmsg[200];
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:sign\naction:sign\ngroupid:%lld\nsendby:soul", QQID, groupid);
	return ask(sendmsg);
}

//成功将数值存储到sign中并返回1，失败则返回0
int connection::getintegral(int64_t QQID, QDdata *sign)
{
	char sendmsg[100];
	char *recData;
	char substring[50];
	int i = 0, t = 0;
	memset(sign, 0, sizeof(sign));
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\naction:get\ntype:sign\nsendby:soul", QQID);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	//返回值的前段为日期，后端为积分，样例为：2019-7-22 500
	while (recData[i] != ' ' && i < strlen(recData))
	{
		sign->date[t] = recData[i];
		i++;
		t++;
	}
	i++;
	t = 0;
	memset(substring, 0, sizeof(substring));
	strcpy(substring, &recData[i]);
	sign->integral = atoi(substring);
	return 1;
}
//改变指定id的积分，失败返回0，成功返回积分的改变值，number不可为0,type为1为检测master，type为0为不检测
int connection::changeintegral(int64_t QQID, int number, int type)
{
	QDdata data;
	masterlist master;
	char sendmsg[100];
	if (number == 0)
		return 0;
	if (type)
	{
		if (number > 0)
		{
			getmaster(&master, QQID);
			if (master.QQID != 0)
			{
				if (!changeintegral(QQID, number*0.8, 0))
					return 0;
				if (!changeintegral(master.QQID, number - number * 0.8, 0))
					return 0;
				return number * 0.8;
			}
			else
			{
				if (!changeintegral(QQID, number, 0))
					return 0;
				return number;
			}
		}
		else
		{
			if (!changeintegral(QQID, number, 0))
				return 0;
			return number;
		}
	}
	else
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "user:%lld\ntype:sign\naction:integral\nnumber:%i\nsendby:soul", QQID, number);
		return atoi(ask(sendmsg));
	}
}
//成功赎回返回1，无需赎回返回2，积分不足返回-number，number为赎回所需积分数，出错返回0
int connection::buy_back(int64_t QQID)
{
	char sendmsg[200];
	char *recData = NULL;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:slave\naction:buy_back\nsendby:soul",QQID);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return atoi(recData);
}
//向服务器发出购买奴隶请求，成功返回1，错误返回0，对方自动使用保护卡返回-1
int connection::buyslave(int64_t masterID, int64_t slaveID)
{
	char sendmsg[200];
	char *recData = NULL;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:slave\naction:buy\nslaveid:%lld\nsendby:soul", masterID, slaveID);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return atoi(recData);
}
//检测是否可以购买某个玩家，在保护期内不可购买返回-1+msg，需要消耗强制购买卡返回-1+msg
//购买自己的奴隶返回-1+msg，试图购买自己的主人返回-1+msg，新手保护期返回-1+msg
//积分不够返回-1+msg，错误返回0，可购买返回购买需要的1+msg，需要消耗强制购买卡返回1+msg
char *connection::buycheck(int64_t masterID, int64_t slaveID)
{
	char sendmsg[200];
	char *recData = NULL;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:slave\naction:buycheck\nslaveid:%lld\nsendby:soul", masterID, slaveID);
	recData = ask(sendmsg);
	return recData;
}
//成功返回1，失败返回0
bool connection::put_Administorforgroup(int64_t QQID, int64_t GroupID, int grade)
{
	char sendmsg[200];
	char *recData;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:administor\naction:put\ngroupid:%lld\ngrade:%i\nsendby:soul", QQID, GroupID, grade);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return atoi(recData);
}
//返回rank，若失败默认返回0
int connection::get_Administorforgroup(int64_t QQID, int64_t GroupID)
{
	char sendmsg[200];
	char *recData;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:administor\naction:get\ngroupid:%lld\nsendby:soul", QQID, GroupID);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return atoi(recData);
}
//
int connection::getuse_fromserver(int64_t GroupID)
{
	char sendmsg[200];
	char *recData;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:group\naction:getuse\nsendby:soul", GroupID);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return atoi(recData);
}
//
int connection::putuse_fromserver(int64_t GroupID,int state)
{
	char sendmsg[200];
	char *recData;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:group\naction:putuse\nstate:%i\nsendby:soul", GroupID,state);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return atoi(recData);
}
//以奴隶主的身份使用取名卡为奴隶取名，成功返回1，失败返回0，缺少取名卡返回-2，不是对方的主人返回-1
int connection::slave_putname(int64_t masterID, int64_t slaveID, char *name)
{
	char sendmsg[200];
	char *recData;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:slave\naction:putname\nslaveid:%lld\nname:%s\nsendby:soul", masterID, slaveID, name);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return atoi(recData);
}
//根据name和user获取对方的id，若不存在该name返回-1，存在且该User为其主人返回1，若存在且该User不为其主人返回其id，出错返回0
int64_t connection::getIDbyname_slave(int64_t user, char *name)
{
	char sendmsg[200];
	char *recData;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:slave\naction:getidbyname\nname:%s\nsendby:soul",user, name);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return atoll(recData);
}
//成功返回1，失败返回0
int connection::deleteslave(int64_t slave)
{
	char sendmsg[200];
	char *recData;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:slave\naction:delete\nsendby:soul", slave);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return atoi(recData);
}
//成功返回1，失败返回0
char *connection::listslave( int64_t QQID)
{
	char sendmsg[200];
	char *recData;
	char sub[20];
	int i = 0;
	int t = 0;
	int x = 0;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:slave\naction:list\nsendby:soul", QQID);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return recData;
}
//将指定id的master成功返回1，失败返回0
int connection::getmaster(masterlist *master, int64_t QQID)
{
	char sendmsg[200];
	char *recData;
	char sub[20];
	int i = 0;
	int t = 0;
	memset(sendmsg, 0, sizeof(sendmsg));
	memset(master, 0, sizeof(master));
	sprintf_s(sendmsg, "user:%lld\ntype:slave\naction:check\nsendby:soul", QQID);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	else if (recData[0] == '0')
	{
		master->QQID = 0;
		while (recData[i] != ' ')
		{
			sub[t] = recData[i];
			t++;
			i++;
		}
		i++;
		t = 0;
	}
	else
	{
		while (recData[i] != ' ')
		{
			sub[t] = recData[i];
			t++;
			i++;
		}
		i++;
		t = 0;
		master->QQID = atoll(sub);
	}
	memset(sub, 0, sizeof(sub));
	while (recData[i] != ' ')
	{
		sub[t] = recData[i];
		t++;
		i++;
	}
	i++;
	t = 0;
	master->ransom = atoi(sub);
	memset(sub, 0, sizeof(sub));
	while (recData[i] != '\0')
	{
		sub[t] = recData[i];
		t++;
		i++;
	}
	i++;
	t = 0;
	master->Time = atol(sub);
	return 1;
}
//列出指定id所拥有的所有道具，返回值即为完整的查询结果，格式为“[CQ:at,qq=QQID],您拥有xxx”
char * connection::listprop(int64_t QQID)
{
	char sendmsg[200];
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:list\nsendby:soul",QQID);
	return ask(sendmsg);
}
//
int connection::saleprop(int64_t QQID, int n, char *name)
{
	char sendmsg[200];
	char *recData;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:sale\nnumber:%i\nname:%s\nsendby:soul", QQID, n, name);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return atoi(recData);
}
//
char *connection::exchangeprop(int64_t QQID, int n, char *name)
{
	char sendmsg[200];
	char *recData;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:exchange\nnumber:%i\nname:%s\nsendby:soul", QQID, n, name);
	recData = ask(sendmsg);
	if (recData == NULL)
	{
		return 0;
	}
	return recData;
}
//为指定的qq进行一次抽奖，返回抽奖结果，抽奖失败返回NULL
char *connection::raffle(int64_t QQID)
{
	char sendmsg[200];
	sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:raffle\nsendby:soul",QQID);
	return ask(sendmsg);
}

//QQID为获取道具者id，rank为道具编号，type为1增加指定道具，type为0减少指定道具，type为2查找指定道具是否存在，失败均返回0，成功返回1，没有该道具返回-1。
int connection::changeprop(int64_t QQID, int rank, int type)
{
	char sendmsg[200];
	memset(sendmsg, 0, sizeof(sendmsg));
	srand(time(NULL));
	if (type == 1)
	{
		sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:add\nrank:%i\nsendby:soul", QQID, rank);
	}
	else if (type == 2)
	{
		sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:find\nrank:%i\nsendby:soul", QQID, rank);
	}
	else
	{
		sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:reduce\nrank:%i\nsendby:soul", QQID, rank);
	}
	return atoi(ask(sendmsg));
}
//初始化链接数据，成功返回1，失败返回0
bool connection::init(int number)
{
	ac = number;
	sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8000);
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	return 1;
}

class game //gameroom and gamemember and gamedata
{
public:
	connection connections;
	struct memberdata
	{
		int64_t QQID = 0;
		int type = 0;//1为卧底，2为平民
		int poll = 0;
		int vote = 0;
		int say = 0;
		memberdata *next = NULL;
	};
	int roomid = 0;
	int gametype = 0;
	int runstate = 0;//游戏房间的运行状态，0为等待，1为开始
	char dinting[15], commoner[15];
	int number = 0;//游戏房间玩家数量
	int deathnumber = 0;//被淘汰玩家的数量
	int64_t *memberlist(int type);
	void addmember(int64_t QQID);
	int deletemember(int64_t QQID);
	int deletemember(int n);
	int startgame(int32_t ac);
	int hadsay(int64_t QQID,char *msg,int32_t ac);
	int hadvote(int64_t voterID,int n);
	int check(int type, int32_t AuthCode);
	int killmember(int64_t QQID);
	int killmember(int rank);
	int memberinhere(int64_t QQID);
	int64_t dintingQQID = 0;
	void putac(int32_t ac);
	void gameover();
	void pollresetting();
	int rank = 0;//发言及投票次序
	void WINAPI onTime();
	//UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2
	bool starttimer();
	bool killtimer();
private:
	MMRESULT timer_id = NULL;
	int32_t AuthCode = 0;
	int max = 0;
	memberdata * head = NULL;
	memberdata * death = NULL;
};
bool game::killtimer()
{
	if (timer_id != NULL)
	{
		timeKillEvent(timer_id);
		CQ_addLog(AuthCode, CQLOG_DEBUG, "谁是卧底", "killtimer");
	}
	return TRUE;
}
void WINAPI onTime1(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	((game*)dwUser)->onTime();
}
bool game::starttimer()
{
	if (timer_id != NULL)
	{
		if (!killtimer())
			return 0;
	}
	timer_id = timeSetEvent(60000, 1, (LPTIMECALLBACK)onTime1, DWORD(this), TIME_PERIODIC);
	if (timer_id == NULL)
	{
		CQ_addLog(AuthCode, CQLOG_DEBUG, "谁是卧底", "can't set timer");
		return FALSE;
	}
	else
		CQ_addLog(AuthCode, CQLOG_DEBUG, "谁是卧底", "start timer");
		return TRUE;
}

void WINAPI game::onTime()
{
	int x;
	char sendmsg[200];
	if (rank == -1)
	{
		check(1, AuthCode);
	}
	else
	{
		memberdata *p = head;
		while (p != NULL)
		{
			if (p->say == 0)
			{
				hadsay(p->QQID, "", AuthCode);
				return;
			}
			p = p->next;
		}
		p = head;
		while (p != NULL)
		{
			CQ_sendPrivateMsg(AuthCode, p->QQID, "出现未知错误，游戏强制结束，房间已被摧毁！");
			p = p->next;
		}
		p = death;
		while (p != NULL)
		{
			CQ_sendPrivateMsg(AuthCode, p->QQID, "出现未知错误，游戏强制结束，房间已被摧毁！");
			p = p->next;
		}
		gameover();
		return;
	}
}
//重置所有存活成员的发言、投票以及被投票数
void game::pollresetting()
{
	memberdata *p = head;
	while (p != NULL)
	{
		p->vote = 0;
		p->poll = 0;
		p->say = 0;
		p = p->next;
	}
}
//
void game::putac(int32_t ac)
{
	AuthCode = ac;
}
//检测某玩家是否在房间列表中，若在则返回其序列号（第3个则返回2），不在则返回-1
int game::memberinhere(int64_t QQID)
{
	memberdata *p = head;
	char sendmsg[200];
	int n = 0;
	if (p == NULL)
	{
		return -1;
	}
	while (p != NULL)
	{
		if (p->QQID == QQID)
		{
			return n;
		}
		else
			p = p->next;
		n++;
	}
	return -1;
}
//将一个玩家移至淘汰列表,成功返回1，失败返回0，若淘汰了卧底则直接返回-1
int game::killmember(int64_t QQID)
{
	memberdata *p = head;
	memberdata *d = death;
	memberdata *t = NULL;
	if (QQID = dintingQQID)
	{
		return -1;
	}
	if (d != NULL)
	{
		while (d->next != NULL)
		{
			d = d->next;
		}
		while (p != NULL)
		{
			if (p->QQID == QQID)
			{
				if (t == NULL)
				{
					head = p->next;
					p->next = NULL;
				}
				else
				{
					t->next = p->next;
					p->next = NULL;
				}
				d->next = p;
				number--;
				deathnumber++;
				return 1;
			}
			t = p;
			p = p->next;
		}
		return 0;
	}
	else
	{
		while (p != NULL)
		{
			if (p->QQID == QQID)
			{
				death = p;
				if (t == NULL)
				{
					head = p->next;
					p->next = NULL;
				}
				else
				{
					t->next = p->next;
					p->next = NULL;
				}
				number--;
				deathnumber++;
				return 1;
			}
			t = p;
			p = p->next;
		}
		return 0;
	}
}
//将存活玩家列表的第n个（0是第一个）淘汰，若淘汰卧底返回-1，成功返回1，失败返回0
int game::killmember(int rank)
{
	memberdata *p = head;
	memberdata *d = death;
	memberdata *t = NULL;
	if (p == NULL)
	{
		return 0;
	}
	for (int x = 0; x < rank; x++)
	{		
		t = p;
		p = p->next;
		if (p == NULL)
		{
			return 0;
		}
	}
	if (p->QQID == dintingQQID)
	{
		return -1;
	}
	if (d == NULL)
	{
		if (t == NULL)
		{
			death = p;
			head = p->next;
			p->next = NULL;
			number--;
			deathnumber++;
			return 1;
		}
		death = p;
		t->next = p->next;
		p->next = NULL;
		number--;
		deathnumber++;
		return 1;
	}
	else
	{
		while (d->next != NULL)
		{
			d = d->next;
		}
		if (t == NULL)
		{
			d->next = p;
			head = p->next;
			p->next = NULL;
			number--;
			deathnumber++;
			return 1;
		}
		d->next = p;
		t->next = p->next;
		p->next = NULL;
		number--;
		deathnumber++;
		return 1;
	}
}
//检查是否全部投票，type1进行强制结算，type0若全部投票则进行结算，没有则返回-1，比赛是否结束若成功均返回1，失败返回0
int  game::check(int type, int32_t ac)
{
	memberdata *p = head;
	int n = 0;
	int t = 0;
	int x = 0;
	char msg[200];
	max = 0;
	if (type)                
	{
		while (p != NULL)
		{
			if (p->vote == 0)
			{
				CQ_sendPrivateMsg(AuthCode, p->QQID, "你超时未投票已被视作弃权，予以扣十分处罚！");
				connections.changeintegral(p->QQID, -10, 0);
			}
			if (p->poll > max)
			{
				max = p->poll;
				n = t;
			}
			t++;
			p = p->next;
		}
		if ((x = killmember(n)) == -1)
		{
			p = head;
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%i号被淘汰了！\n你们发现了卧底，游戏结束，该房间将被摧毁！\n游戏词是:%s-%s", n + 1, commoner, dinting);
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				if (p->type == 2)
				{
					if (0 == connections.changeintegral(p->QQID, 35, 0))
					{
						CQ_addLog(ac, CQLOG_ERROR, "积分模块", "为平民添加积分失败！");
					}
					else
					{
						CQ_sendPrivateMsg(ac, p->QQID, "你获得了35积分！");
					}
				}
				p = p->next;
			}
			p = death;
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				if (0 == connections.changeintegral(p->QQID, 25, 0))
				{
					CQ_addLog(ac, CQLOG_ERROR, "积分模块", "为平民添加积分失败！");
				}
				else
				{
					CQ_sendPrivateMsg(ac, p->QQID, "你获得了25积分！");
				}
				p = p->next;
			}
			gameover();
			return 1;
		}
		else if (x == 0)
		{
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "很抱歉，出现了一个未知的错误，游戏房间将被强制结束！");
			p = head;
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			p = death;
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			gameover();
			return 0;
		}
		else
		{
			p = head;
			t = 0;
			if (number <= 2)
			{
				if (p->QQID == dintingQQID)
				{
					n = 1;
				}
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%i号是卧底，你们没能发现卧底，游戏结束，该房间将被摧毁！\n游戏词是:%s-%s", 2 - n, commoner, dinting);
				while (p != NULL)
				{
					CQ_sendPrivateMsg(ac, p->QQID, msg);
					p = p->next;
				}
				p = death;
				while (p != NULL)
				{
					CQ_sendPrivateMsg(ac, p->QQID, msg);
					p = p->next;
				}
				if (0 == connections.changeintegral(dintingQQID, 50, 0))
				{
					CQ_addLog(ac, CQLOG_ERROR, "积分模块", "为卧底添加积分失败！");
				}
				else
				{
					CQ_sendPrivateMsg(ac, p->QQID, "你获得了50积分！");
				}
				gameover();
				return 1;
			}
			if (0 == connections.changeintegral(dintingQQID, 10, 0))
			{
				CQ_addLog(ac, CQLOG_ERROR, "积分模块", "为卧底添加积分失败！");
			}
			while (p != NULL)
			{
				p->poll = 0;
				p->say = 0;
				p->vote = 0;
				t++;
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%i号被淘汰了，现在你是%i号", n + 1, t);
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			p = death;
			while (p != NULL)
			{
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%i号被淘汰了!", n + 1);
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			rank = 0;
			max = 0;
			starttimer();
			return 1;
		}
	}
	else
	{
		while (p != NULL)
		{
			if (!p->vote)
				return -1;
			if (p->poll > max)
			{
				max = p->poll;
				n = t;
			}
			t++;
			p = p->next;
		}
		if ((x = killmember(n)) == -1)
		{
			p = head;
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%i号被淘汰了！\n你们发现了卧底，游戏结束，该房间将被摧毁！\n游戏词是:%s-%s", n + 1, commoner, dinting);
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				if (p->type == 2)
				{
					if (0 == connections.changeintegral(p->QQID, 35, 0))
					{
						CQ_addLog(ac, CQLOG_ERROR, "积分模块", "为平民添加积分失败！");
					}
				}
				else
				{
					CQ_sendPrivateMsg(ac, p->QQID, "你获得了35积分！");
				}
				p = p->next;
			}
			p = death;
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				if (0 == connections.changeintegral(p->QQID, 25, 0))
				{
					CQ_addLog(ac, CQLOG_ERROR, "积分模块", "为平民添加积分失败！");
				}
				else
				{
					CQ_sendPrivateMsg(ac, p->QQID, "你获得了25积分！");
				}
				p = p->next;
			}
			gameover();
			return -1;
		}
		else if (x == 0)
		{
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "很抱歉，出现了一个未知的错误，游戏房间将被强制结束！");
			p = head;
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			p = death;
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			gameover();
			return 0;
		}
		else
		{
			starttimer();
			p = head;
			t = 0;
			if (number <= 2)
			{
				if (p->QQID == dintingQQID)
				{
					n = 1;
				}
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%i号是卧底，你们没能发现卧底，游戏结束，该房间将被摧毁！\n游戏词是:%s-%s", 2 - n, commoner, dinting);
				while (p != NULL)
				{
					CQ_sendPrivateMsg(ac, p->QQID, msg);
					p = p->next;
				}
				p = death;
				while (p != NULL)
				{
					CQ_sendPrivateMsg(ac, p->QQID, msg);
					p = p->next;
				}
				if (0 == connections.changeintegral(dintingQQID, 50, 0))
				{
					CQ_addLog(ac, CQLOG_ERROR, "积分模块", "为卧底添加积分失败！");
				}
				else
				{
					CQ_sendPrivateMsg(ac, p->QQID, "你获得了50积分！");
				}
				gameover();
				return 1;
			}
			if (0 == connections.changeintegral(dintingQQID, 10, 0))
			{
				CQ_addLog(ac, CQLOG_ERROR, "积分模块", "为卧底添加积分失败！");
			}
			while (p != NULL)
			{
				p->poll = 0;
				p->vote = 0;
				p->say = 0;
				t++;
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%i号被淘汰了，现在你是%i号", n + 1, t);
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			p = death;
			while (p != NULL)
			{
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%i号被淘汰了!", n + 1);
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			rank = 0;
			max = 0;
			return 1;
		}
	}
}
//以id删除一个游戏玩家，成功返回1，失败返回0,删除后失去所有成员则返回-1
int game::deletemember(int64_t QQID)
{
	memberdata *p = head;
	if (p == NULL)
	{
		return 0;
	}
	memberdata *t = head->next;
	if (t == NULL & p->QQID == QQID)
	{
		free(head);
		head = NULL;
		number--;
		return -1;
	}
	else if(t!=NULL & p->QQID==QQID)
	{
		free(head);
		head = t;
		number--;
		return 1;
	}
	else if (t == NULL)
	{
		return FALSE;
	}
	while (t->QQID != QQID)
	{
		if (t->next == NULL)
		{
			return FALSE;
		}
		p = t;
		t = t->next;
	}
	if (t->QQID == QQID)
	{
		free(p->next);
		p->next = t->next;
	}
	number--;
	return TRUE;
}
//以序列号删除一个游戏玩家，成功返回1，失败返回0,删除后失去所有成员则返回-1
int game::deletemember(int n)
{
	memberdata *p = head;
	
	if (n == 0)
	{
		if (head == NULL)
		{
			return 0;
		}
		memberdata *t = head->next;
		if (head->next == NULL)
		{
			free(head);
			head = NULL;
			return -1;
		}
		free(head);
		head = t;
		return 1;
	}
	memberdata *t = head->next;
	if (t == NULL)
	{
		return 0;
	}
	for (int x = 1; x < n; x++)
	{
		p = t;
		t = t->next;
		if (t == NULL)
		{
			return 0;
		}
	}
	free(p->next);
	p->next = t->next;
	return 1;
}
//对玩家的投票进行处理,n即为被投票者的序列号（第x个-1）,成功返回1，玩家已投票返回0
int game::hadvote(int64_t voterID, int n)
{
	memberdata *p = head;
	int t = 0;
	while (p->QQID != voterID)
	{
		if (p->next == NULL)
		{
			return FALSE;
		}
		p = p->next;
	}
	if (p->vote)
	{
		return 0;
	}
	p->vote = 1;
	p = head;
	while (t < n)
	{
		p = p->next;
		if (p == NULL)
		{
			return -1;
		}
		t++;
	}
	p->poll++;
	return 1;
}
//将某个玩家添加至玩家列表
void game::addmember(int64_t QQID)
{
	memberdata *p = head;
	memberdata *newtail;
	newtail = new memberdata;
	for (int x = 0; x < 10000; x++)
	{
		if (newtail != NULL)
		{
			break;
		}
		newtail = new memberdata;
	}
	if (newtail == NULL)
	{
		return;
	}
	else
	{
		memset(newtail, 0, sizeof(memberdata));
	}
	if (p == NULL)
	{
		newtail->QQID = QQID;
		head = newtail;
		number++;
		return;
	}
	while (p->next != NULL)
	{
		p = p->next;
	}
	newtail->QQID = QQID;
	p->next = newtail;
	number++;
	return;
}
//type1为所有存活玩家列表，type0为未投票玩家列表,type-1为被淘汰及观战玩家列表
int64_t * game::memberlist(int type)
{
	int64_t * IDlist;
	int n = 0;
	memberdata *p = NULL;
	
	
	if (type == 1)
	{
		p = head;
		do
		{
			n++;
			p = p->next;
		} while (p != NULL);
		IDlist = (int64_t *)calloc(n, sizeof(int64_t));
		p = head;
		n = 0;
		do
		{
			IDlist[n] = p->QQID;
			n++;
			p = p->next;
		} while (p != NULL);
	}
	else if (type == -1)
	{
		p = death;
		do
		{
			n++;
			p = p->next;
		} while (p != NULL);
		IDlist = (int64_t *)calloc(n, sizeof(int64_t));
		p = head;
		n = 0;
		do
		{
			IDlist[n] = p->QQID;
			n++;
			p = p->next;
		} while (p != NULL);
	}
	else
	{
		do
		{
			if (p->vote == 0)
			{
				n++;
			}
			p = p->next;
		} while (p != NULL);
		IDlist = (int64_t *)calloc(n, sizeof(int64_t));
		p = head;
		n = 0;
		do
		{
			if (p->vote == 0)
			{
				IDlist[n] = p->QQID;
				n++;
			}
			p = p->next;
		} while (p != NULL);
	}
	return IDlist;
}
//开始游戏，向服务器询问游戏词并进行游戏准备处理,游戏已开始返回-1，成功开启返回卧底序列号
int game::startgame(int32_t ac)
{
	connections.init(ac);
	int i = 0;
	int k = 0;
	char sendmsg[200];
	memberdata *p = head;
	AuthCode = ac;
	if (runstate)
	{
		return -1;
	}
	connections.init(ac);
	runstate = 1;
	srand(time(NULL));
	k = 1 + rand() % 41;
	sprintf(sendmsg, "user:1839538956\ntype:game\nrank:%i",k );
	char *msg = connections.ask(sendmsg);
	srand(time(NULL));
	k = rand() % number;
	memset(commoner, 0, sizeof(commoner));
	memset(dinting, 0, sizeof(dinting));
	for (i = 0; i < 15; i++)
	{
		if (msg[i] == ' '| msg[i] == '\0')
			break;
		commoner[i] = msg[i];
	}
	i++;
	for (int t = 0; t < 15; t++, i++)
	{
		if (msg[i] == ' ' | msg[i] == '\0')
		{
			break;
		}
		dinting[t] = msg[i];
	}
	i = 0;
	while (p != NULL)
	{
		CQ_sendPrivateMsg(ac,p->QQID,"游戏开始！");
		CQ_sendPrivateMsg(ac, p->QQID, "当轮到你发言时请给我发送一句话描述我发给你的单词，不要和别人重复，然后将票投给你认为对方描述的事物与你不同的玩家。");
		if (i != k)
		{
			p->type = 2;
			CQ_sendPrivateMsg(AuthCode, p->QQID, commoner);
		}
		else
		{
			p->type = 1;
			dintingQQID = p->QQID;
			CQ_sendPrivateMsg(AuthCode, p->QQID, dinting);
		}
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf(sendmsg, "你是%i号玩家!", i + 1);
		CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
		CQ_sendPrivateMsg(ac, p->QQID, "由1号玩家先发言!");
		p = p->next;
		i++;
	}
	starttimer();
	return k;
}
//如果还未轮到该玩家发言返回0,发言成功返回序列号，已发言过则返回-1，进入投票阶段则返回-2
int game::hadsay(int64_t QQID,char *msg,int32_t ac)
{
	int n = 0;
	memberdata *p = head;
	char sendmsg[200];
	for (; n < number; n++)
	{
		if (QQID == p->QQID)
		{
			if (p->say == 1)
			{
				return -1;
			}
			else
			{
				p->say = 1;
				rank++;
				if (rank == number)
				{
					rank = -1;
					p = head;
					while (p != NULL)
					{
						if (!strcmp(msg, ""))
						{
							CQ_sendPrivateMsg(ac, p->QQID, "最后一名玩家超时未发言，现在进入投票阶段");
						}
						else
						{
							CQ_sendPrivateMsg(ac, p->QQID, msg);
							CQ_sendPrivateMsg(ac, p->QQID, "最后一名玩家已发言，请投票！");
						}
						// mark wait for set timer
						p = p->next;
					}
					p = death;
					while (p != NULL)
					{
						if (!strcmp(msg, ""))
						{
							CQ_sendPrivateMsg(ac, p->QQID, "最后一名玩家超时未发言，现在进入投票阶段并予以扣十分处罚！");
						}
						else
						{
							CQ_sendPrivateMsg(ac, p->QQID, msg);
							CQ_sendPrivateMsg(ac, p->QQID, "最后一名玩家已发言！");
						}
						// mark wait for set timer
						p = p->next;
					}
					starttimer();
					return -2;
				}
				else
				{
					p = head;
					n = 0;
					while (p != NULL)
					{
						if (!strcmp(msg, ""))
						{
							memset(sendmsg, 0, sizeof(sendmsg));
							if (n == rank)
							{
								sprintf(sendmsg, "%i号玩家超时未发言，接下来轮到你发言！",rank);
							}
							else
								sprintf(sendmsg, "%i号玩家超时未发言，接下来轮到%i号玩家发言！", rank, rank + 1);
							CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
						}
						else
						{
							memset(sendmsg, 0, sizeof(sendmsg));
							sprintf(sendmsg, "%i号:%s", rank, msg);
							CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
							memset(sendmsg, 0, sizeof(sendmsg));
							if (n == rank)
							{
								sprintf(sendmsg, "%i号玩家已发言，接下来轮到你发言！",rank);
							}
							else
								sprintf(sendmsg, "%i号玩家已发言，接下来轮到%i号玩家发言！", rank, rank + 1);
							CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
						}						
						// mark wait for set timer
						p = p->next;
						n++;
					}
					p = death;
					while (p != NULL)
					{
						if (!strcmp(msg, ""))
						{
							memset(sendmsg, 0, sizeof(sendmsg));
							sprintf(sendmsg, "%i号玩家超时未发言，接下来轮到%i号玩家发言！", rank, rank + 1);
							CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
						}
						else
						{
							memset(sendmsg, 0, sizeof(sendmsg));
							sprintf(sendmsg, "%i号:%s", rank, msg);
							CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
							memset(sendmsg, 0, sizeof(sendmsg));
							sprintf(sendmsg, "%i号玩家已发言，接下来轮到%i号玩家发言！", rank, rank + 1);
							CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
						}
						// mark wait for set timer
						p = p->next;
					}
				}
				starttimer();
				return rank;
			}
		}
		if (p->say == 0)
		{
			return 0;
		}
		p = p->next;
	}
	return 0;
}
//清除游戏房间的所有数据
void game::gameover()
{
	roomid = 0;
	gametype = 0;
	runstate = 0;
	memset(dinting, 0, sizeof(dinting));
	memset(commoner, 0, sizeof(commoner));
	number = 0;
	deathnumber = 0;
	dintingQQID = 0;
	rank = 0;
	AuthCode = 0;
	max = 0;
	head = NULL;
	death = NULL;
	if (timer_id != NULL)
	{
		killtimer();
	}
}


class werewolf
{
public:
	connection  connections;
	struct memberdata
	{
		int64_t QQID = 0;
		char role[10] = "";
		bool AlreadyDead = FALSE;
		memberdata *next = NULL;
	};//个人数据结构
	int number = 0;//该房间的人数
	int roomid = 0;
	memberdata *head = NULL;
	werewolf *next = NULL;
	bool state = FALSE;//游戏是否处于开始状态
	bool addmember(int64_t QQID);
	int findmember(int64_t QQID);
	int createRoom(int64_t QQID);
	int joinRoom(const char *msg, int64_t QQID);
	int leaveRoom(int64_t QQID);
	int RunRoom(int64_t QQID);
private:
};
//创建一个房间并将操作者加入到该房间中，成功返回roomid，失败返回0
int werewolf::createRoom(int64_t QQID)
{
	werewolf *room = next;
	if (head != NULL)//存在玩家说明该房间已存在，创建新房间加入到链表中
	{
		if (room == NULL)//下一个房间不存在则直接创建房间
		{
			next = new werewolf;
			if (next == NULL)
			{
				return 0;
			}
			return next->createRoom(QQID);
		}
		else//否则调用下一个房间的创建房间函数
		{
			return room->createRoom(QQID);
		}
	}
	else
	{
		if (addmember(QQID) == FALSE)
		{
			return 0;
		}
		srand(time(NULL));
		roomid = 100000 + rand() % 900000;
		return roomid;
	}
}
//将一个成员加入到该房间所在的序列表中，成功返回true，失败返回false
bool werewolf::addmember(int64_t QQID)
{
	memberdata *p = head;
	memberdata *newtail;
	newtail = new memberdata;
	for (int x = 0; x < 10000; x++)
	{
		if (newtail != NULL)
		{
			break;
		}
		newtail = new memberdata;
	}
	if (newtail == NULL)
	{
		return FALSE;
	}
	else
	{
		memset(newtail, 0, sizeof(memberdata));
	}
	if (p == NULL)
	{
		newtail->QQID = QQID;
		head = newtail;
		number++;
		return TRUE;
	}
	while (p->next != NULL)
	{
		p = p->next;
	}
	newtail->QQID = QQID;
	p->next = newtail;
	number++;
	return TRUE;
}
//检测某玩家在房间的序列号，不在返回-1，若在则返回0为第一个的序列号
int werewolf::findmember(int64_t QQID)
{
	memberdata *p = head;
	int n = 0;
	if (p == NULL)
	{
		return -1;
	}
	while (p != NULL)
	{
		if (p->QQID == QQID)
		{
			return n;
		}
		else
			p = p->next;
		n++;
	}
	return -1;
}
//根据Msg将某个玩家加入到房间中，若不存在该房间则返回-1，已加入某个房间则返回1，加入成功返回2，加入失败返回0，该房间已开始返回-2
int werewolf::joinRoom(const char *msg, int64_t QQID)
{
	werewolf *p = next;
	if (head == NULL)
	{
		return -1;
	}
	while (p != NULL)
	{
		if (p->findmember(QQID) > -1)
		{
			return 1;
		}
		p = p->next;
	}
	p = next;
	while (p != NULL)
	{
		if (p->roomid == atoi(msg))
		{
			if (state == TRUE)
			{
				return -2;
			}
			if (p->addmember(QQID) == TRUE)
				return 2;
			else
				return 0;
		}
	}
	return -1;
}
//将玩家从所有房间中退出，成功返回1，不存在于任何房间返回0，试图退出的房间已开始返回-1
int werewolf::leaveRoom(int64_t QQID)
{
	werewolf *p = next;
	if (head == NULL)
	{
		return 0;
	}
	while (p != NULL)
	{
		if (p->findmember(QQID) > -1)
		{
			if (state == 1)
			{
				return -1;
			}
			memberdata *t = head;
			memberdata *p = head->next;
			if (head->QQID == QQID)
			{
				head = head->next;
				t->next = NULL;
				number--;
				return 1;
			}
			else
			{
				while (p != NULL)
				{
					if (p->QQID == QQID)
					{
						t->next = p->next;
						p->next = NULL;
						number--;
						return 1;
					}
					else
					{
						t = p;
						p = p->next;
					}
				}
				return 0;
			}
		}
		p = p->next;
	}
	return 0;
}
//没有任何房间可以开始返回0，如果房间已开始则返回-1，人数不足6人则返回-2，
int werewolf::RunRoom(int64_t QQID)
{
	werewolf *p = next;
	if (head == NULL)
	{
		return 0;
	}
	while (p != NULL)
	{
		if (p->findmember(QQID) == 0)
		{
			if (state == 1)
			{
				return -1;
			}
			if (number <= 5)
			{
				return -2;
			}
			state = 1;
		}
		p = p->next;
	}
	return 0;
}

class Idioms 
{
public:
	connection connections;
	int64_t GroupID;
	struct idiomdata
	{
		char content[10] = "\0";
		idiomdata *next = NULL;
	}idiom;
	struct memberdata
	{
		int64_t QQID = 0;
		int number = 0;
		memberdata *next = NULL;
	}member;
	bool killtimer();
	void WINAPI onTime();
	bool starttimer();
	bool startgame(int64_t fromGroup, int32_t ac);
	int checkidiom(char *content, int64_t fromQQ);
	Idioms * next = NULL;
private:
	idiomdata * now = &idiom;
	int32_t AuthCode;
	MMRESULT timer_id = NULL;
};
bool Idioms::killtimer()
{
	if (timer_id != NULL)
	{
		timeKillEvent(timer_id);
		CQ_addLog(AuthCode, CQLOG_DEBUG, "成语接龙", "killtimer");
	}
	return TRUE;
}
void WINAPI onTime2(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	((Idioms*)dwUser)->onTime();
}
bool Idioms::starttimer()
{
	if (timer_id != NULL)
	{
		if (!killtimer())
			return 0;
	}
	timer_id = timeSetEvent(30000, 1, (LPTIMECALLBACK)onTime2, DWORD(this), TIME_PERIODIC);
	if (timer_id == NULL)
	{
		CQ_addLog(AuthCode, CQLOG_DEBUG, "成语接龙", "can't set timer");
		return FALSE;
	}
	else
		CQ_addLog(AuthCode, CQLOG_DEBUG, "成语接龙", "start timer");
	return TRUE;
}

void WINAPI Idioms::onTime()
{
	int64_t list[3];
	int number[3] = { 0,0,0 };
	char sendmsg[200];
	for (memberdata *p = &member; p != NULL; p = p->next)
	{
		if (p->number > number[0])
		{
			number[2] = number[1];
			list[2] = list[1];
			number[1] = number[0];
			list[1] = list[0];
			number[0] = p->number;
			list[0] = p->QQID;
			continue;
		}
		if (p->number > number[1])
		{
			number[2] = number[1];
			list[2] = list[1];
			number[1] = p->number;
			list[1] = p->QQID;
			continue;
		}
		if (p->number > number[2])
		{
			number[2] = p->number;
			list[2] = p->QQID;
		}
	}
	memset(sendmsg, 0, sizeof(sendmsg));
	if (number[2] == 0)
	{
		if (number[0] == 0)
		{
			CQ_sendGroupMsg(AuthCode, GroupID, "时间到，本次成语接龙结束了！哎，这么大的一个群居然一个能答的都没有！");
			killtimer();
			GroupID = 0;
			return;
		}
		CQ_sendGroupMsg(AuthCode, GroupID, "时间到，本次成语接龙结束了！参与游戏的人数少于三人，不显示本次游戏排名！");
		killtimer();
		GroupID = 0;
		return;
	}
	sprintf_s(sendmsg, "由于超时，本次成语接龙结束了！\n本次的前三名及回答数是:\n%s--%i\n%s--%i\n%s--%i", connections.getat(list[0]), number[0], connections.getat(list[1]), number[1], connections.getat(list[2]), number[2]);
	CQ_sendGroupMsg(AuthCode, GroupID, sendmsg);
	killtimer();
	GroupID = 0;
	return;
}
//
bool Idioms::startgame(int64_t fromGroup, int32_t ac)
{
	connections.init(ac);
	char sendmsg[200];
	sprintf_s(sendmsg, "user:%lld\ntype:idiom\naction:get\nsendby:soul", fromGroup);
	char *recData = connections.ask(sendmsg);
	if (recData == NULL)
	{
		CQ_sendGroupMsg(ac, fromGroup, "成语接龙功能出现未知错误，请联系维护人员处理！");
		return 0;
	}
	else
	{
		GroupID = fromGroup;
		AuthCode = ac;
		strcpy(idiom.content, recData);
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "游戏开始了，抽中的第一个成语为：%s\n请在半分钟内回复且不要重复！", recData);
		CQ_sendGroupMsg(ac, fromGroup, sendmsg);
		starttimer();
		if (next == NULL)
		{
			next = new Idioms;
		}
		return 1;
	}
}
//
int Idioms::checkidiom(char *content, int64_t fromQQ)
{
	std::string first = "";
	std::string second = "";
	ChineseConvert(now->content, first, 1);
	ChineseConvert(content, second, 0);
	if (first == second)
	{
		for (idiomdata *p = &idiom;; p = p->next)
		{
			if (!strcmp(p->content, content))
			{
				return -1;
			}
			if (p == now)
			{
				break;
			}
		}
		char sendmsg[200];
		memset(sendmsg, 0, sizeof(sendmsg));
		sprintf_s(sendmsg, "user:%lld\ntype:idiom\naction:confirm\ncontent:%s\nsendby:soul", fromQQ, content);
		char *recData = connections.ask(sendmsg);
		if (recData == NULL)
		{
			return -2;
		}
		else
		{
			if (atoi(recData) == 1)
			{
			}
			else
			{
				return -2;
			}
		}
		now->next = new idiomdata;
		strcpy(now->next->content, content);
		now = now->next;
		connections.changeintegral(fromQQ, 5, 0);
		for (memberdata *p = &member;; p = p->next)
		{
			if (p->QQID == fromQQ)
			{
				p->number++;
				starttimer();
				return 1;
			}
			else if (p->QQID == 0)
			{
				p->QQID = fromQQ;
				p->number++;
				p->next = new memberdata;
				starttimer();
				return 1;
			}
		}
	}
	else
	{
		return 0;
	}
}

int ChineseConvert(const std::string& one_chinese, std::string& two_chinese, int type) {
	const int spell_value[] = { -20319, -20317, -20304, -20295, -20292, -20283, -20265, -20257, -20242, -20230, -20051, -20036, -20032, -20026,
		-20002, -19990, -19986, -19982, -19976, -19805, -19784, -19775, -19774, -19763, -19756, -19751, -19746, -19741, -19739, -19728,
		-19725, -19715, -19540, -19531, -19525, -19515, -19500, -19484, -19479, -19467, -19289, -19288, -19281, -19275, -19270, -19263,
		-19261, -19249, -19243, -19242, -19238, -19235, -19227, -19224, -19218, -19212, -19038, -19023, -19018, -19006, -19003, -18996,
		-18977, -18961, -18952, -18783, -18774, -18773, -18763, -18756, -18741, -18735, -18731, -18722, -18710, -18697, -18696, -18526,
		-18518, -18501, -18490, -18478, -18463, -18448, -18447, -18446, -18239, -18237, -18231, -18220, -18211, -18201, -18184, -18183,
		-18181, -18012, -17997, -17988, -17970, -17964, -17961, -17950, -17947, -17931, -17928, -17922, -17759, -17752, -17733, -17730,
		-17721, -17703, -17701, -17697, -17692, -17683, -17676, -17496, -17487, -17482, -17468, -17454, -17433, -17427, -17417, -17202,
		-17185, -16983, -16970, -16942, -16915, -16733, -16708, -16706, -16689, -16664, -16657, -16647, -16474, -16470, -16465, -16459,
		-16452, -16448, -16433, -16429, -16427, -16423, -16419, -16412, -16407, -16403, -16401, -16393, -16220, -16216, -16212, -16205,
		-16202, -16187, -16180, -16171, -16169, -16158, -16155, -15959, -15958, -15944, -15933, -15920, -15915, -15903, -15889, -15878,
		-15707, -15701, -15681, -15667, -15661, -15659, -15652, -15640, -15631, -15625, -15454, -15448, -15436, -15435, -15419, -15416,
		-15408, -15394, -15385, -15377, -15375, -15369, -15363, -15362, -15183, -15180, -15165, -15158, -15153, -15150, -15149, -15144,
		-15143, -15141, -15140, -15139, -15128, -15121, -15119, -15117, -15110, -15109, -14941, -14937, -14933, -14930, -14929, -14928,
		-14926, -14922, -14921, -14914, -14908, -14902, -14894, -14889, -14882, -14873, -14871, -14857, -14678, -14674, -14670, -14668,
		-14663, -14654, -14645, -14630, -14594, -14429, -14407, -14399, -14384, -14379, -14368, -14355, -14353, -14345, -14170, -14159,
		-14151, -14149, -14145, -14140, -14137, -14135, -14125, -14123, -14122, -14112, -14109, -14099, -14097, -14094, -14092, -14090,
		-14087, -14083, -13917, -13914, -13910, -13907, -13906, -13905, -13896, -13894, -13878, -13870, -13859, -13847, -13831, -13658,
		-13611, -13601, -13406, -13404, -13400, -13398, -13395, -13391, -13387, -13383, -13367, -13359, -13356, -13343, -13340, -13329,
		-13326, -13318, -13147, -13138, -13120, -13107, -13096, -13095, -13091, -13076, -13068, -13063, -13060, -12888, -12875, -12871,
		-12860, -12858, -12852, -12849, -12838, -12831, -12829, -12812, -12802, -12607, -12597, -12594, -12585, -12556, -12359, -12346,
		-12320, -12300, -12120, -12099, -12089, -12074, -12067, -12058, -12039, -11867, -11861, -11847, -11831, -11798, -11781, -11604,
		-11589, -11536, -11358, -11340, -11339, -11324, -11303, -11097, -11077, -11067, -11055, -11052, -11045, -11041, -11038, -11024,
		-11020, -11019, -11018, -11014, -10838, -10832, -10815, -10800, -10790, -10780, -10764, -10587, -10544, -10533, -10519, -10331,
		-10329, -10328, -10322, -10315, -10309, -10307, -10296, -10281, -10274, -10270, -10262, -10260, -10256, -10254 };

	// 395个字符串，每个字符串长度不超过6  
	const char spell_dict[396][7] = { "a", "ai", "an", "ang", "ao", "ba", "bai", "ban", "bang", "bao", "bei", "ben", "beng", "bi", "bian", "biao",
		"bie", "bin", "bing", "bo", "bu", "ca", "cai", "can", "cang", "cao", "ce", "ceng", "cha", "chai", "chan", "chang", "chao", "che", "chen",
		"cheng", "chi", "chong", "chou", "chu", "chuai", "chuan", "chuang", "chui", "chun", "chuo", "ci", "cong", "cou", "cu", "cuan", "cui",
		"cun", "cuo", "da", "dai", "dan", "dang", "dao", "de", "deng", "di", "dian", "diao", "die", "ding", "diu", "dong", "dou", "du", "duan",
		"dui", "dun", "duo", "e", "en", "er", "fa", "fan", "fang", "fei", "fen", "feng", "fo", "fou", "fu", "ga", "gai", "gan", "gang", "gao",
		"ge", "gei", "gen", "geng", "gong", "gou", "gu", "gua", "guai", "guan", "guang", "gui", "gun", "guo", "ha", "hai", "han", "hang",
		"hao", "he", "hei", "hen", "heng", "hong", "hou", "hu", "hua", "huai", "huan", "huang", "hui", "hun", "huo", "ji", "jia", "jian",
		"jiang", "jiao", "jie", "jin", "jing", "jiong", "jiu", "ju", "juan", "jue", "jun", "ka", "kai", "kan", "kang", "kao", "ke", "ken",
		"keng", "kong", "kou", "ku", "kua", "kuai", "kuan", "kuang", "kui", "kun", "kuo", "la", "lai", "lan", "lang", "lao", "le", "lei",
		"leng", "li", "lia", "lian", "liang", "liao", "lie", "lin", "ling", "liu", "long", "lou", "lu", "lv", "luan", "lue", "lun", "luo",
		"ma", "mai", "man", "mang", "mao", "me", "mei", "men", "meng", "mi", "mian", "miao", "mie", "min", "ming", "miu", "mo", "mou", "mu",
		"na", "nai", "nan", "nang", "nao", "ne", "nei", "nen", "neng", "ni", "nian", "niang", "niao", "nie", "nin", "ning", "niu", "nong",
		"nu", "nv", "nuan", "nue", "nuo", "o", "ou", "pa", "pai", "pan", "pang", "pao", "pei", "pen", "peng", "pi", "pian", "piao", "pie",
		"pin", "ping", "po", "pu", "qi", "qia", "qian", "qiang", "qiao", "qie", "qin", "qing", "qiong", "qiu", "qu", "quan", "que", "qun",
		"ran", "rang", "rao", "re", "ren", "reng", "ri", "rong", "rou", "ru", "ruan", "rui", "run", "ruo", "sa", "sai", "san", "sang",
		"sao", "se", "sen", "seng", "sha", "shai", "shan", "shang", "shao", "she", "shen", "sheng", "shi", "shou", "shu", "shua",
		"shuai", "shuan", "shuang", "shui", "shun", "shuo", "si", "song", "sou", "su", "suan", "sui", "sun", "suo", "ta", "tai",
		"tan", "tang", "tao", "te", "teng", "ti", "tian", "tiao", "tie", "ting", "tong", "tou", "tu", "tuan", "tui", "tun", "tuo",
		"wa", "wai", "wan", "wang", "wei", "wen", "weng", "wo", "wu", "xi", "xia", "xian", "xiang", "xiao", "xie", "xin", "xing",
		"xiong", "xiu", "xu", "xuan", "xue", "xun", "ya", "yan", "yang", "yao", "ye", "yi", "yin", "ying", "yo", "yong", "you",
		"yu", "yuan", "yue", "yun", "za", "zai", "zan", "zang", "zao", "ze", "zei", "zen", "zeng", "zha", "zhai", "zhan", "zhang",
		"zhao", "zhe", "zhen", "zheng", "zhi", "zhong", "zhou", "zhu", "zhua", "zhuai", "zhuan", "zhuang", "zhui", "zhun", "zhuo",
		"zi", "zong", "zou", "zu", "zuan", "zui", "zun", "zuo" };

	try {
		// 循环处理字节数组  
		if (type == 1)
		{
			for (int j = 6, chrasc = 6; j < 8;) {
				// 非汉字处理  
				if (one_chinese.at(j) >= 0 && one_chinese.at(j) < 128) {
					two_chinese += one_chinese.at(j);
					// 偏移下标  
					j++;
					continue;
				}

				// 汉字处理  
				chrasc = one_chinese.at(j) * 256 + one_chinese.at(j + 1) + 256;
				if (chrasc > 0 && chrasc < 160) {
					// 非汉字  
					two_chinese += one_chinese.at(j);
					// 偏移下标  
					j++;
				}
				else {
					// 汉字  
					for (int i = (sizeof(spell_value) / sizeof(spell_value[0]) - 1); i >= 0; --i) {
						// 查找字典  
						if (spell_value[i] <= chrasc) {
							two_chinese += spell_dict[i];
							break;
						}
					}
					// 偏移下标 （汉字双字节）  
					j += 2;
				}
			} // for end  
		}
		else
		{
			for (int j = 0, chrasc = 0; j < 2;) {
				// 非汉字处理  
				if (one_chinese.at(j) >= 0 && one_chinese.at(j) < 128) {
					two_chinese += one_chinese.at(j);
					// 偏移下标  
					j++;
					continue;
				}

				// 汉字处理  
				chrasc = one_chinese.at(j) * 256 + one_chinese.at(j + 1) + 256;
				if (chrasc > 0 && chrasc < 160) {
					// 非汉字  
					two_chinese += one_chinese.at(j);
					// 偏移下标  
					j++;
				}
				else {
					// 汉字  
					for (int i = (sizeof(spell_value) / sizeof(spell_value[0]) - 1); i >= 0; --i) {
						// 查找字典  
						if (spell_value[i] <= chrasc) {
							two_chinese += spell_dict[i];
							break;
						}
					}
					// 偏移下标 （汉字双字节）  
					j += 2;
				}
			} // for end  
		}

	}
	catch (exception _e) {
		std::cout << _e.what() << std::endl;
		return -1;
	}
	return 0;
}