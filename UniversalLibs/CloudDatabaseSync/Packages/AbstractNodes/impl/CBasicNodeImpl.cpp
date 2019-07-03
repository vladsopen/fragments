// CBasicNodeImpl.cpp

#include "CProject.h"
#include "CBasicNodeImpl.h"
#include "CDbNode.h"
#include "CBasicClassImpl.h"
#include "CStoreIfaceGp.h"
#include "CActionIfaceGp.h"
#include "CNodeSubclasses.h"
#include "CRecentValueIfaceGp.h"
#include "CUbroClassIfaceGp.h"

CBasicNodeImpl::CBasicNodeImpl()
{
    _init_AttachToBasicNodeImpl();
}

void CBasicNodeImpl::_init_AttachToBasicNodeImpl()
{
    //x_r->_x_p = this;
}

void CBasicNodeImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToBasicNodeImpl();

    rFAIL("make sure it is OK to expose CBasicNodeImpl!");
}

ref<CBasicNodeImpl> CBasicNodeImpl::NewBasicNodeFromQuery(
        ref<CDbNode> rDbNode,
        ref<CBasicClassImpl> rBasicClass)
{
    rASSERT(!_m_bOneTimeInitBasicNodeImplOk);
    _m_bOneTimeInitBasicNodeImplOk = true;

    ref<CBasicNodeImpl> rNewBasicNode;

    rNewBasicNode->x_rDbNode = rDbNode;
    rNewBasicNode->_x_pBasicClass = rBasicClass;

    return rNewBasicNode;
}

/*
CBasicNodeImpl::~CBasicNodeImpl()
{
    rASSERT(!_m_bBasicNodeImplOpened);
}

void CBasicNodeImpl::InitBasicNode()
{
    rASSERT(!_m_bOneTimeInitBasicNodeImplOk);
    _m_bOneTimeInitBasicNodeImplOk = true;
    _m_bBasicNodeImplOpened = true;
}

void CBasicNodeImpl::CloseBasicNode()
{
    if (_m_bBasicNodeImplOpened)
    {
        _m_bBasicNodeImplOpened = false;
        // ...
    }
}
*/

/*
ref<CAbstractClassIfaceGp> CBasicNodeImpl::OnGetNodeSubclassSession()
{
    ref<CBasicClassImpl> rBasicClass = 
        GetBasicClass();

    if not_null(
        ref<CAbstractClassIfaceGp> rNodeClass = 
            rBasicClass->
                x_rNodeSubclasses->
                    LookupNodeClass(
                        GetNodeClass()))
    {
        return rNodeClass;
    }

    return rBasicClass;
}
*/

ref<dbobject> CBasicNodeImpl::OnHandleGetDbRecordToExposeByPropertyName()
{
    return x_rDbNode;
}

key CBasicNodeImpl::OnGetNodePrimaryKey()
{
    return x_rDbNode->GetKey();
}

sloc CBasicNodeImpl::OnGetVirtualNameForUi()
{
    // dont make it recursive with GetNodeTitleForUi()
    return GetNodeNameAsIs();
}

//str CBasicNodeImpl::OnGetNodeClass()
//{
//    return x_rDbNode->x_sNodeClass;
//}

str CBasicNodeImpl::OnGetPropertyClass()
{
    return x_rDbNode->x_sTraitClass;
}

str CBasicNodeImpl::OnGetNodeCanRead()
{
    return x_rDbNode->x_sNodeCanRead;
}

str CBasicNodeImpl::OnGetNodeCanWrite()
{
    return x_rDbNode->x_sNodeCanWrite;
}

str CBasicNodeImpl::OnGetNodeNameAsIs()
{
    return x_rDbNode->x_sNodeName;
}

void CBasicNodeImpl::OnSetCustomNodeName(
        str value)
{
    rASSERT(x_rDbNode->IsKeyDefined());

    x_rDbNode->x_sNodeName = value;
    x_rDbNode->CommitRef();
}

