# This project demonstrates how to use QtWebAppLib by including the
# sources into this project.


TARGET		= testRadio
TEMPLATE	= app
QT		+= core network
CONFIG		+= console
#DEFINES	+= SUPERVERBOSE
DEFINES		+= QT_NO_SSL
QMAKE_CFLAGS   +=  -g
QMAKE_CXXFLAGS +=  -g
QMAKE_LFLAGS   +=  -g

CONFIG		+= sdrplay-v2
CONFIG		+= rtlsdr
#CONFIG		+= sdrplay_v3

LIBS		+= -lfftw3f -lfaad -lsamplerate -lportaudio -lusb-1.0 -ldl
#LIBS		+= -lmirsdrapi-rsp -lfftw3f -lfaad -lsamplerate -lportaudio -lusb-1.0 -ldl
DEPENDPATH += . \
	          ./src \
	          ./src/ofdm-handling \
	          ./src/support \
	          ./src/support/viterbi-spiral \
	          ./src/support/protection \
	          ./src/backend \
	          ./src/backend/audio \
	          ./src/backend/data \
	          ./src/audio-sink \
	          ./devices \
	          ./httpserver

INCLUDEPATH += . \
	          ./src \
	          ./src/ofdm-handling \
	          ./src/support \
	          ./src/support/viterbi-spiral \
	          ./src/support/protection \
                  ./src/backend \
                  ./src/backend/audio \
                  ./src/backend/data \
	          ./src/audio-sink \
	          ./devices  \
	          ./httpserver
	        
HEADERS += \
	   ./src/radio-interface.h \
	   ./src/html-writer.h \
	   ./src/support/dab-constants.h \
	   ./src/support/dab-params.h \
	   ./src/support/dab-tables.h \
	   ./src/support/fft-handler.h \
	   ./src/support/process-params.h \
	   ./src/support/ringbuffer.h \
	   ./src/support/charsets.h \
	   ./src/support/band-handler.h \
	   ./src/support/viterbi-spiral/spiral-no-sse.h \
	   ./src/support/viterbi-spiral/viterbi-spiral.h \
	   ./src/support/protection/protTables.h \
	   ./src/support/protection/protection.h \
	   ./src/support/protection/eep-protection.h \
	   ./src/support/protection/uep-protection.h \
	   ./src/ofdm-handling/dab-config.h \
	   ./src/ofdm-handling/dab-processor.h \
	   ./src/ofdm-handling/fib-decoder.h \
	   ./src/ofdm-handling/fib-table.h \
	   ./src/ofdm-handling/fic-handler.h \
	   ./src/ofdm-handling/freq-interleaver.h \
	   ./src/ofdm-handling/ofdm-decoder.h \
	   ./src/ofdm-handling/phasereference.h \
	   ./src/ofdm-handling/phasetable.h \
	   ./src/ofdm-handling/sample-reader.h \
	   ./src/ofdm-handling/timesyncer.h \
	   ./src/backend/backend.h \
	   ./src/backend/backend-driver.h \
	   ./src/backend/backend-deconvolver.h \
	   ./src/backend/firecode-checker.h \
	   ./src/backend/frame-processor.h \
	   ./src/backend/galois.h \
	   ./src/backend/mm_malloc.h \
	   ./src/backend/reed-solomon.h \
	   ./src/backend/msc-handler.h \
	   ./src/backend/audio/mp2processor.h \
	   ./src/backend/audio/mp4processor.h \
	   ./src/backend/audio/faad-decoder.h \
	   ./src/backend/audio/neaacdec.h \
	   ./src/backend/data/pad-handler.h \
	   ./src/audio-sink/audio-base.h \
	   ./src/audio-sink/audiosink.h \
	   ./src/audio-sink/newconverter.h \
	   ./devices/device-handler.h \
	   ./httpserver/httpconnectionhandler.h \
           ./httpserver/httpconnectionhandlerpool.h \
           ./httpserver/httpcookie.h \
#           ./httpserver/httpglobal.h \
           ./httpserver/httplistener.h \
           ./httpserver/httprequest.h \
           ./httpserver/httprequesthandler.h \
           ./httpserver/httpresponse.h \
           ./httpserver/httpsession.h \
           ./httpserver/httpsessionstore.h 


SOURCES	+=   \
	   ./src/radio-interface.cpp \
	   ./src/html-writer.cpp \
	   ./main.cpp  \
	   ./src/support/dab-params.cpp \
	   ./src/support/dab-tables.cpp \
	   ./src/support/fft-handler.cpp \
	   ./src/support/charsets.cpp \
	   ./src/support/band-handler.cpp \
	   ./src/support/viterbi-spiral/spiral-no-sse.c \
	   ./src/support/viterbi-spiral/viterbi-spiral.cpp \
	   ./src/support/protection/protTables.cpp \
	   ./src/support/protection/protection.cpp \
	   ./src/support/protection/eep-protection.cpp \
	   ./src/support/protection/uep-protection.cpp \
	   ./src/ofdm-handling/dab-processor.cpp \
	   ./src/ofdm-handling/fib-decoder.cpp \
	   ./src/ofdm-handling/fic-handler.cpp \
	   ./src/ofdm-handling/freq-interleaver.cpp \
	   ./src/ofdm-handling/ofdm-decoder.cpp \
	   ./src/ofdm-handling/phasereference.cpp \
	   ./src/ofdm-handling/phasetable.cpp \
	   ./src/ofdm-handling/sample-reader.cpp \
	   ./src/ofdm-handling/timesyncer.cpp \
	   ./src/backend/backend.cpp \
	   ./src/backend/backend-driver.cpp \
	   ./src/backend/backend-deconvolver.cpp \
	   ./src/backend/firecode-checker.cpp \
	   ./src/backend/galois.cpp \
	   ./src/backend/reed-solomon.cpp \
	   ./src/backend/msc-handler.cpp \
	   ./src/backend/audio/mp2processor.cpp \
	   ./src/backend/audio/mp4processor.cpp \
	   ./src/backend/audio/faad-decoder.cpp \
	   ./src/backend/data/pad-handler.cpp \
	   ./src/audio-sink/audio-base.cpp \
	   ./src/audio-sink/audiosink.cpp \
	   ./src/audio-sink/newconverter.cpp \
	   ./devices/device-handler.cpp \
	   ./httpserver/httpconnectionhandler.cpp \
           ./httpserver/httpconnectionhandlerpool.cpp \
           ./httpserver/httpcookie.cpp \
#	   ./httpserver/httpglobal.cpp \
           ./httpserver/httplistener.cpp \
           ./httpserver/httprequest.cpp \
           ./httpserver/httprequesthandler.cpp \
           ./httpserver/httpresponse.cpp \
           ./httpserver/httpsession.cpp \
           ./httpserver/httpsessionstore.cpp 


#
#
sdrplay-v2 {
	DEFINES		+= __HAVE_SDRPLAY_V2__
	DEPENDPATH	+= ./devices/sdrplay-handler
	INCLUDEPATH	+= ./devices/sdrplay-handler
	HEADERS		+= ./devices/sdrplay-handler/sdrplay-handler.h
	SOURCES		+= ./devices/sdrplay-handler/sdrplay-handler.cpp 
}

rtlsdr {
	DEFINES		+= __HAVE_RTLSDR__
	DEPENDPATH	+= ./devices/rtlsdr-handler
	INCLUDEPATH	+= ./devices/rtlsdr-handler
	HEADERS		+= ./devices/rtlsdr-handler/rtlsdr-handler.h 
	SOURCES		+= ./devices/rtlsdr-handler/rtlsdr-handler.cpp 
}
