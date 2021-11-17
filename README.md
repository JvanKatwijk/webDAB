
---------------------------------------------------------------------
Experimental web DAB radio
---------------------------------------------------------------------

![webDab](/webdab.png?raw=true)

Using the http server from <i> Stefan Frings </i> a straight forward
http server and webinterface was designed for DAB.

The web interface is simple and contains 5 selectors

  * a channel selector,

  * a service selector,

  * a selector for the gain of the SDR device,

  * a selector for setting the autogain on the device

  * a selector for controlling the audio volume

--------------------------------------------------------------------------
How to install the dab-httpserver
--------------------------------------------------------------------------

The software uses the QtWeb server from Stefan Frings and needs Qt
to be installed
A radio.pro file specifies the configuration.

-----------------------------------------------------------------------
Supported devices
-----------------------------------------------------------------------

The program is configured to handle the SDRplay (with the 2.13)
support library and rtlsdr sticks.

--------------------------------------------------------------------------

----------------------------------------------------------------------
Copyright
------------------------------------------------------------------------

        Copyright (C)  2016 .. 2021
        Jan van Katwijk (J.vanKatwijk@gmail.com)
        Lazy Chair Computing

	Copyright httpserver
	Stefan Frings (http://stefanfrings.de)

        The webDAB software is made available under the GPL-2.0.
        The webDAB software is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

