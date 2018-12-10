#ifndef _MOHTTP_H_
#define _MOHTTP_H_
#endif

#include "mongoose.h"
#include "IAPIServer.h"

#include <string>

#include <vector>
#include <set>
#include "XThreadBase.h"
using namespace AVUtils;


using namespace apiserver;

class Mohttp
	: public IAPIServer
	, public XThreadBase
{
public:
	Mohttp(IAPIServerNotify* notify);
	~Mohttp();

	virtual int32_t StartServer(const char* ip, int32_t port);

	virtual void StopServer();

	virtual void RegAPIpath(const char* path);

	virtual void AsyncReply(void* handler, const char* reply);

protected:
	static void HandleRequst(struct mg_connection* nc, int ev, void *ev_data,void *user_data);

	void handleReq(struct mg_connection* nc, int ev, void *ev_data);

	virtual void ThreadProcMain(void);
private:
	IAPIServerNotify * m_pNotify;	

	std::set<std::string> m_setPath;

	mg_mgr m_mgr;

	bool m_bRunning;
};

