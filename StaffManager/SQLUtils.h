#pragma once
#include "mysql.h"
#include "string"

using namespace std;
class SQLUtils
{
public:
	SQLUtils(string serverName,string userName,string password,string databaseName,int port);
	~SQLUtils(void);
private:
	string serverName;
	string userName;
	string password;
	string databaseName;
	int port;


public:
	void setServerName(string serverName);
	void setUserName(string userName);
	void setPassword(string password);
	void setDatabaseName(string databaseName);
	void setPort(int port);

	string getServerName(void);
	string getUserName(void);
	string getPassword(void);
	string getDatabaseName(void);
	int getPort(void);
};

