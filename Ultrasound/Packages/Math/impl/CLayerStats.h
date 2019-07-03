// CLayerStats.h

//
// CLayerStats - stores media stats info for a height range
//

class CLayerStats : public object
{
public:
    CLayerStats();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Average Noise 
    num x_numNormalizedNoiseLevel = 0
            xauto(Get, Set)
            xassert(value >= 0);

    // Average Signal
    num x_numNormalizedSignalLevel = 0
            xauto(Get, Set)
            xassert(value >= 0);

    // Last update 'time'
    int x_nLastUpdateHorizontalRecalcCounter = -oo
            xauto(Get, Set)
            xassert(value < off);

// Operations

// UI

protected:
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
};
