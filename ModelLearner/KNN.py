#!/usr/local/bin/ python

from sqlalchemy import *
from sqlalchemy.sql import select
import logging
from sklearn import tree
logging.getLogger('sqlalchemy.dialects.postgresql').setLevel(logging.INFO)

def get_data(table):
	s = select([table])
	result = c.execute(s)  
	return [row for row in result]

def f(x):
	global maxVal
	global vals
	if (not x in vals):
		maxVal = maxVal + 1
		vals[x] = maxVal
		rev_vals[maxVal] = x
	return vals[x]

def get_selectors(data):
	return [ [f(x) for x in t[0:-1]] for t in data]

def get_predictors(data):
	return [0 if "<" in t[14] else 1 for t in data]


'''engine = create_engine("postgresql+pg8000://postgres:postgres@localhost:10024/postgres", isolation_level="READ COMMITTED")'''
engine = create_engine("postgresql+psycopg2://postgres:postgres@localhost:10021/postgres", isolation_level="READ COMMITTED")
c = engine.connect()

meta = MetaData()

income_trn = Table('income_trn', meta, autoload=True, autoload_with=engine)
income_test = Table('income_test', meta, autoload=True, autoload_with=engine)

trn_data = get_data(income_trn)
test_data = get_data(income_test)

maxVal = 0
vals = dict()
rev_vals = dict()

trn = get_selectors(trn_data)
trn_v = get_predictors(trn_data)

clf = tree.DecisionTreeRegressor()
clf = clf.fit(trn, trn_v)

test = get_selectors(test_data)
test_v = get_predictors(test_data)

testsRun = 0
testsPassed = 0
for t in test:
	if clf.predict(t) == test_v[testsRun]:
		testsPassed = testsPassed + 1
	testsRun = testsRun + 1

100 * testsPassed / testsRun

print("ok")
			