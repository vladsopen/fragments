// ParseQualifiedName.cpp
// (see "CToken.h")
// (see "CCharTokens.h")
// (see "CToken_DeclarationComponents.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"

CToken* CUnqualifiedName::OnTryExtend()
{
    if not_null(
        CQualifiedName* pQualifiedName =
            derive(CQualifiedName, this->GetParentToken()))
    {
        if (pQualifiedName->QueryAndClearPendingExplicitTemplatePrefixedName())
        {
            SetExplicitTemplatePrefix(true);
        }
    }

    // Resolve the name (to check for template)
    if (GetTokenizer()->IsTemplateName(this))
    {
        CToken* pCurrentToken = GetTokenizer()->PeekNextToken(this);
        if (iskindof(CCharLess, pCurrentToken))
        {
            // Skip '<'
            VERIFY(GetTokenizer()->ReadNextToken(this) == pCurrentToken);

            // Collect template arguments if present
            //   word < ... >
            CTemplateArgList* pTemplateArgList = new CTemplateArgList;
            pTemplateArgList->TryParse(this);
            x_npTemplateArgList = pTemplateArgList;

            GetTokenizer()->ExpectCharToken(
                    errn(0xA11E),
                    new CCharGreater,
                    "closing template name argument list",
                    this);
        }
    }

    // Don't extend into CQualifiedName if already inside one
    if (iskindof(CQualifiedName, GetParentToken()))
        return this;

    // Wrap into CQualifiedName for qualifier parsing
    CQualifiedName* pQualifiedName = new CQualifiedName;
    pQualifiedName->SetRootUnqualifiedName(this);

    return pQualifiedName;
}

//////////////////////////////////////////////////////////////////////////////////////
// CCharColonColonQualifier

CToken* CCharColonColonQualifier::OnTryExtend()
{
    // Don't extend into CQualifiedName if already inside one
    if (iskindof(CQualifiedName, GetParentToken()))
        return this;

    // Wrap into CQualifiedName for qualifier parsing
    CQualifiedName* pQualifiedName = new CQualifiedName;
    pQualifiedName->SetGlobalOrigin(true);

    return pQualifiedName;
}

//////////////////////////////////////////////////////////////////////////////////////
// CCharTilde - can signal a destructor

CToken* CCharTilde::OnTryExtend()
{
    // Don't extend if outside of declaration
    if (!iskindof(CDeclaration, GetParentToken()))
        return this;

    // Don't extend into CQualifiedName if already inside one
    if (iskindof(CQualifiedName, GetParentToken()))
        return this;

    // Wrap into CQualifiedName for qualifier parsing
    CQualifiedName* pQualifiedName = new CQualifiedName;
    pQualifiedName->SetDestructorName();

    return pQualifiedName;
}

//////////////////////////////////////////////////////////////////////////////////////
// CQualifiedName

CQualifiedName::CQualifiedName()
{
    m_pRootUnqualifiedName = new CUnqualifiedName;
    x_aQualifier;
    m_bGlobalOrigin = false;
    //m_bDestructorName = false;
    m_bPointerToMember = false;
    m_bPendingExplicitTemplatePrefixedName = false;
}

bool CQualifiedName::QueryAndClearPendingExplicitTemplatePrefixedName()
{
    bool bYes = m_bPendingExplicitTemplatePrefixedName;
    m_bPendingExplicitTemplatePrefixedName = false;
    return bYes;
}

void CQualifiedName::SetRootUnqualifiedName(CUnqualifiedName* pRootUnqualifiedName)
{
    bool bDestructor = IsDestructorName();
    pRootUnqualifiedName->LinkToParent(this);
    m_pRootUnqualifiedName = pRootUnqualifiedName;
    if (bDestructor)
        SetDestructorName();
}

void CQualifiedName::AddQualifier(CQualifier* pQualifier)
{
    x_aQualifier.Add(pQualifier);
    pQualifier->LinkToParent(this);
}

bool CQualifiedName::IsDeclarableName()
{
    CUnqualifiedName* pUnqualifiedName = GetRootUnqualifiedName();
    bool bPointsToOperatorSpec = iskindof(COperatorSpec, pUnqualifiedName);
    return
        !bPointsToOperatorSpec &&
        pUnqualifiedName->GetSourceFragmentLength() > 0;
}

bool CQualifiedName::IsConstructorName()
{
    if (IsDestructorName())
        return false;

    if not_null(CDeclaration* pEnclosingClassDeclaration =
        GetEnclosingClassDeclaration())
    {
        CDeclareTypeName* pClassTypeName =
            pEnclosingClassDeclaration->x_pDeclareTypeName;

        if not_null(CQualifiedName* pClassQualifiedName =
            pClassTypeName->x_npQualifiedName)
        {
            // Does class name match?
            if (pClassQualifiedName->IsSourceEqual(GetRootUnqualifiedName()))
            {
                // Something complex that could not be a constructor?
                if (x_aQualifier.GetCount() > 1)
                    return false;

                // Simple unqualified constructor?
                if (x_aQualifier.GetCount() == 0)
                {
                    if not_null(CToken* pParentToken = GetParentToken())
                    {
                        // Must be followed by '('
                        CToken* pNextToken =
                            GetTokenizer()->PeekTempTokenSkippingSpaceAfter(
                                GetRootUnqualifiedName(),
                                pParentToken);

                        // there is a little bug here:
                        //   template constructor:  TemplateClass<...>()
                        // it won't be recognized as a constructor.

[...]