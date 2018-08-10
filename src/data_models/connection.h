#pragma once

#include <JuceHeader.h>

class SynthModel;

struct Connection
{
    int outputNode() const;
    unsigned int outputPort() const;
    int inputNode() const;
    unsigned int inputPort() const;

    SynthModel getParentSynth();

private:
    Connection(ValueTree& module);
    ValueTree m_tree;

    friend class SynthModel;
};
