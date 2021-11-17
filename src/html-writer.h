
#ifndef	__HTML_WRITER_H
#define	__HTML_WRITER_H
#include	<stdio.h>
#include	<QString>
#include	<mutex>
#include	<QStringList>
#include        "httprequest.h"
#include        "httpresponse.h"
#include        "httprequesthandler.h"

using namespace stefanfrings;

class	html_writer {
public:	
	html_writer		(const QString fileName);
	~html_writer		();
	
void	update_serviceList	(const QStringList &s);

void	update_selectedService	(const QString &s, int);

void	update_ensembleName	(const QString &s);

void	update_dynamicLabel	(const QString &s);

void	update_selectedChannel	(const QString &s, int);

void	set_deviceGain		(int g);

void	set_deviceAGC		(const QString &);

void	set_soundGain		(int g);

bool	isChanged		();

void	update			(HttpResponse &response);
private:
	QString		htmlFile;
	FILE		*fileP;
	QString		selectedChannel;
	int		channelNr;
	QString		selectedService;
	int		serviceNr;
	QString		dynamicLabel;
	QString		ensembleName;
	QStringList	serviceList;
	int		deviceGain;
	QString		deviceAGC;
	int		soundGain;
	int		write (const QString &s, char * buffer, int bp);
	int		write (int, char * buffer, int bp);
	bool		change;
	std::mutex	locker;
	QString		content;
	char		theText [10000];
};
#endif
