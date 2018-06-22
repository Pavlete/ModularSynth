#include "vcf.h"

namespace
{
const int SignalInput = 0;
const int SignalOutput= 0;
}

VCF::VCF() : JuceAudioNode (1,1)
{

}

void VCF::setActive(float freq)
{
   m_filter.setCutFrequency(freq * 50.0f);
}

void VCF::process()
{
    auto inBuffer = getInputData(SignalInput);
    auto outBuffer = getOutputData(SignalOutput);
    if(!outBuffer || !inBuffer)
    {
        return;
    }

    outBuffer->buffer()->clear();

    for(int i = 0; i < inBuffer->numberOfSamples(); i++)
    {
        auto next = m_filter.filter(inBuffer->buffer()->getSample(0,i + outBuffer->startSamples()));
        outBuffer->buffer()->addSample(0, i + inBuffer->startSamples(), next);
        outBuffer->buffer()->addSample(1, i + inBuffer->startSamples(), next);
    }
}

VCF_GUI::VCF_GUI(NodeModel &model)
    :UIAudioNode (model, 1, 1)
{

}

void VCF_GUI::paintContent(Graphics &g)
{
    auto local = getLocalBounds();
    g.setColour(juce::Colours::lightgrey);
    Font f (25, Font::bold);
    g.setFont(f);
    g.drawText("VCF", getLocalBounds(), juce::Justification::centred);
}

GraphFactory VCF_GUI::getModule()
{
    return [](){return std::make_unique<VCF>();};
}
