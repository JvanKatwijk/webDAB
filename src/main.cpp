/**
  @file
  @author Stefan Frings
*/

#include	<QCoreApplication>
#include	<QDir>
#include	"httplistener.h"
#include	"device-handler.h"
#include	"sdrplay-handler.h"
#include	"radio-interface.h"
using namespace stefanfrings;

/** Search the configuration file */
QString searchConfigFile () {
QString binDir	= QCoreApplication::applicationDirPath();
QString appName	= QCoreApplication::applicationName();
QString fileName (appName+".ini");

QStringList searchList;

	searchList. append (binDir);
	searchList. append (binDir+"/etc");
	searchList. append (binDir+"/../etc");
	searchList. append (binDir+"/../../etc"); // for development without shadow build
	searchList. append (binDir+"/../"+appName+"/etc"); // for development with shadow build
	searchList. append (binDir+"/../../"+appName+"/etc"); // for development with shadow build
	searchList. append (binDir+"/../../../"+appName+"/etc"); // for development with shadow build
	searchList. append (binDir+"/../../../../"+appName+"/etc"); // for development with shadow build
	searchList. append (binDir+"/../../../../../"+appName+"/etc"); // for development with shadow build
	searchList. append (QDir::rootPath()+"etc/opt");
	searchList. append (QDir::rootPath()+"etc");

	foreach (QString dir, searchList) {
	   QFile file (dir+"/"+fileName);
	   if (file.exists()) {
            // found
	      fileName = QDir (file. fileName()).canonicalPath();
	      qDebug("Using config file %s",qPrintable(fileName));
	      return fileName;
	   }
	}

// not found
	foreach (QString dir, searchList) {
	   qWarning ("%s/%s not found", qPrintable(dir), qPrintable(fileName));
	}
	qFatal ("Cannot find config file %s",qPrintable (fileName));
}

/**
  Entry point of the program.
*/
int main (int argc, char *argv[]) {
deviceHandler sdrplayHandler ();

	QCoreApplication app (argc,argv);

	app. setApplicationName ("webDab");
	app. setOrganizationName ("Lazy Chair Computing");

//	Find the configuration file
	QString configFileName = searchConfigFile ();

//	Configure static file controller
	QSettings* fileSettings =
	           new QSettings (configFileName, QSettings::IniFormat,&app);
	fileSettings -> beginGroup ("docroot");
//	Configure and start the TCP listener
	QSettings* listenerSettings =
	           new QSettings (configFileName, QSettings::IniFormat,&app);
	listenerSettings -> beginGroup ("listener");
	new HttpListener (listenerSettings,
	                    new radioInterface (&sdrplayHandler, &app), &app);

	qInfo ("Application has started");
	app. exec();
	qInfo("Application has stopped");
}
