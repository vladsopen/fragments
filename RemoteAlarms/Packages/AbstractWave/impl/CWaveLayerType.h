// CWaveLayerType.h
#pragma once

//
// CWaveLayerType -
//
//   Background graphics, wave, markers...
//
//ASSUME_IMPL_FOR_IFACE(CWaveLayerTypeImpl, CWaveLayerType)

class CWaveLayerType : public typeobject
{
public:
    CWaveLayerType();
    //NEW_LOCAL(CWaveLayerTypeImpl, CWaveLayerType)
    //NEW_GP(Ralarm, CWaveLayerTypeImpl, CWaveLayerType)
    DECLARE_NULL_TYPE(GetDerivedList);

// Constants

// Attributes

    // Show/hide data in the layer
    bool IsLayerForWave()
            vhook;
    bool IsLayerForMarker()
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CWaveLayerType> GetDerivedList();

// UI

    type<CWaveLayerType> AsWaveLayerIsWave()
            vhook;

    type<CWaveLayerType> AsWaveLayerIsMarkers()
            vhook;

protected:
    virtual bool OnIsLayerForWave()
            v1pure;
    virtual bool OnIsLayerForMarker()
            v1pure;

    virtual type<CWaveLayerType> OnAsWaveLayerIsWave()
            v1null;
    virtual type<CWaveLayerType> OnAsWaveLayerIsMarkers()
            v1null;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // Wave
    class CWaveLayerIsWave : public CWaveLayerType
    {
    public:
        void flag_List_as_WaveLayerType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(W a v e);

        virtual bool OnIsLayerForWave()
                return
                    true;

        virtual bool OnIsLayerForMarker()
                return
                    false;

    };

    // Markers
    class CWaveLayerIsMarkers : public CWaveLayerType
    {
    public:
        void flag_List_as_WaveLayerType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(M a r k);

        virtual bool OnIsLayerForWave()
                return
                    false;

        virtual bool OnIsLayerForMarker()
                return
                    true;

    };
