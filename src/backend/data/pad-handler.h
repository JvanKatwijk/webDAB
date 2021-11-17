#
/*
 *    Copyright (C) 2013 .. 2017
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the Qt-DAB program
 *    Qt-DAB is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    Qt-DAB is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Qt-DAB; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef	__PAD_HANDLER__
#define	__PAD_HANDLER__

#include	<QObject>
#include	<cstring>
#include	<cstdint>
#include	<vector>

class	radioInterface;

class	padHandler: public QObject {
Q_OBJECT
public:
		padHandler		(radioInterface *);
		~padHandler();
	void	processPAD		(uint8_t *, int16_t, uint8_t, uint8_t);
private:
	radioInterface	*myRadioInterface;
	void	handle_shortPAD		(uint8_t *, int16_t, uint8_t);
	void	handle_variablePAD      (uint8_t *, int16_t, uint8_t);
	void	dynamicLabel		(uint8_t *, int16_t, uint8_t);
	bool	pad_crc			(uint8_t *, int16_t);
	QString	dynamicLabelText;
	int16_t	charSet;
	bool	mscGroupElement;
	int	xpadLength;
	uint8_t	last_appType;
	int16_t	still_to_go;
	std::vector<uint8_t> shortpadData;
        bool	lastSegment;
        bool	firstSegment;
	int16_t	segmentNumber;
//      dataGroupLength is set when having processed an appType 1
        int dataGroupLength;

//
signals:
	void		showLabel			(QString);
};

#endif
