#include "vco.h"

#include "../data_models/nodefactory.h"
#include "../sound_proccesing/waves.h"

namespace
{
REGISTER_FACTORY(Oscillator, VCO)

using namespace measures;

}


VCO::VCO(const std::shared_ptr<VCO_Model>& model)
    : AudioNode (2, 1)
    , m_model (model)
    , m_offset (model->getOffset())
    , m_waveIndex (model->getWaveindex())
    , m_signal (44100)
    , m_frequency (0.0)
    , m_waveforms {waveforms::sin,
                   waveforms::square,
                   waveforms::sawtooth,
                   waveforms::triangle}
{
    m_model->addListener(this);
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
        m_signal.setFrequency(m_frequency * exp2f(m_offset));
        m_signal.setWaveFunction(m_waveforms[m_waveIndex]);
        m_settingsChanged = false;
    }
}

void VCO::process()
{
    updateSettings();

    if(!isActive())
        return;

    auto SignalOutputBuffer = getOutputData(OutputBufferIndex);

    if(!SignalOutputBuffer)
    {
        return;
    }

    SignalOutputBuffer->clear();

    for(int i = 0; i < SignalOutputBuffer->sampleCount(); i++)
    {
        auto sample = m_signal.nextSample() * m_velocity;
        SignalOutputBuffer->addSample(0, i, sample * 0.25f);
        SignalOutputBuffer->addSample(1, i, sample * 0.25f);
    }
}

//------------------//

VCO_GUI::VCO_GUI(const std::shared_ptr<VCO_Model>& model)
    : JuceAudioNode (model, 2, 1)
    , m_model (model)
{
    using namespace measures;
    setSize(basic_module_width + 2 * rotary_width + controls_margin,
            basic_module_height + rotary_height );

    addAndMakeVisible(m_offsetSlider);
    m_offsetSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    m_offsetSlider.setTextBoxStyle (Slider::TextBoxBelow, false, 0, 0);
    m_offsetSlider.addListener(this);
    m_offsetSlider.setRange(-2, 2, 0.5);
    m_offsetSlider.setValue(m_model->getOffset());
    m_offsetSlider.setLookAndFeel(&m_style);
    m_offsetSlider.setName("Offset");

    addAndMakeVisible(m_waveformSlider);
    m_waveformSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    m_waveformSlider.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
    m_waveformSlider.addListener(this);
    m_waveformSlider.setRange(0, 3, 1);
    m_waveformSlider.setValue(m_model->getWaveindex());
    m_waveformSlider.setLookAndFeel(&m_style);
    m_waveformSlider.setName("Wave");
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
