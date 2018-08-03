#pragma once

#include "common/juceaudionode.h"

struct OutVCA_Model
        : public Node
        , public std::enable_shared_from_this<OutVCA_Model>
{
    class Listener: public Node::Listener
    {
        virtual void amplitudeChanged(int amplitude) = 0;
    private:
        void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    };

    OutVCA_Model(const ValueTree&);

    void setVolume(int val);
    int getVolume() const;

    std::function<std::unique_ptr<AudioNode> ()> getAudioFactory() override;
    std::function<std::unique_ptr<JuceAudioNode> ()> getUIFactory() override;
};

class OutVCA
        : public AudioNode
        , public OutVCA_Model::Listener
{
public:
    OutVCA(const std::shared_ptr<OutVCA_Model>&);

    void process() override;

private:
    std::shared_ptr<OutVCA_Model> m_model;
    std::atomic_int m_amplitudeSetting;
    std::atomic_bool m_settingsChanged;

    void amplitudeChanged(int amplitude) override;

    void updateSettings();

    float m_amplitude;
};


class OutVCA_GUI
        : public JuceAudioNode
        , public SliderListener
{
public:
    OutVCA_GUI(const std::shared_ptr<OutVCA_Model>& model);

    void setContent(Rectangle<int> &r) override;
    Slider m_volumeSlider;

    void sliderValueChanged(Slider *slider) override;

private:
    std::shared_ptr<OutVCA_Model> m_vcaModel;
};


