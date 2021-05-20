#ifndef FREQUENTIST_ANALYTIC_ANALYSIS_DEBUGCHANNEL_H
#define FREQUENTIST_ANALYTIC_ANALYSIS_DEBUGCHANNEL_H
// DebugChannel.h: A debug channel which can be used to inject data used for debugging.
// Look at BasicChannel.h for specifications on implementing a Channel
class DebugChannel
{
    public:
        //Returns "beta" right back, with background added in.
        double getMu(double beta) const { return beta; }

        //Gets the observed value.
        int getObserved() const { return observed; }

        //Builds a DebugChannel from the parameters passed in.
        DebugChannel(int iObserved, int iBackground) : observed(iObserved), background(iBackground) {};

    private:
        int observed, background;
};
#endif