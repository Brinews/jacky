# coding=utf-8

from urllib.request import urlopen
from urllib.request import quote

from bs4 import BeautifulSoup
from sqlite_engine import initDB

from OrderedList import OrderedList

import threading

def get_html(url):

   page = urlopen(url)
   req = page.read()
   return req


def get_news(db, key, html_text, num):

   bs = BeautifulSoup(html_text, "html.parser")
   body = bs.body
   data = body.find('div', {'id': 'content_left'})

   score = []
   url = []

   for i in range(1, num+1):
       result = data.find('div', {'id': str(i)})
       a = result.find("a")

       print('------------------------------------- ' + str(i) + '------------------------------------- ')
       print(a.contents)
       print(a.get('href'))
       print()

       url.append(a.get('href'))
       score.append(num+1-i)

   db.do_add(key, url, score)


def getresult(db, cache, num, question):

   if cache.search(question):
       # find the key in cache

       res = db.do_query(question)
       if len(res) > 0:
           print('\nFound In Database\n\n')
           for url in res:
               print('\t'.join(url))
           print()
           return

   # add to cache
   cache.add(question)

   url = "http://www.baidu.com/s?wd=" + quote(question.encode('utf-8')) + "&rn=" + str(num)
   html_doc = get_html(url)
   get_news(db, question, html_doc, num)

def worker(num):
   """thread worker function"""
   print('Searcher: %s' % num)

   db = initDB()
   keycache = OrderedList()

   while True:
       keyword = input('Input the keywords to search:');
       if keyword == 'quit': break

       getresult(db, keycache, n, keyword) 

   return

t = threading.Thread(target=worker, args=(0,))

if __name__ == '__main__':

   q = "google" # for example keywords
   n = 20 # default links number

   t.start()

