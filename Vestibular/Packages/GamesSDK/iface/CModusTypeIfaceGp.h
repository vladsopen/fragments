// CModusTypeIfaceGp.h
#pragma once

class CSessionIfaceGp;

//
// CModusTypeIfaceGp -
//
//   Current Balance session mode - Classic, Game, Monitoring Recording...
//
ASSUME_IMPL_FOR_IFACE(CGameTypeIfaceGp, CModusTypeIfaceGp)

class CModusTypeIfaceGp : public typeobject
{
public:
    CModusTypeIfaceGp();
    //NEW_LOCAL(CModusTypeImpl, CModusTypeIfaceGp)
    NEW_GP(Balance, CGameTypeImpl, CModusTypeIfaceGp)

// Constants

// Attributes

    // Name for manus
    sloc GetShortModusTypeNameForUi()
            vhook;

    // Icon
    res GetLargeModusTypeIcon()
            vhook;

    // Multistage mode is not compatible with either Classic or games
    bool IsMultiStageSupported()
            vhook;

// Operations

// Derived Item Enumeration

    // see iterators in CModusSessionIfaceGp

    // Default monitoring mode
    type<CModusTypeIfaceGp> AsMonitoringRecording()
            vhook;
    type<CModusTypeIfaceGp> AsMonitoringMultiStage()
            vhook;

    // Classic for HomeMon
    type<CModusTypeIfaceGp> AsMonitoringClassic()
            vhook;

    // Games
    type<CModusTypeIfaceGp> AsGameTypeTarget()
            vhook;
    type<CModusTypeIfaceGp> AsGameTypeBunnies()
            vhook;
    type<CModusTypeIfaceGp> AsGameTypeMelody()
            vhook;
    type<CModusTypeIfaceGp> AsGameTypeTargetM()
            vhook;
    type<CModusTypeIfaceGp> AsGameTypeBunniesM()
            vhook;
    type<CModusTypeIfaceGp> AsGameTypeMelodyM()
            vhook;
    type<CModusTypeIfaceGp> AsGameTypeDynamicProbe()
            vhook;
    type<CModusTypeIfaceGp> AsGameTypeCombyProbe()
            vhook;
    //
    // HomeMon games variation
    //

    type<CModusTypeIfaceGp> AsGameTypeTargetHomeMon()
            vhook;
    type<CModusTypeIfaceGp> AsGameTypeBunniesHomeMon()
            vhook;
    type<CModusTypeIfaceGp> AsGameTypeMelodyHomeMon()
            vhook;
    type<CModusTypeIfaceGp> AsGameTypeDynamicTargetHomeMon()
            vhook;

// UI

protected:
    virtual sloc OnGetShortModusTypeNameForUi()
            v1pure;
    virtual res OnGetLargeModusTypeIcon()
            v1pure;
    virtual bool OnIsMultiStageSupported()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsMonitoringRecording()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsMonitoringMultiStage()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsMonitoringClassic()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeTarget()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeBunnies()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeMelody()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeTargetM()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeBunniesM()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeMelodyM()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeDynamicProbe()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeCombyProbe()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeTargetHomeMon()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeBunniesHomeMon()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeMelodyHomeMon()
            v1pure;
    virtual type<CModusTypeIfaceGp> OnAsGameTypeDynamicTargetHomeMon()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
