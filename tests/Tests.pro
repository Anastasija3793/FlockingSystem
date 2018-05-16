TARGET=FlockingTests
CONFIG-=qt
SOURCES+=main.cpp
LIBS+= -lgtest -pthread
INCLUDEPATH+=/usr/local/include
INCLUDEPATH +=./include \
        ../testsLib/include 
macx:CONFIG-=app_bundle
CONFIG+=c++11 console

LIBS+= -L../testsLib -lFlocking
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
	message("including $HOME/NGL")
	include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
	message("Using custom NGL location")
	include($(NGLDIR)/UseNGL.pri)
}
