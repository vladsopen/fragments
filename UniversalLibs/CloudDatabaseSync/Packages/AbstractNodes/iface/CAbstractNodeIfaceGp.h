// CAbstractNodeIfaceGp.h
#pragma once

class CAbstractClassIfaceGp;
class CNodeCommonIfaceGp;

//
// CAbstractNodeIfaceGp -
//
//   Abstract node data for a Node-derived class
//
//ASSUME_IMPL_FOR_IFACE(CAbstractNodeImpl, CAbstractNodeIfaceGp)

class CAbstractNodeIfaceGp : public object
{
public:
    CAbstractNodeIfaceGp();
    //~CAbstractNodeIfaceGp();
    //NEW_LOCAL(CAbstractNodeImpl, CAbstractNodeIfaceGp)
    //NEW_GP(CloudSync, CAbstractNodeImpl, CAbstractNodeIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Shared functions for all kinds of nodes
    ref<CNodeCommonIfaceGp> x_rNodeCommon
            xauto(Get);

    // Class-specific session
    //ref<CAbstractClassIfaceGp> GetNodeSubclassSession()
    //        vhook;

    // Primary key
    key GetNodePrimaryKey()
            vhook;

    // Friendly polimorphic ID
    sloc GetVirtualNameForUi()
            vhook;

// Operations

    // Cleanup
    //void CloseAbstractNode();

// UI

protected:
    //virtual ref<CAbstractClassIfaceGp> OnGetNodeSubclassSession()
    //        v1pure;
    virtual key OnGetNodePrimaryKey()
            v1pure;
    virtual sloc OnGetVirtualNameForUi()
            v1pure;
    virtual ref<dbobject> OnHandleGetDbRecordToExposeByPropertyName()
            v1pure;
private:
    bool _m_bOneTimeInitAbstractNodeIfaceGpOk = false;
    //bool _m_bAbstractNodeIfaceGpOpened = false;

    // Must be defined in all Node classes
    ref<dbobject> HandleGetDbRecordToExposeByPropertyName()
            vhook;

    void _init_AttachToAbstractNodeIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
