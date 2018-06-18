#pragma once

#include <JuceHeader.h>

class AudioBufferWrapper
{
public:
    AudioBufferWrapper();

    AudioBufferWrapper(AudioBuffer<float> *out, int samples, int start);

    ~AudioBufferWrapper();

    int numberOfSamples() const {return m_numSamples;}
    int startSamples() const {return m_startSample;}
    AudioBuffer<float>* buffer() {return m_outputBuffer;}

private:
    AudioBuffer<float> *m_outputBuffer;

    static int m_startSample;
    static int m_numSamples;

    bool m_selfManaged;
};
