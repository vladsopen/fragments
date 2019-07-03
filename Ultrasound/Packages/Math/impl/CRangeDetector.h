// CRangeDetector.h
// ("CRangeDetector.cpp")

//
// CRangeDetector - detects average min/max amplitude levels 
//   in live realtime signal flow curve.
//

class CRangeDetector : public object
{
public:
    CRangeDetector();

// Currently detected levels

    int x_nCurrentMin = -1
        xauto(Get)
        xassert(value >= 0);

    int x_nCurrentMax = -1
        xauto(Get)
        xassert(value >= 0);

// Next signal point

    // Accumulate the next point, update x_nCurrentMin/x_nCurrentMax every nHalfTimePeriod
    void FeedNextPoint(
            int nAmplitude,
            int nHalfTimePeriod);

    // Can read x_nCurrentMin/x_nCurrentMax only after FeedNextPoint() call
    bool IsOutputValid();

private:

    // Updated range will be used after nHalfTimePeriod passed
    bool m_bOutputValid = false;
    int m_nNewMin1stHalf = -1;
    int m_nNewMax1stHalf = -1;
    int m_nNewMin2ndHalf = -1;
    int m_nNewMax2ndHalf = -1;

    // Time counter
    int m_nTimeCounter = oo;

    void UpdateMinMax(
            int nAmplitude,
            out int& out_nMin,
            out int& out_nMax);
};
