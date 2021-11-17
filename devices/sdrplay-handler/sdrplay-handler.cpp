#
/*
 *    Copyright (C) 2014 .. 2017
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Programming
 *
 *    This file is part of the DAB library
 *
 *    DAB library is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    DAB library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with DAB library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include	"sdrplay-handler.h"

#define	DEFAULT_GAIN	40

	sdrplayHandler::sdrplayHandler  ():
	                             _I_Buffer (1024 * 1024) {
float	ver;
mir_sdr_DeviceT devDesc [4];

	this	-> inputRate		= 2048000;
        this    -> frequency		= 220000000;
        this    -> ppmCorrection	= 0;
        this    -> GRdB			= 30;
	this	-> lnaState		= 3;
	this	-> autoGain		= true;


	bool success            = fetchLibrary ();
        if (!success) {
           throw (23);
        }

        success = loadFunctions();
        if (!success) {
           releaseLibrary       ();
           throw (23);
        }

	(void) my_mir_sdr_ApiVersion (&ver);
	if (ver < 2.13) {
	   fprintf (stderr, "please upgrade to sdrplay library 2.13\n");
	   throw (24);
	}

	my_mir_sdr_GetDevices (devDesc, &numofDevs, uint32_t (4));
	if (numofDevs == 0) {
	   fprintf (stderr, "Sorry, no device found\n");
	   throw (25);
	}

	this -> deviceIndex = 0;
	hwVersion = devDesc [deviceIndex]. hwVer;
	fprintf (stderr, "sdrdevice found = %s, hw Version = %d\n",
	                              devDesc [deviceIndex]. SerNo, hwVersion);
	my_mir_sdr_SetDeviceIdx (deviceIndex);

	if (hwVersion == 2) 
	   (void)my_mir_sdr_RSPII_AntennaControl (mir_sdr_RSPII_ANTENNA_A);

	nameofDevice	= std::string (devDesc [deviceIndex]. DevNm);

	switch (hwVersion) {
	   case 1:		// old RSP
	      nrBits		= 12;
	      denominator	= 2048.0;
	      lnaState		= 1;
	      break;

	   case 2:
	   case 3:
	      nrBits		= 14;
	      denominator	= 8192.0;
	      lnaState		= 3;
	      break;

	   default:
	      nrBits		= 14;
	      denominator	= 8192.0;
	      lnaState		= 3;
	}

	my_mir_sdr_AgcControl (autoGain ?
	                 mir_sdr_AGC_100HZ :
	                 mir_sdr_AGC_DISABLE, - GRdB, 0, 0, 0, 0, 0);
	if (!autoGain)
	   my_mir_sdr_RSP_SetGr (GRdB, lnaState, 1, 0);
	running. store (false);
}

	sdrplayHandler::~sdrplayHandler	(void) {
	stopReader ();
	if (numofDevs > 0)
	   my_mir_sdr_ReleaseDeviceIdx (0);
}
//
//
//	For the setting of gain, not using a widget, we map the
//	gain value upon an attenation value and set setexternal Gain
void	sdrplayHandler::setGain	(int32_t g) {
	GRdB	= (59 - 20) * g / 100.0 + 20;
	(void)my_mir_sdr_RSP_SetGr (GRdB, lnaState, 1, 0);
}

void	sdrplayHandler::set_agc (bool b) {
	autoGain	= b;
	my_mir_sdr_AgcControl (b ?
	                 mir_sdr_AGC_100HZ :
	                 mir_sdr_AGC_DISABLE, - GRdB, 0, 0, 0, 0, lnaState);
	if (!b)
	   (void)my_mir_sdr_RSP_SetGr (GRdB, lnaState, 1, 0);
}

static
void myStreamCallback (int16_t		*xi,
	               int16_t		*xq,
	               uint32_t		firstSampleNum, 
	               int32_t		grChanged,
	               int32_t		rfChanged,
	               int32_t		fsChanged,
	               uint32_t		numSamples,
	               uint32_t		reset,
	               uint32_t		hwRemoved,
	               void		*cbContext) {
int16_t	i;
sdrplayHandler	*p	= static_cast<sdrplayHandler *> (cbContext);
std::complex<float> localBuf [numSamples];
float	denominator		= p -> denominator;

	if (reset || hwRemoved)
	   return;
	for (i = 0; i <  (int)numSamples; i ++)
	   localBuf [i] = std::complex<float> (float (xi [i]) / denominator,
	                                       float (xq [i]) / denominator);
	p -> _I_Buffer. putDataIntoBuffer (localBuf, numSamples);
	(void)	firstSampleNum;
	(void)	grChanged;
	(void)	rfChanged;
	(void)	fsChanged;
	(void)	reset;
}

void	myGainChangeCallback (uint32_t	gRdB,
	                      uint32_t	lnaGRdB,
	                      void	*cbContext) {
	(void)gRdB;
	(void)lnaGRdB;	
	(void)cbContext;
}

bool	sdrplayHandler::restartReader	(int32_t frequency) {
int	gRdBSystem;
int	samplesPerPacket;
mir_sdr_ErrT	err;
int	localGRed	= GRdB;

	fprintf (stderr, "restart request for %d\n", frequency);
	if (running. load ())
	   return true;

	err	= my_mir_sdr_StreamInit (&localGRed,
	                              double (inputRate) / 1000000.0,
	                              double (frequency) / 1000000.0,
	                              mir_sdr_BW_1_536,
	                              mir_sdr_IF_Zero,
	                              1,	// lnaEnable do not know yet
	                              &gRdBSystem,
	                              mir_sdr_USE_SET_GR,
	                              &samplesPerPacket,
	                              (mir_sdr_StreamCallback_t)myStreamCallback,
	                              (mir_sdr_GainChangeCallback_t)myGainChangeCallback,
	                              this);
	if (err != mir_sdr_Success) {
	   fprintf (stderr, "Error %d on streamInit\n", err);
	   return false;
	}

        err             = my_mir_sdr_SetDcMode (4, 1);
        err             = my_mir_sdr_SetDcTrackTime (63);
        running. store (true);
        return true;
}

void	sdrplayHandler::stopReader	(void) {
	if (!running. load ())
	   return;

	my_mir_sdr_StreamUninit	();
	_I_Buffer. FlushRingBuffer ();
	running. store (false);
}

int32_t	sdrplayHandler::getSamples (std::complex<float> *V, int32_t size) { 
	return  _I_Buffer. getDataFromBuffer (V, size);
}

int32_t	sdrplayHandler::Samples	(void) {
	return _I_Buffer. GetRingBufferReadAvailable ();
}

void	sdrplayHandler::resetBuffer	(void) {
	_I_Buffer. FlushRingBuffer ();
}

bool	sdrplayHandler::loadFunctions () {
	my_mir_sdr_StreamInit	= (pfn_mir_sdr_StreamInit)
	                    GETPROCADDRESS (this -> Handle,
	                                    "mir_sdr_StreamInit");
	if (my_mir_sdr_StreamInit == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_StreamInit\n");
	   return false;
	}

	my_mir_sdr_StreamUninit	= (pfn_mir_sdr_StreamUninit)
	                    GETPROCADDRESS (this -> Handle,
	                                    "mir_sdr_StreamUninit");
	if (my_mir_sdr_StreamUninit == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_StreamUninit\n");
	   return false;
	}

	my_mir_sdr_SetRf	= (pfn_mir_sdr_SetRf)
	                    GETPROCADDRESS (Handle, "mir_sdr_SetRf");
	if (my_mir_sdr_SetRf == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_SetRf\n");
	   return false;
	}

	my_mir_sdr_SetFs	= (pfn_mir_sdr_SetFs)
	                    GETPROCADDRESS (Handle, "mir_sdr_SetFs");
	if (my_mir_sdr_SetFs == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_SetFs\n");
	   return false;
	}

	my_mir_sdr_SetGr	= (pfn_mir_sdr_SetGr)
	                    GETPROCADDRESS (Handle, "mir_sdr_SetGr");
	if (my_mir_sdr_SetGr == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_SetGr\n");
	   return false;
	}

	my_mir_sdr_RSP_SetGr	= (pfn_mir_sdr_RSP_SetGr)
	                    GETPROCADDRESS (Handle, "mir_sdr_RSP_SetGr");
	if (my_mir_sdr_RSP_SetGr == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_RSP_SetGr\n");
	   return false;
	}

	my_mir_sdr_SetGrParams	= (pfn_mir_sdr_SetGrParams)
	                    GETPROCADDRESS (Handle, "mir_sdr_SetGrParams");
	if (my_mir_sdr_SetGrParams == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_SetGrParams\n");
	   return false;
	}

	my_mir_sdr_SetDcMode	= (pfn_mir_sdr_SetDcMode)
	                    GETPROCADDRESS (Handle, "mir_sdr_SetDcMode");
	if (my_mir_sdr_SetDcMode == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_SetDcMode\n");
	   return false;
	}

	my_mir_sdr_SetDcTrackTime	= (pfn_mir_sdr_SetDcTrackTime)
	                    GETPROCADDRESS (Handle, "mir_sdr_SetDcTrackTime");
	if (my_mir_sdr_SetDcTrackTime == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_SetDcTrackTime\n");
	   return false;
	}

	my_mir_sdr_SetSyncUpdateSampleNum = (pfn_mir_sdr_SetSyncUpdateSampleNum)
	               GETPROCADDRESS (Handle, "mir_sdr_SetSyncUpdateSampleNum");
	if (my_mir_sdr_SetSyncUpdateSampleNum == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_SetSyncUpdateSampleNum\n");
	   return false;
	}

	my_mir_sdr_SetSyncUpdatePeriod	= (pfn_mir_sdr_SetSyncUpdatePeriod)
	                GETPROCADDRESS (Handle, "mir_sdr_SetSyncUpdatePeriod");
	if (my_mir_sdr_SetSyncUpdatePeriod == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_SetSyncUpdatePeriod\n");
	   return false;
	}

	my_mir_sdr_ApiVersion	= (pfn_mir_sdr_ApiVersion)
	                GETPROCADDRESS (Handle, "mir_sdr_ApiVersion");
	if (my_mir_sdr_ApiVersion == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_ApiVersion\n");
	   return false;
	}

	my_mir_sdr_AgcControl	= (pfn_mir_sdr_AgcControl)
	                GETPROCADDRESS (Handle, "mir_sdr_AgcControl");
	if (my_mir_sdr_AgcControl == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_AgcControl\n");
	   return false;
	}

	my_mir_sdr_Reinit	= (pfn_mir_sdr_Reinit)
	                GETPROCADDRESS (Handle, "mir_sdr_Reinit");
	if (my_mir_sdr_Reinit == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_Reinit\n");
	   return false;
	}

	my_mir_sdr_SetPpm	= (pfn_mir_sdr_SetPpm)
	                GETPROCADDRESS (Handle, "mir_sdr_SetPpm");
	if (my_mir_sdr_SetPpm == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_SetPpm\n");
	   return false;
	}

	my_mir_sdr_DebugEnable	= (pfn_mir_sdr_DebugEnable)
	                GETPROCADDRESS (Handle, "mir_sdr_DebugEnable");
	if (my_mir_sdr_DebugEnable == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_DebugEnable\n");
	   return false;
	}

	my_mir_sdr_rspDuo_TunerSel = (pfn_mir_sdr_rspDuo_TunerSel)
	               GETPROCADDRESS (Handle, "mir_sdr_rspDuo_TunerSel");
	if (my_mir_sdr_rspDuo_TunerSel == nullptr) {
           fprintf (stderr, "Could not find mir_sdr_rspDuo_TunerSel\n");
           return false;
        }

	my_mir_sdr_DCoffsetIQimbalanceControl	=
	                     (pfn_mir_sdr_DCoffsetIQimbalanceControl)
	                GETPROCADDRESS (Handle, "mir_sdr_DCoffsetIQimbalanceControl");
	if (my_mir_sdr_DCoffsetIQimbalanceControl == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_DCoffsetIQimbalanceControl\n");
	   return false;
	}


	my_mir_sdr_ResetUpdateFlags	= (pfn_mir_sdr_ResetUpdateFlags)
	                GETPROCADDRESS (Handle, "mir_sdr_ResetUpdateFlags");
	if (my_mir_sdr_ResetUpdateFlags == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_ResetUpdateFlags\n");
	   return false;
	}

	my_mir_sdr_GetDevices		= (pfn_mir_sdr_GetDevices)
	                GETPROCADDRESS (Handle, "mir_sdr_GetDevices");
	if (my_mir_sdr_GetDevices == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_GetDevices");
	   return false;
	}

	my_mir_sdr_GetCurrentGain	= (pfn_mir_sdr_GetCurrentGain)
	                GETPROCADDRESS (Handle, "mir_sdr_GetCurrentGain");
	if (my_mir_sdr_GetCurrentGain == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_GetCurrentGain");
	   return false;
	}

	my_mir_sdr_GetHwVersion	= (pfn_mir_sdr_GetHwVersion)
	                GETPROCADDRESS (Handle, "mir_sdr_GetHwVersion");
	if (my_mir_sdr_GetHwVersion == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_GetHwVersion");
	   return false;
	}

	my_mir_sdr_RSPII_AntennaControl	= (pfn_mir_sdr_RSPII_AntennaControl)
	                GETPROCADDRESS (Handle, "mir_sdr_RSPII_AntennaControl");
	if (my_mir_sdr_RSPII_AntennaControl == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_RSPII_AntennaControl");
	   return false;
	}

	my_mir_sdr_SetDeviceIdx	= (pfn_mir_sdr_SetDeviceIdx)
	                GETPROCADDRESS (Handle, "mir_sdr_SetDeviceIdx");
	if (my_mir_sdr_SetDeviceIdx == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_SetDeviceIdx");
	   return false;
	}

	my_mir_sdr_ReleaseDeviceIdx	= (pfn_mir_sdr_ReleaseDeviceIdx)
	                GETPROCADDRESS (Handle, "mir_sdr_ReleaseDeviceIdx");
	if (my_mir_sdr_ReleaseDeviceIdx == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_ReleaseDeviceIdx");
	   return false;
	}

	my_mir_sdr_RSPII_RfNotchEnable	=
	                               (pfn_mir_sdr_RSPII_RfNotchEnable)
	                GETPROCADDRESS (Handle, "mir_sdr_RSPII_RfNotchEnable");
	if (my_mir_sdr_RSPII_RfNotchEnable == nullptr) {
	   fprintf (stderr, "Could not find mir_sdr_RSPII_RfNotchEnable\n");
	   return false;
	}

	return true;
}

bool	sdrplayHandler::fetchLibrary	() {
#ifdef	__MINGW32__
HKEY APIkey;
wchar_t APIkeyValue [256];
ULONG APIkeyValue_length = 255;


	wchar_t *libname = (wchar_t *)L"mir_sdr_api.dll";
	Handle	= LoadLibrary (libname);
	if (Handle == nullptr) {
	   if (RegOpenKey (HKEY_LOCAL_MACHINE,
	                   TEXT("Software\\MiricsSDR\\API"),
	                   &APIkey) != ERROR_SUCCESS) {
              fprintf (stderr,
	           "failed to locate API registry entry, error = %d\n",
	           (int)GetLastError());
	      return false;
	   }

	   RegQueryValueEx (APIkey,
	                    (wchar_t *)L"Install_Dir",
	                    nullptr,
	                    nullptr,
	                    (LPBYTE)&APIkeyValue,
	                    (LPDWORD)&APIkeyValue_length);
//	Ok, make explicit it is in the 32 or 64 bits section
	   wchar_t *x =
#ifdef	__BITS64__
	        wcscat (APIkeyValue, (wchar_t *)L"\\x64\\mir_sdr_api.dll");
#else
	        wcscat (APIkeyValue, (wchar_t *)L"\\x86\\mir_sdr_api.dll");
#endif
	   RegCloseKey(APIkey);

	   Handle	= LoadLibrary (x);
	   if (Handle == nullptr) {
	      fprintf (stderr, "Failed to open mir_sdr_api.dll\n");
	      return false;
	   }
	}
#else
	Handle		= dlopen ("libusb-1.0.so", RTLD_NOW | RTLD_GLOBAL);
	Handle		= dlopen ("libmirsdrapi-rsp.so", RTLD_NOW);
	if (Handle == nullptr)
	   Handle	= dlopen ("libmir_sdr.so", RTLD_NOW);

	if (Handle == nullptr) {
	   fprintf (stderr, "error report %s\n", dlerror());
	   return false;
	}
#endif
	return true;
}

void	sdrplayHandler::releaseLibrary	() {
#ifdef __MINGW32__
           FreeLibrary (Handle);
#else
           dlclose (Handle);
#endif
}
