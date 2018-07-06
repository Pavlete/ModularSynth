#pragma once

#include <JuceHeader.h>

#include "modularsynth.h"
#include "modularvoice.h"
#include "synthcanvas.h"


//==============================================================================

class MainContentComponent   : public AudioAppComponent,
                               private Timer
{
public:
    MainContentComponent();

    ~MainContentComponent();

    void resized() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

private:
    void timerCallback() override;

    void setMidiInput (int index);

    //==========================================================================
    JuceGraphModel m_synthPath;

    ModularSynth m_modularSynth;
    MidiKeyboardState m_keyboardState;
    MidiKeyboardComponent m_keyboardComponent;
    SynthCanvas m_canvas;

    ComboBox m_midiInputList;
    Label m_midiInputListLabel;
    int m_lastInputIndex = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
