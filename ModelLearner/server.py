################################# Import Libraries ###################################

import os.path
import numpy as np
from sqlalchemy import *
from sqlalchemy.sql import select, column
import logging
from sklearn.neighbors import KNeighborsClassifier
from sklearn.externals import joblib
from bottle import route, run, response
from json import dumps

logging.getLogger('sqlalchemy.dialects.postgresql').setLevel(logging.INFO)

################################# Save/Load Model ###################################

def saveModel(model):
	joblib.dump(model, 'model/save.model') 

def loadModel():
	objPath = 'model/save.model'
	if os.path.isfile(objPath) == True :
		return joblib.load(objPath)
	else:
		return False

################################# Init Connection ###################################

def initConnect(IP):
	engine = create_engine("postgresql+psycopg2://postgres:postgres@"+IP+":5432/postgres", isolation_level="READ COMMITTED")
	connection = engine.connect()
	return [engine, connection]

################################# Select Queries from DB ###################################
	
def dump_table(table, connection):
	s = select([table])
	return connection.execute(s)

def get_test_data(table, id, connection):
	idCol = column('idGraph')
	s = select([table]).where(idCol == id)
	return connection.execute(s)	

################################# Data Formaters ###################################	

def get_selectors(data):
	return [t[2:] for t in data]

def get_predictors(data):
	return [t[1] for t in data]

def get_ids(data):
	return [t[0] for t in data]

def formatData(data):
	tmp = [ t[:] for t in data]
	rslt = []
	for row in tmp :
		line = []
		for cell in row:
			line.append(cell)
		rslt.append(line)
	return rslt	

################################# Data Getters ###################################

def getLearningData(viewName, v_connect):
	engine = v_connect[0]
	connection = v_connect[1]

	meta = MetaData()

	training_data = Table(viewName, meta, autoload=True, autoload_with=engine)
	train_data = formatData(dump_table(training_data, connection))

	v_features = get_selectors(train_data)
	v_output = get_predictors(train_data)

	return [v_features, v_output]

def getTestData(viewName, v_connect, id):

	engine = v_connect[0]
	connection = v_connect[1]

	meta = MetaData()
	
	testing_data = Table('learning_data', meta, autoload=True, autoload_with=engine)
	test_data = formatData(get_test_data(testing_data,id, connection))

	features = get_selectors(test_data)
	realOutput = get_predictors(test_data)
	id = get_ids(test_data)

	return [features, realOutput ,id]


################################# Model Training ###################################

def initServer(recomputeModel = False):
	model = loadModel()
	if ((recomputeModel == False) and (model != False)) :
		return model
	else:
		v_connect = initConnect(IPtrainServer)

		model = trainModel(v_connect)
		saveModel(model)
		return model
	
def trainModel(v_connect):
	data = getLearningData('learning_data', v_connect)

	neighbors = 9
	weights = 'uniform'
	#weights = 'distance'
	
	# Create and fit a nearest-neighbor classifier
	knn = KNeighborsClassifier(algorithm='auto',leaf_size=30,metric='minkowski',metric_params=None,n_neighbors=neighbors,p=5,weights=weights)
	knn.fit(data[0], data[1]) 

	return knn

################################# Model Prediction ###################################
	
def launchPredict(classifier, id):

	v_connect = initConnect(IPtestServer)
	
	data = getTestData('learning_data', v_connect, id)
	
	features = data[0]
	real_class = data[1]
	id = data[2]
	
	predicted_class = classifier.predict(features)[0]
	
	return [predicted_class, real_class, id]

################################# WebService Route /test/<id:int> ###################################

@route('/test/<id:int>')
def callback(id):
	global classifier
	assert isinstance(id, int)
	
	maxIndex = 17
	if id < maxIndex :
		rslt =  launchPredict(classifier, id)
		partPredictedClass = rslt[0]
		partRealClass = rslt[1][0]
		partID = rslt[2][0]
		
		rv = [{ "ID": partID, "predictedClass": partPredictedClass, "realClass": partRealClass }]
		response.content_type = 'application/json'
		return dumps(rv)

	else :
		return "Index Out of Range. Max Index = " + str(maxIndex -1 )

################################# WebService Route / ###################################		
@route('/recomputeModel')
def recomputeModel():
	global classifier
	classifier = initServer(True)
	return "Model has been recomputed."		

################################# WebService Route / ###################################		
@route('/')
def NoIdPassed():
	return "Error : No ID passed via URL. Syntax : http://54.77.188.25:10004/test/id"

################################# Server Initialization ###################################
IPtrainServer = "172.17.0.74"
IPtestServer = "172.17.0.86"
classifier = initServer()
run(host='0.0.0.0', port=8888, debug=True, reloader=True)