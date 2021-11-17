/**
  @file
  @author Jan
*/

#include	<QCoreApplication>
#include	<QDir>
#include	"httplistener.h"
#include	"device-handler.h"
#ifdef	__HAVE_SDRPLAY_V2__
#include	"sdrplay-handler.h"
#endif
#ifdef	__HAVE_SDRPLAY_V3__
#include	"sdrplay-handler_v3.h"
#endif
#ifdef	__HAVE_RTLSDR__
#include	"rtlsdr-handler.h"
#endif

#include	"radio-interface.h"
using namespace stefanfrings;

deviceHandler	*findDevice () {
deviceHandler	*theDevice	= nullptr;
#ifdef __HAVE_RTLSDR__
	try {
	   fprintf (stderr, "trying for rtlsdr\n");
	   theDevice	= new rtlsdrHandler ();
	   return theDevice;
	} catch  (...) {}
#endif
#ifdef	__HAVE_SDRPLAY_V2__
	try {
	   fprintf (stderr, "trying for sdrplay\n");
	   theDevice	= new sdrplayHandler ();
	   return theDevice;
	} catch (...) {}
#endif
#ifdef	__HAVE_SDRPLAY_V3__
	try {
	   theDevice	= new sdrplayHandler_v3 ();
	   return theDevice;
	} catch (...) {}
#endif
	return nullptr;
}

/**
  Entry point of the program.
*/
int main (int argc, char *argv[]) {
deviceHandler *theDevice;
int	port	= 8080;
int	minThreads	= 1;
int	maxThreads	= 1;
int	cleanupInterval	= 60000;
int	readTimeout	= 60000;
int	maxRequestSize	= 16000;
int	maxMultiPartSize	= 10000000;
int	opt;
const char *optionsString = "P:M:m:";

	fprintf (stderr, "experimental webDAB server v0.3 \nCopyright J van Katwijk, Lazy Char Computing\nCopyrigt webServer Stefan Frings\n");

	while ((opt = getopt (argc, argv, optionsString)) != -1) {
	   switch (opt) {
	      case 'P':	port	= atoi (optarg);
	         break;

	      case 'M': maxThreads = atoi (optarg);
	         break;

	      case 'm': minThreads = atoi (optarg);
	         break;

	      default:
	         break;
	   }
	}

	if (maxThreads < minThreads) 
	   maxThreads = minThreads;

	if ((minThreads < 1) || (maxThreads < 1)) {
	   minThreads = 1;
	   maxThreads = 1;
	}
	
	   
	QCoreApplication app (argc,argv);

	app. setApplicationName ("webDab");
	app. setOrganizationName ("Lazy Chair Computing");

	QString configFileName	= QDir::homePath ();
	configFileName. append ("/");
	configFileName. append (".webDab.ini");

//	Configure the DAB settings
	QSettings* dabSettings =
	           new QSettings (configFileName, QSettings::IniFormat,&app);
//	Configure and start the TCP listener
	QSettings* listenerSettings =
	           new QSettings (configFileName, QSettings::IniFormat,&app);
	listenerSettings	-> setValue ("port", port);
	listenerSettings	-> setValue ("readTimeout", readTimeout);
	listenerSettings	-> setValue ("minThreads", minThreads);
	listenerSettings	-> setValue ("cleanupInterval",
	                                                  cleanupInterval);
	listenerSettings	-> setValue ("maxThreads", maxThreads);
	listenerSettings	-> setValue ("maxRequestSize",
	                                                  maxRequestSize);
	listenerSettings	-> setValue ("maxMultiPartSize",
	                                                 maxMultiPartSize);
	
	theDevice	= findDevice ();
	if (theDevice == nullptr) {
	   fprintf (stderr, "server cannot work without device\n");
	   exit (1);
	}
	new HttpListener (listenerSettings,
	                    new radioInterface (theDevice, dabSettings, &app), &app);

	qInfo ("Application has started");
	app. exec();
	qInfo("Application has stopped");
}
