#include "vco.h"
#include <chrono>

namespace
{
static const unsigned int AmplitudeModBufferIndex = 0;
static const unsigned int FreqModBufferIndex = 1;
static const unsigned int OutputBufferIndex = 0;
}

void VCO::setActive(float freq, float velocity)
{
    m_frequency = freq;
    m_velocity = velocity;
    m_settingsChanged = true;
}

void VCO::updateSettings()
{
    if(m_settingsChanged)
    {
        m_settingsChanged = false;
        m_signal.setFrequency(m_frequency * exp2f(m_offset));
        m_signal.setWaveFunction(m_waveforms[m_waveIndex]);
    }
}

void VCO::process()
{
    updateSettings();

    auto outputBuffer = getOutputData(OutputBufferIndex);

    if(!SignalOutputBuffer)
    {
        return;
    }

    for(int i = 0; i < outputBuffer->sampleCount(); i++)
    {
        auto sample = m_signal.nextSample() * m_velocity;
        outputBuffer->setSample(0, i, sample * 0.25f);
        outputBuffer->setSample(1, i, sample * 0.25f);
    }
}

//------------------//

VCO_GUI::VCO_GUI(const std::shared_ptr<VCO_Model>& model)
    : JuceAudioNode (model, 2, 1)
    , m_model (model)
    , m_offsetSlider("Semi",
                     {-2, 2, 0.5},
                     this,
                     Slider::RotaryVerticalDrag,
                     model->getOffset())
    , m_waveformSlider("Wave",
                       {0, 3, 1},
                       this,
                       Slider::RotaryVerticalDrag,
                       model->getWaveindex())
{
    using namespace measures;
    setSize(basic_module_width + 2 * rotary_width + controls_margin,
            basic_module_height + rotary_height );

    addAndMakeVisible(m_offsetSlider);
    addAndMakeVisible(m_waveformSlider);
}

void VCO_GUI::setContent(Rectangle<int> &r)
{   
    m_offsetSlider.setBounds(r.removeFromLeft(measures::rotary_width));
    m_waveformSlider.setBounds(r.removeFromRight(measures::rotary_width));
}

void VCO_GUI::sliderValueChanged(Slider *slider)
{
    if(slider == &m_offsetSlider)
    {
        m_model->setOffset(slider->getValue());
    }
    else if(slider == &m_waveformSlider)
    {
        m_model->setWaveindex(slider->getValue());
    }
}
