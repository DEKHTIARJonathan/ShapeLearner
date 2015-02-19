/* ************* Begin file dbManager.cpp ***************************************/
/*
** 2015 February 18
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

#include "allHeaders.h"
using namespace std;

/**
 * \file dbManager.cpp
 * \brief Interface entre le logiciel et la base de données. Elle est la seule à réaliser des accès à la BDD
 * \version	1.0
 * \author DEKHTIAR Jonathan
 */


/* *******************************************************************
**                           Constructers                            *
*********************************************************************/

DatabaseManager::DatabaseManager(const QString &path, const QString &user, const QString &pass) throw(DBException) : database(new QSqlDatabase()), dbpath(path)
{
	//set database driver to QSQLITE avec une connection ayant pour nom "ShapeLearner.shape"
	*database = QSqlDatabase::addDatabase("QSQLITE", "ShapeLearner.shape");

	QFile file(dbpath);

	bool dbIsNew = !file.exists();

	database->setDatabaseName(dbpath);

	//can be removed
	database->setHostName("localhost");
	database->setUserName(user);
	database->setPassword(pass);

	if(!database->open())
	{
		throw DBException("INITIALISATION Database", "Can not open database. Path = "+file.fileName());
	}
	else
	{
		if (dbIsNew)
			initDB();
		else
		{
			/* ******* AFFICHE TOUTES LES TABLES DE LA DB *********
			 ********
			 *******
			 ******
			 *****
			 ****
			 ***
			 **
			 *
			QStringList list = database->tables(QSql::Tables);
			QStringList::Iterator it = list.begin();
			while( it != list.end() )
			{
				cout << "Table: " << it->toStdString() << "\n";
				++it;
			}
			*/
		}
		database->exec("PRAGMA foreign_keys=ON;");
	}
}

/* *******************************************************************
*                              getters                               *
 ********************************************************************/

const QString DatabaseManager::getpath() const
{
	return dbpath;
}

/* *******************************************************************
*                            DB Requests                             *
 ********************************************************************/

bool DatabaseManager::query(const QString &query) const
{
	QSqlQuery request(*database);

	bool result = request.exec(query);

	if (result)
		return true;
	else
		throw DBException(query, request.lastError().databaseText());
}

bool DatabaseManager::initDB()
{
	QString qry[6];
	
	qry[0] = "CREATE TABLE `GraphClass` (`idGraphClass`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `graphClassName`	varchar("+QString::number(constants::SIZE_MAX_GRAPH_NAME)+") NOT NULL UNIQUE)";
	//qry[1] = "INSERT INTO `GraphClass` ( `graphClassName` ) VALUES ( 'ShockGraph' );";
	qry[1] = "CREATE TABLE `ObjectClass` (`idObjectClass` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `objectClassName`	varchar(255) NOT NULL UNIQUE)";
	qry[2] = "Select 1";
	qry[3] = "Select 1";
	qry[4] = "Select 1";
	qry[5] = "Select 1";
	
	/*
	qry[0] = "create table Note (id integer , title varchar(255), typeNote varchar(255), trashed  BOOL DEFAULT '0' NOT NULL, primary key(id))";
	qry[1] = "create table Article (id integer, txt text, primary key(id), FOREIGN KEY(id) REFERENCES Note(id) ON DELETE CASCADE)";
	qry[2] = "create table Document (id integer, primary key(id), FOREIGN KEY(id) REFERENCES Note(id) ON DELETE CASCADE)";
	qry[3] = "create table Multimedia (id integer, description TEXT, path varchar(255), primary key(id), FOREIGN KEY(id) REFERENCES Note(id) ON DELETE CASCADE)";
	qry[4] = "create table AssocDoc (docMaster integer, note integer, primary key(docMaster, note), FOREIGN KEY(docMaster) REFERENCES Document(id) ON DELETE CASCADE, FOREIGN KEY(note) REFERENCES Note(id) ON DELETE CASCADE)";
	qry[5] = "create table AssocTag (id integer, name varchar(255), primary key(id, name), FOREIGN KEY(id) REFERENCES Note(id) ON DELETE CASCADE)";
	*/
	bool b = true;
	
	for (int i = 0; i<6; ++i)
	{
		if (!query(qry[i]))
			b &= false;
	}

	if (!b)
	{
		QFile::remove(dbpath);
		exit(0);
	}
	return b;
}

unsigned int DatabaseManager::getLastID() const
{
	QSqlQuery query(*database);
	QString sql = "SELECT last_insert_rowid()";

	if(!query.exec(sql))
		throw DBException(sql, query.lastError().databaseText());

	query.next();// Only one result no need of the while loop

	return query.value(0).toUInt();
}

/* *******************************************************************
*                              Deleters                              *
 ********************************************************************/


bool DatabaseManager::flushDB () const
{
	//return deleteNodes() && deleteDAGs();
	return true;
}

/* *******************************************************************
*                     Escapers / Capitalizers                        *
 ********************************************************************/

QString DatabaseManager::escape(QString s) const
{
	s.replace("'", "''");
	return s;
}

QString DatabaseManager::capitalize(QString str) const
{
	str = str.toLower();
	str[0] = str[0].toUpper();
	return str;
}

/* *******************************************************************
*                            Inserters                               *
 ********************************************************************/


/* *******************************************************************
*                             Updaters                               *
 ********************************************************************/

/* *******************************************************************
*                            Retrievers                              *
 ********************************************************************/

/* *******************************************************************
*                             Fillers                                *
 ********************************************************************/

/* *******************************************************************
*                            Destructor                              *
 ********************************************************************/

DatabaseManager::~DatabaseManager()
{
	QString dbname = database->connectionName();
	database->close();
	delete database;
	database = NULL;
	QSqlDatabase::removeDatabase("ShapeLearner.shape");
}

/* *******************************************************************
*                            Singleton                               *
 ********************************************************************/

DatabaseManager* DatabaseManager::s_inst = NULL;

DatabaseManager& DatabaseManager::getInstance(QString path, QString user, QString pass){
	if( s_inst == NULL )
		s_inst = new DatabaseManager(path,user,pass);
	return (*s_inst);
}

void DatabaseManager::destroy()
{
	if( s_inst != NULL ){
		delete s_inst;
		s_inst = NULL;
	}
}