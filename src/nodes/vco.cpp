#include "vco.h"

#include "../soundProc/waves.h"

namespace
{

REGISTER_FACTORY(Oscillator, VCO)

const static String ModuleName = "VCO";

const static int AmplitudeMod = 0;
const static int FreqMod = 1;

const static int SignalOutput = 0;

const static String Offset = "offset";
const static float DefaultOffset = 0;

const static String Waveindex = "waveIndex";
const static int DefaultWaveindex = 0;

}

//------------------//


void VCO_Model::Listener::valueTreePropertyChanged(ValueTree &tree, const Identifier &id)
{
    if(id.toString() == Offset)
    {
        offsetChanged(tree.getProperty(id));
    }
    else if(id.toString() == Waveindex)
    {
        waveIndexChanged(tree.getProperty(id));
    }
}

VCO_Model::VCO_Model(int x, int y)
    : Node(x, y, ModuleName)
{ }

VCO_Model::VCO_Model(const ValueTree &tree)
    : Node(tree)
{ }

void VCO_Model::setOffset(float val)
{
    m_tree.setProperty(Offset, val, nullptr);
}

float VCO_Model::getOffset() const
{
    return m_tree.getProperty(Offset, DefaultOffset);
}

void VCO_Model::setWaveindex(int val)
{
    m_tree.setProperty(Waveindex, val, nullptr);
}

int VCO_Model::getWaveindex() const
{
    return m_tree.getProperty(Waveindex, DefaultWaveindex);
}

std::function<std::unique_ptr<AudioGraphNode> ()> VCO_Model::getAudioFactory()
{
    return [this](){return std::make_unique<VCO>(shared_from_this());};
}

std::function<std::unique_ptr<JuceAudioNode> ()> VCO_Model::getUIFactory()
{
    return [this](){return std::make_unique<VCO_GUI>(shared_from_this());};
}


//------------------//

VCO::VCO(const std::shared_ptr<VCO_Model>& model)
    : AudioGraphNode (2, 1)
    , m_model (model)
    , m_offset (model->getOffset())
    , m_waveIndex (model->getWaveindex())
    , m_signal (44100)
    , m_waveforms {waveforms::sin,
                   waveforms::square,
                   waveforms::sawtooth,
                   waveforms::triangle}
{
    m_model->addListener(this);
}

void VCO::setActive(float freq)
{
    m_frequency = freq;
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

void VCO::offsetChanged(float offset)
{
    m_offset = offset;
    m_settingsChanged = true;
}

void VCO::waveIndexChanged(int index)
{
    m_waveIndex = index;
    m_settingsChanged = true;
}

void VCO::process()
{
    updateSettings();

    auto AmplitudeModBuffer = getInputData(AmplitudeMod);
    auto FreqModBuffer = getInputData(FreqMod);

    auto SignalOutputBuffer = getOutputData(SignalOutput);

    if(!SignalOutputBuffer)
    {
        return;
    }

    SignalOutputBuffer->buffer()->clear();

    for(int i = 0; i < SignalOutputBuffer->numberOfSamples(); i++)
    {
        auto sample = m_signal.nextSample();
        SignalOutputBuffer->buffer()->addSample(0,i + SignalOutputBuffer->startSamples(), sample*0.25f);
        SignalOutputBuffer->buffer()->addSample(1,i + SignalOutputBuffer->startSamples(), sample*0.25f);
    }
}

//------------------//

VCO_GUI::VCO_GUI(const std::shared_ptr<VCO_Model>& model)
    : JuceAudioNode (model, 2, 1)
    , m_model (model)
{
    setSize(300, 125);

    addAndMakeVisible(m_offsetSlider);
    m_offsetSlider.addListener(this);
    m_offsetSlider.setRange(-2, 2, 0.5);
    m_offsetSlider.setValue(m_model->getOffset());

    addAndMakeVisible(m_waveformSlider);
    m_waveformSlider.addListener(this);
    m_waveformSlider.setRange(0, 3, 1);
    m_waveformSlider.setValue(m_model->getWaveindex());
}

void VCO_GUI::setContent(Rectangle<int> &r)
{
    m_offsetSlider.setBounds(r.removeFromTop(getHeight()/2));
    m_waveformSlider.setBounds(r);
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
