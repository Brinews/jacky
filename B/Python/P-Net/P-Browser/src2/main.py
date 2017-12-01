# coding=utf-8

from urllib.request import urlopen
from urllib.request import quote

from bs4 import BeautifulSoup
from sqlite_engine import initDB

from OrderedList import OrderedList

import threading

import jieba
import jieba.analyse

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

   relatewords = []

   for i in range(1, num+1):
       result = data.find('div', {'id': str(i)})
       a = result.find("a")

       print('------------------------------------- ' + str(i) + '------------------------------------- ')
       print(a.contents)
       print(a.get('href'))

       for seg in a.contents:
           relatewords.append(seg)

       url.append(a.get('href'))
       score.append(num+1-i)

   allwords = ''
   for w in relatewords:
       allwords += str(w)

   #print(allwords)
   tags = jieba.analyse.extract_tags(allwords)
   tagsw = ",".join(tags)
   print(tagsw)

   db.do_add(key, url, score)
   db.do_insert(key, tagsw)

   return url


def getresult(db, cache, num, question):

   if cache.search(question):
       # find the key in cache

       res = db.do_query(question)
       if len(res) > 0:
           print('\nFound In Database\n\n')
           urlList = []
           for url in res:
               print('\t'.join(url))
               urlList.append(url)
           print()
           return urlList

   # add to cache
   cache.add(question)

   url = "http://www.baidu.com/s?wd=" + quote(question.encode('utf-8')) + "&rn=" + str(num)
   html_doc = get_html(url)

   return get_news(db, question, html_doc, num)

def worker(num, keys, n, keycache):
   """thread worker function"""
   print('Searcher thread: [%s]' % num)

   db = initDB()
   getresult(db, keycache, n, keys) 


def start_search(keys, num, keycache):
   t = threading.Thread(target=worker, args=(0, keys, num, keycache))
   t.start()

if __name__ == '__main__':
   q = "google" # for example keywords
   keycache = OrderedList()
   n = 20 # default links number
   start_search(q, n, keycache)
