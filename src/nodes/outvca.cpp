#include "outvca.h"

#include "nodefactory.h"

#include "../graph/audionode.h"

namespace
{

REGISTER_FACTORY( , OutVCA)

const static String ModuleName = "OutVCA";

const static int SignalInput = 0;
const static int SignalOutput= 0;

const static String Volume = "volume";
const static int DefaultVolume = -9;

}

void OutVCA_Model::Listener::valueTreePropertyChanged(ValueTree &tree, const Identifier &id)
{
    if(id.toString() == Volume)
    {
        amplitudeChanged(tree.getProperty(id));
    }
}

OutVCA_Model::OutVCA_Model(const ValueTree &tree)
    : Node(tree)
{
    m_tree.setProperty(IDs::IsInitNode, true, nullptr);
    m_tree.setProperty(IDs::InitPort, 0, nullptr);
}


void OutVCA_Model::setVolume(int val)
{
    m_tree.setProperty(Volume, val, nullptr);
}

int OutVCA_Model::getVolume() const
{
    return m_tree.getProperty(Volume, DefaultVolume);
}

std::function<std::unique_ptr<AudioNode> ()> OutVCA_Model::getAudioFactory()
{
    return [&](){return std::make_unique<OutVCA>(this->shared_from_this());};
}

std::function<std::unique_ptr<JuceAudioNode> ()> OutVCA_Model::getUIFactory()
{
    return [&](){return std::make_unique<OutVCA_GUI>(this->shared_from_this());};
}

//----------------------------------------------------------------------------------------//


OutVCA::OutVCA(const std::shared_ptr<OutVCA_Model>& model)
    : AudioNode (1,1)
    , m_model(model)
    , m_amplitudeSetting (model->getVolume())
    , m_settingsChanged(true)
{
    m_model->addListener(this);
}

void OutVCA::updateSettings()
{
    if(m_settingsChanged)
    {
        m_amplitude = Decibels::decibelsToGain<float>(m_amplitudeSetting.load());
        m_settingsChanged = false;
    }
}

void OutVCA::amplitudeChanged(int amplitude)
{
    m_amplitudeSetting = amplitude;
    m_settingsChanged = true;
}

void OutVCA::process()
{
    updateSettings();

    auto inBuffer = getInputData(SignalInput);
    auto outBuffer = getOutputData(SignalOutput);

    if(!outBuffer || !inBuffer)
    {
        return;
    }

    for(int i = 0; i < inBuffer->sampleCount(); i++)
    {
        auto nextLeftSample = inBuffer->sample(0,i) * m_amplitude;
        auto nextRightSample = inBuffer->sample(1,i) * m_amplitude;
        outBuffer->addSample(0, i, nextLeftSample);
        outBuffer->addSample(1, i, nextRightSample);
    }
}


//----------------------------------------------------------------------------------------//


OutVCA_GUI::OutVCA_GUI(const std::shared_ptr<OutVCA_Model>& model)
    : JuceAudioNode (model, 1, 0)
    , m_vcaModel (model)
{
    setSize(125, 125);

    addAndMakeVisible(m_volumeSlider);
    m_volumeSlider.addListener(this);
    m_volumeSlider.setRange(-60, 0, 1);
    m_volumeSlider.setValue(m_vcaModel->getVolume());
}

void OutVCA_GUI::setContent(Rectangle<int> &r)
{
    m_volumeSlider.setBounds(r);
}

void OutVCA_GUI::sliderValueChanged(Slider *slider)
{
    if(slider == &m_volumeSlider)
    {
        m_vcaModel->setVolume(slider->getValue());
    }
}
