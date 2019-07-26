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
int Detection(const char *msg);//���
int Identify(const char *msg);
char *strcattxt(char *a, char *b);
char *strcatdat(char *a, char *b);
char *newstrcat(char *a, char *b);

//�ṹ��洢master��Ϣ
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
//��������������н����Ķ���
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
	char *raffle(int64_t QQID);
	int getmaster(masterlist *master, int64_t QQID);
	int listslave(int64_t *list, int64_t QQID); //mark �����������б��Ϊֱ�ӷ��ؿɷ��͵���Ϣ��������Ϣ��ʽΪQQID��name��������ǩ��״̬��
	int deleteslave(int64_t slave);
	int buy_back(int64_t QQID);
	int buyslave(int64_t masterID, int64_t slaveID);
	int slave_putname(int64_t masterID, int64_t slaveID, char *name);
	int64_t getIDbyname_slave(int64_t user, char *name);
	char *buycheck(int64_t masterID, int64_t slaveID);
	bool put_Administorforgroup(int64_t QQID, int64_t GroupID, int rank);//mark ��δ��ɵĹ���
	char *sign(int64_t QQID, int64_t groupid);
	int changeintegral(int64_t QQID, int number,int type);
	int getintegral(int64_t QQID,QDdata *sign);
	char *getat(int64_t QQID);
	char *ask(char *content);
	char *askagain(char *content);
	int getweatherlist(int64_t *grouplist, int kind);
};
//��ȡ��ӦKind��qqlist���������Ӧqq��������Ԥ�����ɹ�����������grouplist������1��ʧ�ܷ���0��kind0Ϊ����qq��kind1ΪȺqq
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
	char sendmsg[20];
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:sign\naction:getat\nsendby:soul", QQID);
	return ask(sendmsg);
}
//ʧ�ܷ���NULL
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
			Sleep(1000);//����1s����������ʱ��
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
			Sleep(1000);//����1s����������ʱ��
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
		return NULL;//������������
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
		return NULL;//����������ʧ��
	}
	if (-1 == send(sclient, content, strlen(content), 0))
	{
		closesocket(sclient);
		return NULL;//��������ʧ��
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
		return NULL;//������������
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

//�ɹ�����ֵ�洢��sign�в�����1��ʧ���򷵻�0
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
	//����ֵ��ǰ��Ϊ���ڣ����Ϊ���֣�����Ϊ��2019-7-22 500
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
//�ı�ָ��id�Ļ��֣�ʧ�ܷ���0���ɹ����ػ��ֵĸı�ֵ��number����Ϊ0,typeΪ1Ϊ���master��typeΪ0Ϊ�����
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
//�ɹ���ط���1��������ط���2�����ֲ��㷵��-number��numberΪ��������������������0
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
//���������������ū�����󣬳ɹ�����1�����󷵻�0���Է��Զ�ʹ�ñ���������-1
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
//����Ƿ���Թ���ĳ����ң��ڱ������ڲ��ɹ��򷵻�-1+msg����Ҫ����ǿ�ƹ��򿨷���-1+msg
//�����Լ���ū������-1+msg����ͼ�����Լ������˷���-1+msg�����ֱ����ڷ���-1+msg
//���ֲ�������-1+msg�����󷵻�0���ɹ��򷵻ع�����Ҫ��1+msg����Ҫ����ǿ�ƹ��򿨷���1+msg
char *connection::buycheck(int64_t masterID, int64_t slaveID)
{
	char sendmsg[200];
	char *recData = NULL;
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:slave\naction:buycheck\nslaveid:%lld\nsendby:soul", masterID, slaveID);
	recData = ask(sendmsg);
	return recData;
}
//
bool connection::put_Administorforgroup(int64_t QQID, int64_t GroupID, int rank)
{

}
//��ū���������ʹ��ȡ����Ϊū��ȡ�����ɹ�����1��ʧ�ܷ���0��ȱ��ȡ��������-2�����ǶԷ������˷���-1
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
//����name��user��ȡ�Է���id���������ڸ�name����-1�������Ҹ�UserΪ�����˷���1���������Ҹ�User��Ϊ�����˷�����id��������0
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
//�ɹ�����1��ʧ�ܷ���0
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
//�ɹ�����1��ʧ�ܷ���0
int connection::listslave(int64_t *list, int64_t QQID)
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
	while (recData[i] != '\0')
	{
		while (recData[i] != ' ' & recData[i] != '\0')
		{
			sub[t] = recData[i];
			i++;
			t++;
		}
		i++;
		t = 0;
		list[x] = atoll(sub);
		x++;
		if (x == 10)
			break;
		memset(sub,0,sizeof(sub));
	}
	return 1;
}
//��ָ��id��master�ɹ�����1��ʧ�ܷ���0
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
//�г�ָ��id��ӵ�е����е��ߣ�����ֵ��Ϊ�����Ĳ�ѯ�������ʽΪ��[CQ:at,qq=QQID],��ӵ��xxx��
char * connection::listprop(int64_t QQID)
{
	char sendmsg[200];
	memset(sendmsg, 0, sizeof(sendmsg));
	sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:list\nsendby:soul",QQID);
	return ask(sendmsg);
}

