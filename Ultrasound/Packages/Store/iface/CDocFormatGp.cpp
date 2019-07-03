// CDocFormatGp.cpp

#include <VirtOs.h>
#include "CDocFormatGp.h"
#include "CMatrixFormatGp.h"
#include "CRawSlotGp.h"

CDocFormatGp::CDocFormatGp()
{
}

void CDocFormatGp::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    rFAIL("make sure it is OK to expose CDocFormatGp!");
}
