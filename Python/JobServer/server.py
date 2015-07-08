################################# Import Libraries ################################
import os.path
import sys

sys.path.append('../Config/')
dllsPath = os.path.dirname(os.path.realpath(__file__))+'\dlls'
os.environ['PATH'] = dllsPath + ';' + os.environ['PATH']
from loadConf import loadDBConf, loadAPIConf

import api

#######################################################################################
#######################################################################################
#######################################################################################
 

configDB = loadDBConf()
configAPI = loadAPIConf()

jobServerDB = {'ip':configDB['jobDB']['ip'], 'port':configDB['jobDB']['port'], 'dbUser':configDB['jobDB']['dbUser'], 'dbPass':configDB['jobDB']['dbPass'], 'dbName':configDB['jobDB']['dbName']}
jobServerAPI = {'ip':configAPI['jobServer']['ip'], 'port':configAPI['jobServer']['port'], 'local':configAPI['jobServer']['local']}

api = api.API(jobServerDB, jobServerAPI['port'], jobServerAPI['local'])
api.start()