//Ϊָ����qq����һ�γ齱�����س齱������齱ʧ�ܷ���NULL
char *connection::raffle(int64_t QQID)
{
	char sendmsg[200];
	sprintf_s(sendmsg, "user:%lld\ntype:prop\naction:raffle\nsendby:soul",QQID);
	return ask(sendmsg);
}

//QQIDΪ��ȡ������id��rankΪ���߱�ţ�typeΪ1����ָ�����ߣ�typeΪ0����ָ�����ߣ�typeΪ2����ָ�������Ƿ���ڣ�ʧ�ܾ�����0���ɹ�����1��û�иõ��߷���-1��
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
//��ʼ���������ݣ��ɹ�����1��ʧ�ܷ���0
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
		int type = 0;//1Ϊ�Եף�2Ϊƽ��
		int poll = 0;
		int vote = 0;
		int say = 0;
		memberdata *next = NULL;
	};
	int roomid = 0;
	int gametype = 0;
	int runstate = 0;//��Ϸ���������״̬��0Ϊ�ȴ���1Ϊ��ʼ
	char dinting[15], commoner[15];
	int number = 0;//��Ϸ�����������
	int deathnumber = 0;//����̭��ҵ�����
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
	int rank = 0;//���Լ�ͶƱ����
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
		CQ_addLog(AuthCode, CQLOG_DEBUG, "˭���Ե�", "killtimer");
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
		CQ_addLog(AuthCode, CQLOG_DEBUG, "˭���Ե�", "can't set timer");
		return FALSE;
	}
	else
		CQ_addLog(AuthCode, CQLOG_DEBUG, "˭���Ե�", "start timer");
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
			CQ_sendPrivateMsg(AuthCode, p->QQID, "����δ֪������Ϸǿ�ƽ����������ѱ��ݻ٣�");
			p = p->next;
		}
		p = death;
		while (p != NULL)
		{
			CQ_sendPrivateMsg(AuthCode, p->QQID, "����δ֪������Ϸǿ�ƽ����������ѱ��ݻ٣�");
			p = p->next;
		}
		gameover();
		return;
	}
}
//�������д���Ա�ķ��ԡ�ͶƱ�Լ���ͶƱ��
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
//���ĳ����Ƿ��ڷ����б��У������򷵻������кţ���3���򷵻�2���������򷵻�-1
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
//��һ�����������̭�б�,�ɹ�����1��ʧ�ܷ���0������̭���Ե���ֱ�ӷ���-1
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
//���������б�ĵ�n����0�ǵ�һ������̭������̭�Ե׷���-1���ɹ�����1��ʧ�ܷ���0
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
//����Ƿ�ȫ��ͶƱ��type1����ǿ�ƽ��㣬type0��ȫ��ͶƱ����н��㣬û���򷵻�-1�������Ƿ�������ɹ�������1��ʧ�ܷ���0
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
				CQ_sendPrivateMsg(AuthCode, p->QQID, "�㳬ʱδͶƱ�ѱ�������Ȩ�����Կ�ʮ�ִ�����");
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
			sprintf(msg, "%i�ű���̭�ˣ�\n���Ƿ������Եף���Ϸ�������÷��佫���ݻ٣�\n��Ϸ����:%s-%s", n + 1, commoner, dinting);
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				if (p->type == 2)
				{
					if (0 == connections.changeintegral(p->QQID, 35, 0))
					{
						CQ_addLog(ac, CQLOG_ERROR, "����ģ��", "Ϊƽ����ӻ���ʧ�ܣ�");
					}
					else
					{
						CQ_sendPrivateMsg(ac, p->QQID, "������35���֣�");
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
					CQ_addLog(ac, CQLOG_ERROR, "����ģ��", "Ϊƽ����ӻ���ʧ�ܣ�");
				}
				else
				{
					CQ_sendPrivateMsg(ac, p->QQID, "������25���֣�");
				}
				p = p->next;
			}
			gameover();
			return 1;
		}
		else if (x == 0)
		{
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "�ܱ�Ǹ��������һ��δ֪�Ĵ�����Ϸ���佫��ǿ�ƽ�����");
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
				sprintf(msg, "%i�����Եף�����û�ܷ����Եף���Ϸ�������÷��佫���ݻ٣�\n��Ϸ����:%s-%s", 2 - n, commoner, dinting);
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
					CQ_addLog(ac, CQLOG_ERROR, "����ģ��", "Ϊ�Ե���ӻ���ʧ�ܣ�");
				}
				else
				{
					CQ_sendPrivateMsg(ac, p->QQID, "������50���֣�");
				}
				gameover();
				return 1;
			}
			if (0 == connections.changeintegral(dintingQQID, 10, 0))
			{
				CQ_addLog(ac, CQLOG_ERROR, "����ģ��", "Ϊ�Ե���ӻ���ʧ�ܣ�");
			}
			while (p != NULL)
			{
				p->poll = 0;
				p->say = 0;
				p->vote = 0;
				t++;
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%i�ű���̭�ˣ���������%i��", n + 1, t);
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			p = death;
			while (p != NULL)
			{
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%i�ű���̭��!", n + 1);
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
			sprintf(msg, "%i�ű���̭�ˣ�\n���Ƿ������Եף���Ϸ�������÷��佫���ݻ٣�\n��Ϸ����:%s-%s", n + 1, commoner, dinting);
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				if (p->type == 2)
				{
					if (0 == connections.changeintegral(p->QQID, 35, 0))
					{
						CQ_addLog(ac, CQLOG_ERROR, "����ģ��", "Ϊƽ����ӻ���ʧ�ܣ�");
					}
				}
				else
				{
					CQ_sendPrivateMsg(ac, p->QQID, "������35���֣�");
				}
				p = p->next;
			}
			p = death;
			while (p != NULL)
			{
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				if (0 == connections.changeintegral(p->QQID, 25, 0))
				{
					CQ_addLog(ac, CQLOG_ERROR, "����ģ��", "Ϊƽ����ӻ���ʧ�ܣ�");
				}
				else
				{
					CQ_sendPrivateMsg(ac, p->QQID, "������25���֣�");
				}
				p = p->next;
			}
			gameover();
			return -1;
		}
		else if (x == 0)
		{
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "�ܱ�Ǹ��������һ��δ֪�Ĵ�����Ϸ���佫��ǿ�ƽ�����");
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
				sprintf(msg, "%i�����Եף�����û�ܷ����Եף���Ϸ�������÷��佫���ݻ٣�\n��Ϸ����:%s-%s", 2 - n, commoner, dinting);
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
					CQ_addLog(ac, CQLOG_ERROR, "����ģ��", "Ϊ�Ե���ӻ���ʧ�ܣ�");
				}
				else
				{
					CQ_sendPrivateMsg(ac, p->QQID, "������50���֣�");
				}
				gameover();
				return 1;
			}
			if (0 == connections.changeintegral(dintingQQID, 10, 0))
			{
				CQ_addLog(ac, CQLOG_ERROR, "����ģ��", "Ϊ�Ե���ӻ���ʧ�ܣ�");
			}
			while (p != NULL)
			{
				p->poll = 0;
				p->vote = 0;
				p->say = 0;
				t++;
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%i�ű���̭�ˣ���������%i��", n + 1, t);
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			p = death;
			while (p != NULL)
			{
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%i�ű���̭��!", n + 1);
				CQ_sendPrivateMsg(ac, p->QQID, msg);
				p = p->next;
			}
			rank = 0;
			max = 0;
			starttimer();
			return 1;
		}
	}
}
//��idɾ��һ����Ϸ��ң��ɹ�����1��ʧ�ܷ���0,ɾ����ʧȥ���г�Ա�򷵻�-1
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
//�����к�ɾ��һ����Ϸ��ң��ɹ�����1��ʧ�ܷ���0,ɾ����ʧȥ���г�Ա�򷵻�-1
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
//����ҵ�ͶƱ���д���,n��Ϊ��ͶƱ�ߵ����кţ���x��-1��,�ɹ�����1�������ͶƱ����0
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
		t++;
	}
	p->poll++;
	return 1;
}
//��ĳ��������������б�
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
//type1Ϊ���д������б�type0ΪδͶƱ����б�,type-1Ϊ����̭����ս����б�
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
//��ʼ��Ϸ���������ѯ����Ϸ�ʲ�������Ϸ׼������,��Ϸ�ѿ�ʼ����-1���ɹ����������Ե����к�
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
		CQ_sendPrivateMsg(ac,p->QQID,"��Ϸ��ʼ��");
		CQ_sendPrivateMsg(ac, p->QQID, "���ֵ��㷢��ʱ����ҷ���һ�仰�����ҷ�����ĵ��ʣ���Ҫ�ͱ����ظ���Ȼ��ƱͶ������Ϊ�Է��������������㲻ͬ����ҡ�");
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
		sprintf(sendmsg, "����%i�����!", i + 1);
		CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
		CQ_sendPrivateMsg(ac, p->QQID, "��1������ȷ���!");
		p = p->next;
		i++;
	}
	starttimer();
	return k;
}
//�����δ�ֵ�����ҷ��Է���0,���Գɹ��������кţ��ѷ��Թ��򷵻�-1������ͶƱ�׶��򷵻�-2
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
							CQ_sendPrivateMsg(ac, p->QQID, "���һ����ҳ�ʱδ���ԣ����ڽ���ͶƱ�׶�");
						}
						else
						{
							CQ_sendPrivateMsg(ac, p->QQID, msg);
							CQ_sendPrivateMsg(ac, p->QQID, "���һ������ѷ��ԣ���ͶƱ��");
						}
						// mark wait for set timer
						p = p->next;
					}
					p = death;
					while (p != NULL)
					{
						if (!strcmp(msg, ""))
						{
							CQ_sendPrivateMsg(ac, p->QQID, "���һ����ҳ�ʱδ���ԣ����ڽ���ͶƱ�׶β����Կ�ʮ�ִ�����");
						}
						else
						{
							CQ_sendPrivateMsg(ac, p->QQID, msg);
							CQ_sendPrivateMsg(ac, p->QQID, "���һ������ѷ��ԣ�");
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
								sprintf(sendmsg, "%i����ҳ�ʱδ���ԣ��������ֵ��㷢�ԣ�",rank);
							}
							else
								sprintf(sendmsg, "%i����ҳ�ʱδ���ԣ��������ֵ�%i����ҷ��ԣ�", rank, rank + 1);
							CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
						}
						else
						{
							memset(sendmsg, 0, sizeof(sendmsg));
							sprintf(sendmsg, "%i��:%s", rank, msg);
							CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
							memset(sendmsg, 0, sizeof(sendmsg));
							if (n == rank)
							{
								sprintf(sendmsg, "%i������ѷ��ԣ��������ֵ��㷢�ԣ�",rank);
							}
							else
								sprintf(sendmsg, "%i������ѷ��ԣ��������ֵ�%i����ҷ��ԣ�", rank, rank + 1);
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
							sprintf(sendmsg, "%i����ҳ�ʱδ���ԣ��������ֵ�%i����ҷ��ԣ�", rank, rank + 1);
							CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
						}
						else
						{
							memset(sendmsg, 0, sizeof(sendmsg));
							sprintf(sendmsg, "%i��:%s", rank, msg);
							CQ_sendPrivateMsg(ac, p->QQID, sendmsg);
							memset(sendmsg, 0, sizeof(sendmsg));
							sprintf(sendmsg, "%i������ѷ��ԣ��������ֵ�%i����ҷ��ԣ�", rank, rank + 1);
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
//�����Ϸ�������������
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
	};//�������ݽṹ
	int number = 0;//�÷��������
	int roomid = 0;
	memberdata *head = NULL;
	werewolf *next = NULL;
	bool state = FALSE;//��Ϸ�Ƿ��ڿ�ʼ״̬
	bool addmember(int64_t QQID);
	int findmember(int64_t QQID);
	int createRoom(int64_t QQID);
	int joinRoom(const char *msg, int64_t QQID);
	int leaveRoom(int64_t QQID);
	int RunRoom(int64_t QQID);
