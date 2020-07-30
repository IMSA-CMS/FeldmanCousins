#pragma once
//BasicChannel.h: A basic implementation of the template described in FirstAnalyticParticleGraph.h.
class BasicChannel
{
    public:
        //Gets mu for a given beta.
        double getMu(double beta) const;

        //Gets observed value.
        int getObserved() const { return observed; }

        //Builds a BasicChannel from the parameters passed in.
        BasicChannel(double iConstCoeff, double iLinCoeff, double iSqCoeff, double iBgOne, double iBgTwo, int iObserved);
    private:
        double constCoeff, linCoeff, sqCoeff, bgOne, bgTwo;
        int observed;
};

