#
/*
 *    Copyright (C) 2010, 2011, 2012
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
 *    DAB server is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with DAB server; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	We have to create a simple virtual class here, since we
 *	want the interface with different devices (including  filehandling)
 *	to be transparent
 */
#ifndef	__DEVICE_HANDLER__
#define	__DEVICE_HANDLER__

#include	<stdint.h>
#include	<complex>
using namespace std;

class	deviceHandler {
public:
			deviceHandler 	();
virtual			~deviceHandler 	();
virtual		bool	restartReader	(int32_t);
virtual		void	stopReader	();
virtual		int32_t	getSamples	(std::complex<float> *, int32_t);
virtual		int32_t	Samples		();
virtual		void	resetBuffer	();
virtual		int16_t	bitDepth	() { return 10;}
virtual		void	setGain		(int32_t);
virtual		bool	has_agc		();
virtual		void	set_agc		(bool b);
//
//	for the sdrplay
virtual		void	set_ifgainReduction (int);
virtual		void	set_lnaState	(int);
//
protected:
		int32_t	lastFrequency;
	        int32_t	vfoOffset;
	        int	theGain;
};
#endif

