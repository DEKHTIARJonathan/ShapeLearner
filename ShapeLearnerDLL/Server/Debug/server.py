import ctypes
from threading import Thread
import atexit

def cleanEnvironment():
	# Wait for everything to finish
	dll.waitBeforeClosing()
	timeThread.join()
 
class TimingThread(Thread):
 
	def __init__(self, _timer):
		''' Constructor. '''
		Thread.__init__(self)
		self.__timingDLL = ctypes.CDLL('timeTrickDLL-d.dll')
		self.__timingDLL.changeTime.argtypes = [ctypes.c_uint]
		self.timer = _timer
 
	def run(self):
		self.__timingDLL.changeTime(self.timer)
 
 
# Run following code when the program starts
if __name__ == '__main__':
	atexit.register(cleanEnvironment)
	
	timeThread = TimingThread(3)
	timeThread.setName('timeThread')
	# Start running the threads!
	timeThread.start()
	
	
	dll=ctypes.CDLL('ShapeLearnerDLL-d.dll')
	dll.openDataBase.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_uint, ctypes.c_char_p]
	dll.openDataBase('postgres', 'postgres', 'postgres', '54.77.188.25', 10101, 'structure.sql')
	#dll.openDataBase('postgres', 'postgres', 'postgres', '54.77.188.25', 10101, '')
	
	
	dll.signBinaryImage.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
	dll.signBinaryImage('AmortisseurA00.ppm', 'Amortisseur');
	dll.signBinaryImage('AmortisseurA10.ppm', 'Amortisseur');
	
	print('Main Terminating...')