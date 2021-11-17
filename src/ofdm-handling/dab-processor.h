#
/*
 *    Copyright (C) 2015 .. 2020
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the DAB server
 *
 *    DAB server is free software; you can redistribute it and/or modify
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
#
#ifndef	__DAB_PROCESSOR__
#define	__DAB_PROCESSOR__
/*
 *	dabProcessor is the embodying of all functionality related
 *	to the actal DAB processing.
 */
#include	<QThread>
#include	<QObject>
#include	<QByteArray>
#include	<QStringList>
#include	<vector>
#include	<cstdint>
#include	"dab-constants.h"
#include	"sample-reader.h"
#include	"phasereference.h"
#include	"ofdm-decoder.h"
#include	"fic-handler.h"
#include	"msc-handler.h"
#include	"device-handler.h"
#include	"ringbuffer.h"
//

class	radioInterface;
class	dabParams;

class dabProcessor: public QThread {
Q_OBJECT
public:
		dabProcessor  	(radioInterface *,
	                         deviceHandler *);
		~dabProcessor			();
	void		start			(int32_t);
	void		stop			();
//
//	inheriting from our delegates
//	for the ficHandler:
	bool		is_audioService		(const QString &s);
        void		dataforAudioService     (const QString &,
	                                             audiodata *);
//	for the mscHandler
	void		reset_Services		();
	void		stopService		();
	bool		set_audioChannel	(audiodata *,
	                                             RingBuffer<int16_t> *);
private:
	int		frequency;
	int		threshold;
	deviceHandler	*inputDevice;
	dabParams	params;
	sampleReader	myReader;
	radioInterface	*myRadioInterface;
	ficHandler	my_ficHandler;
	mscHandler	my_mscHandler;
	phaseReference	phaseSynchronizer;
	ofdmDecoder	my_ofdmDecoder;

	int16_t		attempts;
	int32_t		T_null;
	int32_t		T_u;
	int32_t		T_s;
	int32_t		T_g;
	int32_t		T_F;
	int32_t		nrBlocks;
	int32_t		carriers;
	int32_t		carrierDiff;
	int16_t		fineOffset;
	int32_t		coarseOffset;
	bool		correctionNeeded;
	std::vector<std::complex<float>	>ofdmBuffer;
virtual	void		run();
signals:
	void		setSynced		(bool);
	void		No_Signal_Found		();
};
#endif

