#include"createFile.h"

void CreateAllFile(Connection *conn,string userName)
{
      CreateAnalog(conn);

      CreateState(conn);

      CreateAlarmMap(conn);

      CreateStateMap(conn);

      CreateIdNumberMap(conn,userName);

     CreateCarGroup(conn,userName);
}

void CreateAnalog(Connection *conn)
{
    
    
    //创建一个xml文档对象
    TiXmlDocument *myDocument=new TiXmlDocument();

    //创建文档声明
    TiXmlDeclaration * declaration=new TiXmlDeclaration("1.0","UTF-8","no");
    myDocument->LinkEndChild(declaration);

    TiXmlElement *RootElement=new TiXmlElement("模拟量");
    myDocument->LinkEndChild(RootElement);
    
    ResolutionDBOperate(conn,"T_RESOLUTIONPACKAGE",RootElement);
    ResolutionDBOperate(conn,"T_ResolutionAnalog",RootElement);
    ResolutionDBOperate(conn,"T_ResolutionAlarm",RootElement);
    ResolutionDBOperate(conn,"T_ResolutionElectricAnalog",RootElement);
    
    myDocument->SaveFile("ordinary.xml");
}

void CreateState(Connection *conn)
{
    //创建一个xml文档对象
    TiXmlDocument *myDocument=new TiXmlDocument();

    //创建文档声明
    TiXmlDeclaration * declaration=new TiXmlDeclaration("1.0","UTF-8","no");
    myDocument->LinkEndChild(declaration);

    TiXmlElement *RootElement=new TiXmlElement("状态量");
    myDocument->LinkEndChild(RootElement);
    
    ResolutionDBOperate(conn,"T_RESOLUTIONPACKAGE",RootElement);
    ResolutionDBOperate(conn,"T_ResolutionState1",RootElement);
    ResolutionDBOperate(conn,"T_ResolutionState2",RootElement);
    ResolutionDBOperate(conn,"T_ResolutionState3",RootElement);
    ResolutionDBOperate(conn,"T_ResolutionElectricState",RootElement);
    
    myDocument->SaveFile("Status.xml");
}

void CreateAlarmMap(Connection *conn)
{
    //创建一个xml文档对象
    TiXmlDocument *myDocument=new TiXmlDocument();

    //创建文档声明
    TiXmlDeclaration * declaration=new TiXmlDeclaration("1.0","UTF-8","no");
    myDocument->LinkEndChild(declaration);

    TiXmlElement *RootElement=new TiXmlElement("AlarmMap");
    myDocument->LinkEndChild(RootElement);
    
    MapDBOperate(conn,"T_VehicleAlarmType",RootElement);
    
    myDocument->SaveFile("AlarmMap.xml");
}

void CreateIdNumberMap(Connection*conn,string clientID)
{
    //创建一个xml文档对象
    TiXmlDocument *myDocument=new TiXmlDocument();

    //创建文档声明
    TiXmlDeclaration * declaration=new TiXmlDeclaration("1.0","UTF-8","no");
    myDocument->LinkEndChild(declaration);

    TiXmlElement *RootElement=new TiXmlElement("CarIDMap");
    myDocument->LinkEndChild(RootElement);

    
    IdNumberMapOperate(conn,"T_vehiclenfo",clientID,RootElement);
    
    myDocument->SaveFile("IdNumberMap.xml");
}

void IdNumberMapOperate(Connection*conn,string table,string userName,TiXmlElement *RootElement)
{
    Statement *stmt=conn->createStatement("select T_vehicleinfo.deviceid,T_vehicleinfo.vehicleNum from T_vehicleinfo ,T_user where T_vehicleinfo.clientid=T_user.clientid and T_user.username='"+userName+"'");
    ResultSet *rs=stmt->executeQuery();
    cout<<table<<" query success!"<<endl;

    while(rs->next()==true)
    {
        string carId=rs->getString(1);
        string carNum=rs->getString(2);
     

        TiXmlElement *elem=new TiXmlElement("Map");
        RootElement->LinkEndChild(elem);

        elem->SetAttribute("Data",carId);
        elem->SetAttribute("Mean",carNum);
        
    }
    cout<<table<<" create!"<<endl;

    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);
}

void CreateStateMap(Connection*conn)
{
    //创建一个xml文档对象
    TiXmlDocument *myDocument=new TiXmlDocument();

    //创建文档声明
    TiXmlDeclaration * declaration=new TiXmlDeclaration("1.0","UTF-8","no");
    myDocument->LinkEndChild(declaration);

    TiXmlElement *RootElement=new TiXmlElement("StateMap");
    myDocument->LinkEndChild(RootElement);
    
    StateMapDBOperate(conn,"T_ResolutionStateContent",RootElement);
    
    myDocument->SaveFile("StateMap.xml");
}

