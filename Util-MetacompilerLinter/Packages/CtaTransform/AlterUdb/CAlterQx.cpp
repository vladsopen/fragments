// CAlterQx.cpp

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CWriter.h"
#include "CWriteRegeneratedText.h"
#include "CSourceLines.h"
#include "CTransformContext.h"
#include "CAlterQx.h"
#include "CAlterMember.h"
#include "CCodeMarking.h"
#include "CGlobalDbContext.h"
#include "lib__idparser.h"
#include "CMapQxTypes.h"
#include "CParseQxCode.h"

CAlterQx::CAlterQx()
{
    m_sQxClass = ""; //"qxdata";

    m_pExpression = new CExpression;
}

const char* CAlterQx::OnGetDefaultTextRepresentation()
{
    return "Qx";
}

bool CAlterQx::OnIsSemicolonAtEnd()
{
    return true; // don't care, only for regen?
}

// UPDATE:
//#include "lib__CtaForIntelliSense.h"

bool CAlterQx::VerifyNoArguments(
        str sFlagName,
        str sArgument)
{
    if (sArgument.Len() > 0)
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xAA37),
            "cta_::" + sFlagName + " does not accept any parameters "
                "(found '" + sArgument + "')",
            nullable<CToken*>());
        return false;
    }
    return true;
}

CToken* CAlterQx::OnTryExtend()
{
    //CToken* pReturnToken = new CWhiteSpace;

    /*
    if not_null(
        FindFirstParentOfKind(new CExpression))
    {
    }
    else
    */
    {
        m_pExpression->x_bForceAnyCommaContinueExpression = true;
            // for Qx expr, expr, expr;
    }

    m_pExpression->TryParse(this);

    /* this kills valid termination for _Set(Qx());
    // ';' must follow 
    if (!GetTokenizer()->ExpectCharToken(
            errn(0xC02F),
            new CCharSemicolon,
            "closing Qx() statement",
            this))
    {
        // ignore
    }
    */

    return this;
}

void CAlterQx::OnTransform()
{
    super::OnTransform();

    static int s_nQxUniqueId = 0;
    s_nQxUniqueId++;
    m_sUniqueNewQx = "qxNew_" + Str(s_nQxUniqueId);

    //CToken* pTerminatingToken = 
    //    new CCharSemicolon;

    int iToken = 0;

    //
    // Recursive subexpression parser
    //

    str sDefiniteQxClass;
    CParseQxCode* pParseQxCode = new CParseQxCode;
    pParseQxCode->
        ParseQxParenLevel(
            0, // iParenLevel
            out iToken,
            out sDefiniteQxClass,
            m_pExpression,
            m_sUniqueNewQx,
            this);

    if (sDefiniteQxClass != "")
    {
        m_sQxClass = sDefiniteQxClass;
    }
    else
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xCDC0),
            "cannot detect qxclass",
            this);

        m_sQxClass = "qxundefined";
    }

    // We transform: 
    //
    //    rQuery->qxXxxx = Qx(a b c);
    //
    // to---> 
    //
    //    qxNew.build< a b c >;
    //    rQuery->qxXxxx = qxNew;
    //

    if not_null(
        CStatement* pAssignmentStatement =
            this->FindFirstParentStatement())
    {
        CGlobalDbContext* pGlobalDbContext = 
            GetTransformContext()->GetGlobalDbContext();
        CMapQxTypes* pMapQxTypes = 
            pGlobalDbContext->GetMapQxTypes();

        str sCeeClass = 
            pMapQxTypes->
                LookupCeeTypeForQxClass(
                    m_sQxClass);

        str sAssignCeeTo;
        str sAfterCeeAssign;
        if (sCeeClass != ""
            &&
            sCeeClass != "qxstring") 
            // key, utc etc cannot be turned to str
            //   and they will not generate NOP-warning if not assigned
        {
            sAssignCeeTo = 
                sCeeClass + " check_" + m_sUniqueNewQx + " = ";

            // silence "unused" warning
            sAfterCeeAssign = 
                " check_" + m_sUniqueNewQx + ";";
        }

        str sNewQx = 
            " "
            "\n"
            "// Qx: ----- new qx -----\n"
            "    " + m_sQxClass + " " + m_sUniqueNewQx + ";\n"
            "\n"
            "// Qx: ----- Binding -----\n"
            "\n" +
            pParseQxCode->x_sGeneratedBinding + "\n"
            "\n"
            "// Qx: ----- SQL -----\n"
            "\n"
            "    " + m_sUniqueNewQx + "._udb_SetSql(\n"
            "    \"" + pParseQxCode->x_sGeneratedSql + "\"\n"
            "    );\n"
            "\n"
            "// Qx: ----- C++ check -----\n"
            "\n"
            "    if (false)\n"
            "    {\n"
            "    " + sAssignCeeTo + pParseQxCode->x_sGeneratedCeeSyntax + " ;" +
                sAfterCeeAssign + "\n"
            "    }\n"
            "\n"
            "// Qx end.\n"
            "\n"
            ;

        pAssignmentStatement->
            AddAfterBeforeToken(
                sNewQx);
    }
    else
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xC055),
            "cannot locate assignment statement in which Qx expression "
                "is declared",
            this);
    }
}

void CAlterQx::OnRegenerate(CWriter* pWriter)
{
    //SetSourceFragmentToGenerated();

    if (m_sUniqueNewQx == "")
    {
        GetTokenizer()->ReportInternalTransformError(
            errn(0xC110),
            "m_sUniqueNewQx is not initialized in OnRegenerate",
            this);
    }

    pWriter->
        WriteTokenMultiLineStr(

[...]
