#pragma once

#include "../UI/nodes_common/juceaudionode.h"
#include "../proccess_graph/audionode.h"

struct OutVCA_Model
    : public Node
    , public std::enable_shared_from_this<OutVCA_Model>
{
    class Listener : public Node::Listener
    {
    public:
        virtual void amplitudeChanged(float amplitude) = 0;
    private:
        void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    };

    OutVCA_Model(const ValueTree&);

    void setAmplitude(float value);
    float getAmplitude() const;

    std::function<std::unique_ptr<AudioNode> ()> getAudioFactory() override;
    std::function<std::unique_ptr<JuceAudioNode> ()> getUIFactory() override;

};

//------------------//

class OutVCA
    : public AudioNode
    , public OutVCA_Model::Listener
{
public:
    OutVCA(const std::shared_ptr<OutVCA_Model>&);

    void process() override;
private:
    std::shared_ptr<OutVCA_Model> m_model;
    std::atomic<float> m_amplitude;
    std::atomic_bool m_settingsChanged;

    void amplitudeChanged(float amplitude) override;

    void updateSettings();

    const unsigned int InputBufferIndex = 0;
    const unsigned int OutputBufferIndex = 0;

    //Place your own variables
    float m_amplitudeSetting;
};

//------------------//

class OutVCA_GUI
    : public JuceAudioNode
    , public Slider::Listener
{
public:
    OutVCA_GUI(const std::shared_ptr<OutVCA_Model>&);

    void setContent(Rectangle<int> &r) override;
    void sliderValueChanged(Slider *slider) override;
private:
    std::shared_ptr<OutVCA_Model> m_model;

    Slider m_volumeSlider;
};


