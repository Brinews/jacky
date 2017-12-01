# encoding=utf8

import sqlite3
import unittest

def None2NULL(x):
    if x == None:
        return 'NULL'
    else:
        return str(x)

class SqliteEngine(object):
    '''
    operates sqlite3 db
    '''

    m_db_name = ''
    m_db_conn = None

    def __init__(self, dbname='./sqlite3.db'):
        '''
        Constructor
        '''
        self.m_db_name = dbname
        
    def Init(self):
        if self.m_db_conn != None:
            print('db is already connected')
        self.m_db_conn = sqlite3.connect(self.m_db_name)
    
    def LoadSchema(self, sql_file='./createsql.txt'):
        sqls = []
        f = open(sql_file)
        for line in f:
            sqls.append(str(line).strip())
        f.close()
        
        c = self.m_db_conn.cursor()
        c.executescript('''%s''' % ' '.join(sqls))        
    
    def SQL(self, sql=''):
        if sql == '':
            return
        
        #print('Debug Info === ', sql)
        
        c = self.m_db_conn.cursor()
        c.execute(sql)
        self.m_db_conn.commit()
        info = c.fetchall()
        
        result = []
        for item in info:
            item = map(None2NULL, item)
            result.append(item)
        return result
    
    def do_query(self, qrykey):
        sql = '''select * from records where querykeys='%s' order by score desc '''%qrykey
        info = self.SQL(sql)
        #for item in info:
        #    print('\t'.join(item))
        return info

    def do_query_tag(self, qrykey):
        sql = '''select * from tags where querykeys='%s' '''%qrykey
        info = self.SQL(sql)
        #for item in info:
        #    print('\t'.join(item))
        return info

    def do_insert(self, querykeys, relatewords):
        sql = '''insert into tags values ('%s', '%s')''' % (querykeys, relatewords)
        self.SQL(sql)

    def do_add(self, querykeys, results, scores):
        
        idx = 0
        for r in results:
            sql = '''insert into records values ('%s', '%s', '%d', date('now'))''' % (querykeys, r, scores[idx])
            self.SQL(sql)
            idx += 1
    
    def do_update(self, querykeys, result, score):
        sql = '''update records set score='%d' where querykeys='%s' and links = '%s' ''' % (score, querykeys, result)
        self.SQL(sql)

    def do_delete(self, querykeys, result):
        
        sql = '''delete from records where querykeys='%s' and result='%s' ''' % (querykeys, result)
        self.SQL(sql)

def initDB():
    se = SqliteEngine(dbname='./sqlite3_test.db')
    se.Init()
    se.LoadSchema()

    return se

def connDB():
    se = SqliteEngine(dbname='./sqlite3_test.db')
    se.Init()

    return se

def test():
    se = SqliteEngine(dbname='./sqlite3_test.db')
    se.Init()
    se.LoadSchema()
    print(se.SQL('select * from records'))
    res = ['result.com/page=1']
    scores = [10]
    se.do_add('key', res, scores)
    print(se.do_query('key'))
    print(se.SQL('select * from records'))
    
if __name__ == '__main__':
    test()
