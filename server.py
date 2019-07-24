import socket
import pymysql
import time
import weather
import numpy
import processfor
import random

def processforsign(dictionary,connection):
    outcome = '0'
    if dictionary['action'] == 'get':
        return getintegral(dictionary['user'],connection)
    elif dictionary['action'] == 'integral':
        return changeintegral(dictionary['user'],int(dictionary['number']),connection)
    elif dictionary['action'] == 'date':
        return changedate(dictionary['user'],dictionary['groupid'],dictionary['date'],connection)
    elif dictionary['action'] == 'list':
        return listsign(dictionary['groupid'],dictionary['date'],connection)
    elif dictionary['action'] == 'sign':
        return sign(dictionary,connection)

def getsigndate(QQID,connection):
    cursor = connection.cursor()
    sql = "select date from sign where id = '%s'"%QQID
    try:
        if(0 == cursor.execute(sql)):
            return 1
        else:
            result = cursor.fetchone()[0]
            nowtime = time.localtime(time.time())
            if (str(nowtime.tm_year)+'-'+str(nowtime.tm_mon)+'-'+str(nowtime.tm_mday)) == result:
                return 0
            else:
                return 1
    except BaseException as e:
        print(e)
        return 0

def updatesigndate(QQID,groupid,connection):
    cursor = connection.cursor()
    sql = "select * from sign where id = '%s'"%QQID
    nowtime = time.localtime(time.time())
    try:
        if(0 == cursor.execute(sql)):
            sql = "insert into sign(id,groupid,date) values('%s','%s')"%(QQID,groupid,str(nowtime.tm_year)+'-'+str(nowtime.tm_mon)+'-'+str(nowtime.tm_mday))
            if(0 == cursor.execute(sql)):
                return 0
            else:
                connection.commit()
                return 1
        else:
            sql = "update sign set date = '%s',groupid = '%s' where id = '%s'"%(str(nowtime.tm_year)+'-'+str(nowtime.tm_mon)+'-'+str(nowtime.tm_mday),groupid,QQID)
            if(0 == cursor.execute(sql)):
                return 0
            else:
                connection.commit()
                return 1
    except BaseException as e:
        print(e)
        return 0

