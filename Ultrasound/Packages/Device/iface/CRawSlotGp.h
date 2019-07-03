// CRawSlotGp.h
#pragma once

class CDocIfaceGp;

//
// CRawSlotGp - 
//
//   Identification info for one of device channel raw audio input source.
//   Formely (LegacyD) this was just a channel number.
//
//   This is a part of CDocFormatGp used for Doc initialization.
//   The RawSlot data links RawCapture(s) to Doc(s).
//
//ASSUME_IMPL_FOR_IFACE(CRawSlotGpImpl, CRawSlotGp)

class CRawSlotGp : public object
{
public:
    CRawSlotGp();
    //NEW_LOCAL(CRawSlotGpImpl, CRawSlotGp)
    //NEW_GP(MyProject, CRawSlotGpImpl, CRawSlotGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Scanner channel
    int x_iScannerChannel = -1
            xauto(Get, Set)
            xassert(value >= -1 && value < oo);

    // Cluster index within the channel 
    //   (see also local type<>s for polymorphic Cluster features)
    int x_iScannerCluster = -1
            xauto(Get, Set)
            xassert(value >= -1 && value < oo);

    // Used to access converters in Doc context
    ptr<CDocIfaceGp> _x_pFirstDocDispatch
            xauto(Get, Set);

// Operations

    // Compare object for online raw data stream dispatch
    bool IsMatchingRawSlot(
            ref<CRawSlotGp> rRawSlot);

// UI

protected:
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
