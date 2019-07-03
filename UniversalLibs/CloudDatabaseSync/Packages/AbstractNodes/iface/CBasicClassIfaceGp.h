// CBasicClassIfaceGp.h
#pragma once

#include "CAbstractClassIfaceGp.h"

class CUbroRefineIfaceGp;
class CRecentValueIfaceGp;
class CFilterIfaceGp;
class CUbroSessionIfaceGp;
class CBasicNodeIfaceGp;

//
// CBasicClassIfaceGp -
//
//   Generic Node object for Browser
//
ASSUME_IMPL_FOR_IFACE(CBasicClassIface, CBasicClassIfaceGp)
ASSUME_IMPL_FOR_IFACE(CBasicClassImpl, CBasicClassIface)

class CBasicClassIfaceGp : public CAbstractClassIfaceGp
{
public:
    CBasicClassIfaceGp();
    //~CBasicClassIfaceGp();
    //NEW_LOCAL(CBasicClassImpl, CBasicClassIfaceGp)
    NEW_GP(CloudSync, CBasicClassImpl, CBasicClassIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Register client-defined node class extension
    void RegisterDerivedNodeClassSession(
            ref<CAbstractClassIfaceGp> rAbstractClass)
            vhook;

    // Query all kinds of nodes
    queryorcount QueryBasicNodes(
            out iter& out_i,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode,
            key keyAt)
            vhook;

    // By primary key
    bool QueryBasicNodeByKey(
            key keyLookup,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode)
            vhook;

    // Query nodes by Filter
    queryorcount QueryBasicNodesInFilter(
            out iter& out_i,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode,
            key keyOptionalPrimary,
            ref<CFilterIfaceGp> rFilter,
            ref<CUbroRefineIfaceGp> rUbroRefine)
            vhook;

    // Query static properties from class hierarchy
private:
    // UNUSED?
    bool IterateNodeClassTraits(
            str sFilterForNodeClass,
            out iter& out_i,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode)
            vhook;
    bool IterateNodeAndSuperClassTraits(
            ref<CAbstractClassIfaceGp> rDerivedAbstractClass,
            out iter& out_i,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode)
            vhook;
public:

    // Query actually assigned (dynamic?) properties for any Nodes
    static const str C_sDontSkipAnyValues = "<no-skip-any>";
        // pFilter must be null()!
    queryorcount QueryActualTraits(
            out iter& out_i,
            out ref<CRecentValueIfaceGp>& out_rValue,
            key keyOnlyForObject,
            key keyOnlyForTrait,
            ptr<CFilterIfaceGp> pFilter, // = null()!
            str sSkipValuesEqualTo,
            key keyOnlyForTie)
            vhook;

    // Iterate multiple CDb-classes for a node key
        SEE_ALSO(IterateNodeUbroClasses())
    queryorcount QueryTieClassAssignments(
            out iter& out_i,
            out str& out_sClassId,
            out ref<CRecentValueIfaceGp>& out_rClassAssignment,
            key keyOnlyForObject)
            vhook;

    /* very complex nested iterators not really required

    // Iterate predefined properties declared in all CDb-classes
    //   for the node key
    queryorcount QueryDeclaredTraits(
            out iter& out_i,
            out ref<CRecentValueIfaceGp>& out_rValue,
            key keyOnlyForObject,
            ptr<CFilterIfaceGp> pFilter, // = null()!
            str sSkipValuesEqualTo,
            key keyOnlyForTie)
            vhook;

    // Iterate first predefined properties
    //    then append dynamic traits if any
    //    (COUNT may fail!)
    queryorcount QueryAllTraits(
            out iter& out_i,
            out ref<CRecentValueIfaceGp>& out_rValue,
            key keyOnlyForObject,
            ptr<CFilterIfaceGp> pFilter, // = null()!
            str sSkipValuesEqualTo,
            key keyOnlyForTie)
            vhook;
    */

    // List all classes
    bool IterateAllKnownNodeClasses(
            out iter& out_i,
            out ref<CAbstractClassIfaceGp>& out_rNodeClass)
            vhook;

    // Lookup class by id
    ptr<CAbstractClassIfaceGp> LookupNodeClass(
            str sClassId)
            vhook;

// UI

protected:
    SEE_ALSO(CBasicClassImpl) // F12-lookup

    virtual void OnRegisterDerivedNodeClassSession(
            ref<CAbstractClassIfaceGp> rAbstractClass)
            v1pure;
    virtual queryorcount OnQueryBasicNodes(
            out iter& out_i,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode,
            key keyAt)
            v1pure;
    virtual bool OnQueryBasicNodeByKey(
            key keyLookup,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode)
            v1pure;
    virtual queryorcount OnQueryBasicNodesInFilter(
            out iter& out_i,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode,
            key keyOptionalPrimary,
            ref<CFilterIfaceGp> rFilter,
            ref<CUbroRefineIfaceGp> rUbroRefine)
            v1pure;
    virtual bool OnIterateNodeClassTraits(
            str sFilterForNodeClass,
            out iter& out_i,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode)
            v1pure;
    virtual bool OnIterateNodeAndSuperClassTraits(
            ref<CAbstractClassIfaceGp> rDerivedAbstractClass,
            out iter& out_i,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode)
            v1pure;
    virtual queryorcount OnQueryActualTraits(
            out iter& out_i,
            out ref<CRecentValueIfaceGp>& out_rValue,
            key keyOnlyForObject,
            key keyOnlyForTrait,
            ptr<CFilterIfaceGp> pFilter,
            str sSkipValuesEqualTo,
            key keyOnlyForTie)
            v1pure;
    virtual queryorcount OnQueryTieClassAssignments(
            out iter& out_i,
            out str& out_sClassId,
            out ref<CRecentValueIfaceGp>& out_rClassAssignment,
            key keyOnlyForObject)
            v1pure;
    virtual bool OnIterateAllKnownNodeClasses(
            out iter& out_i,
            out ref<CAbstractClassIfaceGp>& out_rNodeClass)
            v1pure;
    virtual ptr<CAbstractClassIfaceGp> OnLookupNodeClass(
            str sClassId)
            v1pure;

private:
    //bool _m_bOneTimeInitBasicClassIfaceGpOk = false;
    //bool _m_bBasicClassIfaceGpOpened = false;

    void _init_AttachToBasicClassIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
