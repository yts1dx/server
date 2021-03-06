#ifndef CREATEFILE_H
#define CREATEFILE_H

#include<stdio.h>
#include<iostream>
#include"tinyxml.h"
#include"tinystr.h"
#include<occi.h>
#include<string.h>
#include<string>

using namespace std;
using namespace oracle::occi;
void CreateAnalog(Connection *conn);
void ResolutionDBOperate(Connection*conn,string table,TiXmlElement *RootElement);
void CreateState(Connection *conn);
void MapDBOperate(Connection*conn,string table,TiXmlElement*RootElement);
void CreateAlarmMap(Connection *conn);
void CreateStateMap(Connection*conn);
void StateMapDBOperate(Connection*conn,string table,TiXmlElement*RootElement);
void CreateIdNumberMap(Connection*conn,string clientID);
void IdNumberMapOperate(Connection*conn,string table,string clientID,TiXmlElement *RootElement);
void CreateCarGroup(Connection*conn,string userName);
void CreateAllFile(Connection *conn,string userName);

#endif
