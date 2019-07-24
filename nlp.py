import numpy
import math
import jieba

def init():
        return numpy.load('c:\workstation\worddata.npy').tolist()

def cos(a,b):
	return numpy.matmul(a,b)/(math.sqrt((a**2).sum())*math.sqrt((b**2).sum()))

def textfind(text,worddata):
	a=numpy.zeros(len(worddata))
	for i in jieba.lcut(text):
		if i in worddata:
			a[worddata.index(i)]+=1
	return a

def add(word,worddata):
	a=jieba.lcut(word)
	for i in a:
		if i not in worddata:
			worddata.append(i)
			numpy.save('worddata.npy',worddata)
	return 1

def manage(word1,word2,worddata):
        add(word1,worddata)
        add(word2,worddata)
        a=textfind(word1,worddata)
        b=textfind(word2,worddata)
        return cos(a,b)

question=["ai社是干什么的？",
          "你怎么知道我问的是哪句话呢？",
          "什么是人工智能？",
          "人工智能有什么用？",
          "怎么加入ai社？",
          "ai社有什么活动呢？",
          "ai的前途怎么样？",
          "社团里有什么大佬能带带我们吗？",
          "ai会不会背叛我们？",
          "我没有电脑我能报名吗？",
          "学习人工智能需要什么教材吗？",
          "学习ai究竟有什么要求？",
          "我对ai感兴趣但并不想学习可以报名吗？",
          "如何报名干事，干事有什么用？"]
answer=["我们是近期成立，立志于向大家科普与内部研究人工智能的社团。",
        "因为技术储备不足，我们采用了极为简单且低效的余弦对比，只能在较相似的范围内进行对比，你可以尝试更改词语顺序进行询问！",
        "人工智能是研究、开发用于模拟、延伸和扩展人的智能的理论、方法、技术及应用系统的一门新的技术科学。简而言之是一种训练程序以在某些层面上代替人类进行自动化处理的学科。",
        "人工智能可以被应用到无人驾驶，智能家居，兴趣推测，工作助手，情感助理等一系列方面，可以说是只要是你想的到的地方都可以应用到人工智能，只是难度的问题。",
        "请发送'k 姓名 学院 班级 电话号码 宿舍'给我就可以加入我们社团(要求严格按照格式来，k小写，收到成功加入的消息才成功)，记得向工作人员索取会员群二维码或者直接加入486712165！",
        "我们预期会举办一些科普讲座，向大家介绍ai的一些实际应用，以及如果你想学ai你应该先学会什么，还有ai的一些专有名词及其含义。",
        "就目前而言ai仍旧是一座空中阁楼，尽管有着辉煌的前景但是距离实现还有很长一段距离。",
        "大佬是没有的...ai是一个十分困难的学科，只能说加入我们社团后大家一同学习，互相交流下会比闭门造车更有效率。",
        "目前的人工智能与科幻小说中的人工智能有着本质上的区别，目前的人工智能仅仅是一段针对某个事物进行学习的程序，并没有所谓的思维，也没有通过学习而成长出一些奇奇怪怪的功能的能力！",
        "当然可以，人工智能的学习并不需要过多的实践，更多的是对理论知识的学习及理解！",
        "对应的教材无论是实体书或是网络课程乃至论文层出不穷，只要你想学就不需要担心教材的问题！",
        "首先毫无疑问的是兴趣，如果你连兴趣都没有自然谈不上要求，其次最好有着一定的数学功底（不是必须的！），然后就是刻苦刻苦再刻苦啦！",
        "当然可以，我们的活动并非强制性的，社员只有权利没有义务，而想要成为我们的干事也可以选择宣传部或是秘书部！",
        "在博学广场上找到ai社的帐篷领取简历并报名就可以等我们的面试通知啦！干事是一个社团的中流砥柱，也是社团的下一任接班人，每个部门的干事负责的事物请当面询问！"]