def sign(dictionary,connection):
    cursor = connection.cursor()
    nowtime = time.localtime(time.time())
    try:
        if(getsigndate(dictionary['user'],connection)):
            sql = "select masterid from master where id = '%s'"%dictionary['user']
            if(0 == cursor.execute(sql)):
                sql = "select * from sign where groupid = '%s' and date = '%s'"%(dictionary['groupid'],str(nowtime.tm_year)+'-'\
                                                                             +str(nowtime.tm_mon)+'-'+str(nowtime.tm_mday))
                n = cursor.execute(sql)
                if n < 3:
                    changeintegral(dictionary['user'],80-n*10,connection)
                    sql = "select integral from sign where id = '%s'"%dictionary['user']
                    cursor.execute(sql)
                    number = cursor.fetchone()[0]
                    updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                    return "[CQ:at,qq=%s]签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(dictionary['user'],n+1,80-n*10,number)
                elif n >= 3:
                    changeintegral(dictionary['user'],50,connection)
                    sql = "select integral from sign where id = '%s'"%dictionary['user']
                    cursor.execute(sql)
                    number = cursor.fetchone()[0]
                    updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                    return "[CQ:at,qq=%s]签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(dictionary['user'],n+1,50,number)
            else:
                result = cursor.fetchone()
                if(result[0] == '0'):
                    sql = "select * from sign where groupid = '%s' and date = '%s'"%(dictionary['groupid'],str(nowtime.tm_year)+'-'\
                                                                             +str(nowtime.tm_mon)+'-'+str(nowtime.tm_mday))
                    n = cursor.execute(sql)
                    if n < 3:
                        changeintegral(dictionary['user'],80-n*10,connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        number = cursor.fetchone()[0]
                        updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                        return "[CQ:at,qq=%s]签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(dictionary['user'],n+1,80-n*10,number)
                    elif n >= 3:
                        changeintegral(dictionary['user'],50,connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        number = cursor.fetchone()[0]
                        updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                        return "[CQ:at,qq=%s]签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(dictionary['user'],n+1,50,number)
                else:
                    sql = "select * from sign where groupid = '%s' and date = '%s'"%(dictionary['groupid'],str(nowtime.tm_year)+'-'\
                                                                             +str(nowtime.tm_mon)+'-'+str(nowtime.tm_mday))
                    n = cursor.execute(sql)
                    if n < 3:
                        changeintegral(dictionary['user'],64-n*8,connection)
                        changeintegral(result[0],16-n*2,connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        number = cursor.fetchone()[0]
                        updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                        return "[CQ:at,qq=%s]签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(dictionary['user'],n+1,64-n*8,number)
                    elif n>=3:
                        changeintegral(dictionary['user'],40,connection)
                        changeintegral(result[0],10,connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        number = cursor.fetchone()[0]
                        updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                        return "[CQ:at,qq=%s]签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(dictionary['user'],n+1,40,number)
        else:
            return "[CQ:at,qq=%s]\n你今天已经签到过了，请不要重复签到！"%dictionary['user']
    except BaseException as e:
        print(e)
        return '不好意思，傻馒出现了一点小问题，请联系维护人员修复'

def listsign(groupid,date,connection):
    cursor = connection.cursor()
    sql = "select * from sign where groupid = '%s' and date = '%s'"%(groupid,date)
    try:
        n = cursor.execute(sql)
        return str(n)
    except BaseException as e:
        print(e)
        return '-1'

def getintegral(QQID,connection):
    cursor = connection.cursor()
    sql="select date,integral from sign where id = '%s'"%QQID
    try:
        if(0==cursor.execute(sql)):
            return '0 0';
        connection.commit()
        result = cursor.fetchone()
        outcome=""
        outcome = str(result[0]) + " " + str(result[1])
        print(outcome)
        return outcome
    except BaseException as e:
        print(e)
        return '0 0'

def changeintegral(QQID,change,connection):
    cursor = connection.cursor()
    sql = "select integral from sign where id = '%s'"%QQID
    if(0 == cursor.execute(sql)):
        sql="insert into sign(id,integral) values('%s',%i)"%(QQID,change)
    else:
        if change > 0:
            sql="update sign set integral = integral + %i where id = '%s'"%(change,QQID)
        else:
            sql="update sign set integral = integral - %i where id = '%s'"%(-change,QQID)
    if(0==cursor.execute(sql)):
        return '0'
    else:
        try:
            connection.commit()
        except:
            return '0';
        else:
            return '1';

def changedate(QQID,groupid,date,connection):
    cursor = connection.cursor()
    sql = "select integral from sign where id = '%s'"%QQID
    if(0 == cursor.execute(sql)):
        sql="insert into sign(id,date,groupid) values('%s','%s','%s')"%(QQID,date,groupid)
    else:
        sql="update sign set date = '%s',groupid = '%s' where id = '%s'"%(date,groupid,QQID)
    if(0==cursor.execute(sql)):
        return '0'
    else:
        try:
            connection.commit()
        except:
            return '0';
        else:
            return '1';

def getdinting(id,connection):
    cursor = connection.cursor()
    sql = "select * from dinting where id = %s"%id
    try:
        if(0 == cursor.execute(sql)):
            return '0';
        else:
            result = cursor.fetchone()
            outcome=str(result[1])+ " " + str(result[2])
            print(outcome)
            return outcome
    except BaseException as e:
        print(e)
        return '0'

def getdict(data):
    dictionary={}
    data=data.split()
    for i in data:
        keyanddata=i.split(":")
        if len(keyanddata)<2:
            continue
        dictionary[keyanddata[0]]=keyanddata[1]
    return dictionary

def processfortally(dictionary,connection,date):
    cursor = connection.cursor()
    outcome = '0'
    if dictionary['action'] == 'add':
        if 'category' not in dictionary:
            autogetcategory(dictionary)
        sql = "insert into cashbook(userid,\
        date,category,number,remarks) values(%s,%s,%s,%s,%s\
        )"%(dictionary['user'],date,dictionary['category'],\
            dictionary['number'],dictionary['remarks'])
        if(0 == cursor.execute(sql)):
            return '0'
        else:
            try:
                connection.commit()
                return '1'
            except:
                return '0'
    elif dictionary['action'] == 'total':
        total = 0
        outcome='本月账本如下:'
        sql = "select sum(number) from cashbook where userid = '%s' and date regexp '%s' and category = '%s'"%(dictionary['user'],dictionary['date'],'饮食')
        if(0 == cursor.execute(sql)):
            return '0'
        result = cursor.fetchone()
        if(result[0] == None):
            result = 0
        else:
            result =result[0]
        outcome+='\n饮食:'
        outcome+=str(result)
        outcome+='元'
        total += result
        sql = "select sum(number) from cashbook where userid = '%s' and date regexp '%s' and category = '%s'"%(dictionary['user'],dictionary['date'],'购物')
        if(0 == cursor.execute(sql)):
            return '0'
        result = cursor.fetchone()
        if(result[0] == None):
            result = 0
        else:
            result =result[0]
        outcome+='\n购物:'
        outcome+=str(result)
        outcome+='元'
        total += result
        sql = "select sum(number) from cashbook where userid = '%s' and date regexp '%s' and category = '%s'"%(dictionary['user'],dictionary['date'],'unknow')
        if(0 == cursor.execute(sql)):
            return '0'
        result = cursor.fetchone()
        if(result[0] == None):
            result = 0
        else:
            result =result[0]
        outcome+='\n未知:'
        outcome+=str(result)
        outcome+='元'
        total+=result
        sql = "select sum(number) from cashbook where userid = '%s' and date regexp '%s' and category = '%s'"%(dictionary['user'],dictionary['date'],'收入')
        if(0 == cursor.execute(sql)):
            return '0'
        result = cursor.fetchone()
        if(result[0] == None):
            result = 0
        else:
            result =result[0]
        outcome+='\n收入:+'
        outcome+=str(result)
        outcome+='元'
        total += result
        sql = "select sum(number) from cashbook where userid = '%s' and date regexp '%s' and category = '%s'"%(dictionary['user'],dictionary['date'],'交通')
        if(0 == cursor.execute(sql)):
            return '0'
        result = cursor.fetchone()
        if(result[0] == None):
            result = 0
        else:
            result =result[0]
        outcome+='\n交通:'
        outcome+=str(result)
        outcome+='元'
        total += result
        sql = "select sum(number) from cashbook where userid = '%s' and date regexp '%s' and category = '%s'"%(dictionary['user'],dictionary['date'],'借还')
        if(0 == cursor.execute(sql)):
            return '0'
        result = cursor.fetchone()
        if(result[0] == None):
            outcome+='\n借还:0元'
            return outcome
        elif(result[0] >= 0):
            outcome+='\n借还:+'
        outcome+=str(result[0])
        outcome+='元'
        if(total>0):
            outcome+='\n总计:+'
        outcome+='\n总计:'
        outcome+=str(total)
        outcome+='元'
        return outcome
    elif dictionary['action'] == 'reduce':
        sql = "delete from cashbook where userid == '%s' \
        and rank = %s"%(dictionary['user'],dictionary['rank'])
        if(0 == cursor.execute(sql)):
            return '0'
        else:
            try:
                connection.commit()
                return '1'
            except:
                return '0'
    elif dictionary['action'] == 'list':
        if 'category' not in dictionary:
            sql = "select category,remarks,number,rank from cashbook where userid = '%s' and date regexp '%s.+'"%(dictionary['user'],dictionary['date'])
            if(0 == cursor.execute(sql)):
                return "你的本月帐单还是空的！"
            else:
                outcome="%s月账单如下:"%dictionary['date']
                for i in range(0,cursor.rowcount):
                    result=cursor.fetchone()
                    outcome+='\n'+str(result[3])+':'+"%f"%result[2]+'元-'+str(result[0])+' '+str(result[1])
            print(outcome)
            return outcome
        else:
            sql = "select category,remarks,number,rank from cashbook where userid = '%s' and \
category = '%s' and date regexp '%s.+'"%(dictionary['user'],dictionary['category'],dictionary['date'])
            if(0 == cursor.execute(sql)):
                return "你的本月帐单还是空的！"
            else:
                outcome="%s月账单如下:"%dictionary['date']
                for i in range(0,cursor.rowcount):
                    result=cursor.fetchone()
                    outcome+='\n'+str(result[3])+':'+"%f"%result[2]+'元 '+str(result[1])
            print(outcome)
            return outcome
    return '0'

def processforslave(dictionary,connection):
    cursor = connection.cursor()
    outcome = '0'
    if dictionary['action']=='check':
        sql='select masterid,ransom,date from master where id="%s"'%dictionary['user']
        if(0==cursor.execute(sql)):
            return '0 0 0'
        result = cursor.fetchone()
        return '%s %i %s'%(result[0],result[1],result[2])
    elif dictionary['action']=='buy':
        sql='delete from master where id = "%s"'%dictionary['id']
        cursor.execute(sql)
        sql='insert into master(id,masterid,date,ransom) values("%s","%s","%s",%s)\
'%(dictionary['id'],dictionary['user'],dictionary['date'],dictionary['ransom'])
        if(0==cursor.execute(sql)):
            try:
                connections.commit()
                return '1'
            except:
                return '0'
        try:
            connection.commit()
            return '1'
        except:
            return '0'
        return '1'
    elif dictionary['action']=='list':
        sql="select id from master where masterid = '%s'"%dictionary['user']
        i=cursor.execute(sql)
        if(i==0):
            return '0'
        outcome=""
        for x in range(0,i):
            result=cursor.fetchone()
            outcome+=result[0]
            outcome+=' '
        return outcome
    elif dictionary['action']=='delete':
        sql="delete from master where id = '%s'"%dictionary['user']
        if(0==cursor.execute(sql)):
            return '0'
        try:
            connection.commit()
            return '1'
        except:
            return '0'
        return '1'
    return '0'

def processforvalidate(dictionary,connection):
    cursor = connection.cursor()
    outcome='0'
    if dictionary['action'] == 'add':
        sql = 'insert into validate(validate) values("%s")'%dictionary['user']
        try:
            if(0 == cursor.execute(sql)):
                return '0'
            else:
                connection.commit()
                return '1'
        except BaseException as e:
            print(e)
            return '0'
    elif dictionary['action'] == 'confirm':
        sql = 'select validate from validate where validate = "%s"'%dictionary['user']
        try:
            if(0 == cursor.execute(sql)):
                return '0'
            else:
                sql = 'delete from validate where validate = "%s"'%dictionary['user']
                cursor.execute(sql)
                connection.commit()
                return '1'
        except BaseException as e:
            print(e)
            return '0'

def processforweather(dictionary,connection):
    cursor = connection.cursor()
    outcome = '0'
    if dictionary['action'] == "add":
        sql = 'select id from weather where id = "%s"'%dictionary['user']
        try:
            if(0 == cursor.execute(sql)):
                sql = 'insert into weather(id,kind) values("%s",%s)'%(dictionary['user'],dictionary['kind'])
                if(0 == cursor.execute(sql)):
                    return '0'
                else:
                    connection.commit()
                    return '1'
        except BaseException as e:
            print(e)
            return '0'
        return '1'
    elif dictionary['action'] == 'delete':
        sql = 'select id from weather where id = "%s"'%dictionary['user']
        try:
            if(0 == cursor.execute(sql)):
                return '1'
            else:
                sql = 'delete from weather where id = "%s"'%(dictionary['user'])
                if(0 == cursor.execute(sql)):
                    return '0'
                else:
                    connection.commit()
                    return '1'
        except BaseException as e:
            print(e)
            return '0'
    elif dictionary['action'] == 'list':
        sql = 'select id from weather where kind = %s'%dictionary['kind']
        try:
            if(0 == cursor.execute(sql)):
                return '0 '
            else:
                outcome = '%i '%cursor.rowcount
                for i in range(0,cursor.rowcount):
                    result = cursor.fetchone()
                    outcome += result[0]
                    outcome += ' '
                return outcome
        except BaseException as e:
            print(e)
            return '0 '
    elif dictionary['action'] == 'get':
        getweather = weather.SearchWeather()
        outcome = getweather.main(city = "武汉")
        print(outcome)
        return outcome

def getmean(things,result):
	number = 0
	money = 0
	for i in range(0,10):
		number += result[i]
		money += things[i] * result[i]
	return money / number

def randomlawmoney(result):
	result[0]=random.randint(result[0],result[0]+5)
	result[2]=random.randint(result[2],result[2]+5)
	result[3]=random.randint(result[3],result[3]+5)
	result[4]=random.randint(result[4],result[4]+5)

def randomhighmoney(result):
	result[1]=random.randint(result[1]-5,result[1])
	result[5]=random.randint(result[5]-5,result[5])
	result[6]=random.randint(result[6]-5,result[6])
	result[7]=random.randint(result[7]-5,result[7])
	result[8]=random.randint(result[8]-5,result[8])
	result[9]=random.randint(result[9]-5,result[9])

def createrafflelist():
    things = [35, 150, 35, 35, 0, 70, 100, 120, 150, 50]
    result = []
    number = 0
    for i in range(0,10):
        result.append(random.randint(400,600))
    result[1]=random.randint(100,200)
    result[6]=random.randint(200,300)
    result[7]=random.randint(200,300)
    result[8]=random.randint(200,300)
    while(getmean(things,result)>50):
        randomhighmoney(result)
        randomlawmoney(result)
    for i in range(0,10):
        number += result[i]
    return result,number;
    
def processforprop(dictionary,connection):
    cursor = connection.cursor()
    outcome = '0'
    if dictionary['action']=="add":
        sql = 'select name from prop where id = "0" and kind = "%s"'%dictionary['rank']
        try:
            if(0 == cursor.execute(sql)):
                return '数据库因未知原因出错，请联系维护人员进行处理。'
            else:
                result = cursor.fetchone()
                name = result[0]
        except BaseException as e:
            print(e)
            return '数据库因未知原因出错，请联系维护人员进行处理。'
        sql="select number from prop where id = '%s' and kind = %s"%(dictionary['user'],dictionary['rank'])
        if(0==cursor.execute(sql)):            
            sql="insert into prop(id,kind,number,name) values('%s',%s,1,'%s')"%(dictionary['user'],dictionary['rank'],name)
            if (0 == cursor.execute(sql)):
                return '0'
            else:
                try:
                    connection.commit()
                    return '1'
                except:
                    return '0'
        else:
            sql="update prop set number =(select number where id = '%s' and kind = %s)+1 where id = '%s' and kind = %s"%(dictionary['user'],dictionary['rank'],dictionary['user'],dictionary['rank'])
            if(0==cursor.execute(sql)):
                return '0'
            else:
                try:
                    connection.commit()
                    return '1'
                except:
                    return '0'
    elif dictionary['action']=='reduce':
        sql = 'select name from prop where id = "0" and kind = "%s"'%dictionary['rank']
        try:
            if(0 == cursor.execute(sql)):
                return '数据库因未知原因出错，请联系维护人员进行处理。'
            else:
                result = cursor.fetchone()
                name = result[0]
        except BaseException as e:
            print(e)
            return '数据库因未知原因出错，请联系维护人员进行处理。'
        sql = "select number from prop where id = '%s' and kind = %s"%(dictionary['user'],dictionary['rank'])
        if(0==cursor.execute(sql)):            
            return '-1'
        else:
            try:
                connection.commit()
            except:
                return '0'
            result=cursor.fetchone()
            if int(result[0]) <= 0:
                return '-1'
            elif int(result[0]) == 1:
                sql = 'delete from prop where id = "%s" and kind = %s'%(dictionary['user'],dictionary['rank'])
            else:
                sql="update prop set number =(select number where id = '%s' and kind = %s)-1 where id = '%s' and kind = %s"%(dictionary['user'],dictionary['rank'],dictionary['user'],dictionary['rank'])
            if (0 == cursor.execute(sql)):
                return '0'
            else:
                try:
                    connection.commit()
                    return '1'
                except:
                    return '0'
    elif dictionary['action']=='find':
        sql="select number from prop where id = '%s' and kind = %s"%(dictionary['user'],dictionary['rank'])
        if(0==cursor.execute(sql)):
            return '-1'
        else:
            return '1'
    elif dictionary['action']=='list':
        sql="select number,name from prop where id = '%s'"%dictionary['user']
        if(0==cursor.execute(sql)):
            return "[CQ:at,qq=%s]你的道具还是空的！"%dictionary['user']
        else:
            outcome="[CQ:at,qq=%s]拥有:"%dictionary['user']
            for i in range(0,cursor.rowcount):
                result=cursor.fetchone()
                outcome+='\n'+str(result[0])+'个'+str(result[1])
        return outcome
    elif dictionary['action'] == 'shop':
        sql = 'select name,number from prop where id = "0"'
        try:
            if(0 == cursor.execute(sql)):
                return '数据库因未知原因出错，请联系维护人员进行处理。'
            else:
                outcome = '┌────────┐\n'
                for i in range(0,cursor.rowcount):
                    result = cursor.fetchone()
                    if result[1] <= 0:
                        continue
                    outcome+= '│%-5s  售价%-5i│\n'%(result[0],result[1])
                outcome+= '└────────┘\n如果有想要的道具可以兑换哦'
                return outcome
        except BaseException as e:
            print(e)
            return '数据库因未知原因出错，请联系维护人员进行处理。'
    elif dictionary['action'] == 'raffle':
        result, number = createrafflelist()
        n = random.randint(1,number)
        i = -1
        while n > 0:
            i += 1
            n -= result[i]
        if i < 4 :
            return str(-i - 1)
        else:
            return str(i)
    elif dictionary['action'] == 'exchange':
        sql = 'select integral from sign where id = "%s"'%dictionary['user']
        try:
            if(0 == cursor.execute(sql)):
                return '我好像没能查询到你的积分记录呢，请联系维护人员进行处理。'
            else:#若查询到积分
                integral = cursor.fetchone()[0]
                sql = 'select kind,number from prop where id = "0" and name = "%s"'%dictionary['name']
                if(0 == cursor.execute(sql)):
                    return '商店中没有这张卡呢！'
                else:#获取到卡kind与number
                    [kind,number] = cursor.fetchone()
                    if number <= 0 :
                        return '商店中没有这张卡呢！'
                    elif number > integral:
                        return '虽然傻馒很想赚你的小钱钱，但是你的积分好像不够呢！'
                    else:
                        if('0' == changeintegral(dictionary['user'],-number,connection)):
                            return '支付积分因未知原因出错，请联系维护人员进行处理'
                        else:
                            dictionary['action'] = 'add'
                            dictionary['rank'] = str(kind)
                            if('0' == processforprop(dictionary,connection)):
                                if('0' == changeintegral(dictionary['user'],number,connection)):
                                    return '补偿积分因未知原因出错，请联系维护人员进行处理'
                                else:
                                    return '邮寄的道具在路上失踪了呢，真是不好意思，积分已保留。'
                            else:
                                return '道具已添加，欢迎下次再来哦！'
        except BaseException as e:
            print(e)
            return '数据库因未知原因出错，请检查损失截图并联系维护人员进行处理。'
                
HOST='127.0.0.1'
PORT=8000
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind((HOST,PORT))
s.listen(1000)
connection = pymysql.connect(host='127.0.0.1',port=3306,user='root',password='my@Zheng123',db='qq',charset='gbk')
n=-1
import nlp
worddata=nlp.init()
print("is wait!")
while 1 :
    outcome='0'
    conn,addr = s.accept()
    nowtime=time.localtime(time.time())
    print(addr,"%i:%i"%(nowtime.tm_hour,nowtime.tm_min))
    try:
        data=conn.recv(1024).decode('gbk')
    except BaseException as e:
        print(e)
        continue
    print(data)
    if data == "":
        conn.send(b'0')
        conn.close()
    if data[0]=='t':
        if nowtime.tm_hour == 21 and nowtime.tm_min < 1:
            conn.send(b'1')
        else:
            conn.send(b'0')
        print('t')
    elif data.find(':prop') != -1:
        dictionary = getdict(data)
        outcome = processforprop(dictionary,connection)
        conn.send(outcome.encode('gbk'))
        print(outcome)
    elif data.find(':slave') != -1:
        dictionary = getdict(data)
        outcome = processforslave(dictionary,connection)
        conn.send(outcome.encode('gbk'))
        print(outcome)
    elif data.find(':tally') != -1:
        dictionary = getdict(data)
        outcome = processfortally(dictionary,connection)
        conn.send(outcome.encode('gbk'))
        print(outcome)
    elif data.find(':game') != -1:
        a=data.split()
        outcome=getdinting(a[2].split(':')[1],connection)
        conn.send(outcome.encode('gbk'))
        print('dinting')
    elif data.find(':sign') != -1:
        dictionary = getdict(data)
        outcome = processforsign(dictionary,connection)
        conn.send(outcome.encode('gbk'))
        print('sign')
    elif data.find(':weather') != -1:
        dictionary = getdict(data)
        outcome = processforweather(dictionary,connection)
        conn.send(outcome.encode('gbk'))
        print('weather')
    elif data.find(':validate') != -1:
        dictionary = getdict(data)
        outcome = processforvalidate(dictionary,connection)
        conn.send(outcome.encode('gbk'))
        print('validate')
    else:
        maxp=0
        n=-1
        for i in range(0,len(nlp.question)):
           p=nlp.manage(data,nlp.question[i],worddata)
           if p>maxp and p>0.75:
               n=i
               maxp=p
        if n==-1:
            conn.send(b"0")
        else:
            conn.send(nlp.answer[n].encode('gbk'))
        print('nlp')
    conn.close()
