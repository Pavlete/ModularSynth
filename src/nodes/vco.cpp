#include "vco.h"

#include "../soundProc/waves.h"

VCO::VCO() : JuceAudioNode(0,1), m_signal(44100)
{
    m_signal.setWaveFunction(waveforms::sin);
}

void VCO::setActive(float freq)
{
    m_signal.setFrequency(static_cast<int>(freq));
    m_active = true;
}

void VCO::process()
{    
    auto buf = getOutputData(0);
    if(!buf)
    {
        return;
    }
    buf->buffer()->clear();
    for(int i = 0; i < buf->numberOfSamples(); i++)
    {
        buf->buffer()->addSample(0,i + buf->startSamples(), m_signal.nextSample()*0.25f);
    }
}

VCO_GUI::VCO_GUI(NodeModel& model)
    : UIAudioNode (model, 0,1)
{}

void VCO_GUI::paintContent(Graphics &g)
{
    auto local = getLocalBounds();
    g.setColour(juce::Colours::grey);
    Font f (25, Font::bold);
    g.setFont(f);
    g.drawText("VCO", getLocalBounds(), juce::Justification::centred);
}

GraphFactory VCO_GUI::getModule()
{
    return [](){return std::make_unique<VCO>();};
}
