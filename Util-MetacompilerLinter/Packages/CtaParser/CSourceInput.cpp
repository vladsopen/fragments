// CSourceInput.cpp
// (see "CSourceInput.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "CSourceInput.h"

CSourceInput::CSourceInput()
{
    m_sText;
    m_iCurrentPos = 0;
}

void CSourceInput::SetSourceText(str sSourceText)
{
    // Only used once
    ASSERT(m_sText.Len() == 0);
    ASSERT(m_iCurrentPos == 0);

    m_sText = sSourceText;
    m_iCurrentPos = 0;
}

const char* CSourceInput::GetCurrentSourcePointer()
{
    return m_sText.c() + m_iCurrentPos;
}

int CSourceInput::GetRemainedLength()
{
    return m_sText.Len() - m_iCurrentPos;
}

int CSourceInput::ConvertSourcePointerToIndex(const char* szSourcePos)
{
    ASSERT(szSourcePos >= m_sText.c());
    ASSERT(szSourcePos <= m_sText.c() + m_sText.Len());
    ASSERT_SOFT(szSourcePos >= GetCurrentSourcePointer());

    return szSourcePos - m_sText.c();
}

bool CSourceInput::IsWithinProcessedInput(const char* szSourcePos)
{
    return szSourcePos >= m_sText.c() &&
           szSourcePos < GetCurrentSourcePointer();
}

bool CSourceInput::IsWithinSourceText(const char* szSourcePos)
{
    return szSourcePos >= m_sText.c() &&
           szSourcePos < m_sText.c() + m_sText.Len();
}

void CSourceInput::AdvanceCurrentPosition(int nMoveForward)
{
    ASSERTM(nMoveForward > 0,
            "an attempt to move back in the source input stream");
    ASSERTM(m_iCurrentPos + nMoveForward <= m_sText.Len(),
            "an attempt to move over the EOF in the source input stream");

    m_iCurrentPos += nMoveForward;

    ASSERT(m_iCurrentPos <= m_sText.Len()); // can be at EOF but no further
}

void CSourceInput::RestoreCurrentPositionTo(const char* szNewCurrentPos)
{
    if ok(szNewCurrentPos >= m_sText.c() &&
          szNewCurrentPos < m_sText.c() + m_sText.Len())
    {
        m_iCurrentPos = szNewCurrentPos - m_sText.c();
    }
}

str CSourceInput::GetSourceContextQuote(const char* szAroundPoint)
{
    // Verify the source pointer
    if (!IsWithinSourceText(szAroundPoint))
    {
        szAroundPoint = GetCurrentSourcePointer();
        //return "<<failed to report source text quote>>\n";
    }

    // Find the start position to report
    const char* szReport = szAroundPoint;
    {
        int nLineCount = 0;
        int nCharCount = 0;
        while (szReport > m_sText.c())
        {
            szReport--;
            if (szReport[0] == '\n')
            {
                nLineCount++;
                if (nLineCount > C_nMinLinesToGoBack &&
                    nCharCount > C_nMinCharsToGoBack)
                    break;
            }
            else
            {
                nCharCount++;
            }
        }
    }

    // To the start of line
    if (szReport[0] == '\n')
        szReport++;

    // Go to the end of fragment
    const str C_sLinePrefix = ">>> ";
    str sResult = C_sLinePrefix;
    {
        int nLineCount = 0;
        int nCharCount = 0;
        while (szReport[0] != 0)
        {
            // Next char
            char cCurrent = szReport[0];
            szReport++;

            // Ignore
            if (cCurrent == '\r')
                continue;

            // New line?
            if (cCurrent == '\n')
            {
                // Count lines since szAroundPoint position
                if (szReport > szAroundPoint)
                    nLineCount++;
                nCharCount++;

                // Mark the reported point
                if (nLineCount == 1)
                    sResult += "     <--";

                // Enough text dumped?
                if (nLineCount > C_nMinLinesToGoForward &&
                    nCharCount > C_nMinCharsToGoForward)
                    break;
            }
            else
            {
                // Count chars since szAroundPoint position
                if (szReport > szAroundPoint)
                    nCharCount++;
            }
            sResult += StrChar(cCurrent);
        }
    }

    sResult = sResult.
        GetStripRight(CHARMAP("\r\n")).
            GetReplaced("\n", "\n" + C_sLinePrefix + " ");

    return sResult + "\n";
}
