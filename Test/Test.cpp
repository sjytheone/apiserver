// Test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "IAPIServer.h"
#include <iostream>
using namespace apiserver;

#define TRSAPI_REQUEST "/TRServer/Request"
#define TRSAPI_SETRECVMODE "/TRServer/SetRecvMode"
#define TRSAPI_CLOSETRTASK "/TRServer/CloseTRTask"
#define TRSAPI_HEALTH "/TRServer/Health"
#define TRSAPI_TASKINFO "/TRServer/TaskInfo" //获取当前任务个数以及最大任务数
#define TRSAPI_AVAILABLE "/TRServer/Available" //当前任务数小于最大任务数
//[result:1]
#define TRSAPI_CLOSEALLTASK "/TRServer/CloseALLTRTask" //关闭所有转码任务

#define TRSAPI_PORT "10803"

#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/prettywriter.h"
using namespace rapidjson;

class MyNotify
	: public IAPIServerNotify
{
public:
	MyNotify()
		: m_pServer(NULL)
	{

	}
	~MyNotify() {}

	IAPIServer* m_pServer;
	void SetServer(IAPIServer* server)
	{
		m_pServer = server;
	}

	virtual bool OnAPItrigger(void* handler, const char* api, const char* param)
	{
		int32_t x = 0;
		if (strcmp(api, TRSAPI_AVAILABLE) == 0)
		{
			if (m_pServer)
			{
				m_pServer->AsyncReply(handler, "Server Still Running!");
			}
		}
		else if (strcmp(api, TRSAPI_TASKINFO) == 0)
		{
			StringBuffer buffer;
			Writer<StringBuffer> writer(buffer);
			writer.StartObject();
			writer.String("USEDTASKS");
			writer.Int(15);
			writer.String("MAXTASK");
			writer.Int(20);
			writer.EndObject();

			StringBuffer data;
			PrettyWriter<StringBuffer> writer1(data);
			writer1.StartObject();
			writer1.String("APINAME");
			writer1.String(TRSAPI_TASKINFO);
			writer1.String("RESULT");
			writer1.String(buffer.GetString());
			writer1.EndObject();

			if (m_pServer)
			{
				m_pServer->AsyncReply(handler, data.GetString());
			}
		}
		return true;
	}

private:

};

int main()
{
	MyNotify notify;

	IAPIServer* myServer = IAPIServer::Create(&notify);
	if (myServer)
	{
		notify.SetServer(myServer);
		myServer->StartServer("0.0.0.0", 3366);

		myServer->RegAPIpath(TRSAPI_SETRECVMODE);
		myServer->RegAPIpath(TRSAPI_REQUEST);
		myServer->RegAPIpath(TRSAPI_HEALTH);
		myServer->RegAPIpath(TRSAPI_AVAILABLE);
		myServer->RegAPIpath(TRSAPI_TASKINFO);
	}

	std::cin.get();

	myServer->StopServer();
		
	delete myServer;
    return 0;
}