private:
};
//����һ�����䲢�������߼��뵽�÷����У��ɹ�����roomid��ʧ�ܷ���0
int werewolf::createRoom(int64_t QQID)
{
	werewolf *room = next;
	if (head != NULL)//�������˵���÷����Ѵ��ڣ������·�����뵽������
	{
		if (room == NULL)//��һ�����䲻������ֱ�Ӵ�������
		{
			next = new werewolf;
			if (next == NULL)
			{
				return 0;
			}
			return next->createRoom(QQID);
		}
		else//���������һ������Ĵ������亯��
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
//��һ����Ա���뵽�÷������ڵ����б��У��ɹ�����true��ʧ�ܷ���false
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
//���ĳ����ڷ�������кţ����ڷ���-1�������򷵻�0Ϊ��һ�������к�
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
//����Msg��ĳ����Ҽ��뵽�����У��������ڸ÷����򷵻�-1���Ѽ���ĳ�������򷵻�1������ɹ�����2������ʧ�ܷ���0���÷����ѿ�ʼ����-2
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
//����Ҵ����з������˳����ɹ�����1�����������κη��䷵��0����ͼ�˳��ķ����ѿ�ʼ����-1
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
//û���κη�����Կ�ʼ����0����������ѿ�ʼ�򷵻�-1����������6���򷵻�-2��
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