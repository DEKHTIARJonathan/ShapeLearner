################################# Loading config File ################################
import sys
sys.path.append('../Config/')
from loadConf import loadConf

################################# Import Libraries ################################
import os.path
from bottle import route, run, response, static_file, request, error
from json import dumps

import ctypes
from threading import Thread
import time 
 
class TimingThread(Thread):
 
	def __init__(self, _timer):
		''' Constructor. '''
		Thread.__init__(self)
		self.__timingDLL = ctypes.CDLL('timeTrickDLL.dll')
		self.__timingDLL.changeTime.argtypes = [ctypes.c_uint]
		self.timer = _timer
 
	def run(self):
		self.__timingDLL.changeTime(self.timer)	
		
		
class ShockGrThread(Thread):
 
	def __init__(self, _dll, _img, _class):
		''' Constructor. '''
		Thread.__init__(self)
		self.__dll = _dll
		self.__img = _img
		self.__class = _class
		
	def run(self):
		self.__dll.signBinaryImage(self.__img, self.__class)
		#self.__dll.waitBeforeClosing()

#################################### WebService Route / #####################################

@error(404)
def error404(error):
	return static_file("404.html", root='C:\\Users\\Administrator\\Desktop\\ShapeLearnerPackage\\\ShockGraphAPI\\html')
	
@error(500)
def error500(error):
	return error

@route('/static/<filename:path>')
def getStaticFile(filename):
	extension = str.lower(os.path.splitext(filename)[1][1:])
	if  extension == 'jpeg'or extension == 'jpg':
		return static_file(filename, root='C:\\Users\\Administrator\\Desktop\\ShapeLearnerPackage\\ShockGraphAPI\\static', mimetype='image/jpg')
	elif extension == 'png':
		return static_file(filename, root='C:\\Users\\Administrator\\Desktop\\ShapeLearnerPackage\\ShockGraphAPI\\static', mimetype='image/png')
	elif extension == 'css':
		return static_file(filename, root='C:\\Users\\Administrator\\Desktop\\ShapeLearnerPackage\\ShockGraphAPI\\static', mimetype='text/css')
	elif extension == 'js':
		return static_file(filename, root='C:\\Users\\Administrator\\Desktop\\ShapeLearnerPackage\\ShockGraphAPI\\static', mimetype='text/javascript')  
		
@route('/')
def homepage():
	global partID
	shockThread = ShockGrThread(dll, lst[partID][0], lst[partID][1])
	shockThread.setName('shockThread1')
	shockThread.start()
	partID = partID + 1
	return static_file("index.html", root='C:\\Users\\Administrator\\Desktop\\ShapeLearnerPackage\\\ShockGraphAPI\\html')
	
################################# Server Initialization #####################################
config = loadConf()
	
timeThread = TimingThread(3)
timeThread.setName('timeThread')

timeThread.start()
time.sleep(1)

dll=ctypes.CDLL('ShapeLearnerDLL.dll')
dll.openDataBase.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_uint, ctypes.c_char_p]
dll.signBinaryImage.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
dll.openDataBase('postgres', 'postgres', 'postgres', '54.77.188.25', 10101, 'structure.sql')

timeThread.join()

lst = [['AmortisseurA00.ppm', 'Amortisseur'],['AmortisseurA10.ppm', 'Amortisseur']]

partID = 0

run(server='paste', host='0.0.0.0', port=8000)