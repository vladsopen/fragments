// UiLoginDialogContent.cpp

#include "CProject.h"
#include "CLogin.h"
#include "CUserSelectorIface.h"
#include "CGlobalConfigIface.h"
#include "CUserListIface.h"
#include "CSessionImpl.h"
#include "VirtOs_CAppGp.h"
#include "CLimaStatusIfaceGp.h"
#include "CNetConnectionCommonIfaceGp.h"

void UiLoginDialogContent::Ui()
{
    m_bExtra = m_bExtra || sys::GIsAccess(...);

    ref<CSessionImpl> rSession = 
        m_rLogin->GetSession();
    ref<CUserListIface> rUserList =
        rSession->x_rUserList;

    ref<CUserSelectorIface> rSelector =
        CProject::GGetIt().
            x_rGlobalConfig->
                x_rLoggedInUserSelector;

    rSelector->x_bOnlyShowExpertUsers = true;

    bool bLocalSenderSetup =
        rSelector->GetSelectedUserKey() == 
            rUserList->GetLocalSenderUserKeyMaybeInactive();

    //
    // this
    //
    {
        ref<CUiPanelGp> rUi = this;

        m_rLogin->_m_ptrmapUiLoginDialogContent += rUi;

        rUi->SetDialogSubPanel();
    }

    //
    // top aligner
    //
    {
        ref<CUiPanelGp> rUi;

        rUi->SetMinX(scale(550));
        rUi->SetMinY(scale(400));

        rUi->SetAlignContentToTop();

        //
        // current section panel
        //
        {
            ref<CUiPanelGp> rUi;

            //rUi->SetAlignContentToTop();

            rUi->SetDialogSubPanel();

            rUi->SetMarginAtTop(scale(10));

            //rUi->SetMaxY(off);

            //
            // From Local: ...
            //
            {
                ref<CUiRadioGp> rUi;

                rUi->SetIdleUpdate(true);

                rUi->SetText(
                    TRANSLATE(
                        "Sending Or[... UI-string-removed]");

                rUi->SetTip(
                    TRANSLATE(
                        "Sending wo[... UI-string-removed]");

                //rUi->SetFontBold(true);

                rUi->SetAlikeX(_m_alikexEditLabel);

                rUi->SetCheck(
                    bLocalSenderSetup);

                rUi->OnClick = [=]
                {
                    rSelector->
                        SelectUserKey(
                            rUserList->GetLocalSenderUserKeyMaybeInactive(),
                            rUserList);

                    rUi->GetParentFrame()->InvalidateContent();
                }
            }
            {
                ref<CGlobalConfigIface> rConfig =
                    CProject::GGetIt().
                        x_rGlobalConfig;

                ref<CUiEditGp> rUi;

                rUi->SetText(
                    bLocalSenderSetup?
                        rConfig->x_sLocalUserName
                        :
                        "");

                rUi->SetPreferredX(scale(200));

                rUi->SetMaxX(oo);

                rUi->SetTipAsPrev();

                //rUi->SetReadOnlyMode(true);

                if (!bLocalSenderSetup)
                {
                    rUi->SetDisableInfo(
                        TRANSLATE(
                            "An expert [... UI-string-removed]");
                }

                rUi->OnComplete = [=]
                {
                    if (bLocalSenderSetup)
                    {
                        rConfig->x_sLocalUserName = 
                            rUi->InputPlainText().GetStripBoth().
                                GetLeftAbbrev(32, "|");

                        rUserList->
                            RefreshUser(
                                rUserList->GetLocalSenderUserKeyMaybeInactive(),
                                rConfig->x_sLocalUserName);
                    }
                }
            }

            NL();

            //
            // From Expert: ...
            //
            {
                ref<CUiRadioGp> rUi;

                rUi->SetIdleUpdate(true);

                rUi->SetText(
                    TRANSLATE(
                        "Receiving [... UI-string-removed]");

                rUi->SetTip(
                    TRANSLATE(
                        "Receiving [... UI-string-removed]");

                //rUi->SetFontBold(true);

                rUi->SetAlikeX(_m_alikexEditLabel);

                rUi->SetCheck(!bLocalSenderSetup);

                rUi->OnClick = [=]
                {
                    rSelector->
                        SelectUserKey(
                            key(),
                            rUserList);

                    rUi->GetParentFrame()->InvalidateContent();
                }
            }

            //
            // From: selector handler
            //
            {
                ref<CUiPanelGp> rUi;

                if (bLocalSenderSetup)
                {
                    rUi->SetDisableInfo(
                        TRANSLATE(
                            "Sending wo[... UI-string-removed]");
                }

                if (bLocalSenderSetup)
                {
                    //
                    // stub
                    //
                    {
                        ref<CUiStaticGp> rUi;

                        rUi->SetText("-");
                    }
                }
                else
                {
                    //
                    // User Selector
                    //
                    {
                        ref<UiContainer> rUi = 
                            rSelector->
                                NewUiUserSelectorDropDown(
                                    rUserList);
                    }
                }
            }

            NL();

            //
            // a pad
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(" ");
                rUi->SetFontSize(12);
            }

            NL();

            //
            // Key: ______ 
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(
                    TRANSLATE(
                        "Key[... UI-string-removed]");

                rUi->SetTip(
                    TRANSLATE(
                        "License ke[... UI-string-removed]");

                //rUi->SetFontBold(true);

                rUi->SetAlikeX(_m_alikexEditLabel);
            }
            {
                ref<CUiEditGp> rUi;

                rUi->SetText(
                    CAppGp::GGet()->
                        x_rLimaStatus->
                            GetCurrentTokenLabel().
                            GetStub("<demo>"));

                rUi->SetPreferredX(scale(200));

                rUi->SetMaxX(oo);

                rUi->SetTipAsPrev();

                rUi->SetReadOnlyMode(true);
                rUi->SetDisableInfo(
                    TRANSLATE(
                        "Automatic [... UI-string-removed]");

                rUi->OnChange = [=]
                {
                }
            }

            NL();

            //
            // Password: ______ 
            //
            {
                ref<CUiStaticGp> rUi;

                rUi->SetText(
                    TRANSLATE(
                        "Password:"[... UI-string-removed]");

                rUi->SetTip(
                    TRANSLATE(
                        "Personal p[... UI-string-removed]");

                //rUi->SetFontBold(true);

                rUi->SetAlikeX(_m_alikexEditLabel);
            }
            {
[...]
            