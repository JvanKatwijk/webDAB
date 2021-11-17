/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include "global.h"
#include "requestmapper.h"
#include	"radio-interface.h"

	RequestMapper::RequestMapper(QObject* parent)
	                                    :HttpRequestHandler(parent) {
	qDebug("RequestMapper: created");
}

	RequestMapper::~RequestMapper() {
	qDebug ("RequestMapper: deleted");
}

void RequestMapper::service (HttpRequest& request,
	                              HttpResponse& response) {
QByteArray path = request. getPath();

	qDebug ("RequestMapper: path = %s", path. data());

	fprintf (stderr, "Request %s\n", path. data ());
	if (path. startsWith ("/set_channel")) {
	   fprintf (stderr, "select een channel\n");
	   fprintf (stderr, "Parameter %s\n",
	                      request. getParameter ("Channels"). data ());
	   the_radio -> selectChannel (
	                  QString (request. getParameter ("Channels"). data ()), response);
	}
	else
	if (path. startsWith ("/select-service")) {
	   fprintf (stderr, "select een service\n");
	   fprintf (stderr, "Parameter %s\n",
	                       request. getParameter ("radio"). data ());
	   the_radio -> selectService (
	                  QString (request. getParameter ("radio"). data ()),
	                                  response);
	}
	else
	if (path. startWith ("/device_gain")) {
	   fprintf (stderr, "set the device gain\n");
	   fprintf (stderr, "parameter %s\n",
	                       request. getparameter ("device-gain"). data ());
	   theRadio -> update_deviceGain (
	                   request. getParameter ("device-gain"). toInt (),
	                                  response);
	}
	else
	if (path. startWith ("device-agc")) {
	   fprintf (stderr, "set the device agc\n");
           fprintf (stderr, "parameter %s\n",
                               request. getparameter ("device-agc"). data ());
           theRadio -> update_deviceGain (
                           request. getParameter ("device-agc"). toInt (),
                                          response);
        }
        else
	if (path. startWith ("audio-gain")) {
	   fprintf (stderr, "set the audio gain\n");
           fprintf (stderr, "parameter %s\n",
                               request. getparameter ("audio-gain"). data ());
           theRadio -> update_deviceGain (
                           request. getParameter ("audio-gain"). toInt (),
                                          response);
        }
        else
	   theRadio => update (response);

	qDebug ("RequestMapper: finished request");
}


