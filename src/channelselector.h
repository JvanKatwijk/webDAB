
#ifndef __CHANNEL_SELECTOR_H
#define __CHANNEL_SELECTOR_H

#include	<QString>
#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

/**
*/


class channelSelector : public HttpRequestHandler {
    Q_OBJECT
public:
    /** Constructor */
    channelSelector	();
    /** Generates the response */
    void selectChannel (const QString & channel, HttpResponse& response);
};

#endif // FORMCONTROLLER_H

