
#include	<stdio.h>
#include	<stdint.h>
#include	<stdlib.h>
#include	"channelselector.h"
#include	<QStringList>

QStringList Services;

	channelSelector::channelSelector	() {}
void	channelSelector::selectChannel		(const QString &s,
	                                         HttpResponse& response) {
static int cnt	= 1;
	Services	<< "Radio " + QString::number (cnt++);

	response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");
	response. write  ("<html lang=\"en\">\n");
	response. write  ("<head>\n");
	response. write  ("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	response. write  ("<title>WebDAB</title>\n");
	response. write ("<link href=\"/radiostyle.css\" rel=\"stylesheet\" type=\"text/css\">");
	response. write ("<link href=\"../radiostyle.css\" rel=\"stylesheet\" type=\"text/css\">");
	response. write  ("</head>\n");
	response. write  ("<body>\n");
	response. write  ("<header>\n");
	response. write  ("\t<h2> Experimental Web DAB Interface</h2>\n");
	char *formData	= getenv ("QUERY_STRING");
	response. write  (formData);
	response. write  ("</header>\n");
	response. write  ("<table>\n <tr>\n");
	response. write  ("<td><form name=\"myList\" action=\"/set_channel\" method=\"post\"> \n");
	response. write  ("<select id=\"Channels\" name=\"Channels\"></select>\n");
	response. write  ("<input type=\"submit\" name=\"submit\" value=\"select channel\" /> \n");
	response. write  ("</form></td>\n"); 
	response. write  ("	<td align=\'center\' width=\'200\'> <span id=\"selectedService\"></span> </td>\n");
	response. write  ("<td align=\'right\' width=\'130\'><span id=\"datetime\"></span></td>\n");
	response. write  ("	</tr> </table>\n");

	response. write  ("<section>\n");
	response. write  ("<nav id=\"serviceSection\"> </nav>\n");
	response. write  ("<article> <h1> selected service </h1>\n");
	response. write  ("<span id=\"servicedata\"></span>\n");
	response. write  ("</article>\n");
	response. write  ("<footer>\n");
	response. write  ("<span id=\"dynamicLabel\"></span>\n");
	response. write  ("</footer>\n");
	response. write  ("<script src=\"/channels.js\"></script>\n");
	response. write  ("<script src=\"/myscript.js\"></script>\n");
	response. write  ("<script>\n");
	response. write  ("display_ct ();\n");
	response. write  ("setInterval (display_ct, 1000);\n");
	response. write  ("setupChannels	();\n");
	QString serviceList = "var serviceList = [";
	if (Services. size () > 0)
	   serviceList += "\"" + Services. at (0) + "\" ";
	for (int i = 1; i < Services. size (); i ++) 
	   serviceList += ", \"" + Services. at (i) + "\" ";
	serviceList += "];\n";
	response. write (serviceList. toLatin1 ());
//	response. write  ("var serviceList = [\"abc\", \"def\", \"ghi\", \"jkl\"];\n");
	response. write  ("setupServices (serviceList);\n");
	response. write  ("	setupServiceData (\"nieuwe service data\");\n");
	QString dynamicLabel = "you selected " + s;
	QByteArray command = "\tsetupDynamicLabel (\" " + dynamicLabel. toLatin1 () +
	                                     "\");\n";
	response. write  (command);
//	response. write  ("       setupDynamicLabel (\"new dynamicLabel\");\n");
	response. write  ("</script>\n");
	response. write  ("</body>\n");
	response. write  ("</html> \n");
}

