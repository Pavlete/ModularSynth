#include "audiobufferwrapper.h"

int AudioBufferWrapper::m_startSample;
int AudioBufferWrapper::m_numSamples;

AudioBufferWrapper::AudioBufferWrapper()
    : m_outputBuffer(new AudioBuffer<float>(16,1024))
    , m_selfManaged(true)
{}

AudioBufferWrapper::AudioBufferWrapper(AudioBuffer<float> *out, int samples, int start)
    : m_outputBuffer(out)
    , m_selfManaged(false)
{
    m_startSample = start;
    m_numSamples = samples;
}

AudioBufferWrapper::~AudioBufferWrapper()
{
    if(m_selfManaged)
    {
        delete m_outputBuffer;
    }
}
