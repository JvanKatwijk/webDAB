#
/*
 *    Copyright (C) 2013 .. 2017
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of Qt-DAB
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
 *
 *	Once the bits are "in", interpretation and manipulation
 *	should reconstruct the data blocks.
 *	Ofdm_decoder is called for Block_0 and the FIC blocks,
 *	its invocation results in 2 * Tu bits
 */
#include	<vector>
#include	"ofdm-decoder.h"
#include	"radio-interface.h"
#include	"phasetable.h"
#include	"fic-handler.h"
#include	"freq-interleaver.h"
#include	"dab-params.h"

/**
  *	\brief ofdmDecoder
  *	The class ofdmDecoder is
  *	taking the data from the ofdmProcessor class in, and
  *	will extract the Tu samples, do an FFT and extract the
  *	carriers and map them on (soft) bits
  */
	ofdmDecoder::ofdmDecoder	(radioInterface *mr,
	                                 uint8_t	dabMode):
	                                    params (dabMode),
	                                    my_fftHandler (dabMode),
	                                    myMapper (dabMode) {
	this	-> myRadioInterface	= mr;
	this	-> T_s			= params. get_T_s();
	this	-> T_u			= params. get_T_u();
	this	-> nrBlocks		= params. get_L();
	this	-> carriers		= params. get_carriers();

	this	-> T_g			= T_s - T_u;
	fft_buffer			= my_fftHandler. getVector();
	phaseReference			.resize (T_u);
}

	ofdmDecoder::~ofdmDecoder() {
}
//
void	ofdmDecoder::stop() {
}

void	ofdmDecoder::reset() {
}

/**
  */
void	ofdmDecoder::processBlock_0 (
	                std::vector <std::complex<float> > buffer) {
	memcpy (fft_buffer, buffer. data(),
	                             T_u * sizeof (std::complex<float>));

	my_fftHandler. do_FFT();
/**
  *	we are now in the frequency domain, and we keep the carriers
  *	as coming from the FFT as phase reference.
  */
	memcpy (phaseReference. data (), fft_buffer,
	                   T_u * sizeof (std::complex<float>));
}
//
//	Just interested. In the ideal case the constellation of the
//	decoded symbols is precisely in the four points 
//	k * (1, 1), k * (1, -1), k * (-1, -1), k * (-1, 1)
//	To ease computation, we map all incoming values onto quadrant 1
static inline
float square (float v) {
	return v * v;
}

/**
  *	for the other blocks of data, the first step is to go from
  *	time to frequency domain, to get the carriers.
  *	we distinguish between FIC blocks and other blocks,
  *	only to spare a test. The mapping code is the same
  */

static	int	cnt	= 0;
void	ofdmDecoder::decode (std::vector <std::complex<float>> buffer,
	                     int32_t blkno, int16_t *ibits) {
int16_t	i;
std::complex<float> conjVector [T_u];

	memcpy (fft_buffer, &((buffer. data()) [T_g]),
	                               T_u * sizeof (std::complex<float>));

//fftlabel:
/**
  *	first step: do the FFT
  */
	my_fftHandler. do_FFT();
/**
  *	a little optimization: we do not interchange the
  *	positive/negative frequencies to their right positions.
  *	The de-interleaving understands this
  */
//toBitsLabel:
/**
  *	Note that from here on, we are only interested in the
  *	"carriers", the useful carriers of the FFT output
  */
	for (i = 0; i < carriers; i ++) {
	   int16_t	index	= myMapper.  mapIn (i);
	   if (index < 0) 
	      index += T_u;
/**
  *	decoding is computing the phase difference between
  *	carriers with the same index in subsequent blocks.
  *	The carrier of a block is the reference for the carrier
  *	on the same position in the next block
  */
	   std::complex<float>	r1 = fft_buffer [index] *
	                                    conj (phaseReference [index]);
	   conjVector [index] = r1;
	   float ab1	= abs (r1);
//	split the real and the imaginary part and scale it
//	we make the bits into softbits in the range -127 .. 127
	   ibits [i]		=  - (real (r1) * 255) / ab1;
	   ibits [carriers + i] =  - (imag (r1) * 255) / ab1;
	}

	memcpy (phaseReference. data(), fft_buffer,
	                            T_u * sizeof (std::complex<float>));
}

