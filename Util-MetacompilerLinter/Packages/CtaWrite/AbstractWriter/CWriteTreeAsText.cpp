// CWriteTreeAsText.cpp
// (see "CWriteTree.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "CWriter.h"
#include "CWriteTree.h"

CWriteTreeAsText::CWriteTreeAsText()
: CWriteTree()
{
}

CWriteTreeAsText::CTreeLevelAsString::CTreeLevelAsString(
        CWriteTreeAsText* pParentWriteTree,
        nullable<CTreeLevel*> npParentLevel)
: CTreeLevel(pParentWriteTree, npParentLevel)
{
}

str CWriteTreeAsText::CTreeLevelAsString::OnExport()
{
    // Write Indents for All Parents
    CTreeLevel* pTravelTreeLevel = this;
    str appendResult;
    while (true)
    {
        if not_null(CTreeLevel* pParentTreeLevel = pTravelTreeLevel->GetParentLevel())
        {
            pTravelTreeLevel = pParentTreeLevel;
            appendResult += "    ";
        }
        else
            break;
    }

    appendResult += GetText();
    appendResult += C_szDosFileNewLine;
    return appendResult;
}

CWriteTreeAsText::CTreeLevel* CWriteTreeAsText::OnNewTreeLevel(
        nullable<CTreeLevel*> npParentLevel)
{
    return new CTreeLevelAsString(this, npParentLevel);
}
