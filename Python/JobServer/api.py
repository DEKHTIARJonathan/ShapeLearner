################################# Import Libraries ################################
import os.path
from bottle import route, run, response, static_file, request, error, Bottle, template
from json import dumps
from sqlalchemy import *
from sqlalchemy.sql import select, column

#################################### WebService Route / #####################################
class API:
	def __init__(self, credentials, port, local):
		self._app = Bottle()
		self._route()
		
		v_connect = self._initConnect(credentials)
		self._engine = v_connect[0]
		self._connection = v_connect[1]
		
		self._local = local
		self._port = port
		
		if local:
			self._host = '127.0.0.1'
		else:
			self._host = '0.0.0.0'
	
	def start(self):
		self._app.run(server='paste', host=self._host, port=self._port)
		
	def _route(self):
		self._app.hook('before_request')(self._strip_path)
		self._app.error(400)(self._error404)
		self._app.error(500)(self._error500)
		
		self._app.route('/updateJob', method="POST", callback=self._updateJob)
		self._app.route('/getJobStatus', method="POST", callback=self._getJob)
		self._app.route('/initdb', callback=self._initDB)
		self._app.route('/static/<filename:path>', callback=self._getStaticFile)
		self._app.route('/createJob', callback=self._createJob)
		self._app.route('/', callback=self._homepage)
	
	def _initConnect(self, credentials):
		engine = create_engine("postgresql+psycopg2://"+credentials['dbUser']+":"+credentials['dbPass']+"@"+credentials['ip']+":"+credentials['port']+"/"+credentials['dbName'], isolation_level="READ COMMITTED")
		connection = engine.connect()
		return [engine, connection]
	
	def _strip_path(self):
		request.environ['PATH_INFO'] = request.environ['PATH_INFO'].rstrip('/')
	
	def _error404(self):
		return static_file("404.html", root=os.getcwd()+'\\html')
		
	def _error500(self, error):
		return error
		
	def _getStaticFile(self, filename):
		extension = str.lower(os.path.splitext(filename)[1][1:])
		if  extension == 'jpeg'or extension == 'jpg':
			return static_file(filename, root=os.getcwd()+'\\static', mimetype='image/jpg')
		elif extension == 'png':
			return static_file(filename, root=os.getcwd()+'\\static', mimetype='image/png')
		elif extension == 'css':
			return static_file(filename, root=os.getcwd()+'\\static', mimetype='text/css')
		elif extension == 'js':
			return static_file(filename, root=os.getcwd()+'\\static', mimetype='text/javascript')  
			
	def _createJob(self):
		sql = '''INSERT INTO jobs("jobStatus", "partID", "partName", "serverIP", "serverPort", "message", "updateDate") VALUES ('Not Started', 0, '', '', '', '', now()) RETURNING "idJob" as id;'''
		result = self._engine.execute(sql).fetchone()

		rv = {"jobID": result['id']}
		response.content_type = 'application/json'
		return dumps(rv)

	def _updateJob(self):
		
		sql = """
		UPDATE jobs 
		
		SET 
		"jobStatus" = '"""+request.json["jobStatus"]+"""', 
		"partID" = """+request.json["partID"]+""", 
		"partName" = '"""+request.json["partName"]+"""', 
		"serverIP" = '"""+request.json["serverIP"]+"""', 
		"serverPort" = '"""+request.json["serverPort"]+"""', 
		"message" = '"""+request.json["message"].replace("'", "''")+"""', 
		"updateDate" = NOW()

		WHERE "idJob" = """+request.json["jobID"]+"""  

		RETURNING "idJob" as id;
		"""
		
		result = self._engine.execute(sql).fetchone()
		rv = {"jobID": result['id']}
		
		response.content_type = 'application/json'
		return dumps(rv)
	
	def _getJob(self):
		
		sql = """
			SELECT
				"idJob",
				"jobStatus", 
				"partID", 
				"partName", 
				"serverIP", 
				"serverPort", 
				"message", 
				"updateDate"
			FROM jobs
			WHERE "idJob" = """ + str(request.json["idJob"]) + """;
		"""
		
		result = self._engine.execute(sql).fetchone()
		rv = {"idJob": result['idJob'], "jobStatus": result['jobStatus'], "partID": result['partID'], "partName": result['partName'], "serverIP": result['serverIP'], 
		"serverPort": result['serverPort'], "message": result['message'], "updateDate": result['updateDate'].isoformat()}		
		
		response.content_type = 'application/json'
		return dumps(rv)
	
	def _initDB(self):
		f = open('structure.sql', 'r')
		result = ""
		try:
			sql = str(f.read())
			result = self._engine.execute(sql)
		finally:
			f.close()
			rv = {"status": "ok"}
			response.content_type = 'application/json'
			return dumps(rv)

	def _homepage(self):
		return static_file("index.html", root=os.getcwd()+'\\html')
