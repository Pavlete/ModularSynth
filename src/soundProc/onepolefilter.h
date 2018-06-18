#ifndef ONEPOLEFILTER_H
#define ONEPOLEFILTER_H

class OnePoleFilter
{
public:
    OnePoleFilter( );

    void setCutFrequency(int freq);
    float filter(float sample);

private:
    float m_a0, m_b1, m_memCell;
};

#endif // ONEPOLEFILTER_H