str CBasicNodeImpl::OnGetNodeTitleForUi()
{
    if (m_sCacheNodeTitle != "")
        return m_sCacheNodeTitle;

    str sForcedName = GetNodeNameAsIs();
    if (sForcedName != "")
    {
        m_sCacheNodeTitle = sForcedName;
    }
    else
    {
        key keyNode = GetNodePrimaryKey();
        rASSERT(keyNode != key());

        ref<CBasicClassImpl> rBasicClass = 
            GetBasicClass();

        // iterate all node classes
        ref<CRecentValueIfaceGp> rClassRecord;
        str sClassId;
        str sAllClasses;
        for (
            iter iClass;
            rBasicClass->
                QueryTieClassAssignments(
                    out iClass,
                    out sClassId,
                    out rClassRecord,
                    keyNode);)
        {
            if not_null(
                ref<CAbstractClassIfaceGp> rNodeClass = 
                    rBasicClass->
                        x_rNodeSubclasses->
                            LookupNodeClass(
                                sClassId))
            {
                ref<CAbstractNodeIfaceGp> rThisAbstractNode = 
                    rNodeClass->
                        GetAbstractNodeByKey(
                            keyNode);

                str sNextName = 
                    rThisAbstractNode->GetVirtualNameForUi();

                if (m_sCacheNodeTitle != "")
                    m_sCacheNodeTitle += " ";
                m_sCacheNodeTitle += sNextName;

                if (sAllClasses != "")
                    sAllClasses += " ";
                sAllClasses += sClassId;
            }
            else
            {
                rFAIL("unknown node class: <" + sClassId + ">");
            }
        }

        // nothing found?
        if (m_sCacheNodeTitle == "")
        {
            m_sCacheNodeTitle = 
                sAllClasses + ": " +
                    keyNode.DumpWithSignature();
        }
    }

    return m_sCacheNodeTitle;
    
    /*
    return 
        GetNodeNameAsIs().
            GetStripBoth().
            GetStub(
                //"NoName-" + 
                    x_rDbNode->GetKey().DumpWithSignature());
    */
}

str CBasicNodeImpl::OnGetNodeSubtitle()
{
    return 
        x_rDbNode->x_sNodeSubtitle;
}

utc CBasicNodeImpl::OnGetNodeLastChange()
{
    return 
        x_rDbNode->x_utcNodeLastChange;
}

bool CBasicNodeImpl::OnIterateNodeUbroClasses(
        out iter& out_iClass,
        out ref<CUbroClassIfaceGp>& out_rUbroClass)
{
    key keyNode = GetNodePrimaryKey();
    rASSERT(keyNode != key());

    ref<CBasicClassImpl> rBasicClass = 
        GetBasicClass();

    // iterate all node classes
    ref<CRecentValueIfaceGp> rClassRecord;
    /*
    while (rBasicClass->
            QueryActualTraits(
                out out_iClass,
                out rClassRecord,
                keyNode,
                key(), // all traits (classes here)
                ptr<CFilterIfaceGp>(),
                C_sScalarFieldOffObject, // sSkipValuesEqualTo
                C_keyTieClass))
    */
    str sClassId;
    while (
        rBasicClass->
            QueryTieClassAssignments(
                out out_iClass,
                out sClassId,
                out rClassRecord,
                keyNode))
    {
        //str sClassId = 
        //    rClassRecord->x_keyValueForTrait.Get()._internal_ConvertToKeyStorage();

        if not_null(
            ref<CAbstractClassIfaceGp> rNodeClass = 
                rBasicClass->
                    x_rNodeSubclasses->
                        LookupNodeClass(
                            sClassId))
        {
            if not_null(
                ref<CUbroClassIfaceGp> rUbroClass = 
                    rNodeClass->AskUbroClassFromClass())
            {
                out_rUbroClass = rUbroClass;
                return true;
            }
        }
        else
        {
            rFAIL("unknown node class: <" + sClassId + ">");
        }
    }

    // end of iter
    return false;
}

