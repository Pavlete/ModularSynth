#pragma once

#include <JuceHeader.h>

class SynthModel;

struct Connection
{
    Connection(const Connection& mod);

    int connectionOutID() const;
    int connectionOutPort() const;
    int connectionInID() const;
    int connectionInPort() const;

    SynthModel getParentSynth();

private:
    Connection(ValueTree& module);
    ValueTree m_tree;

    friend class SynthModel;
};
