#-*- coding:utf-8 -*-

import sys 
  
import jieba  
import jieba.analyse  

wf = open('clean_title.txt','w+')  
for line in open('clean_data.csv'): 
    item = line.strip('\n').split('\t')
    tags = jieba.analyse.extract_tags(item[0])
    tagsw = ",".join(tags)
    wf.write(tagsw)  
wf.close()  
