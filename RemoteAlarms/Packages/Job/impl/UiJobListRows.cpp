// UiJobListRows.cpp

#include "CProject.h"
#include "CJobListImpl.h"
#include "CJobStateImpl.h"
#include "CJobFilterSetup.h"

void UiJobListRows::Ui()
{
    sloc slocFilterState = 
        m_rJobList->
            x_rJobFilterSetup->
                GetFilteringStateReport();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rJobList->_m_ptrmapUiJobListRows += rUi;
    }

    //
    // Filter State report
    //
    {
        ref<CUiStaticGp> rUi;

        rUi->SetText(
            slocFilterState);

        rUi->SetAlignContentToCenterX();
        rUi->SetMaxX(oo);
    }

    NL();

    // iterate events
    {
        ref<CJobStateIface> rIterJobIface;
        int nVisible = 0;
        for (
            iter i;
            m_rJobList->
                IterateJobs(
                    out i,
                    out rIterJobIface);)
        {
            ref<CJobStateImpl> rIterJob = 
                rIterJobIface;

            if (!m_rJobList->
                    x_rJobFilterSetup->
                        IsJobVisible(
                            rIterJob))
            {
                continue;
            }

            nVisible++;

            //
            // Next Job
            //
            {
                ref<UiContainer> rUi = 
                    rIterJob->
                        NewUiJobStateRow();
            }

            NL();
        }

        if (nVisible == 0)
        {
            //
            // "Nothing here"
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(
                    slocFilterState == ""?
                        TRANSLATE(
                            "No package[... UI-string-removed]");

                rUi->SetAlignContentToCenterX();
                rUi->SetMaxX(oo);
            }

            NL();
        }
    }
}
