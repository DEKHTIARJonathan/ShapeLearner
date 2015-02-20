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

/**
 * \file dbManager.cpp
 * \brief Interface entre le logiciel et la base de données. Elle est la seule à réaliser des accès à la BDD
 * \version	1.0
 * \author DEKHTIAR Jonathan
 */

#include "allHeaders.h"
using namespace std;



/* *******************************************************************
**                           Constructers                            *
*********************************************************************/

DatabaseManager::DatabaseManager(const QString &dbName, const QString &dbPath, const QString &user, const QString &pass, const QString &hostname) throw(DBException) : database(new QSqlDatabase()), dbPath(dbPath)
{
	/*
	Driver availables:
		QDB2		IBM DB2
		QIBASE		Borland InterBase Driver
		QMYSQL		MySQL Driver
		QOCI		Oracle Call Interface Driver
		QODBC		ODBC Driver (includes Microsoft SQL Server)
		QPSQL		PostgreSQL Driver
		QSQLITE		SQLite version 3 or above
		QSQLITE2	SQLite version 2
		QTDS		Sybase Adaptive Server
	*/

	//set database driver to QSQLITE avec une connection ayant pour nom dbName
	QString fullpath = dbPath+QDir::separator()+dbName;

	*database = QSqlDatabase::addDatabase("QSQLITE", dbName);

	QFile file(fullpath);

	bool dbIsNew = !file.exists();

	database->setDatabaseName(fullpath);

	//can be removed
	database->setHostName(hostname);
	database->setUserName(user);
	database->setPassword(pass);

	if(!database->open() && !database->isOpen() && !database->isValid() && !file.isOpen())
	{
		throw DBException("INITIALISATION Database", "Can not open database. Path = "+fullpath);
	}
	else
	{
		if (dbIsNew)
			if (!initDB())
				throw DBException("Erreur lors de l'initialisation de la BDD", database->lastError().databaseText());
		else
		{
			/* ******* AFFICHE TOUTES LES TABLES DE LA DB *********
			*********
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
		#ifdef _DEBUG
			cout << "Db linked at the following path : " <<fullpath.toStdString() << endl;
		#endif
		
	}
}

/* *******************************************************************
*                              getters                               *
 ********************************************************************/

const QString DatabaseManager::getPath() const
{
	return dbPath;
}

const QString DatabaseManager::getName() const
{
	return database->databaseName();
}

/* *******************************************************************
*                            DB Requests                             *
 ********************************************************************/

bool DatabaseManager::query(const QString &query) const throw(DBException)
{
	QSqlQuery request(*database);

	database->transaction();

	foreach (QString singleQuery, query.split(';'))
	{
		if (singleQuery.trimmed().isEmpty())
			continue;

		if (!request.exec(singleQuery))
		{
			database->rollback();
			throw DBException(query, request.lastError().databaseText());
			return false;
		}

		request.finish();
	}

	database->commit();

	return true;
}

bool DatabaseManager::initDB()
{
	QString script;
	
	script =	"CREATE TABLE `GraphClass`"
				"("
					"`graphClassName`	VARCHAR("+QString::number(constants::SIZE_MAX_GRAPH_NAME)+") NOT NULL UNIQUE PRIMARY KEY,"
					"`directGraph`		BOOLEAN DEFAULT '0',"
					"`acyclicGraph`		BOOLEAN DEFAULT '0'"
				");"
	
				"CREATE TABLE `ObjectClass`"
				"("
					"`objectClassName`	VARCHAR("+QString::number(constants::SIZE_MAX_CLASS_NAME)+") NOT NULL UNIQUE PRIMARY KEY"
				");"

				"CREATE TABLE `Graphs`"
				"("
					"`idGraph` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
					"`graphClass`	VARCHAR("+QString::number(constants::SIZE_MAX_GRAPH_NAME)+") NOT NULL,"
					"`objectClass`	VARCHAR("+QString::number(constants::SIZE_MAX_CLASS_NAME)+") NOT NULL,"
					"`objectName`	VARCHAR("+QString::number(constants::SIZE_MAX_OBJ_NAME)+") NOT NULL UNIQUE,"
					"`viewNumber`	INTEGER DEFAULT '1' NOT NULL,"
					"FOREIGN KEY(`graphClass`) REFERENCES `GraphClass`(`graphClassName`) ON DELETE CASCADE,"
					"FOREIGN KEY(`objectClass`) REFERENCES `ObjectClass`(`objectClassName`) ON DELETE CASCADE"
				");"

				"CREATE INDEX `index_Graphs_objectName` ON `Graphs`(`objectName`);"
				"CREATE INDEX `index_Graphs_objectClass` ON `Graphs`(`objectClass`);"
				"CREATE INDEX `index_Graphs_graphClass` ON `Graphs`(`graphClass`);"

				"CREATE TABLE `Nodes`"
				"("
					"`idNode`		INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
					"`index`		INTEGER DEFAULT '1' NOT NULL,"
					"`level`		INTEGER DEFAULT '1' NOT NULL,"
					"`mass`			INTEGER DEFAULT '1' NOT NULL,"
					"`type`			INTEGER DEFAULT '1' NOT NULL,"
					"`pointCount`	INTEGER DEFAULT '1' NOT NULL,"
					"`label`		INTEGER DEFAULT '1' NOT NULL,"
					"`refGraph`		INTEGER NOT NULL,"
					"FOREIGN KEY(`refGraph`) REFERENCES `Graphs`(`idGraph`) ON DELETE CASCADE"
				");"

				"CREATE INDEX `index_Node_refGraph` ON `Nodes`(`refGraph`);"

				"CREATE TABLE `Points`"
				"("
					"`idPoint`		INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
					"`xCoord`		REAL DEFAULT '1' NOT NULL,"
					"`yCoord`		REAL DEFAULT '1' NOT NULL,"
					"`radius`		REAL DEFAULT '1' NOT NULL,"
					"`refNode`		INTEGER NOT NULL,"
					"`refGraph`		INTEGER NOT NULL,"
					"FOREIGN KEY(`refNode`) REFERENCES `Nodes`(`idNode`) ON DELETE CASCADE,"
					"FOREIGN KEY(`refGraph`) REFERENCES `Graphs`(`idGraph`) ON DELETE CASCADE"
				");"

				"CREATE INDEX `index_Point_refNode` ON `Points`(`refNode`);"
				"CREATE INDEX `index_Point_refGraph` ON `Points`(`refGraph`);"
				"CREATE INDEX `index_Point_region2D` ON `Points` (`xCoord` ASC, `yCoord` ASC);"

				"CREATE TABLE `Edges`"
				"("
					"`idEdge`		INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
					"`source`		INTEGER NOT NULL,"
					"`target`		INTEGER NOT NULL,"
					"`weight`		INTEGER DEFAULT '1' NOT NULL,"
					"`refGraph`		INTEGER NOT NULL,"
					"FOREIGN KEY(`source`) REFERENCES `Nodes`(`idNode`) ON DELETE CASCADE,"
					"FOREIGN KEY(`target`) REFERENCES `Nodes`(`idNode`) ON DELETE CASCADE,"
					"FOREIGN KEY(`refGraph`) REFERENCES `Graphs`(`idGraph`) ON DELETE CASCADE"
				");"

				"CREATE INDEX `index_Edges_source` ON `Edges`(`source`);"
				"CREATE INDEX `index_Edges_target` ON `Edges`(`target`);"
				"CREATE INDEX `index_Edges_refGraph` ON `Edges`(`refGraph`);"
	
				"INSERT INTO `GraphClass` ( `graphClassName` ) VALUES ( 'ShockGraph' );";

	bool b = true;
	
	if (!query(script))
	{
		QFile::remove(dbPath+QDir::separator()+getName());
		b = false;
	}

	return b;
}

unsigned int DatabaseManager::getLastID() const throw(DBException)
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

// bool DatabaseManager::flushDB () const {return true;}

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
	QSqlDatabase::removeDatabase(getName());
}

/* *******************************************************************
*                            Singleton                               *
 ********************************************************************/

DatabaseManager* DatabaseManager::s_inst = NULL;

DatabaseManager& DatabaseManager::getInstance(QString dbName, QString dbPath, QString user, QString pass, QString hostname){
	if( s_inst == NULL )
		s_inst = new DatabaseManager(dbName, dbPath, user, pass, hostname);
	return (*s_inst);
}

void DatabaseManager::destroy()
{
	if( s_inst != NULL ){
		delete s_inst;
		s_inst = NULL;
	}
}