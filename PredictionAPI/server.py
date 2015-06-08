################################# Import Libraries ################################
import os.path
import operator
import numpy as np
from sqlalchemy import *
from sqlalchemy.sql import select, column
import logging
from sklearn.neighbors import KNeighborsClassifier
from sklearn.externals import joblib
from bottle import route, run, response, static_file, request, error
from json import dumps
logging.getLogger('sqlalchemy.dialects.postgresql').setLevel(logging.INFO)

################################# Save/Load Model #################################
def saveModel(model):
	joblib.dump(model, 'model/knn.model') 

def loadModel():
	objPath = 'model/knn.model'
	if os.path.isfile(objPath) == True :
		return joblib.load(objPath)
	else:
		return False

################################# Init Connection #################################
def initConnect(IP):
	engine = create_engine("postgresql+psycopg2://postgres:postgres@"+IP+":5432/postgres", isolation_level="READ COMMITTED")
	connection = engine.connect()
	return [engine, connection]

################################# Select Queries from DB ##########################	
def dump_table(table, connection):
	s = select([table])
	return connection.execute(s)

def get_test_data(table, id, connection):
	idCol = column('idGraph')
	s = select([table]).where(idCol == id)
	return connection.execute(s)	

################################# Data Formaters ##################################
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

################################# Model Training ##################################
def initServer(recomputeModel = False, _neighbors = 53, _weights = 'distance'):
    model = loadModel()
    if ((recomputeModel == False) and (model != False)) :
        return model
    else:
        v_connect = initConnect(IPtrainServer)

        model = trainModel(v_connect, _neighbors, _weights)
        saveModel(model)
        return model

def trainModel(v_connect, _neighbors, _weights):
    data = getLearningData('learning_data', v_connect)
    
    # Create and fit a nearest-neighbor classifier
    knn = KNeighborsClassifier(n_neighbors=_neighbors,weights=_weights)
    knn.fit(data[0], data[1])
    
    outputs = sorted(set(data[1]))
    
    return [knn, outputs]

################################# Model Prediction ##################################
def launchPredict(classifier, id, n_precision = -1, p_precision = -1):
	
	if n_precision == 0:
		n_precision = 1 #On modifie la variable pour ne pas obtenir une output vide.
	v_connect = initConnect(IPtestServer)
	
	data = getTestData('learning_data', v_connect, id)
	
	features = data[0]
	id = data[2][0]
	
	probaArray = classifier.predict_proba(features)[0]
	
	probaDict = dict(zip(outputs, probaArray))
	probaDescSorted = sorted(probaDict.items(), key=operator.itemgetter(1), reverse=True)
	
	if n_precision != -1 and n_precision < len(probaDescSorted) :
		probaDescSorted = probaDescSorted[0:n_precision]
	
	
	if p_precision != -1 :
		probaDescSorted = [i for i in probaDescSorted if i[1] >= (p_precision / 100)]
	
	return [probaDescSorted, id]

############### WebService Route /predict/id/<id:int>/nmax/<nmax:int> ################
@route('/predict/id/<id:int>/nmax/<nmax:int>')
def callbackNmax(id, nmax):
	global classifier
	assert isinstance(id, int)
	assert isinstance(nmax, int)
	
	strErr = ""
	
	if id < 0 :
		strErr = "id parameter can't be negative\n"
	if nmax <= 0 :
		strErr = strErr + "nmax parameter can't be negative or null\n"
	
	if strErr != "" :
		return strErr
	else :
		maxIndex = 17
		if id < maxIndex and id > 0 :
			rslt =  launchPredict(classifier, id, nmax)
			partPredictedClasses = rslt[0]
			partID = rslt[1]
			
			rv = [{"predictedClasses": partPredictedClasses,  "partID": partID }]
			response.content_type = 'application/json'
			return dumps(rv)

		else :
			return "Index Out of Range. Max Index = " + str(maxIndex -1 )

############### WebService Route /predict/id/<id:int>/pmax/<pmax:float> ###############

@route('/predict/id/<id:int>/pmax/<pmax:float>')
def callbackPmax(id, pmax):
	global classifier
	assert isinstance(id, int)
	assert isinstance(pmax, float)
	
	strErr = ""
	
	if id < 0 :
		strErr = "id parameter can't be negative\n"
	if pmax < 0 :
		strErr = strErr + "pmax parameter can't be negative\n"
	elif pmax > 100 :
		strErr = strErr + "pmax parameter can't be greater than 100\n"
	
	if strErr != "" :
		return strErr
	else :
		maxIndex = 18
		if id < maxIndex +1 and id > 0 :
			rslt =  launchPredict(classifier, id, -1, pmax)
			partPredictedClasses = rslt[0]
			partID = rslt[1]
			
			rv = [{"predictedClasses": partPredictedClasses,  "partID": partID }]
			response.content_type = 'application/json'
			return dumps(rv)
		else :
			return "Index Out of Range. Max Index = " + str(maxIndex -1 )

######## WebService Route /predict/id/<id:int>/nmax/<nmax:int>/pmax/<pmax:float> ########
@route('/predict/id/<id:int>/nmax/<nmax:int>/pmax/<pmax:float>')
def callback(id, nmax, pmax):
	global classifier
	assert isinstance(id, int)
	assert isinstance(nmax, int)
	assert isinstance(pmax, float)
	
	strErr = ""
	
	if id < 0 :
		strErr = "id parameter can't be negative\n"
	if nmax <= 0 :
		strErr = strErr + "nmax parameter can't be negative or null\n"
	if pmax < 0 :
		strErr = strErr + "pmax parameter can't be negative\n"
	elif pmax > 100 :
		strErr = strErr + "pmax parameter can't be greater than 100\n"
	
	if strErr != "" :
		return strErr
	else :
		maxIndex = 17
		if id < maxIndex and id > 0 :
			rslt =  launchPredict(classifier, id, nmax, pmax)
			partPredictedClasses = rslt[0]
			partID = rslt[1]
			
			rv = [{"predictedClasses": partPredictedClasses,  "partID": partID }]
			response.content_type = 'application/json'
			return dumps(rv)

		else :
			return "Index Out of Range. Max Index = " + str(maxIndex -1 )
			
########################## WebService Route /recomputeModel #################################
@route('/recomputeModel')
def recomputeModel():
	global classifier
	global outputs
	init = initServer(True)
	classifier = init[0]
	outputs = init[1]
	return "Model has been recomputed."		

#################################### WebService Route / #####################################
@error(404)
def error404(error):
    return static_file("404.html", root='/myapp/')
	
@error(500)
def error500(error):
    return error
	
@route('/images/<filename:path>')
def getImages(filename):
	extension = str.lower(os.path.splitext(filename)[1][1:])
	if  extension == 'jpeg'or extension == 'jpg':
		return static_file(filename, root='/myapp/images', mimetype='image/jpg')
	elif extension == 'png':
		return static_file(filename, root='/myapp/images', mimetype='image/png')
     
	
@route('/')
def NoIdPassed():
    return static_file("index.html", root='/myapp/')

################################# Server Initialization #####################################
IPtrainServer = "172.17.1.124"
IPtestServer = "172.17.1.117"
init = initServer()
classifier = init[0]
outputs = init[1]
run(server='paste', host='0.0.0.0', port=8888, debug=True, reloader=True)