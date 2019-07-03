// CWaveChannelType.h
#pragma once

//
// CWaveChannelType -
//
//    Nature: ECG, accel, bat, Markers...
//
//ASSUME_IMPL_FOR_IFACE(CWaveChannelTypeImpl, CWaveChannelType)

class CWaveChannelType : public typeobject
{
public:
    CWaveChannelType();
    //NEW_LOCAL(CWaveChannelTypeImpl, CWaveChannelType)
    //NEW_GP(Ralarm, CWaveChannelTypeImpl, CWaveChannelType)
    DECLARE_NULL_TYPE(GetDerivedList);

// Constants

// Attributes

    // Time scale
    num GetChannelFreq()
            vhook;

    // PreferredY%
    int GetRelativeChannelHeight()
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CWaveChannelType> GetDerivedList();

    type<CWaveChannelType> AsWaveChannelIsEcg()
            vhook;

    type<CWaveChannelType> AsWaveChannelIsMarkers()
            vhook;

    type<CWaveChannelType> AsWaveChannelIsMotion()
            vhook;

    type<CWaveChannelType> AsWaveChannelIsBattery()
            vhook;

// UI

protected:
    virtual num OnGetChannelFreq()
            v1pure;
    virtual int OnGetRelativeChannelHeight()
            v1pure;

    virtual type<CWaveChannelType> OnAsWaveChannelIsEcg()
            v1null;
    virtual type<CWaveChannelType> OnAsWaveChannelIsMarkers()
            v1null;
    virtual type<CWaveChannelType> OnAsWaveChannelIsMotion()
            v1null;
    virtual type<CWaveChannelType> OnAsWaveChannelIsBattery()
            v1null;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // ECG
    class CWaveChannelIsEcg : public CWaveChannelType
    {
    public:
        void flag_List_as_WaveChannelType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(E C G);

        virtual num OnGetChannelFreq()
                return
                    CParseEcgFile::C_nEcgEcgFreq;

        virtual int OnGetRelativeChannelHeight()
                return
                    100;
    };

    // Markers
    class CWaveChannelIsMarkers : public CWaveChannelType
    {
    public:
        void flag_List_as_WaveChannelType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(M a r k);

        virtual num OnGetChannelFreq()
                return
                    0;

        virtual int OnGetRelativeChannelHeight()
                return
                    100;
    };

    // Motion (Accelerometer)
    class CWaveChannelIsMotion : public CWaveChannelType
    {
    public:
        void flag_List_as_WaveChannelType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(M o t i);

        virtual num OnGetChannelFreq()
                return
                    CParseEcgFile::C_nEcgMotionAndBatteryFreq;

        virtual int OnGetRelativeChannelHeight()
                return
                    20;
    };

    // Battery charge
    class CWaveChannelIsBattery : public CWaveChannelType
    {
    public:
        void flag_List_as_WaveChannelType();

    protected:

        virtual num OnGetChannelFreq()
                return
                    CParseEcgFile::C_nEcgMotionAndBatteryFreq;

        virtual int OnGetRelativeChannelHeight()
                return
                    0;
    };
