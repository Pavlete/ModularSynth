#include "outvca.h"

#include "../data_models/nodefactory.h"

namespace
{
REGISTER_FACTORY( , OutVCA)
}


void OutVCA::updateSettings()
{
    if(m_settingsChanged)
    {
        m_settingsChanged = false;
        m_amplitudeSetting = Decibels::decibelsToGain<float>(m_amplitude.load());
    }
}

void OutVCA::process()
{
    updateSettings();

    auto inBuffer = getInputData(InputBufferIndex);
    auto outBuffer = getOutputData(OutputBufferIndex);

    if(!outBuffer || !inBuffer)
    {
        return;
    }

    for(int i = 0; i < inBuffer->sampleCount(); i++)
    {
        auto nextLeftSample = inBuffer->sample(0,i) * m_amplitudeSetting;
        auto nextRightSample = inBuffer->sample(1,i) * m_amplitudeSetting;
        outBuffer->addSample(0, i, nextLeftSample);
        outBuffer->addSample(1, i, nextRightSample);
    }
}


//----------------------------------------------------------------------------------------//


OutVCA_GUI::OutVCA_GUI(const std::shared_ptr<OutVCA_Model>& model)
    : JuceAudioNode (model, 1, 0)
    , m_model (model)
{
    setSize(125, 125);

    addAndMakeVisible(m_volumeSlider);
    m_volumeSlider.addListener(this);
    m_volumeSlider.setRange(-60, 0, 1);
    m_volumeSlider.setValue(m_model->getAmplitude());
}

void OutVCA_GUI::setContent(Rectangle<int> &r)
{
    m_volumeSlider.setBounds(r);
}

void OutVCA_GUI::sliderValueChanged(Slider *slider)
{
    if(slider == &m_volumeSlider)
    {
        m_model->setAmplitude(slider->getValue());
    }
}
