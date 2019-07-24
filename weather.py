import re
import pymysql
import requests
from bs4 import BeautifulSoup

class SearchWeather():
    def __init__(self):
        self.HEADERS ={
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 ''(KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36'}
        self.CONNECTION = pymysql.connect(host='localhost',user='root',password='my@Zheng123',db='test',charset='utf8',cursorclass=pymysql.cursors.DictCursor)

    def getcityCode(self,cityName):
        SQL = "SELECT cityCode FROM cityWeather WHERE cityName='%s'" % cityName
        try:
            with self.CONNECTION.cursor() as cursor:
                cursor.execute(SQL)
                self.CONNECTION.commit()
                result = cursor.fetchone()
                return result['cityCode']
        except Exception as e:
            print(repr(e))

    def getWeather(self,cityCode,cityname):
        url = 'http://www.weather.com.cn/weather/%s.shtml' % cityCode
        html = requests.get(url,headers = self.HEADERS)
        html.encoding='utf-8'
        soup=BeautifulSoup(html.text,'lxml')
        weather = ""
        item=soup.find("div", {'id': '7d'}).find('ul').find_all('li')[1]
        date,detail = item.find('h1').string, item.find_all('p')
        title = detail[0].string
        templow = detail[1].find("i").string
        temphigh = detail[1].find('span').string if detail[1].find('span')  else ''
        advise=soup.find("li",{"id":"chuanyi"}).find("p").string
        wind,direction = detail[2].find('span')['title'], detail[2].find('i').string
        weather += '这里是武理知音天气预报，【%s】%s天气如下：【%s】，温度：【%s~%s】，%s：【%s】,%s' % (cityname,date,title,templow,temphigh,wind,direction,advise)
        if weather.find("雨")>0:
            weather += "上下学记得带伞哟！"
        return weather

    def main(self,city):
        cityCode = self.getcityCode(city)
        detail = self.getWeather(cityCode,city)
        return detail

if __name__ == "__main__":
    weather = SearchWeather()
    weather.main(city="武汉")

