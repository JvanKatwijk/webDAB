

#include	<stdio.h>
#include	<stdint.h>
#include	<stdlib.h>
#include	"radio-interface.h"
#include	"html-writer.h"
#include	"device-handler.h"

	radioInterface::radioInterface		(deviceHandler *theDevice,
	                                         QSettings *dabSettings,
	                                            QObject *p):
	                                              dabHandler (this,
	                                                          theDevice),
	                                              pcmBuffer (32768) {
	this	-> theDevice	= theDevice;
	this	-> dabSettings	= dabSettings;
	this	-> theText	= new html_writer ("./test.html");
	try {
	   this	-> soundOut	= new audioSink (5);
	} catch (int e) {
	   fprintf (stderr, "failing opening out stream\n");
	   exit (3);
	}

	dabSettings	-> beginGroup ("dabData");
	ifGain		= dabSettings -> value ("ifGain", 50). toInt ();
	audioGain	= dabSettings -> value ("audioGain", 50). toInt ();
	dabSettings	-> endGroup ();
	theDevice       -> setGain (ifGain);
        theText         -> set_deviceGain (ifGain);
	soundOut        -> setGain (audioGain);
        theText         -> set_soundGain (audioGain);
}
	radioInterface::~radioInterface		() {
	dabSettings	-> beginGroup ("dabData");
	dabSettings	-> setValue ("ifGain", ifGain);
	dabSettings	-> setValue ("audioGain", audioGain);
	dabSettings	-> endGroup ();
//	stop sequence
}

void	radioInterface::service			(HttpRequest& request,
	                                         HttpResponse& response) {
QByteArray path = request. getPath();

//	fprintf (stderr, "Request %s\n", path. data ());
	if (path. startsWith ("/set_channel")) {
	   selectChannel (
	                  QString (request.
	                           getParameter ("Channels"). data ()),
	                                  response);
	}
	else
	if (path. startsWith ("/select-service")) {
	   selectService (
	                  QString (request.
	                           getParameter ("radio"). data ()),
	                                  response);
	}
	else
	if (path. startsWith ("/set_ifgain")) {
	   QString par = QString (request. getParameter ("ifgain"));
//	   fprintf (stderr, "parameter for set_ifgain %s\n",
//	                          par. toLatin1 (). data ());
	   update_deviceGain (par. toInt (), response);
	}
	else
	if (path. startsWith ("/set_autogain")) {
	      QString par = QString (request. getParameter ("autogain"));
//	      fprintf (stderr, "parameter for set_autogain %s\n",
//	               par. toLatin1 (). data ());
           update_deviceAGC (
                           request. getParameter ("autogain"),
                                          response);
        }
        else
	if (path. startsWith ("/set_audiogain")) {
	   QString par = QString (request. getParameter ("audiogain"));
//	   fprintf (stderr, "parameter for set_audiogain %s\n",
//	                             par. toLatin1 (). data ());
           update_soundGain (par. toInt (), response);
        }
        else
	  theText ->  update (response);

	qDebug ("RequestMapper: finished request");
}

int	getServiceNr (QStringList services, const QString &s) {
	for (int i = 0; services. length (); i ++)
	   if (services. at (i) == s)
	      return i;
	return 0;
}

void	radioInterface::selectService		(const QString &s,
	                                         HttpResponse& response) {
audiodata ad;
int serviceNr	= getServiceNr (Services, s);
	theText	-> update_dynamicLabel ("you selected service " + s);
	theText	-> update_selectedService (s, serviceNr);
	dabHandler. stopService ();
	dabHandler. dataforAudioService (s, &ad);
	if (!ad. defined)
	   return;
	dabHandler. set_audioChannel (&ad, &pcmBuffer);
	// do the selection
	theText -> update (response);
}

void	radioInterface::selectChannel		(const QString &s,
	                                         HttpResponse& response) {
int frequency	= theBand. Frequency (s);
int	channelNr	= theBand. channelNr (s);
	dabHandler. stopService ();
	Services. clear ();
	theText		-> update_serviceList (Services);
	theText		-> update_ensembleName	("");
	theText		-> update_dynamicLabel ("you selected channel " + s);
	theText		-> update_selectedChannel (s, channelNr);
	theDevice	-> stopReader ();
	dabHandler. stop ();
	theDevice	-> restartReader (frequency);
	dabHandler. start (frequency);
	theText		-> update (response);
}

void	radioInterface::update_deviceGain	(int v,
	                                         HttpResponse& response) {
	ifGain		= v;
	theDevice	-> setGain (v);
	theText		-> set_deviceGain (v);
	theText		-> update (response);
	dabSettings	-> beginGroup ("dabData");
	dabSettings	-> setValue ("ifGain", v);
	dabSettings	-> endGroup ();
}

void	radioInterface::update_deviceAGC	(const QString &s,
	                                         HttpResponse& response) {
	if (!theDevice -> has_agc ()) 
	   return;
	theDevice	-> set_agc ("on");
	theText		-> set_deviceAGC (s);
	theText		-> update (response);
}

void	radioInterface::update_soundGain	(int g,
	                                         HttpResponse &response) {
	audioGain	= g;
	soundOut	-> setGain (g);
	theText		-> set_soundGain (g);
	theText		-> update (response);
	dabSettings	-> beginGroup ("dabData");
	dabSettings	-> setValue ("audioGain", g);
	dabSettings	-> endGroup ();
}

void	radioInterface::timedUpdate		(HttpResponse &response) {
	theText -> update (response);
}
//
//	The following functions are slots and called from the radio
void	radioInterface::set_dynamicLabel	(const QString &s) {
	theText	-> update_dynamicLabel (s);
}

void	radioInterface::showLabel (QString s) {
	theText	-> update_dynamicLabel (s);
}


void	radioInterface::nameofEnsemble	(int id, const QString &s) {
	(void)id;
	theText	-> update_ensembleName	(s);
	fprintf (stderr, "found ensemble %s\n", s. toLatin1 (). data ());
}

void	radioInterface::addtoEnsemble	(const QString &s, int id) {
	(void)id;
	Services << s;
	theText	-> update_serviceList (Services);
	fprintf (stderr, "added service %s\n", s. toLatin1 (). data ());
}

void	radioInterface::nrServices      (int nrServices) {
	(void)nrServices;
}

void	radioInterface::newAudio	(int amount, int rate) {
int16_t vec [amount];

	while (pcmBuffer. GetRingBufferReadAvailable() > amount) {
	   pcmBuffer. getDataFromBuffer (vec, amount);
	      soundOut -> audioOut (vec, amount, rate);
        }
}
//
//	dummies
void	radioInterface::setStereo	(bool b) {
	(void)b;
}

void	radioInterface::newFrame	(int n) {
	(void)n;
}

