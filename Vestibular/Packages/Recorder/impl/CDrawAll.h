// CDrawAll.h
#pragma once

#include "CDrawAnythingRecorded.h"

//
// CDrawAll - 
//
//   All graphs at once.
//
//ASSUME_IMPL_FOR_IFACE(CDrawAllImpl, CDrawAll)

class CDrawAll : public CDrawAnythingRecorded
{
public:
    CDrawAll();
    //NEW_LOCAL(CDrawAllImpl, CDrawAll)
    //NEW_GP(MyProject, CDrawAllImpl, CDrawAll)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent - see base

// Operations

// UI

    // DrawAll
    ref<UiContainer> NewUiDrawAll();
    ptrmap<CUiPanelGp> _m_ptrmapUiDrawAll;

protected:
    virtual void OnHandleRecordingChange()
            ;
    virtual void OnResetScalling()
            vnull;
    virtual ref<UiContainer> OnNewUiDrawMainGraph()
            ;
private:
    //bool _m_bOneTimeInitOk = false;

    void _init_AttachToDrawAll();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

// (UiDrawAll.cpp)
class UiDrawAll : public UiContainer
{
public:
    virtual void Ui();

    ref<CDrawAll> m_rDrawAll;

private:
    void EqualizeRouteGraphSizes(
            ref<CUiPanelGp> rUi);
};
