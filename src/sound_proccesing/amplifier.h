#pragma once

#include <math.h>
#include <algorithm>
#include <atomic>

class Amplifier
{
public:
    Amplifier( )
      : m_amplificationFactor(0.2)
    {}

    void setAmplificationFactorDb(const double volumeDb)
    {
        m_amplificationFactor.store(std::min< double >(pow(10, volumeDb / 20), 1));
    }

    double getAmplificationFactorDb( ) const { return 20 * log10(m_amplificationFactor.load( )); }

    void setAmplificationFactor(const double volume)
    {
        m_amplificationFactor.store(std::min< double >(volume, 1));
    }

    double getAmplificationFactor( ) const { return m_amplificationFactor.load( ); }

    double doAmplify(double input) { return input * m_amplificationFactor.load( ); }

private:
    std::atomic< double > m_amplificationFactor;
};