void CreateCarGroup(Connection*conn,string userName)
{
    //创建文件
    FILE* file=fopen("CarGroup.txt","w+");
    char content[1024];
    
    bzero(content,sizeof(content));
    //snprintf(content,sizeof(content),"哈尔滨公交\r\n");
    Statement *stmtCmy=conn->createStatement("select clientname from t_user a,t_client b  where a.clientid=b.clientid and a.username='"+userName+"'");
    ResultSet *rsCmy=stmtCmy->executeQuery();
    if(rsCmy->next()==true)
    {
        string cmyName=rsCmy->getString(1)+"\r\n";
        cout<<cmyName<<endl;
        snprintf(content,sizeof(content),"%s",cmyName.c_str());
    }
    fwrite(content,sizeof(char),strlen(content),file);
    stmtCmy->closeResultSet(rsCmy);
    conn->terminateStatement(stmtCmy);

    
    Statement *stmt=conn->createStatement("select distinct t_vehiclegroup.groupname from t_vehicleInfo,t_vehiclegroup,t_user where t_vehicleinfo.clientid=t_user.clientid and t_vehicleinfo.groupid=t_vehiclegroup.groupid and t_user.username='"+userName+"'");
    ResultSet *rs=stmt->executeQuery();
    
    while(rs->next()==true)
    {
        string groupName=rs->getString(1);
        //cout<<groupName<<"---------"<<endl;
        bzero(content,sizeof(content));
        snprintf(content,sizeof(content),"%s",groupName.c_str());
        fwrite(content,sizeof(char),strlen(content),file);
        

        Statement *stmtCar=conn->createStatement("select t_vehicleinfo.vehiclenum from t_vehicleInfo,t_vehiclegroup,t_user where t_vehicleinfo.clientid=t_user.clientid and t_vehicleinfo.groupid=t_vehiclegroup.groupid and t_user.username='"+userName+"'"+" and t_vehiclegroup.groupname='"+groupName+"'");

        ResultSet *rsCar=stmtCar->executeQuery();

        while(rsCar->next()==true)
        {
            string carNum=rsCar->getString(1);
            //cout<<carNum<<endl;

            bzero(content,sizeof(content));
            snprintf(content,sizeof(content),"%s",(" "+carNum).c_str());
            fwrite(content,sizeof(char),strlen(content),file);
        }

        bzero(content,sizeof(content));
        snprintf(content,sizeof(content),"%s","\r\n");
        fwrite(content,sizeof(char),strlen(content),file);
        
        stmt->closeResultSet(rsCar);
        conn->terminateStatement(stmtCar);
        
    }
    cout<<"CarGroup create!"<<endl;

    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);

    
    fclose(file);
}

void ResolutionDBOperate (Connection*conn,string table,TiXmlElement *RootElement)
{
    Statement *stmt=conn->createStatement("select dataname,startpoision,infolen from "+table);
    ResultSet *rs=stmt->executeQuery();
    cout<<table<<" query success!"<<endl;

    while(rs->next()==true)
    {
        string style=rs->getString(1);
        string start=rs->getString(2);
        string len=rs->getString(3);

        TiXmlElement *elem=new TiXmlElement("DataName");
        RootElement->LinkEndChild(elem);

        elem->SetAttribute("Style",style);
        elem->SetAttribute("StartPoision",start);
        elem->SetAttribute("InfoLen",len);
    }
    cout<<table<<" create!"<<endl;

    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);
}

void MapDBOperate(Connection*conn,string table,TiXmlElement*RootElement)
{
     Statement *stmt=conn->createStatement("select AlarmTypeID,AlarmTypeName from "+table);
    ResultSet *rs=stmt->executeQuery();
    cout<<table<<" query success!"<<endl;

    while(rs->next()==true)
    {
        string data=rs->getString(1);
        string mean=rs->getString(2);
        

        TiXmlElement *elem=new TiXmlElement("Map");
        RootElement->LinkEndChild(elem);

        elem->SetAttribute("Data",data);
        elem->SetAttribute("Mean",mean);
        
    }
    cout<<table<<" create!"<<endl;

    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);
}

void StateMapDBOperate(Connection*conn,string table,TiXmlElement*RootElement)
{
     Statement *stmt=conn->createStatement("select * from "+table);
    ResultSet *rs=stmt->executeQuery();
    cout<<table<<" query success!"<<endl;

    while(rs->next()==true)
    {
        string stateName=rs->getString(1);
        string stateIllustration=rs->getString(2);
        string stateData=rs->getString(3);
        string stateMeaning=rs->getString(4);
        

        TiXmlElement *elem=new TiXmlElement("Map");
        RootElement->LinkEndChild(elem);

        elem->SetAttribute("StateName",stateName);
        elem->SetAttribute("StateIllustration",stateIllustration);
        elem->SetAttribute("StateData",stateData);
        elem->SetAttribute("stateMeaning",stateMeaning);
        
    }
    cout<<table<<" create!"<<endl;

    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);
}
