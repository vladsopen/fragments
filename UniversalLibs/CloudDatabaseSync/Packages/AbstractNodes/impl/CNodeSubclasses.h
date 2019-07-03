// CNodeSubclasses.h
#pragma once

class CAbstractClassIfaceGp;
class CBasicClassImpl;

//
// CNodeSubclasses - see Iface
//

class CNodeSubclasses : public object
{
public:
    CNodeSubclasses();
    //~CNodeSubclasses();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    ptr<CBasicClassImpl> _x_pBasicClass
            xauto(Get, Set);

// Operations

    // Register client-defined node class extension
    void RegisterNodeClass(
            ref<CAbstractClassIfaceGp> rNodeClass);

    // List all registrations
    bool IterateAllKnownNodeClasses(
            out iter& out_i,
            out ref<CAbstractClassIfaceGp>& out_rNodeClass);

    // Lookup registration by class id
    ptr<CAbstractClassIfaceGp> LookupNodeClass(
            str sClassId);

// UI

protected:
private:
    map<str, ref<CAbstractClassIfaceGp> > _m_mapNodeClass;

    // check class id syntax
    static bool GIsValidNodeClassId(
            str sClassId);

    void _init_AttachToClassesMap();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
