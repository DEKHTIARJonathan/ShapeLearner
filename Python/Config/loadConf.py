################################# Import Libraries ################################
import os.path
import xml.etree.ElementTree as XML

def loadDBConf(confPath = '../Config/conf.xml'):
	configurations = XML.parse(confPath).getroot()

	servers = dict()

	for serv in configurations.iter('DBserver'):
	
		serverName = serv.attrib['serverName']
		serverPort = serv.attrib['port']
		serverIP = serv.attrib['ip']
		dbUser = serv.attrib['dbUser']
		dbPass = serv.attrib['dbPass']
		dbName = serv.attrib['dbName']
		
		servers[serverName] = {'ip':serverIP, 'port':serverPort, 'dbUser':dbUser, 'dbPass':dbPass, 'dbName':dbName }
		
	return servers
	
def loadAPIConf(confPath = '../Config/conf.xml'):
	configurations = XML.parse(confPath).getroot()

	servers = dict()

	for serv in configurations.iter('APIserver'):
	
		serverName = serv.attrib['serverName']
		serverPort = serv.attrib['port']
		serverIP = serv.attrib['ip']
		serverLocal = serv.attrib['local']
		
		servers[serverName] = {'ip':serverIP, 'port':serverPort, 'local':str2bool(serverLocal)}
		
	return servers
	
def str2bool(v):
  return v.lower() == "true"