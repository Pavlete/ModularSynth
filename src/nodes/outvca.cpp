#include "outvca.h"

OutVCA::OutVCA()
    : JuceAudioNode (1,1)
    , m_amplitude (0.25f)
{}

void OutVCA::process()
{
    auto inBuffer = getInputData(SignalInput);
    auto outBuffer = getOutputData(SignalOutput);
    if(!outBuffer || !inBuffer)
    {
        return;
    }

    for(int i = 0; i < inBuffer->numberOfSamples(); i++)
    {
        auto nextLeftSample = inBuffer->buffer()->getSample(0,i + inBuffer->startSamples()) * m_amplitude;
        auto nextRightSample = inBuffer->buffer()->getSample(1,i + inBuffer->startSamples()) * m_amplitude;
        outBuffer->buffer()->addSample(0, i + outBuffer->startSamples(), nextLeftSample);
        outBuffer->buffer()->addSample(1, i + outBuffer->startSamples(), nextRightSample);
    }
}


OutVCA_GUI::OutVCA_GUI(NodeModel& model)
    :UIAudioNode (model, 1, 0)
{}

void OutVCA_GUI::paintContent(Graphics &g)
{
    auto local = getLocalBounds();
    g.setColour(juce::Colours::lightgrey);
    Font f (25, Font::bold);
    g.setFont(f);
    g.drawText("VCA", getLocalBounds(), juce::Justification::centred);
}

GraphFactory OutVCA_GUI::getModule()
{
    return [](){return std::make_unique<OutVCA>();};
}
