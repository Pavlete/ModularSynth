# Compiler flags
QMAKE_CXXFLAGS = -Wall
unix:  QMAKE_CXXFLAGS += -I/usr/include/freetype2 -I/usr/include `pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama` -pthread -DLINUX=1
win32: QMAKE_CXXFLAGS += -mstackrealign -D__MINGW__=1 -D__MINGW_EXTENSION=

QMAKE_CXXFLAGS_DEBUG   = -g -ggdb  -O0
QMAKE_CXXFLAGS_RELEASE =  -O3

QMAKE_CFLAGS         = $$QMAKE_CXXFLAGS
QMAKE_CFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE
QMAKE_CFLAGS_DEBUG   = $$QMAKE_CXXFLAGS_DEBUG

INCLUDEPATH = JUCE/modules \
              JuceLibraryCode

# Linker flags
LIBS = -L$$DESTDIR
unix:  LIBS += -L/usr/X11R6/lib/ `pkg-config --libs alsa freetype2 libcurl x11 xext xinerama` -lGL -ldl -lpthread -lrt
win32: LIBS += -lgdi32 -luser32 -lkernel32 -lcomctl32 -lcomdlg32 -limm32 -lole32 -loleaut32 -lopengl32 -lrpcrt4 -lshlwapi -luuid -lversion -lwininet -lwinmm -lws2_32 -lwsock32 -static -lpthread
win32: QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
QMAKE_LFLAGS_DEBUG += -fvisibility=hidden

# Source and header files
SOURCES = \
        "JuceLibraryCode/juce_audio_basics.cpp" \
        "JuceLibraryCode/juce_audio_devices.cpp" \
        "JuceLibraryCode/juce_audio_formats.cpp" \
        "JuceLibraryCode/juce_audio_utils.cpp" \
        "JuceLibraryCode/juce_audio_processors.cpp" \
        "JuceLibraryCode/juce_core.cpp" \
        "JuceLibraryCode/juce_cryptography.cpp" \
        "JuceLibraryCode/juce_data_structures.cpp" \
        "JuceLibraryCode/juce_events.cpp" \
        "JuceLibraryCode/juce_graphics.cpp" \
        "JuceLibraryCode/juce_gui_basics.cpp" \
        "JuceLibraryCode/juce_gui_extra.cpp" \
        "JuceLibraryCode/juce_opengl.cpp" \
        "JuceLibraryCode/juce_video.cpp" \



