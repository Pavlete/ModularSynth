#pragma once

#include <JuceHeader.h>

class AudioBufferWrapper
{
public:
    AudioBufferWrapper();
    AudioBufferWrapper(AudioBuffer<float> &out, int samples, int start);

    ~AudioBufferWrapper();

    void clear();

    inline void setSample(int channel, int sampleNumber, float value)
    {
        m_selfManaged? m_outputBuffer->setSample(channel, sampleNumber + m_startSample, value)
                     : m_outputBuffer->addSample(channel, sampleNumber + m_startSample, value) ;
    }

    inline float sample(int channel, int sampleNumber) const
    {
        return m_outputBuffer->getSample(channel, sampleNumber + m_startSample);
    }

    int sampleCount() const;

private:
    AudioBuffer<float> *m_outputBuffer;

    static int m_startSample;
    static int m_numSamples;

    const bool m_selfManaged;
};
