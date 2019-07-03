// CSourceLines.cpp
// (see "CSourceLines.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "CSourceLines.h"

CSourceLines::CSourceLines()
{
    m_aLineInfo;

    m_iMaxSourceTextPosReached = -1;

    m_iCurrentOriginalLineNumber = 0;
    m_pathCurrentOriginalSourceFile = "";
}

void CSourceLines::PrepareToSourceText(
        //[Range(0 ... oo), SoftMax(1000000)]
        int nSourceTextLength,
        //[Assert(Len() > 0)]
        path pathInitialSourceFile)
{
    ASSERTM(m_aLineInfo.GetCount() == 0,
            "PrepareToSourceText() must be called *before* translation started");
    ASSERT(nSourceTextLength >= 0);
    ASSERT(pathInitialSourceFile.Len() > 0);

    m_aLineInfo.SetCount(
        m_aLineInfo.GetCount(),
        nSourceTextLength / (C_nAverageCharsPerLine * 2));

    m_pathCurrentOriginalSourceFile = pathInitialSourceFile;

    // Start first line
    AddAnotherLine();
}

nullable<CSourceLines::CLineInfo*> CSourceLines::GetLineInfo(
        int iParsedLine)
{
    nullable<CLineInfo*> npLineInfo;

    // Return null if no such line number
    if (iParsedLine >= 0 &&
        iParsedLine < m_aLineInfo.GetCount())
    {
        npLineInfo = m_aLineInfo[iParsedLine];
    }

    return npLineInfo;
}

void CSourceLines::HandleAnotherNewLine(
        int iSourceTextPos)
{
    ASSERT(iSourceTextPos >= 0);
    ASSERTM(m_aLineInfo.GetCount() > 0,
            "missing CSourceLines::PrepareToSourceText()?");

    // Already parsed at this pos?
    if (iSourceTextPos <= m_iMaxSourceTextPosReached)
        return;

    // Store the pos in case tokenizer will go backward
    m_iMaxSourceTextPosReached = iSourceTextPos;
    m_iCurrentOriginalLineNumber++;

    // Start next line
    AddAnotherLine();
}

void CSourceLines::AddAnotherLine()
{
    CLineInfo* pNewLineInfo = new CLineInfo;
    m_aLineInfo.Add(pNewLineInfo);
    pNewLineInfo->SetOriginalSourceFile(m_pathCurrentOriginalSourceFile);
    pNewLineInfo->SetOriginalLineNumber(m_iCurrentOriginalLineNumber);
}

CSourceLines::CLineInfo* CSourceLines::GetLastLineInfo()
{
    return m_aLineInfo.GetLast();
}

void CSourceLines::SetCurrentOriginalLineNumber(int iOriginalLineNumber)
{
    ASSERT(iOriginalLineNumber >= 0 ||
           iOriginalLineNumber == 0 - 1 // in case #line directive is in the first line
           );
    ASSERT_SOFT(iOriginalLineNumber < 1000000);

    CLineInfo* pLineInfo = GetLastLineInfo();
    pLineInfo->SetOriginalLineNumber(iOriginalLineNumber);

    m_iCurrentOriginalLineNumber = iOriginalLineNumber;
}

void CSourceLines::SetCurrentOriginalSourceFile(path pathOriginalSourceFile)
{
    ASSERT(pathOriginalSourceFile.Len() > 0);
    ASSERT_SOFT(pathOriginalSourceFile.Len() > sizeof("x:\\f.h")-1);

    CLineInfo* pLineInfo = GetLastLineInfo();
    pLineInfo->SetOriginalSourceFile(pathOriginalSourceFile);

    m_pathCurrentOriginalSourceFile = pathOriginalSourceFile;
}

int CSourceLines::GetCurrentOriginalLineNumber()
{
    return m_iCurrentOriginalLineNumber;
}

void CSourceLines::PreservePreprocessorDirective(str sPreprocessorDirective)
{
    CLineInfo* pLineInfo = GetLastLineInfo();
    pLineInfo->SetPreservedDirective(sPreprocessorDirective);
}

str CSourceLines::GetSourceFileLineReference(int iLineInfo)
{
    str sReferenceText;

    // Get line info
    nullable<CLineInfo*> npLineInfo = GetLineInfo(iLineInfo);

    if (npLineInfo.IsNull())
    {
        sReferenceText +=
            "<<invalid source line #" +
            Str(iLineInfo + 1) +
            ">>\r\n";

        // Report for the last file line
        iLineInfo = m_aLineInfo.GetCount() - 1;
    }

    // Reget line info
    npLineInfo = GetLineInfo(iLineInfo);

    if not_null(CLineInfo* pLineInfo = npLineInfo)
    {
        sReferenceText +=
            pLineInfo->GetOriginalSourceFile().GetReplaced("\\\\", "\\") +
            "(" +
            Str(pLineInfo->GetOriginalLineNumber() + 1) +
            ")";
    }

    return sReferenceText;
}
