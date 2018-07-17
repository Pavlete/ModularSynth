# Automatically generated qmake file, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

TEMPLATE = app
CONFIG  += warn_off
CONFIG  += c++14

include(Juce.pri)

# Source and header files
SOURCES += \
    src/maincomponent.cpp \
    src/Main.cpp \
    src/modularvoice.cpp \
    src/modularsynth.cpp \
    src/soundProc/adsrenvelope.cpp \
    src/soundProc/glide.cpp \
    src/soundProc/onepolefilter.cpp \
    src/soundProc/signal.cpp \
    src/nodes/vco.cpp \
    src/synthcanvas.cpp \
    src/modulemenu.cpp \
    src/nodes/outvca.cpp \
    src/nodes/common/audiobufferwrapper.cpp \
    src/nodes/vcf.cpp \
    src/nodes/common/juceaudionode.cpp \
    src/nodes/common/juceconnection.cpp \
    src/nodes/common/socket.cpp \
    src/synthmodel.cpp \
    src/nodes/common/ongoingconnection.cpp


HEADERS += \
    src/maincomponent.h \
    src/nodes/common/nodeview.h\
    src/nodes/common/nodemodel.h\
    src/modularvoice.h \
    src/graph/audioprocessgraph.h \
    src/graph/processgraph.h \
    src/graph/audiographobservable.h \
    src/modularsynth.h \
    src/soundProc/adsrenvelope.h \
    src/soundProc/amplifier.h \
    src/soundProc/glide.h \
    src/soundProc/onepolefilter.h \
    src/soundProc/signal.h \
    src/soundProc/waves.h \
    src/nodes/vco.h \
    src/synthcanvas.h \
    src/modulemenu.h \
    src/nodes/outvca.h \
    src/nodes/common/audiobufferwrapper.h \
    src/nodes/vcf.h \
    src/nodes/common/juceaudionode.h \
    src/nodes/common/juceconnection.h \
    src/nodes/common/audiographmodel.h \
    src/nodes/common/socket.h \
    src/nodes/common/settingsobserver.h \
    src/synthmodel.h \
    src/nodes/common/ongoingconnection.h



