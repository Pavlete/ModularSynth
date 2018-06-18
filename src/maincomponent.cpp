#include "maincomponent.h"

#include "nodes/vco.h"


MainContentComponent::MainContentComponent()
    : m_modularSynth  (m_keyboardState, m_synthPath.getObservable())
    , m_keyboardComponent (m_keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    , m_canvas (m_synthPath)
{
    addAndMakeVisible (m_midiInputListLabel);
    m_midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
    m_midiInputListLabel.attachToComponent (&m_midiInputList, true);

    auto midiInputs = MidiInput::getDevices();
    addAndMakeVisible (m_midiInputList);
    m_midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
    m_midiInputList.addItemList (midiInputs, 1);

    for (auto midiInput : midiInputs)
    {
        if (deviceManager.isMidiInputEnabled (midiInput))
        {
            setMidiInput (midiInputs.indexOf (midiInput));
            break;
        }
    }

    if (m_midiInputList.getSelectedId() == 0)
        setMidiInput (0);

    addAndMakeVisible (m_keyboardComponent);
    setAudioChannels (0, 2);

    addAndMakeVisible(m_canvas);

    setSize (800, 600);
    startTimer (400);
}

MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
}

void MainContentComponent::resized()
{
    m_midiInputList    .setBounds (200, 10, getWidth() - 210, 20);
    m_keyboardComponent.setBounds (10,  40, getWidth() - 20, 200);
    m_canvas.setBounds (10,  220, getWidth() - 20, getHeight() - 220);
}

void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    m_modularSynth.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    m_modularSynth.getNextAudioBlock (bufferToFill);
}

void MainContentComponent::releaseResources()
{
    m_modularSynth.releaseResources();
}

void MainContentComponent::timerCallback()
{
    m_keyboardComponent.grabKeyboardFocus();
    stopTimer();
}

void MainContentComponent::setMidiInput(int index)
{
    auto list = MidiInput::getDevices();

    deviceManager.removeMidiInputCallback (list[m_lastInputIndex], m_modularSynth.getMidiCollector());

    auto newInput = list[index];

    if (! deviceManager.isMidiInputEnabled (newInput))
        deviceManager.setMidiInputEnabled (newInput, true);

    deviceManager.addMidiInputCallback (newInput, m_modularSynth.getMidiCollector());
    m_midiInputList.setSelectedId (index + 1, dontSendNotification);

    m_lastInputIndex = index;
}
