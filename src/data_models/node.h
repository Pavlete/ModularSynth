#pragma once

#include <JuceHeader.h>

class JuceAudioNode;
class AudioNode;
class SynthModel;

struct Node
{
    class Listener : public ValueTree::Listener
    {
        void valueTreeChildAdded (ValueTree &, ValueTree &) override {}
        void valueTreeChildRemoved (ValueTree &, ValueTree &, int) override{}
        void valueTreeChildOrderChanged (ValueTree &, int, int ) override {}
        void valueTreeParentChanged  (ValueTree &) override {}
    };

    virtual ~Node() = default;

    int x() const;
    int y() const;
    int ID() const;

    String name() const;

    void setX(int x);
    void setY(int y);


    bool isInitModule() const;
    int initPort() const;

    void addListener(Listener* listener);

    SynthModel getParentSynth();

    virtual std::function<std::unique_ptr<AudioNode>()> getAudioFactory() = 0;
    virtual std::function<std::unique_ptr<JuceAudioNode>()> getUIFactory() = 0;

protected:
    Node(const ValueTree& module);

    ValueTree m_tree;

    friend class SynthModel;
};
