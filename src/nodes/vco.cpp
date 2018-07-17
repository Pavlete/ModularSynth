#include <cmath>

#include "vco.h"

#include "../soundProc/waves.h"

VCO::VCO(SettingsSubject<VCOSettings>& subject)
    : processGraph::AudioProcessGraph<AudioBufferWrapper>::AudioNode(0,1)
    , SettingsObserver<VCOSettings>(subject)
    , m_signal(44100)
    , m_waveforms {waveforms::sin,
                   waveforms::square,
                   waveforms::sawtooth,
                   waveforms::triangle}
{
    m_signal.setWaveFunction(waveforms::sin);
}

void VCO::setActive(float freq)
{
    m_signal.setFrequency(freq * getSettings()->pitchOfset);
    m_frequency = freq;
}

void VCO::process()
{
    updateSettings();

    auto buf = getOutputData(0);
    if(!buf)
    {
        return;
    }
    buf->buffer()->clear();

    for(int i = 0; i < buf->numberOfSamples(); i++)
    {
        auto sample = m_signal.nextSample();
        buf->buffer()->addSample(0,i + buf->startSamples(), sample*0.25f);
        buf->buffer()->addSample(1,i + buf->startSamples(), sample*0.25f);
    }
}

void VCO::updateSettings()
{
    if(m_settingsChanged)
    {
        auto settings = getSettings();
        m_signal.setFrequency(m_frequency * settings->pitchOfset);
        m_signal.setWaveFunction(m_waveforms.at(settings->signalWave));
        m_settingsChanged = false;
    }
}


//----------------------------------------------------------//



VCO_GUI::VCO_GUI(Node& model)
    : JuceAudioNode (model, 0,1)

{
    setSize(300, 125);

    addAndMakeVisible(m_offsetSlider);
    m_offsetSlider.addListener(this);
    m_offsetSlider.setRange(-2, 2, 0.1);
    m_offsetSlider.setValue(0);

    addAndMakeVisible(m_waveformSlider);
    m_waveformSlider.addListener(this);
    m_waveformSlider.setRange(0, 3, 1);
    m_waveformSlider.setValue(0);
}

void VCO_GUI::setContent(Rectangle<int> &r)
{
    m_offsetSlider.setBounds(r.removeFromTop(getHeight()/2));
    m_waveformSlider.setBounds(r);
}

GraphFactory VCO_GUI::getModule()
{
    return [&](){return std::make_unique<VCO>(*this);};
}

void VCO_GUI::sliderValueChanged(Slider *slider)
{
    if(slider == &m_offsetSlider)
    {
        setParameter(exp2f(static_cast<float>(slider->getValue())),
                 &VCOSettings::pitchOfset);
    }
    else if(slider == &m_waveformSlider)
    {
        setParameter(slider->getValue(),
                 &VCOSettings::signalWave);
    }
}
