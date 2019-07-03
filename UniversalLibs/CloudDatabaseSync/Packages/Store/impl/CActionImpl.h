// CActionImpl.h
#pragma once

#include "CActionIface.h"

class CDbAction;

//
// CActionImpl - see Iface
//

class CActionImpl : public CActionIface
{
public:
    CActionImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // DB record
    ref<CDbAction> x_rDbAction
            xauto(Get, Set);

// Operations

// UI

protected:

    // Gp

    virtual key OnGetActionPrimaryKey()
            ;
    virtual key OnGetKeyOnNode()
            ;
    virtual key OnGetKeyTrait()
            ;
    virtual key OnGetKeyTie()
            ;
    virtual utc OnGetUtcUpdate()
            ;
    virtual str OnGetActionIp()
            ;
    virtual utc OnGetActionUploadUtc()
            ;
    virtual str OnGetNewTraitScalar()
            ;
    virtual key OnGetNewTraitNonscalar()
            ;
    virtual str OnGetActionDebugDump()
            ;
    virtual void OnDefineAction(
            key keyOnNode,
            key keyTrait,
            key keyTie,
            str sScalar,
            key keyNonscalar)
            ;
    virtual bool OnIsKillAction()
            ;
    virtual void OnDefineKillAction(
            key keyOnNode)
            ;

    // Iface
    virtual str OnExportActionNetScript()
            ;
    virtual void OnImportActionNetScript(
            str sScript,
            unidef unidefLateMeta,
            out str& out_sError)
            ;
private:
    //bool _m_bOneTimeInitActionImplOk = false;

    void _init_AttachToActionImpl();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
