#pragma once

#include <JuceHeader.h>

class AudioBufferWrapper
{
public:
    AudioBufferWrapper();
    AudioBufferWrapper(AudioBuffer<float> &out, int samples, int start);

    ~AudioBufferWrapper();

    void clear();

    void addSample(int channel, int sampleNumber, float value);

    float sample(int channel, int sampleNumber) const;
    int sampleCount() const;

private:
    AudioBuffer<float> *m_outputBuffer;

    static int m_startSample;
    static int m_numSamples;

    bool m_selfManaged;
};
