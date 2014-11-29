#include "StdAfx.h"
#include "SQLUtils.h"


SQLUtils::SQLUtils(string serverName,string userName,string password,string databaseName,int port)
{
	this->serverName = serverName;
	this->userName = userName;
	this->password = password;
	this->databaseName = databaseName;
	this->port = port;
}


SQLUtils::~SQLUtils(void)
{
}


void SQLUtils::setServerName(string serverName)
{
	this->serverName = serverName;
}


void SQLUtils::setUserName(string userName)
{
	this->userName = userName;
}


void SQLUtils::setPassword(string password)
{
	this->password = password;
}


void SQLUtils::setDatabaseName(string databaseName)
{
	this->databaseName = databaseName;
}


void SQLUtils::setPort(int port)
{
	this->port = port;
}


string SQLUtils::getServerName(void)
{
	return this->serverName;
}


string SQLUtils::getUserName(void)
{
	return this->userName;
}


string SQLUtils::getPassword(void)
{
	return this->password;
}


string SQLUtils::getDatabaseName(void)
{
	return this->databaseName;
}


int SQLUtils::getPort(void)
{
	return this->port;
}
