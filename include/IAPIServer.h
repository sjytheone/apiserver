#ifndef _IAPISERVER_H_
#define _IAPISERVER_H_

#ifdef _WIN32
#ifdef _USEDLL
#ifdef API_EXPORT
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif 
#else
#define API
#endif
#else //_WIN32
#define API __attribute__ ((visibility("default")))
#endif

//#define API

#if ( defined(_MSC_VER) && _MSC_VER < 1600)
	typedef signed char        int8_t;
	typedef short              int16_t;
	typedef int                int32_t;
	typedef long long          int64_t;
	typedef unsigned char      uint8_t;
	typedef unsigned short     uint16_t;
	typedef unsigned int       uint32_t;
	typedef unsigned long long uint64_t;
#else
#include <stdint.h>
#endif // ( defined(_MSC_VER) && _MSC_VER > 1600)

namespace apiserver{

	class API IAPIServerNotify
	{
	public:
		virtual ~IAPIServerNotify(){};

		//当注册的API触发时
		//handler: http request handler
		//api: webapi path
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

		//回复时需要使用handler
		virtual void AsyncReply(void* handler,const char* reply) = 0;

		//创建IAPIServer实例
		static IAPIServer* Create(IAPIServerNotify* pNotify);
	};
}
#endif