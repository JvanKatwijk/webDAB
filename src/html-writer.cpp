#include	"html-writer.h"
#include	<stdio.h>
#include	<QFile>

	html_writer::html_writer	(const QString htmlFile) {
//	this -> fileP	= fopen (htmlFile. toLatin1 (). data (), "r");
	this	-> htmlFile	= htmlFile;
	QFile theFile (htmlFile);
	if (theFile. open (QIODevice::ReadOnly | QIODevice::Text)) {
	   QTextStream stream (&theFile);
	   while (!stream. atEnd ()) {
	      content. append (stream. readLine () + "\n");
	   }
	}
	
	strcpy (theText, content. toLatin1 (). data ());
//	fprintf (stderr, "%s\n", theText);
	change		= true;
	serviceList	<< "test1";
	serviceList	<< "test2";
	serviceList	<< "test3";
	serviceList << "test2";
	soundGain	= 50;
	deviceGain	= 50;
	deviceAGC	= "off";
	ensembleName	= "dit is het begin";
	dynamicLabel	= "dit is dynamicLabel";
	selectedChannel	= "5A";
	channelNr	= 0;
	serviceNr	= 0;
	selectedService	= "no service";
}

	html_writer::~html_writer	() {
}

void	html_writer::update_serviceList		(const QStringList &s) {
	serviceList	= s;
	change		= true;
}

void	html_writer::update_selectedService	(const QString &s, int nr) {
	this	-> serviceNr	= nr;
	selectedService	= s;
	change		= true;
}

void	html_writer::update_ensembleName	(const QString &s) {
	ensembleName	= s;
	change		= true;
}

void	html_writer::update_selectedChannel	(const QString &s,
	                                         int channelNr) {
	selectedChannel		= s;
	this	-> channelNr	= channelNr;
//	fprintf (stderr, "channelNr = %d\n", channelNr);
	change		= true;
}

//
//	for the following functions we only ned to remember what
//	the settings were for an update
void	html_writer::set_deviceGain		(int g) {
	deviceGain	= g;
//	fprintf (stderr, "deviceGain = %d\n", g);
	
}

void	html_writer::set_deviceAGC		(const QString &s) {
	deviceAGC	= s;
}

void	html_writer::set_soundGain		(int g) {
	soundGain	= g;
//	printf (stderr, "soundgain = %d\n", g);
}

void	html_writer::update_dynamicLabel	(const QString &s) {
	dynamicLabel	= s;
	change		= true;
}

void	html_writer::update	(HttpResponse &response) {
char buffer [10000];
int bufferP	= 0;
int readP	= 0;

//	fprintf (stderr, "going for an update\n");
//	fprintf (stderr, "met de tekst %s\n", theText);
	locker. lock ();
//	this -> fileP	= fopen (htmlFile. toLatin1 (). data (), "r");
	response. setHeader("Content-Type", "text/html; charset=ISO-8859-1");

	while (true) {
//	   int c = fgetc (fileP);
	   int c = theText [readP ++];
//	   if (feof (fileP))
//	      break;
	   if (c == 0)
	      break;
	   if (c != '$') {
	      buffer [bufferP] = c;
	      bufferP ++;
	      continue;
	   }
//	   c = fgetc (fileP);
//	   if (feof (fileP))
//	      break;		// should not happen
	   c = theText [readP ++];
	   if (c == 0)
	      break;
	   switch (c) {
	      case 'E':		// replace this by ensemblename
	         bufferP = write (ensembleName, buffer, bufferP);
	         continue;

	      case 'N':		// replace this by service number
	         bufferP	= write (serviceNr, buffer, bufferP);
	         break;

	      case 'C':
	         bufferP	= write (selectedChannel, buffer, bufferP);
	         break;

	      case 'B':
	         bufferP	= write (channelNr, buffer, bufferP);
	         break;

	      case 'S':		// replace this by service list;
	         buffer [bufferP] = '[';
	         bufferP ++;
//	we add an additional space, needed for an empty list
	         buffer [bufferP] = ' ';
	         bufferP ++;
	         foreach (QString s, serviceList) {
	            bufferP = write (s, buffer, bufferP);
	            buffer [bufferP] = ',';
	            bufferP ++;
	         }
	         buffer [bufferP - 1] = ']';
	         continue;

	      case 'P':		// replace by selected service
	         bufferP	= write (selectedService, buffer, bufferP);
	         break;

	      case 'D':		// replace this by dynamic label
	         bufferP = write (dynamicLabel, buffer, bufferP);
	         continue;

	      case 'G':		// replace this by device gain
	         bufferP = write (deviceGain, buffer, bufferP);
	         continue;

	      case 'A':		
	         bufferP = write (deviceAGC, buffer, bufferP);
	         continue;

	      case 'V':		// replace this by "geluid"
	         bufferP = write (soundGain, buffer, bufferP);
	         continue;
	
	      default:
	         buffer [bufferP] = '$';
	         bufferP ++;
	         buffer [bufferP] = c;
	         bufferP ++;
	         break;
	   }
	}
	buffer [bufferP] = 0;
	bufferP ++;
	QByteArray temp = QString (buffer). toUtf8 ();
//	fprintf (stderr, "%s\n", buffer);
        response. write  (temp);
//	fclose (this -> fileP);
	locker. unlock ();
//	change	= false;
}

int	html_writer::write (const QString &s, char * buffer, int bp) {
const char *temp = s. toLatin1 (). data ();

//	fprintf (stderr, "we update %s\n", s. toLatin1 (). data ());
        buffer [bp++] = '\"';
        while (*temp != 0)
           buffer [bp ++] =  *temp ++;
        buffer [bp++] = '\"';
        return bp;
}

int	html_writer::write (int c, char * buffer, int bp) {
const QString s = QString::number (c);
	return write (s, buffer, bp);
const char *temp = s. toLatin1 (). data ();
	while (*temp != 0)
	   buffer [bp ++] = *temp ++;
	return bp;
}

