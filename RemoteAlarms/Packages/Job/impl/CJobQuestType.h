// CJobQuestType.h
#pragma once

class CTaskIface;

//
// CJobQuestType -
//
//   Quering server subchannels
//
//ASSUME_IMPL_FOR_IFACE(CJobQuestTypeImpl, CJobQuestType)

class CJobQuestType : public typeobject
{
public:
    CJobQuestType();
    //NEW_LOCAL(CJobQuestTypeImpl, CJobQuestType)
    //NEW_GP(Ralarm, CJobQuestTypeImpl, CJobQuestType)
    DECLARE_NULL_TYPE(GetDerivedList);

// Constants

// Attributes

    // Unique string ("-up")
    str GetQuestFolderEnding()
            vhook;

    // "AuthDevice", "DeviceSerial"
    str GetQuestKeyAsStr()
            vhook;

    // ByAuthDevice, ByDeviceSerial
    str GetQuestKeyFromTask(
            ref<CTaskIface> rTask)
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CJobQuestType> GetDerivedList();

// UI

protected:

    // a helper
    static str GMakeStringJsonSafe(
            str sUnsafe);

    // common JSON
    str ComposeQuestAskStruct(
            str sInnerJson);

    virtual str OnGetQuestFolderEnding()
            v1pure;
    virtual str OnGetQuestKeyAsStr()
            v1pure;
    virtual str OnGetQuestKeyFromTask(
            ref<CTaskIface> rTask)
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // Is device online/offline, config
    class CJobQuestTypeIsDeviceStatus : public CJobQuestType
    {
    public:
        void flag_List_as_JobQuestType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(S t u s);

        virtual str OnGetQuestFolderEnding()
                return
                    "-qstatus";

        virtual str OnGetQuestKeyAsStr()
                return
                    CTaskIface::C_sKeyCloudAuthDevice;

        virtual str OnGetQuestKeyFromTask(
                ref<CTaskIface> rTask)
                return
                    rTask->x_sAuthDevice;

    public:

        str ComposeQuestForStatus(
                str sSendSnapshotTaskId,
                int nEcgMin,
                int nEcgMax,
                str sRemoteDebug);

    };

    // Device Pairing
    class CJobQuestTypeIsDevicePairing : public CJobQuestType
    {
    public:
        void flag_List_as_JobQuestType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(P a i r);

        virtual str OnGetQuestFolderEnding()
                return
                    "-qpair";

        virtual str OnGetQuestKeyAsStr()
                return
                    CTaskIface::C_sKeyCloudDeviceSerial;

        virtual str OnGetQuestKeyFromTask(
                ref<CTaskIface> rTask)
                return
                    rTask->x_sDeviceSerial;

    public:

        str ComposeQuestForPairing(
                str sPairedAuthDevice,
                sloc slocPatient);

    };
