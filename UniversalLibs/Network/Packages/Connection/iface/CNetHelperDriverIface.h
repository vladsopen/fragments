// CNetHelperDriverIface.h
#pragma once

#include "CNetHelperDriverIfaceGp.h"

//
// CNetHelperDriverIface - see ifacegp
//

class CNetHelperDriverIface : public CNetHelperDriverIfaceGp
{
public:
    CNetHelperDriverIface();
    NEW_LOCAL(CNetHelperDriverImpl, CNetHelperDriverIface)
    //NEW_GP(Net, CNetHelperDriverImpl, CNetHelperDriverIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Multiple extensions
    void JustMoveExtraFilesForDownload(
            path pathBox,
            path pathMoveToFolder,
            out str& out_sError)
            vhook;

// UI

protected:
    virtual void OnJustMoveExtraFilesForDownload(
            path pathBox,
            path pathMoveToFolder,
            out str& out_sError)
            v1pure;
private:
    //bool _m_bOneTimeInitNetHelperDriverIfaceOk = false;

    void _init_AttachToNetHelperDriverIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
