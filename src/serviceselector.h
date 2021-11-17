
#ifndef __SERVICE_SELECTOR_H
#define __SERVICE_SELECTOR_H

#include	<QString>
#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

/**
*/

class serviceSelector : public HttpRequestHandler {
    Q_OBJECT
public:
    /** Constructor */
    serviceSelector	();
    /** Generates the response */
    void selectService (const QString & channel, HttpResponse& response);
};

#endif 

