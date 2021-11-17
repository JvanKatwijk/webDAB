
#ifndef		__RADIO_INTERFACE__
#define		__RADIO_INTERFACE__
#include	<stdio.h>
#include	<stdint.h>
#include	<stdlib.h>
#include	<QSettings>
#include        <QString>
#include        <QStringList>
#include	"httprequest.h"
#include	"httpresponse.h"
#include	"httprequesthandler.h"

using namespace stefanfrings;

#include	"process-params.h"
#include	"dab-processor.h"
#include	"band-handler.h"
#include	"audiosink.h"

class	deviceHandler;
class	html_writer;

class radioInterface: public HttpRequestHandler {
Q_OBJECT
public:
		radioInterface		(deviceHandler *,
	                                 QSettings *,  QObject* parent=0);
		~radioInterface		();
	void	service			(HttpRequest& request,
	                                 HttpResponse& response);
	void	selectService		(const QString &, HttpResponse &);
	void	selectChannel		(const QString &, HttpResponse&);
	void	update_deviceGain	(int, HttpResponse&);
	void	update_deviceAGC	(const QString &, HttpResponse&);
	void	update_soundGain	(int, HttpResponse&);
	void	timedUpdate		(HttpResponse&);
//
//	
	void	set_dynamicLabel	(const QString &s);
	void	update			(HttpResponse &);
private:
	deviceHandler	*theDevice;
	QSettings	*dabSettings;
	dabProcessor	dabHandler;
	html_writer	*theText;
	QStringList	Services;
	bandHandler	theBand;
	RingBuffer<int16_t> pcmBuffer;
	audioSink	*soundOut;
	int		ifGain;
	int		audioGain;
public slots:
	void		addtoEnsemble	(const QString &, int);
	void		nameofEnsemble	(int, const QString &);
	void		nrServices	(int);
	void		newAudio	(int, int);
	void		showLabel	(QString);

	void		newFrame	(int);
	void		setStereo	(bool);
};
#endif
