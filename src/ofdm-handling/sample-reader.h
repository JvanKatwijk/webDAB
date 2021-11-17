#
/*
 *    Copyright (C) 2013 .. 2020
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the Qt-DAB program
 *
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
#
#ifndef	__SAMPLE_READER__
#define	__SAMPLE_READER__
/*
 *	Reading the samples from the input device. Since it has its own
 *	"state", we embed it into its own class
 */
#include	<QObject>
#include	<sndfile.h>
#include	<cstdint>
#include	<atomic>
#include	<vector>
#include	"dab-constants.h"
#include	"device-handler.h"
#include	"ringbuffer.h"
//
//      Note:
//      It was found that enlarging the buffersize to e.g. 8192
//      cannot be handled properly by the underlying system.

class	radioInterface;
class	sampleReader : public QObject {
Q_OBJECT
public:
			sampleReader	(radioInterface *mr,
	                         	deviceHandler *theRig);

			~sampleReader	();
		void	setRunning	(bool b);
		float	get_sLevel	();
		std::complex<float> getSample	(int32_t);
	        void	getSamples	(std::complex<float> *v,
	                                 int32_t n, int32_t phase);
private:
		radioInterface	*myRadioInterface;
		deviceHandler	*theRig;
		RingBuffer<std::complex<float>> *spectrumBuffer;
		int32_t		currentPhase;
		std::atomic<bool>	running;
		int32_t		bufferContent;
		float		sLevel;
	        int32_t		corrector;
};

#endif