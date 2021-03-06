#
/*
 *    Copyright (C) 2013 .. 2017
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
#include	"sample-reader.h"
#include	"radio-interface.h"

static
std::complex<float> oscillatorTable [INPUT_RATE];

	sampleReader::sampleReader (radioInterface *mr,
	                            deviceHandler	*theRig) {
	this	-> theRig	= theRig;
	currentPhase	= 0;
	sLevel		= 0;
        for (int i = 0; i < INPUT_RATE; i ++)
           oscillatorTable [i] = std::complex<float>
	                            (cos (2.0 * M_PI * i / INPUT_RATE),
                                     sin (2.0 * M_PI * i / INPUT_RATE));

	bufferContent	= 0;
	corrector	= 0;
	running. store (true);
}

	sampleReader::~sampleReader() {
}

void	sampleReader::setRunning (bool b) {
	running. store (b);
}

float	sampleReader::get_sLevel() {
	return sLevel;
}

std::complex<float> sampleReader::getSample (int32_t phaseOffset) {
std::complex<float> temp;

	corrector	= phaseOffset;
	if (!running. load())
	   throw 21;

///	bufferContent is an indicator for the value of ... -> Samples()
	if (bufferContent == 0) {
	   bufferContent = theRig -> Samples();
	   while ((bufferContent <= 2048) && running. load()) {
	      usleep (10);
	      bufferContent = theRig -> Samples(); 
	   }
	}

	if (!running. load())	
	   throw 20;
//
//	so here, bufferContent > 0
	theRig -> getSamples (&temp, 1);
	bufferContent --;
//	OK, we have a sample!!
//	first: adjust frequency. We need Hz accuracy
	currentPhase	-= phaseOffset;
	currentPhase	= (currentPhase + INPUT_RATE) % INPUT_RATE;

	temp		*= oscillatorTable [currentPhase];
	sLevel		= 0.00001 * jan_abs (temp) + (1 - 0.00001) * sLevel;
	return temp;
}

void	sampleReader::getSamples (std::complex<float>  *v,
	                          int32_t n, int32_t phaseOffset) {
int32_t		i;

	corrector	= phaseOffset;
	if (!running. load())
	   throw 21;
	if (n > bufferContent) {
	   bufferContent = theRig -> Samples();
	   while ((bufferContent < n) && running. load()) {
	      usleep (10);
	      bufferContent = theRig -> Samples();
	   }
	}

	if (!running. load())	
	   throw 20;
//
//	so here, bufferContent >= n
	n	= theRig -> getSamples (v, n);
	bufferContent -= n;
//	OK, we have samples!!
//	first: adjust frequency. We need Hz accuracy
	for (i = 0; i < n; i ++) {
	   currentPhase	-= phaseOffset;
//
//	Note that "phase" itself might be negative
	   currentPhase	= (currentPhase + INPUT_RATE) % INPUT_RATE;
	   v [i]	*= oscillatorTable [currentPhase];
	   sLevel	= 0.00001 * jan_abs (v [i]) + (1 - 0.00001) * sLevel;
	}
}

