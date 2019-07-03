// CNodeBasicCacheAction.cpp

#include "CProject.h"
//#include "DeclareKeys.h"
#include "CNodeBasicCacheAction.h"
#include "CActionIfaceGp.h"
#include "CBasicClassImpl.h"
#include "CDbNode.h"
#include "CDbGlobalNodes.h"
#include "CStoreIfaceGp.h"
#include "CNodeSubclasses.h"
#include "CBasicNodeImpl.h"
#include "CDbRecentTraits.h"
#include "CActionIface.h"
#include "CRecentValueIfaceGp.h"

void CNodeBasicCacheAction::InterpretActionForNodes(
        ref<CActionIface> rAction,
        ref<CBasicClassImpl> rBasicClass)
{
    utc utcNow = sys::GGetUtc();

    // every our object is attached to the singleton root record
    ref<CDbGlobalNodes> rDbGlobal = 
        rBasicClass->
            QueryGlobalNodes();
    ref<CStoreIfaceGp> rStore = 
        rBasicClass->GetCoreStore();

    // the Tie
    key keyTie = 
        rAction->GetKeyTie();
    rASSERT(keyTie != null());

    //
    // Prepare Trait data
    //

    key keyTrait = 
        rAction->GetKeyTrait();
    rASSERT(keyTrait != null());

    // must be present in our Node cache
    ref<CDbNode> rDbTrait =
        rDbGlobal->
            _x_dblistNode.Get().
                EnsureRowAdded(
                    keyTrait);

    //
    // Prepare Node data
    //

    key keyNode = 
        rAction->GetKeyOnNode();
    rASSERT(keyNode != null());
    bool bCommitNode = false;

    // must be present in our Node cache
    ref<CDbNode> rDbNode =
        rDbGlobal->
            _x_dblistNode.Get().
                EnsureRowAdded(
                    keyNode);

    //
    // Is changed Trait special?
    //

    str sScalar = 
        rAction->GetNewTraitScalar();
    key keyNonscalar = 
        rAction->GetNewTraitNonscalar();

    // Update cached fields according to trait class
    key keyTraitName = 
        rDbTrait->GetKey();
        //rDbTrait->x_sTraitClass;

    //
    // Save changes
    //

    if (bCommitNode)
    {
        // mark updated
        rDbNode->x_utcNodeLastChange = sys::GGetUtc();

        rDbNode->CommitRef();
    }

    //
    // Update recent trait value map
    //

    ref<CDbRecentTraits> rDbRecentTraits;
    if (rBasicClass->
            QueryRecentTraitUpdateRecord(
                out_IGNORE(iter),
                out rDbRecentTraits,
                keyNode,
                keyTraitName,
                keyTie))
    {
        // update existing record
        rASSERT(rDbRecentTraits->x_keyNodeUpdated == keyNode);
        rASSERT(rDbRecentTraits->x_keyTraitUpdated == keyTrait);
        rASSERT(rDbRecentTraits->x_keyTieUpdated == keyTie);
    }
    else
    {
        // create new one first time
        rDbGlobal->
            _x_dblistRecentTraits.Get().
                AddNewRow(
                    rDbRecentTraits);

        rDbRecentTraits->x_keyNodeUpdated = keyNode;
        rDbRecentTraits->x_keyTraitUpdated = keyTraitName;
        rDbRecentTraits->x_keyTieUpdated = keyTie;
    }

    // write updated property value
[...]
    rDbRecentTraits->CommitRef();

    // debug
    if (debug::GIsTest()
        &&
        rAction->x_sActionRecognizedByTableClass == ""
        &&
        !rAction->x_bOnlyUpdateExistingNodes)
    {
        str sLogPiece = 
            rAction->x_sDebugLogUnrecognizedAction;

        if (sLogPiece == "")
        {
            sLogPiece =
                "  (?) " +
                    rAction->GetActionDebugDump() +
                    sDeleteNotes
                    ;
        }

        TESTLOG(
            "+CacheAction",
            str() +
                sLogPiece + "  (NOT IN CLASS) "
                "\n")
    }
}

void CNodeBasicCacheAction::DoDeleteBasicNode(
        ref<CBasicClassImpl> rBasicClass,
        ref<CDbNode> rDbNode)
{
    // delete the node
    //rDbNode->
    //    DeleteFromDb();

    rBasicClass->
        GetCoreUdbConnection()->
            x_rUdbStorage->
                HandleDeleteUdbArrayRecord(
                    rDbNode->GetObjectClassInfo(),
                    rDbNode->GetKey(),
                    rBasicClass->GetCoreUdbConnection()); // unused for Sqlite

    //TODO: delete it as trait? (if it's a trait)
    //  or if it's a part of other tie-relationships
    // ...

    // delete all properties
    ref<CDbRecentTraits> rIterDbRecentTraits;
    for (
        iter i;
[...]