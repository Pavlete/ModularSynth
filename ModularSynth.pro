# Automatically generated qmake file, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

TEMPLATE = app
CONFIG  += warn_off
CONFIG  += c++14

include(Juce.pri)

# Source and header files
SOURCES += \
    src/maincomponent.cpp \
    src/main.cpp \
    src/data_models/connection.cpp \
    src/data_models/node.cpp \
    src/data_models/synthmodel.cpp \
    src/proccess_graph/audiobufferwrapper.cpp \
    src/proccess_graph/audionode.cpp \
    src/proccess_graph/processgraph.cpp \
    src/sound_proccesing/adsrenvelope.cpp \
    src/sound_proccesing/glide.cpp \
    src/sound_proccesing/modularsynth.cpp \
    src/sound_proccesing/modularvoice.cpp \
    src/sound_proccesing/onepolefilter.cpp \
    src/sound_proccesing/signal.cpp \
    src/UI/nodes_common/juceaudionode.cpp \
    src/UI/nodes_common/juceconnection.cpp \
    src/UI/nodes_common/ongoingconnection.cpp \
    src/UI/nodes_common/socket.cpp \
    src/UI/modulemenu.cpp \
    src/UI/synthcanvas.cpp \
    src/synth_nodes/mixer.cpp \
    src/synth_nodes/mixer_base.cpp \
    src/synth_nodes/outvca.cpp \
    src/synth_nodes/outvca_base.cpp \
    src/synth_nodes/vco.cpp \
    src/synth_nodes/vco_base.cpp



HEADERS += \
    src/maincomponent.h \
    src/modularvoice.h \
    src/data_models/connection.h \
    src/data_models/identifiers.h \
    src/data_models/node.h \
    src/data_models/nodefactory.h \
    src/data_models/synthmodel.h \
    src/proccess_graph/audiobufferwrapper.h \
    src/proccess_graph/audionode.h \
    src/proccess_graph/processgraph.h \
    src/sound_proccesing/adsrenvelope.h \
    src/sound_proccesing/amplifier.h \
    src/sound_proccesing/glide.h \
    src/sound_proccesing/modularsynth.h \
    src/sound_proccesing/modularvoice.h \
    src/sound_proccesing/onepolefilter.h \
    src/sound_proccesing/signal.h \
    src/sound_proccesing/waves.h \    
    src/UI/nodes_common/juceaudionode.h \
    src/UI/nodes_common/juceconnection.h \
    src/UI/nodes_common/ongoingconnection.h \
    src/UI/nodes_common/socket.h \
    src/UI/modulemenu.h \
    src/UI/synthcanvas.h \
    src/synth_nodes/mixer.h \
    src/synth_nodes/outvca.h \
    src/synth_nodes/vco.h


