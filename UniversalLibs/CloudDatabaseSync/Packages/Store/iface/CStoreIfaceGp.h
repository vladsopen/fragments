// CStoreIfaceGp.h
#pragma once

class CUdbCacheIfaceGp;
class CActionIfaceGp;
class CQueryIfaceGp;
class CSessionIfaceGp;

//
// CStoreIfaceGp -
//
//   Manages access to the journalled database for a CloudSync session.
//
//ASSUME_IMPL_FOR_IFACE(CStoreImpl, CStoreIfaceGp)

class CStoreIfaceGp : public object
{
public:
    CStoreIfaceGp();
    //~CStoreIfaceGp();
    //NEW_LOCAL(CStoreImpl, CStoreIfaceGp)
    NEW_GP(CloudSync, CStoreImpl, CStoreIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    // (keys moved to "lib__db_const.h")

// Attributes

    ref<CUdbCacheIfaceGp> GetUdbCache()
            vhook;

    // parent
    ref<CSessionIfaceGp> GetSession()
            vhook;

    // The folder from which we branch all local storage
    path GetDatabaseRootFolder()
            vhook;

    // INTERNAL db for addons
    ref<CUdbConnectionIfaceGp> GetStoreLowLevelUdbConnection()
            vhook;

    // APP-LEVEL Table wrapped connection
    ptr<CUdbConnectionIfaceGp> GetTablesHighLevelUdbConnection()
            vhook;

// Operations

    // External flush request (created for Udb handler)
    void FlushStore()
            vhook;

    // Query Nodes
    bool IterateActionsForNode(
            key keyNode,
            out iter& out_i,
            out ref<CActionIfaceGp>& out_rAction)
            vhook;

    // Cache and publish locally generated change
    ref<CActionIfaceGp> RecordNewLocalAction(
            key keyOnNode,
            key keyTrait,
            key keyTie,
            str sScalar,
            key keyNonscalar,
            ptr<CActionIfaceGp> pAction)
            vhook;
    void RecordNewLocalActionObject(
            ref<CActionIfaceGp> rAction)
            vhook;

    // RecordNewLocalAction if value is different
    void EnsureLastTraitForNode(
            key keyOnNode,
            key keyTrait,
            key keyTie,
            str sScalar,
            key keyNonscalar,
            ptr<CActionIfaceGp> pAction)
            vhook;

    // Most recent value
    bool QueryLastTraitForNode(
            key keyNode,
            key keyTrait,
            key keyOnlyForTie,
            out ref<CActionIfaceGp>& out_rAction)
            vhook;

// UI

protected:
    virtual ref<CUdbCacheIfaceGp> OnGetUdbCache()
            v1pure;
    virtual ref<CSessionIfaceGp> OnGetSession()
            v1pure;
    virtual path OnGetDatabaseRootFolder()
            v1pure;
    virtual ref<CUdbConnectionIfaceGp> OnGetStoreLowLevelUdbConnection()
            v1pure;
    virtual ptr<CUdbConnectionIfaceGp> OnGetTablesHighLevelUdbConnection()
            v1pure;
    virtual void OnFlushStore()
            v1pure;
    virtual bool OnIterateActionsForNode(
            key keyNode,
            out iter& out_i,
            out ref<CActionIfaceGp>& out_rAction)
            v1pure;
    virtual ref<CActionIfaceGp> OnRecordNewLocalAction(
            key keyOnNode,
            key keyTrait,
            key keyTie,
            str sScalar,
            key keyNonscalar,
            ptr<CActionIfaceGp> pAction)
            v1pure;
    virtual void OnRecordNewLocalActionObject(
            ref<CActionIfaceGp> rAction)
            v1pure;
    virtual void OnEnsureLastTraitForNode(
            key keyOnNode,
            key keyTrait,
            key keyTie,
            str sScalar,
            key keyNonscalar,
            ptr<CActionIfaceGp> pAction)
            v1pure;
    virtual bool OnQueryLastTraitForNode(
            key keyNode,
            key keyTrait,
            key keyOnlyForTie,
            out ref<CActionIfaceGp>& out_rAction)
            v1pure;
private:
    //bool _m_bOneTimeInitStoreIfaceGpOk = false;
    //bool _m_bStoreIfaceGpOpened = false;

    void _init_AttachToStoreIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
