import socket
import pymysql
import time
import weather
import numpy
import processfor
import random

def getyday(day):
    if day == '0':
        return 0
    [year,month,day]=day.split('-')
    yday = [0,31,59,90,120,151,181,212,243,273,304,334]
    return yday[int(month)]+int(day)

def processforsign(dictionary,connection):
    outcome = '0'
    if dictionary['action'] == 'get':
        return getintegral(dictionary['user'],connection)
    elif dictionary['action'] == 'integral':
        return changeintegral(dictionary['user'],int(dictionary['number']),connection)
    elif dictionary['action'] == 'sign':
        return sign(dictionary,connection)

def processforAdministor(dictionary,connection):
    cursor = connection.cursor()
    if dictionary['action'] == 'put':
        sql = "insert into administor values('%s',%s,'%s')"%(dictionary['user'],dictionary['grade'],dictionary['groupid'])
        if 0 == cursor.execute(sql):
            return '0'
        return '1'
    elif dictionary['action'] == 'get':
        sql = "select grade from administor where id = '%s' and groupid = '%s'"%(dictionary['user'],dictionary['groupid'])
        if 0 == cursor.execute(sql):
            sql = "select grade from administor where id = '%s' and groupid = '0'"%dictionary['user']
            if 0 == cursor.execute(sql):
                return '0'
            grade = cursor.fetchone()[0]
            return str(grade)
        groupgrade = cursor.fetchone()[0]
        sql = "select grade from administor where id = '%s' and groupid = '0'"%dictionary['user']
        if 0 == cursor.execute(sql):
            return str(groupgrade)
        grade = cursor.fetchone()[0]
        if(grade > groupgrade):
            return str(grade)
        else:
            return str(grougrade)
    return '0'

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
                yday = getyday(str(nowtime.tm_year)+'-'+str(nowtime.tm_mon)+'-'+str(nowtime.tm_mday))
                if nowtime.tm_yday < yday:
                    if nowtime.tm_yday - yday <= -363:
                        return 2
                    else:
                        return 1
                if nowtime.tm_yday - yday <=2:
                    return 2
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
        number = getsigndate(dictionary['user'],connection)
        if(number == 1):
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
                    return "%s签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,80-n*10,number)
                elif n >= 3:
                    changeintegral(dictionary['user'],50,connection)
                    sql = "select integral from sign where id = '%s'"%dictionary['user']
                    cursor.execute(sql)
                    number = cursor.fetchone()[0]
                    updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                    return "%s签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,50,number)
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
                        return "%s签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,80-n*10,number)
                    elif n >= 3:
                        changeintegral(dictionary['user'],50,connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        number = cursor.fetchone()[0]
                        updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                        return "%s签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,50,number)
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
                        return "%s签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,64-n*8,number)
                    elif n>=3:
                        changeintegral(dictionary['user'],40,connection)
                        changeintegral(result[0],10,connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        number = cursor.fetchone()[0]
                        updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                        return "%s签到成功\n你是本群第%i个签到的人\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,40,number)
        elif number == 2:
            sql = "select masterid from master where id = '%s'"%dictionary['user']
            if(0 == cursor.execute(sql)):
                sql = "select * from sign where groupid = '%s' and date = '%s'"%(dictionary['groupid'],str(nowtime.tm_year)+'-'\
                                                                             +str(nowtime.tm_mon)+'-'+str(nowtime.tm_mday))
                n = cursor.execute(sql)
                if n < 3:
                    changeintegral(dictionary['user'],90-n*10,connection)
                    sql = "select integral from sign where id = '%s'"%dictionary['user']
                    cursor.execute(sql)
                    number = cursor.fetchone()[0]
                    updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                    return "%s签到成功\n你是本群第%i个签到的人且连续签到\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,90-n*10,number)
                elif n >= 3:
                    changeintegral(dictionary['user'],60,connection)
                    sql = "select integral from sign where id = '%s'"%dictionary['user']
                    cursor.execute(sql)
                    number = cursor.fetchone()[0]
                    updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                    return "%s签到成功\n你是本群第%i个签到的人且连续签到\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,60,number)
            else:
                result = cursor.fetchone()
                if(result[0] == '0'):
                    sql = "select * from sign where groupid = '%s' and date = '%s'"%(dictionary['groupid'],str(nowtime.tm_year)+'-'\
                                                                             +str(nowtime.tm_mon)+'-'+str(nowtime.tm_mday))
                    n = cursor.execute(sql)
                    if n < 3:
                        changeintegral(dictionary['user'],90-n*10,connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        number = cursor.fetchone()[0]
                        updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                        return "%s签到成功\n你是本群第%i个签到的人且连续签到\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,90-n*10,number)
                    elif n >= 3:
                        changeintegral(dictionary['user'],60,connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        number = cursor.fetchone()[0]
                        updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                        return "%s签到成功\n你是本群第%i个签到的人且连续签到\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,60,number)
                else:
                    sql = "select * from sign where groupid = '%s' and date = '%s'"%(dictionary['groupid'],str(nowtime.tm_year)+'-'\
                                                                             +str(nowtime.tm_mon)+'-'+str(nowtime.tm_mday))
                    n = cursor.execute(sql)
                    if n < 3:
                        changeintegral(dictionary['user'],72-n*8,connection)
                        changeintegral(result[0],18-n*2,connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        number = cursor.fetchone()[0]
                        updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                        return "%s签到成功\n你是本群第%i个签到的人且连续签到\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,72-n*8,number)
                    elif n>=3:
                        changeintegral(dictionary['user'],48,connection)
                        changeintegral(result[0],12,connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        number = cursor.fetchone()[0]
                        updatesigndate(dictionary['user'],dictionary['groupid'],connection)
                        return "%s签到成功\n你是本群第%i个签到的人且连续签到\n获得积分+%i\n你现在的积分为:%i"%(getat(dictionary['user'],connection),n+1,48,number)
        else:
            return "%s\n你今天已经签到过了，请不要重复签到！"%getat(dictionary['user'],connection)
    except BaseException as e:
        print(e)
        return '不好意思，傻馒出现了一点小问题，请联系维护人员修复'

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
    try:
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
            sql = "select integral from sign where id = '%s'"%QQID
            if(0 == cursor.execute(sql)):
                return '0'
            else:
                integral = cursor.fetchone()[0]
                if integral < 0:
                    sql = "update sign set integral = 0 where id = '%s'"%QQID
                    if(0 == cursor.execute(sql)):
                        return '0'
                connection.commit()
                return '1'
    except BaseException as e:
        print(e)
        return '0'

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

def slave_buyneedintegral(slave_id,connection):
    cursor = connection.cursor()
    sql = "select masterid,ransom from master where id = '%s'"%slave_id
    if(0 == cursor.execute(sql)):
        sql = "select integral from sign where id = '%s'"%slave_id
        if(0 == cursor.execute(sql)):#无记录，不可购买返回0,0
            return 0,0
        else:
            result = cursor.fetchone()[0]
            return 1,result + 100#可购买，所需积分
    else:
        result = cursor.fetchone()
        if result[0] == '0':
            sql = "select integral from sign where id = '%s'"%slave_id
            if(0 == cursor.execute(sql)):
                return 0,0
            else:
                integral = cursor.fetchone()[0]
                return 1,integral + 100#可购买，所需积分
        else:
            return 2,result[1]+100#可购买，赎金

def getat(id,connection):
    cursor = connection.cursor()
    sql = "select name from sign where id = '%s'"%id
    if 0 == cursor.execute(sql):
        return '[CQ:at,qq=%s]'%id
    name = cursor.fetchone()[0]
    if name == '无':
        return '[CQ:at,qq=%s]'%id
    else:
        return name

def processforslave(dictionary,connection):
    cursor = connection.cursor()
    outcome = '0'
    if dictionary['action']=='check':
        sql='select masterid,ransom,date from master where id="%s"'%dictionary['user']
        if(0==cursor.execute(sql)):
            return '0 0 0'
        result = cursor.fetchone()
        return '%s %i %s'%(result[0],result[1],result[2])
    elif dictionary['action'] == 'buy_back':
        nowtime = time.time()
        sql = 'select ransom,masterid from master where id = "%s"'%dictionary['user']
        if(0 == cursor.execute(sql)):
            return '2'
        result = cursor.fetchone()
        ransom = result[0]
        masterid = result[1]
        if masterid == '0':
            return '2'
        else:
            sql = 'select integral from sign where id = "%s"'%dictionary['user']
            if(0 == cursor.execute(sql)):
                return '-%i'%ransom
            else:
                integral = cursor.fetchone()[0]
                if integral >= ransom:
                    sql = 'update sign set integral = integral - %i where id = "%s"'%(ransom,dictionary['user'])
                    if(0 == cursor.execute(sql)):
                        return '0'
                    sql = 'update sign set integral = integral + %i where id = "%s"'%(ransom*0.8,masterid)
                    if(0 == cursor.execute(sql)):
                        return '0'
                    sql = 'update master set date = %i,masterid = 0 where id = "%s"'%(nowtime,dictionary['user'])
                    if(0 == cursor.execute(sql)):
                        return '0'
                    connection.commit()
                    return '1'
                else:
                    return '-%i'%ransom
    elif dictionary['action'] == 'buycheck':
        try:
            sql = "select * from master where masterid = '%s' and id = '%s'"%(dictionary['user'],dictionary['slaveid'])
            if 0 != cursor.execute(sql):
                return '-1 请不要重复购买自己的奴隶！'
            sql = "select * from master where masterid = '%s' and id = '%s'"%(dictionary['slaveid'],dictionary['user'])
            if 0 != cursor.execute(sql):
                return '-1 你不能购买你的主人，请先赎回！'
            sql = "select * from master where masterid = '%s'"%dictionary['user']
            if cursor.execute(sql) >= 10:
                return "-1 你已经拥有十个奴隶，你无法再购买更多了！"
            n,buyneed_integral = slave_buyneedintegral(dictionary['slaveid'],connection)
            if n == 0:
                return "-1 对方处于新手保护期，在对方获得积分之前，你暂时无法购买！"
            elif n == 1:
                nowtime = time.time()
                sql = "select date from master where id = '%s'"%dictionary['slaveid']
                if 0 == cursor.execute(sql):
                    sql = "select integral from sign where id = '%s'"%dictionary['user']
                    if(0 == cursor.execute(sql)):
                        return "-1 购买对方需要%i积分，你的积分不足，无法购买！"%buyneed_integral
                    else:
                        user_integral = cursor.fetchone()[0]
                        if user_integral >= buyneed_integral:
                            return "1 购买对方需要%i积分，确认购买请输1，取消请输0！"%buyneed_integral
                        else:
                            return "-1 购买对方需要%i积分，你的积分不足，无法购买！"%buyneed_integral
                date = cursor.fetchone()[0]
                if date + 3*24*60*60 > nowtime:
                    sql = "select number from prop where id = '%s' and kind = 2"%dictionary['user']
                    if(0 == cursor.execute(sql)):
                        return "-1 购买对方需要%i积分和一张强制购买卡，你没有强制保护卡无法购买！"%buyneed_integral
                    else:
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        if(0 == cursor.execute(sql)):
                            return "-1 购买对方需要%i积分，你的积分不足，无法购买！"%buyneed_integral
                        user_integral = cursor.fetchone()[0]
                        if user_integral >= buyneed_integral:
                            return "1 购买对方需要%i积分和一张强制购买卡，确认购买请输1，取消购买请输0！"%buyneed_integral
                        else:
                            return "-1 购买对方需要%i积分，你的积分不足，无法购买！"%buyneed_integral
                else:
                    sql = "select integral from sign where id = '%s'"%dictionary['user']
                    if(0 == cursor.execute(sql)):
                        return "-1 购买对方需要%i积分，你的积分不足，无法购买！"%buyneed_integral
                    else:
                        user_integral = cursor.fetchone()[0]
                        if user_integral >= buyneed_integral:
                            return "1 购买对方需要%i积分，确认购买请输1，取消请输0"%buyneed_integral
                        else:
                            return "-1 购买对方需要%i积分，你的积分不足，无法购买"%buyneed_integral
            elif n == 2:
                nowtime = time.time()
                sql = "select date from master where id = '%s'"%dictionary['slaveid']
                if 0 == cursor.execute(sql):
                    sql = "select integral from sign where id = '%s'"%dictionary['user']
                    if(0 == cursor.execute(sql)):
                        return "-1 购买对方需要%i赎金，你的积分不足，无法购买"%buyneed_integral
                    else:
                        user_integral = cursor.fetchone()[0]
                        if user_integral >= buyneed_integral:
                            return "1 购买对方需要%i赎金，确认购买请输1，取消请输0"%buyneed_integral
                        else:
                            return "-1 购买对方需要%i赎金，你的积分不足，无法购买"%buyneed_integral
                date = cursor.fetchone()[0]
                if date + 3*24*60*60 > nowtime:
                    sql = "select number from prop where id = '%s' and kind = 2"%dictionary['user']
                    if(0 == cursor.execute(sql)):
                        return "-1 购买对方需要%i赎金和一张强制购买卡，你没有强制保护卡无法购买"%buyneed_integral
                    else:
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        if(0 == cursor.execute(sql)):
                            return "-1 购买对方需要%i赎金，你的积分不足，无法购买"%buyneed_integral
                        user_integral = cursor.fetchone()[0]
                        if user_integral >= buyneed_integral:
                            return "1 购买对方需要%i赎金和一张强制购买卡，确认购买请输1，取消购买请输0！"%buyneed_integral
                        else:
                            return "-1 购买对方需要%i赎金，你的积分不足，无法购买"%buyneed_integral
                else:
                    sql = "select integral from sign where id = '%s'"%dictionary['user']
                    if(0 == cursor.execute(sql)):
                        return "-1 购买对方需要%i赎金，你的积分不足，无法购买"%buyneed_integral
                    else:
                        user_integral = cursor.fetchone()[0]
                        if user_integral >= buyneed_integral:
                            return "1 购买对方需要%i赎金，确认购买请输1，取消请输0"%buyneed_integral
                        else:
                            return "-1 购买对方需要%i赎金，你的积分不足，无法购买"%buyneed_integral
        except BaseException as e:
            print(e)
            return "-1 对不起，服务器出现未知错误，请截图联系维护人员反馈"
    elif dictionary['action'] == 'putname':
        sql = "select number from prop where id = '%s' and kind = 5"%dictionary['user']
        try:
            if(0 == cursor.execute(sql)):
                return '-1'
            else:
                number = cursor.fetchone()[0]
                if number <= 0:
                    return '0'
            sql = "select * from sign where id = '%s'"%dictionary['slaveid']
            if 0 == cursor.execute(sql):
                return '0'
            else:
                if dictionary['user'] == dictionary['slaveid']:
                    sql = "update sign set name = '%s' where id = '%s'"%(dictionary['name'],dictionary['slaveid'])
                else:
                    sql = "select masterid from master where id = '%s'"%dictionary['slaveid']
                    if 0 == cursor.execute(sql):
                        return '-1'
                    masterid = cursor.fetchone()[0]
                    if masterid == dictionary['user']:
                        sql = "update sign set name = '%s' where id = '%s'"%(dictionary['name'],dictionary['slaveid'])
                    else:
                        return '-1'
                    if 0 == cursor.execute(sql):
                        return '0'
                    else:
                        if number == 1:
                            sql = "delete from prop where id = '%s' and kind = 5"%dictionary['user']
                        elif number >1:
                            sql = "update prop set number = number -1 where id = '%s' and kind = 5"%dictionary['user']
                        if(0 == cursor.execute(sql)):
                            return '0'
                        connection.commit()
                        return '1'
                if 0 == cursor.execute(sql):
                    return '0'
                else:
                    connection.commit()
                    return '1'
        except BaseException as e:
            print(e)
            return '0'
    elif dictionary['action'] == 'getidbyname':
        sql = 'select id from sign where name = "%s"'%dictionary['name']
        try:
            if 0 == cursor.execute(sql):
                return '-1'
            id = cursor.fetchone()[0]
            return id
        except BaseException as e:
            print(e)
            return '0'
    elif dictionary['action'] == 'getat':
        sql = 'select name from sign where id = "%s"'%dictionary['user']
        try:
            if 0 == cursor.execute(sql):
                return '[CQ:at,qq=%s]'%dictionary['user']
            name = cursor.fetchone()[0]
            if name == '无':
                return '[CQ:at,qq=%s]'%dictionary['user']
            else:
                return name
        except BaseException as e:
            print(e)
            return '[CQ:at,qq=%s]'%dictionary['user']
    elif dictionary['action'] == 'buy':
        cursor = connection.cursor()
        sql = "select date,masterid from master where id = '%s'"%dictionary['slaveid']
        nowtime = time.time()
        try:
            if(0 == cursor.execute(sql)):
                sql = "select number from prop where id = '%s' and kind = 3"%dictionary['slaveid']
                if(0 == cursor.execute(sql)):
                    n,buyneed_integral = slave_buyneedintegral(dictionary['slaveid'],connection)
                    sql = "select integral from sign where id = '%s'"%dictionary['user']
                    cursor.execute(sql)
                    integral = cursor.fetchone()[0]
                    if integral < buyneed_integral :
                        return '0'
                    sql = "update sign set integral = integral - %i where id = '%s'"%(buyneed_integral,dictionary['user'])
                    if(0 == cursor.execute(sql)):
                        return '0'
                    else:
                        sql = "update sign set integral = integral + %i where id = '%s'"%(buyneed_integral*0.5,dictionary['slaveid'])
                        if(0 == cursor.execute(sql)):
                            return '0'
                        else:
                            sql = "insert into master(id,masterid,date,ransom) values('%s','%s',%i,%i)"%(dictionary['slaveid'],dictionary['user'],nowtime,buyneed_integral)
                            if(0 == cursor.execute(sql)):
                                return '0'
                            else:
                                connection.commit()
                                return '1'
                else:
                    number = cursor.fetchone()[0]
                    if number == 1:
                        sql = "delete from prop where id = '%s' and kind = 3"%dictionary['slaveid']
                        if(0 == cursor.execute(sql)):
                            return '0'
                        else:
                            sql = "insert into master(id,date,masterid,ransom) values('%s',%i,0,0)"%(dictionary['slaveid'],nowtime)
                            if(0 == cursor.execute(sql)):
                                return '0'
                            else:
                                connection.commit()
                                return '-1'
                    else:
                        sql = "update prop set number = number -1 where id = '%s' and kind = 3"%dictionary['slaveid']
                        if(0 == cursor.execute(sql)):
                            return '0'
                        else:
                            sql = "insert into master(id,date,masterid,ransom) values('%s',%i,0,0)"%(dictionary['slaveid'],nowtime)
                            if(0 == cursor.execute(sql)):
                                return '0'
                            else:
                                connection.commit()
                                return '-1'
            else:
                result = cursor.fetchone()
                date = result[0]
                masterid = result[1]
                if (date+3*24*60*60<=nowtime):
                    sql = "select number from prop where id = '%s' and kind = 3"%dictionary['slaveid']
                    if(0 == cursor.execute(sql)):
                        n,buyneed_integral = slave_buyneedintegral(dictionary['slaveid'],connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        integral = cursor.fetchone()[0]
                        if integral < buyneed_integral :
                            return '0'
                        sql = "update sign set integral = integral - %i where id = '%s'"%(buyneed_integral,dictionary['user'])
                        if(0 == cursor.execute(sql)):
                            return '0'
                        else:
                            if masterid == '0':
                                sql = "update sign set integral = integral + %i where id = '%s'"%(buyneed_integral*0.5,dictionary['slaveid'])
                                if(0 == cursor.execute(sql)):
                                    return '0'
                                else:
                                    sql = "update master set masterid = '%s',date = %i,ransom = %i where id = '%s'"%(dictionary['user'],nowtime,buyneed_integral+100,dictionary['slaveid'])
                                    if(0 == cursor.execute(sql)):
                                        return '0'
                                    else:
                                        connection.commit()
                                        return '1'
                            else:
                                sql = "update sign set integral = integral + %i where id = '%s'"%(buyneed_integral*0.8,masterid)
                                if(0 == cursor.execute(sql)):
                                    return '0'
                                else:
                                    sql = "update master set masterid = '%s',date = %i,ransom = %i where id = '%s'"%(dictionary['user'],nowtime,buyneed_integral+100,dictionary['slaveid'])
                                    if(0 == cursor.execute(sql)):
                                        return '0'
                                    else:
                                        connection.commit()
                                        return '1'
                    else:
                        number = cursor.fetchone()[0]
                        if number == 1:
                            sql = "delete from prop where id = '%s' and kind = 3"%dictionary['slaveid']
                            if(0 == cursor.execute(sql)):
                                return '0'
                            else:
                                sql = "update master set date = %i where id = '%s'"%(nowtime,dictionary['slaveid'])
                                if(0 == cursor.execute(sql)):
                                    return '0'
                                else:
                                    connection.commit()
                                    return '-1'
                        else:
                            sql = "update prop set number = number -1 where id = '%s' and kind = 3"%dictionary['slaveid']
                            if(0 == cursor.execute(sql)):
                                return '0'
                            else:
                                sql = "update master set date = %i where id = '%s'"%(nowtime,dictionary['slaveid'])
                                if(0 == cursor.execute(sql)):
                                    return '0'
                                else:
                                    connection.commit()
                                    return '-1'
                else:
                    sql = "select number from prop where id = '%s' and kind = 2"%dictionary['user']
                    cursor.execute(sql)
                    number = cursor.fetchone()[0]
                    if number == 1:
                        sql = "delete from prop where id = '%s' and kind = 2"%dictionary['user']
                        if 0 == cursor.execute(sql):
                            return '0'
                        n,buyneed_integral = slave_buyneedintegral(dictionary['slaveid'],connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        integral = cursor.fetchone()[0]
                        if integral < buyneed_integral :
                            return '0'
                        sql = "update sign set integral = integral - %i where id = '%s'"%(buyneed_integral,dictionary['user'])
                        if(0 == cursor.execute(sql)):
                            return '0'
                        else:
                            if masterid == '0':
                                sql = "update sign set integral = integral + %i where id = '%s'"%(buyneed_integral*0.5,dictionary['slaveid'])
                                if(0 == cursor.execute(sql)):
                                    return '0'
                                else:
                                    sql = "update master set masterid = '%s',date = %i,ransom = %i where id = '%s'"%(dictionary['user'],nowtime,buyneed_integral+100,dictionary['slaveid'])
                                    if(0 == cursor.execute(sql)):
                                        return '0'
                                    else:
                                        connection.commit()
                                        return '1'
                            else:
                                sql = "update sign set integral = integral + %i where id = '%s'"%(buyneed_integral*0.8,masterid)
                                if(0 == cursor.execute(sql)):
                                    return '0'
                                else:
                                    sql = "update master set masterid = '%s',date = %i,ransom = %i where id = '%s'"%(dictionary['user'],nowtime,buyneed_integral+100,dictionary['slaveid'])
                                    if(0 == cursor.execute(sql)):
                                        return '0'
                                    else:
                                        connection.commit()
                                        return '1'
                    else:
                        sql = "update prop set number = number -1 where id = '%s' and kind = 2"%dictionary['user']
                        if(0 == cursor.execute(sql)):
                            return '0'
                        n,buyneed_integral = slave_buyneedintegral(dictionary['slaveid'],connection)
                        sql = "select integral from sign where id = '%s'"%dictionary['user']
                        cursor.execute(sql)
                        integral = cursor.fetchone()[0]
                        if integral > buyneed_integral :
                            return '0'
                        sql = "update sign set integral = integral - %i where id = '%s'"%(buyneed_integral,dictionary['user'])
                        if(0 == cursor.execute(sql)):
                            return '0'
                        else:
                            if masterid == '0':
                                sql = "update sign set integral = integral + %i where id = '%s'"%(buyneed_integral*0.5,dictionary['slaveid'])
                                if(0 == cursor.execute(sql)):
                                    return '0'
                                else:
                                    sql = "update master set masterid = '%s',date = %i,ransom = %i where id = '%s'"%(dictionary['user'],nowtime,buyneed_integral+100,dictionary['slaveid'])
                                    if(0 == cursor.execute(sql)):
                                        return '0'
                                    else:
                                        connection.commit()
                                        return '1'
                            else:
                                sql = "update sign set integral = integral + %i where id = '%s'"%(buyneed_integral*0.8,masterid)
                                if(0 == cursor.execute(sql)):
                                    return '0'
                                else:
                                    sql = "update master set masterid = '%s',date = %i,ransom = %i where id = '%s'"%(dictionary['user'],nowtime,buyneed_integral+100,dictionary['slaveid'])
                                    if(0 == cursor.execute(sql)):
                                        return '0'
                                    else:
                                        connection.commit()
                                        return '1'
        except BaseException as e:
            print(e)
            return '0'
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
        sql="update master set masterid = '0' and ransom = 0 where id = '%s'"%dictionary['user']
        try:
            if 0 == cursor.execute(sql):
                return '0'
            else:
                connection.commit()
                return '1'
        except BaseException as e:
            print(e)
            return '0'
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
            sql="select number from prop where id = '%s' and kind = %s"%(dictionary['user'],dictionary['rank'])
            if(0==cursor.execute(sql)):            
                sql="insert into prop(id,kind,number,name) values('%s',%s,1,'%s')"%(dictionary['user'],dictionary['rank'],name)
                if (0 == cursor.execute(sql)):
                    return '0'
                else:
                    connection.commit()
                    return '1'
            else:
                sql="update prop set number =(select number where id = '%s' and kind = %s)+1 where id = '%s' and kind = %s"%(dictionary['user'],dictionary['rank'],dictionary['user'],dictionary['rank'])
                if(0==cursor.execute(sql)):
                    return '0'
                else:
                    connection.commit()
                    return '1'
        except BaseException as e:
            print(e)
            return '数据库因未知原因出错，请联系维护人员进行处理。'
    elif dictionary['action']=='reduce':
        sql = 'select name from prop where id = "0" and kind = "%s"'%dictionary['rank']
        try:
            if(0 == cursor.execute(sql)):
                return '数据库因未知原因出错，请联系维护人员进行处理。'
            else:
                name = cursor.fetchone()[0]
                sql = "select number from prop where id = '%s' and kind = %s"%(dictionary['user'],dictionary['rank'])
                if 0 == cursor.execute(sql):
                    return '-1'
                    return '-1'
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
                    connection.commit()
                    return '1'
        except BaseException as e:
            print(e)
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
            return "%s你的道具还是空的！"%getat(dictionary['user'],connection)
        else:
            outcome="%s拥有:"%getat(dictionary['user'],connection)
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
                outcome = ''
                for i in range(0,cursor.rowcount):
                    result = cursor.fetchone()
                    if result[1] <= 0:
                        continue
                    outcome+= '%-5s  售价%-5i\n'%(result[0],result[1])
                outcome+= '如果有想要的道具可以兑换哦'
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
                        return '虽然傻馒知道你很想要，但是这张卡不能从商店兑换哦！'
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
    elif data.find(':administor') != -1:
        dictionary = getdict(data)
        outcome = processforAdministor(dictionary,connection)
        conn.send(outcome.encode('gbk'))
        print('administor')
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
