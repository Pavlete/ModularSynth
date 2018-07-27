#pragma once

#include <JuceHeader.h>

#include "audiobufferwrapper.h"
#include "../../graph/audioprocessgraph.h"

using AudioGraphNode = processGraph::AudioProcessGraph<AudioBufferWrapper>::AudioNode;
class JuceAudioNode;
class SynthModel;

struct Node
{
    Node(int x, int y, String type);
    Node(const Node& mod);

    class Listener : public ValueTree::Listener
    {
        void valueTreeChildAdded (ValueTree &, ValueTree &) override {}
        void valueTreeChildRemoved (ValueTree &, ValueTree &, int) override{}
        void valueTreeChildOrderChanged (ValueTree &, int, int ) override {}
        void valueTreeParentChanged  (ValueTree &) override {}
    };

    virtual ~Node() = default;

    CachedValue<int> moduleX;
    CachedValue<int> moduleY;
    CachedValue<int> moduleId;

    void addListener(Listener* listener);

    bool isInitModule();
    int initPort();

    SynthModel getParentSynth();

    virtual  std::function<std::unique_ptr<AudioGraphNode>()> getAudioFactory() = 0;
    virtual std::function<std::unique_ptr<JuceAudioNode>()> getUIFactory() = 0;

    ValueTree m_tree;
protected:
    Node(const ValueTree& module);

private:

    void initCached();

    friend class SynthModel;
};
