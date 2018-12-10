#include "Mohttp.h"
#include "mongoose.h"
#include "XLog.h"

IAPIServer* IAPIServer::Create(IAPIServerNotify* pNotify)
{
	return new Mohttp(pNotify);
}

Mohttp::Mohttp(IAPIServerNotify * notify)
	: m_pNotify(notify)
{

}

Mohttp::~Mohttp()
{

}

int32_t Mohttp::StartServer(const char * ip, int32_t port)
{
	char _port[12];
	snprintf(_port, sizeof(_port), "%d", port);
	//init
	mg_mgr_init(&m_mgr, NULL);
	struct mg_connection* nc = mg_bind(&m_mgr, _port, HandleRequst,(void*)this);

	if (NULL == nc)
		return -1;

	mg_set_protocol_http_websocket(nc);

	m_bRunning = true;
	StartThread();

	return 0;
}

void Mohttp::StopServer()
{
	m_bRunning = false;
	WaitForStop();
	mg_mgr_free(&m_mgr);
}

void Mohttp::RegAPIpath(const char * path)
{
	m_setPath.insert(std::string(path));
}

void Mohttp::AsyncReply(void* handler, const char* reply)
{
	//mg_printf((mg_connection*)handler, "HTTP/1.1 200 OK\r\niConnection: close\r\nContent-Type: text/html\r\nContent-Length: %u\r\n\r\n%s\r\n",
	//	(uint32_t)strlen(reply), reply);

	mg_connection* nc = (mg_connection*)handler;
	/* Send headers */
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
	/* Compute the result and send it back as a JSON object */
	mg_printf_http_chunk(nc, "%s", reply);
	mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void Mohttp::HandleRequst(struct mg_connection* nc, int ev, void *ev_data, void *user_data)
{
	//处理Request消息
	if (ev == MG_EV_HTTP_REQUEST)
	{
		Mohttp* p = (Mohttp*)user_data;
		if (p)
		{
			p->handleReq(nc, ev, ev_data);
		}
	}
}

void Mohttp::handleReq(struct mg_connection* nc, int ev, void *ev_data)
{
	http_message* hm = (http_message*)ev_data;
	//method: POST GET etc..
	std::string method(hm->method.p, hm->method.len);
	std::string uri(hm->uri.p, hm->uri.len);

	std::set<std::string>::iterator it = m_setPath.find(uri);
	if (it != m_setPath.end())
	{

		std::string body(hm->body.p, hm->body.len);
		//int32_t bodyLen = hm->body.len + 1;
		//char* data = new char[bodyLen];
		//memset(data, 0, bodyLen);
		//data[bodyLen] = '\0';
		//memcpy(data, hm->body.p, bodyLen - 1);
		if (m_pNotify)
		{
			m_pNotify->OnAPItrigger(nc, uri.c_str(), body.c_str());
		}
		//delete[]data;
	}
	else
	{
		AsyncReply(nc, "Invalid API");
	}
}

void Mohttp::ThreadProcMain(void)
{
	while (m_bRunning)
	{
		// 100 ms
		mg_mgr_poll(&m_mgr, 100);
	}
	XLOGI("CAPIServer --thread exited!");
}
