# APIServer

APIServer是一款基于libevent的简易WebAPI服务，用于处理http的post与get信令，多平台支持。

APIServer_mongoose 

#### 1.How to build

Windows ：直接运行build_win32目录下的sln文件，vc90 for vs2008, vc120 for vs2013.

linux : 运行make

#### 2.Interface

使用include下导出的头文件IAPIServer.h，rapidjson为只需依赖头文件的Json解析库（用于解析或组合json字符串）.


```
class API IAPIServerNotify
{
public:
    virtual ~IAPIServerNotify(){};

    //当注册的API触发时
    //handler: http request handler
    //api: web path
    //param: json data or string data
    virtual bool OnAPItrigger(void* handler, const char* api, const char* param) = 0;
};

class API IAPIServer
{
public:
    virtual ~IAPIServer(){}

    //Start Server
    //设置ip地址以及端口号，一般ip设置为"0.0.0.0"
    //访问时使用http://ip:port/xxx
    virtual int32_t StartServer(const char* ip,int32_t port) = 0;

    virtual void StopServer() = 0;
    
    //注册api地址，只有在这里注册过的地址才会触发OnAPItrigger消息.主要是为了防止不必要的事件响应影响api的调用
    virtual void RegAPIpath(const char* path) = 0;

	//用于对对应http请求进行回复
	virtual void AsyncReply(void* handler,const char* reply) = 0;
	
    //创建IAPIServer实例
    static IAPIServer* Create(IAPIServerNotify* pNotify);
};
```
#### 3.Quick Start

step1.创建一个继承IAPIServerNotify的类型，用于接收api触发后的消息

step2.创建IAPIServer实例，并启动服务，服务将在另外的线程中接收api响应，此时你需要自己对程序的主线程进行阻塞

step3.调用RegAPIpath注册你需要响应的web地址.只有你才注册的地址才会收到消息响应.

Demo如下

```
class CAPITransfer
	: public IAPIServerNotify
{
public:
	CAPITransfer(void);
	~CAPITransfer(void);
	virtual bool OnAPItrigger(void* handler,const char* api, const char* param);
	void Start(const char* ip,int port);
	void Stop();
	static CAPITransfer& Instance();
private:
	IAPIServer* m_pServer;
};

CAPITransfer::CAPITransfer(void)
:m_pServer(NULL)
{
}

CAPITransfer::~CAPITransfer(void)
{
}
void CAPITransfer::Start(const char* ip,int port)
{
    if (!m_pServer)
    {
	m_pServer = IAPIServer::Create(this);
	m_pServer->RegAPIpath("/TRServer/Request");
	m_pServer->RegAPIpath("/TRServer/CloseTRTask");
	int ret = m_pServer->StartServer(ip,port);
    }
}

void CAPITransfer::Stop()
{
    if (m_pServer)
    {
    	m_pServer->StopServer();
    	delete m_pServer;
    	m_pServer = NULL;
    }
}

CAPITransfer& CAPITransfer::Instance()
{
    static CAPITransfer transfer;
    return transfer;
}

bool CAPITransfer::OnAPItrigger(void* handler,const char* api, const char* param)
{
    int datalen = strlen(param);
    std::string s(param);
    //
    if (strcmp(api,"/TRServer/Request") == 0)
    {
	    //处理你自己的事务逻辑
    }
    else if(strcmp(api,"/TRServer/CloseTRTask") == 0)
    {
    	//CloseTransecodeTask(param);
    }
	
    //通过handler回复http请求
	m_pServer->AsyncReply(handler,"Running Well!");
	
    return true;
}
```
