// CProcedureIface.cpp

#include "CProject.h"
#include "CProcedureIface.h"
#include "CProcedureImpl.h"
#include "CRecordTypeIface.h"

CProcedureIface::CProcedureIface()
{
    _init_AttachToProcedureIface();
}

void CProcedureIface::_init_AttachToProcedureIface()
{
    //x_r->_x_p = this;
}

void CProcedureIface::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToProcedureIface();

    rFAIL("make sure it is OK to expose CProcedureIface!");
}
