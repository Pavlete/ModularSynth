#pragma once

#include "../UI/common/juceaudionode.h"
#include "../proccess_graph/audionode.h"

struct Mixer_Model
    : public Node
    , public std::enable_shared_from_this<Mixer_Model>
{
    class Listener : public Node::Listener
    {
    public:
        virtual void input1AmplitudeChanged(int input1Amplitude) = 0;
        virtual void input2AmplitudeChanged(int input2Amplitude) = 0;
    private:
        void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    };

    Mixer_Model(const ValueTree&);

    void setInput1amplitude(int value);
    int getInput1amplitude() const;

    void setInput2amplitude(int value);
    int getInput2amplitude() const;

    std::function<std::unique_ptr<AudioNode> ()> getAudioFactory() override;
    std::function<std::unique_ptr<JuceAudioNode> ()> getUIFactory() override;
};

//------------------//

class Mixer
    : public AudioNode
    , public Mixer_Model::Listener
{
public:
    Mixer(const std::shared_ptr<Mixer_Model>&);

    void process() override;
private:
    std::shared_ptr<Mixer_Model> m_model;
    std::atomic<int> m_input1Amplitude;
    std::atomic<int> m_input2Amplitude;
    std::atomic_bool m_settingsChanged;

    void input1AmplitudeChanged(int input1Amplitude) override;
    void input2AmplitudeChanged(int input2Amplitude) override;

    void updateSettings();

    const unsigned int Input1BufferIndex = 0;
    const unsigned int Input2BufferIndex = 1;
    const unsigned int OutputBufferIndex = 0;

    //Place your own variables
    float m_input1attenuation;
    float m_input2attenuation;

};

//------------------//

class Mixer_GUI
    : public JuceAudioNode
    , public Slider::Listener
{
public:
    Mixer_GUI(const std::shared_ptr<Mixer_Model>&);

    void setContent(Rectangle<int> &r) override;
    void sliderValueChanged(Slider *slider) override;
private:
    std::shared_ptr<Mixer_Model> m_model;

    Slider m_input1Slider;
    Slider m_input2Slider;
};
