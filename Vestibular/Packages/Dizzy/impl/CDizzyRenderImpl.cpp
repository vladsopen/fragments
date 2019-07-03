// CDizzyRenderImpl.cpp

#include "CProject.h"
#include "CDizzyRenderImpl.h"
#include "CDbFlowConfigIface.h"
#include "CSessionIface.h"
#include "CDbStepConfigIface.h"
#include "CStepConfigIface.h"
#include "CDizzyAnimator.h"
#include "CDizzy3d.h"

CDizzyRenderImpl::CDizzyRenderImpl()
{
    _init_AttachToDizzyRenderImpl();
}

void CDizzyRenderImpl::_init_AttachToDizzyRenderImpl()
{
    //x_r->_x_p = this;
}

void CDizzyRenderImpl::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToDizzyRenderImpl();

    rFAIL("make sure it is OK to expose CDizzyRenderImpl!");
}

void CDizzyRenderImpl::OnRenderByStepConfig(
        ref<CDbStepConfigIface> rDbStepConfig,
        bool bAllowSecondMonitor,
        bool bForceSecondMonitor)
{
    this->x_rRenderingDbStepConfig = rDbStepConfig;
    m_bAllowSecondMonitor = bAllowSecondMonitor;
    m_bForceSecondMonitor = bForceSecondMonitor;

    UpdateRenderWindowSituation();
}

void CDizzyRenderImpl::OnStopRendering()
{
    this->x_rRenderingDbStepConfig = null();
    m_bForceSecondMonitor = false;

    UpdateRenderWindowSituation();
}

void CDizzyRenderImpl::OnClearPrimaryMonitor()
{
    if (!m_bUsingSecondMonitor)
    {
        StopRendering();
    }
}

void CDizzyRenderImpl::UpdateRenderWindowSituation()
{
    ref<CDbStepConfigIface> rDbStepConfig = 
        x_rRenderingDbStepConfig;

    // prepare animator
    x_rDizzyAnimator->x_rAnimateDbStepConfig = 
        x_rRenderingDbStepConfig;

    // close old windows
    _m_ptrmapUiDizzyRenderOutput.CloseAllFrames();

    // reopen again
    // we need to always change screen size
    //if (rDbStepConfig->x_bStepInUse)
    {
        //if (_m_ptrmapUiDizzyRenderOutput.GetCountClearingNulls() == 0)
        {
            //
            // Render on second monitor - fixed OpenGL window
            //

            rect rectSecondMonitor = 
                sys::GGetSecondaryMonitorRectUvCompatible();
                //sys_GetMonitorRectFromPoint(
                //    point(
                //        sys::GGetPrimaryMonitorResolution().w + 1,
                //        sys::GGetPrimaryMonitorResolution().h / 2));

            //if (!rDbFlowConfig->x_bUseExternalMonitorToDizzy)
            if (!m_bAllowSecondMonitor)
            {
                rectSecondMonitor = rect();
            }

            // resort to first monitor?
            if (rectSecondMonitor == rect() &&
                !m_bForceSecondMonitor)
            {
                size sizePrimary = sys::GGetPrimaryMonitorResolution();
                rectSecondMonitor = 
                    rect(0, 0, sizePrimary.w, sizePrimary.h);
            }

            // small preview window when no second monitor
            if ((
                    m_bForceSecondMonitor 
                    &&
                    rectSecondMonitor == rect()
                )
                ||
                debug("Dizzy.AlwaysSmallPreview?").InputBool()
                ||
                (sys::GIsAccess(...) && sys::GIsAccess(...)))
            {
                rect rectDesktop = sys::GGetPrimaryMonitorDesktopRect();
                // Preview: SetInstantSize
                // dont overlap dialog Close button
                int nWidth = 80;
                int nHeight = 60;
                //int nWidth = 80 * 2;
                //int nHeight = 60 * 2;
                rectSecondMonitor = 
                    rect(
                        rectDesktop.right - nWidth,
                        rectDesktop.bottom - nHeight,
                        rectDesktop.right,
                        rectDesktop.bottom);
            }

            if (rectSecondMonitor.left >= sys::GGetPrimaryMonitorResolution().w - 1)
            {
                m_bUsingSecondMonitor = true;
            }

            // force hide window for silly ATI
            if (!rDbStepConfig->x_bStepInUse)
            {
                rectSecondMonitor = rect(0, 0, 0, 0);
            }

            x_rDizzyAnimator->
                x_rDizzy3d->
                    SetWindowRect(
                        rectSecondMonitor);
        }
    }
}

ref<UiContainer> CDizzyRenderImpl::NewUiDizzyRenderOutput()
{
    ref<UiDizzyRenderOutput> rUi;

    rUi->m_rDizzyRender = this;

    return rUi;
}
