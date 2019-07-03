// CAbstractClassIfaceGp.h
#pragma once

#include "CAddonSessionIfaceGp.h"

class CUbroClassIfaceGp;
class CAbstractNodeIfaceGp;
class CBasicClassIfaceGp;
class CBasicNodeIfaceGp;
class CActionIfaceGp;

//
// CAbstractClassIfaceGp -
//
//  A session specifically for Node-based classes
//

class CAbstractClassIfaceGp : public CAddonSessionIfaceGp
{
public:
    CAbstractClassIfaceGp();
    //~CAbstractClassIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // infinite recursion detection
    static const int C_nMaxNodeClassDerivedDepth = 10;

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Internal short PRD class name
    str GetNodeClassId()
            vhook;

    // Inheritance hierarchy
    ptr<CAbstractClassIfaceGp> GetNodeSuperClass()
            vhook;

    // Read properties by names (CloudSync-keys)
    unidef _x_unidefExposedDbClassTemplate;
    unidef _x_unidefExposedDbClassTemplate_Get();

// Operations

    // Called after Node has updated its own cache
    void HandleAfterNodeActionInterpreter(
            ref<CActionIfaceGp> rAction,
            ref<CBasicNodeIfaceGp> rBasicNode,
            ref<CBasicNodeIfaceGp> rNodeTrait,
            ref<CBasicClassIfaceGp> rBasicClass)
            vhook;

    // Unused?
    ref<CAbstractNodeIfaceGp> GetAbstractNodeByKey(
            key keyNodeOfMyClass)
            vhook;

    // TableClass returns app class callback
    ptr<CUbroClassIfaceGp> AskUbroClassFromClass()
            vhook;

// UI

protected:
    virtual str OnGetNodeClassId()
            v1pure;
    virtual ptr<CAbstractClassIfaceGp> OnGetNodeSuperClass()
            v1pure;
    virtual void OnHandleAfterNodeActionInterpreter(
            ref<CActionIfaceGp> rAction,
            ref<CBasicNodeIfaceGp> rBasicNode,
            ref<CBasicNodeIfaceGp> rNodeTrait,
            ref<CBasicClassIfaceGp> rBasicClass)
            v1pure;
    virtual ref<CAbstractNodeIfaceGp> OnGetAbstractNodeByKey(
            key keyNodeOfMyClass)
            v1pure;
    virtual ptr<CUbroClassIfaceGp> OnAskUbroClassFromClass()
            v1st
            return
                null();
    // private:
    virtual ref<dbobject> OnHandleGetDbRecordToExposeByPropertyName()
            v1pure;
private:
    //bool _m_bOneTimeInitAbstractClassIfaceGpOk = false;
    //bool _m_bAbstractClassIfaceGpOpened = false;
    bool _m_bOneTimeExposedDbClassPropertiesOk = false;

    // For quering class properties by names
    ref<dbobject> HandleGetDbRecordToExposeByPropertyName()
            vhook;

    void _init_AttachToAbstractClassIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
