// CBasicNodeIfaceGp.h
#pragma once

#include "CAbstractNodeIfaceGp.h"

class CUbroClassIfaceGp;
class CAbstractClassIfaceGp;

//
// CBasicNodeIfaceGp -
//
//   Encapsulates all access to the Node data
//
ASSUME_IMPL_FOR_IFACE(CBasicNodeIface, CBasicNodeIfaceGp)
ASSUME_IMPL_FOR_IFACE(CBasicNodeImpl, CBasicNodeIface)

class CBasicNodeIfaceGp : public CAbstractNodeIfaceGp
{
public:
    CBasicNodeIfaceGp();
    //~CBasicNodeIfaceGp();
    //NEW_LOCAL(CBasicNodeImpl, CBasicNodeIfaceGp)
    NEW_GP(CloudSync, CBasicNodeImpl, CBasicNodeIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    //// Class-specific session
    //ref<CAbstractClassIfaceGp> GetNodeSubclassSession()
    //        vhook;
        // see super

    // Key
    //key GetNodePrimaryKey()
    //        vhook;
        // see super

    // Node SubClass
    //str GetNodeClass()
    //        vhook;

    // Name for trait meta-node
    str GetPropertyClass()
            vhook;

    // Node access rights
    str GetNodeCanRead()
            vhook;
    str GetNodeCanWrite()
            vhook;

    // Title
    void SetCustomNodeName(
            str value)
            vhook;
    str GetNodeNameAsIs()
            vhook;
    str GetNodeTitleForUi()
            vhook;
    str GetNodeSubtitle()
            vhook;

    // Last action referring here
    utc GetNodeLastChange()
            vhook;

// Operations

    // One-time opener
    //void InitBasicNode();

    // Cleanup
    //void CloseBasicNode();


    //
    // See also node iterators in CBasicClassIfaceGp
    //

    // Seek (mainly) app callbacks associated with TableNode
    bool IterateNodeUbroClasses(
            out iter& out_iClass,
            out ref<CUbroClassIfaceGp>& out_rUbroClass)
            vhook;

// UI

protected:
    SEE_ALSO(CBasicNodeImpl) // F12-lookup

    //virtual str OnGetNodeClass()
    //        v1pure;
    virtual str OnGetPropertyClass()
            v1pure;
    virtual str OnGetNodeCanRead()
            v1pure;
    virtual str OnGetNodeCanWrite()
            v1pure;
    virtual void OnSetCustomNodeName(
            str value)
            v1pure;
    virtual str OnGetNodeNameAsIs()
            v1pure;
    virtual str OnGetNodeTitleForUi()
            v1pure;
    virtual str OnGetNodeSubtitle()
            v1pure;
    virtual utc OnGetNodeLastChange()
            v1pure;
    virtual bool OnIterateNodeUbroClasses(
            out iter& out_iClass,
            out ref<CUbroClassIfaceGp>& out_rUbroClass)
            v1pure;

private:
    //bool _m_bOneTimeInitBasicNodeIfaceGpOk = false;
    //bool _m_bBasicNodeIfaceGpOpened = false;

    void _init_AttachToBasicNodeIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
