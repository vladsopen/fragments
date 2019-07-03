// CCacheAction.cpp

#include "CProject.h"
#include "CCacheAction.h"
#include "CActionIface.h"
#include "CDbRoot.h"
#include "CStoreImpl.h"
#include "CDbAction.h"
#include "CAddonMapIface.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"
#include "CActionImpl.h"

CCacheAction::CCacheAction()
{
    _init_AttachToCacheAction();
}

void CCacheAction::_init_AttachToCacheAction()
{
    //x_r->_x_p = this;
}

void CCacheAction::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToCacheAction();

    rFAIL("make sure it is OK to expose CCacheAction!");
}

void CCacheAction::InterpretActionToCache(
        ref<CActionIface> rAction,
        ref<CStoreImpl> rStore)
{
    // interpret literally first
    DoInterpretActionToCache(
        rAction,
        rStore);

    bool bRecognized = true;

    // 1) 
    //    003tSpectrm --TieXxx--> sMyProp = "Value"  (!=) 
    // to
    //    003tSpectrm_TieXxx_sMyProp --TieProp--> keyMmTrait = ""[sMyProp]
    //
    {
        ref<CActionImpl> rActionMm =
            PrepareMmAction(
                rAction,
                // make sure mmdbobject::m_keyMmTrait key is set
                key::GConstKeyStr(C_szDbObjectMmTrait),
                "", // sScalar unused by key
                rAction->GetKeyTrait());

        // rescan handlers
        DoInterpretActionToCache(
            rActionMm,
            rStore);

        bRecognized = rActionMm->x_sActionRecognizedByTableClass != "";
    }

    // 2) 
    //    003tSpectrm --TieXxx--> sMyProp = "Value"  (!=) 
    // to
    //    003tSpectrm_TieXxx_sMyProp --TieProp--> keyMmTie = ""[TieXxx]
    //
    {
        ref<CActionImpl> rActionMm =
            PrepareMmAction(
                rAction,
                // make sure mmdbobject::m_keyMmTie key is set
                key::GConstKeyStr(C_szDbObjectMmTie),
                "", // sScalar unused by key
                rAction->GetKeyTie());

        // rescan handlers
        DoInterpretActionToCache(
            rActionMm,
            rStore);

        bRecognized = rActionMm->x_sActionRecognizedByTableClass != "";
    }

    // 3) (str Scalar part of value)
    //    003tSpectrm --TieXxx--> sMyProp = "Value" 
    // to
    //    003tSpectrm_TieXxx_sMyProp --TieProp--> sMmScalar = "Value"  
    //
    if (bRecognized)
    {
        ref<CActionImpl> rActionMm =
            PrepareMmAction(
                rAction,
                // update str part of CDb M2M mmdbobject part of record
[...]

