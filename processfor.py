import pymysql

def processforcard(dictionary,connection):
    outcome = '0'
    cursor = connection.cursor()
    if dictionary['action'] == 'add':
        sql = 'select QQ from school_card where id = "%s"'%dictionary['id']
        try:
            if(0 == cursor.execute(sql)):
                sql = 'select id from school_card where QQ = "%s"'%dictionary['user']
                if(0 == cursor.execute(sql)):
                    sql = 'insert into school_card(id,QQ) values("%s","%s")'%(dictionary['id'],dictionary['user'])
                    if(0 == cursor.execute(sql)):
                        return '因为未知原因你的绑定申请上传失败了，请联系维护人员重试!'
                    else:
                        connection.commit()
                        return '你的绑定申请上传成功，非常感谢你对连锁保护的支持，也请向更多人宣传该功能与武理知音的存在，让我们能为更多同学提供服务于帮助！'
                else:
                    result = cursor.fetchone()
                    string = "****"
                    string+= result[0][4:]
                    return '你当前使用的qq好像已经绑定了id为%s的校园卡，若该卡不是你的校园卡那么请联系维护人员解绑，为你带来不便我们深表歉意！'%string
            else:
                result = cursor.fetchone()
                return '当前校园卡已绑定qq，若他人误绑你的校园卡请联系维护人员介入，给你带来不便我们深表歉意！'%result[0]
        except BaseException as e:
            print(e)
            return '因为未知原因你的绑定申请上传失败了，请联系维护人员重试!'
    elif dictionary['action'] == 'find':
        sql = 'select QQ from school_card where id = "%s"'%dictionary['id']
        try:
            if(0 == cursor.execute(sql)):
                return '非常抱歉的通知你，该校园卡并没有绑定qq号，感谢你对连锁保护的支持，请向更多人宣传该功能与武理知音的存在，让我们能为更多同学提供服务于帮助！'
            else:
                result = cursor.fetchone()
                return '该校园卡绑定的qq号为:%s，若您无法联系上对方可联系维护人员介入！'%result[0]
        except BaseException as e:
            print(e)
            return '因为未知原因你的绑查询申请上传失败了，请联系维护人员重试!'
    elif dictionary['action'] == 'delete':
        sql = 'select QQ from school_card where QQ = "%s"'%dictionary['user']
        try:
            if(0 == cursor.execute(sql)):
                return '你当前qq号并没有绑定校园卡，感谢你对武理知音的支持！'
            else:
                sql = 'delete from school_card where QQ = "%s"'%dictionary['user']
                if(0 == cursor.execute(sql)):
                    return '因为未知原因你的解绑申请上传失败了，请联系维护人员重试！'
                else:
                    connection.commit()
                    return '你的校园卡成功解绑，是傻馒哪里做的不好吗?可以向维护人员反馈或者在我的说说下面评论哦。感谢你对武理知音的支持！'
        except BaseException as e:
            print(e)
            return '因为未知原因你的解绑申请上传失败了，请联系维护人员重试!'


            