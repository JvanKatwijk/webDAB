/**
  @file
  @author Stefan Frings
*/

#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class	RadioInterface;
/**
  The request mapper dispatches incoming HTTP requests to controller classes
  depending on the requested path.
*/

class RequestMapper : public HttpRequestHandler {
Q_OBJECT
Q_DISABLE_COPY(RequestMapper)
public:
	RequestMapper	(RadioInterface *, QObject* parent=0);
	~RequestMapper	();

//	Dispatch incoming HTTP requests to different controllers
//	depending on the URL.
//	@param request The received HTTP request
//	@param response Must be used to return the response
	void service (HttpRequest& request, HttpResponse& response);
private:
	RadioInterface	*theRadio;
};

#endif // REQUESTMAPPER_H
