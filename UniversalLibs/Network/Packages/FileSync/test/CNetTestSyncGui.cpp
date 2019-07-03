// CNetTestSyncGui.cpp

#include "CNetProject.h"
#include "CNetTestSyncGui.h"
#include "VirtOs_file.h"
#include "CNetDriverSendatumIfaceGp.h"
#include "CFileMultipartSplitIfaceGp.h"
#include "CNetSyncProgressIfaceGp.h"
#include "CNetSyncOutImpl.h"
#include "CNetSyncInImpl.h"
#include "CNetLocatorIfaceGp.h"
#include "VirtOs_dir.h"
#include "VirtOs_CPathList.h"
#include "VirtOs_ask.h"

CNetTestSyncGui::CNetTestSyncGui()
{
    _init_AttachToNetTestSyncGui();
}

void CNetTestSyncGui::_init_AttachToNetTestSyncGui()
{
    //x_r->_x_p = this;
}

void CNetTestSyncGui::OnExposeContent(
        ref<CExpose> rExpose)
{
    __super::OnExposeContent(rExpose);
    OnExposeContentGenerated(rExpose);

    // update child pointers
    _init_AttachToNetTestSyncGui();

    rFAIL("make sure it is OK to expose CNetTestSyncGui!");
}

/*
CNetTestSyncGui::~CNetTestSyncGui()
{
    rASSERT(!_m_bOpened);
}

void CNetTestSyncGui::CloseNetTestSyncGui()
{
    if (_m_bOpened)
    {
        _m_bOpened = false;
        // ...
    }
}
*/

void CNetTestSyncGui::InitNetTestSyncGui()
{
    if (_m_bOpened)
    {
        return;
    }

    rASSERT(!_m_bInited);
    _m_bInited = true;
    _m_bOpened = true;

    _m_ontimerTestSync.
        SetTimeoutInTicks(
            300,
            this);

    path pathTop =
        sys::GGetTempFolder().
            GetAppendedPath("TestSyncGui");
    x_pathTop = pathTop;

    x_pathSendFile = 
        pathTop.
            GetAppendedPath("SendMe.txt");

    x_pathSendFolder = 
        pathTop.
            GetAppendedPath("SendAll");
    dir(x_pathSendFolder).CreateDir(out_IGNORE_sError);

    x_pathUploadTemp =
        pathTop.
            GetAppendedPath("UploadQueue");

    x_pathDownloadTemp =
        pathTop.
            GetAppendedPath("DownloadParts");

    x_pathReceptionFolder =
        pathTop.
            GetAppendedPath("ReceiveTest");

    if (!file(x_pathSendFile).IsExist())
    {
        str sTest = 
            ""
                "The Internet is the global system of interconnected computer networks"
                " that use the Internet protocol suite (TCP/IP) to link devices worldwide. It is a network of networks that consists of private, public, academic, business, and government networks of local to global scope, linked by a broad array of electronic, wireless, and optical networking technologies. The Internet carries an extensive range of information resources and services, such as the inter-linked hypertext documents and applications of the World Wide Web (WWW), electronic mail, telephony, and peer-to-peer networks for file sharing.\n"
                "The origins of the Internet date back to research commissioned by the United States federal government in the 1960s to build robust, fault-tolerant communication via computer networks.[1] The primary precursor network, the ARPANET, initially served as a backbone for interconnection of regional academic and military networks in the 1980s. The funding of the National Science Foundation Network as a new backbone in the 1980s, as well as private funding for other commercial extensions, led to worldwide participation in the development of new networking technologies, and the merger of many networks.[2] The linking of commercial networks and enterprises by the early 1990s marks the beginning of the transition to the modern Internet,[3] and generated a sustained exponential growth as generations of institutional, personal, and mobile computers were connected to the network. Although the Internet was widely used by academia since the 1980s, the commercialization incorporated its services and technologies into virtually every aspect of modern life.\n"
                "Internet use grew rapidly in the West from the mid-1990s and from the late 1990s in the developing world.[4] In the 20 years since 1995, Internet use has grown 100-times, measured for the period of one year, to over one third of the world population.[5][6] Most traditional communications media, including telephony, radio, television, paper mail and newspapers are being reshaped or redefined by the Internet, giving birth to new services such as email, Internet telephony, Internet television music, digital newspapers, and video streaming websites. Newspaper, book, and other print publishing are adapting to website technology, or are reshaped into blogging, web feeds and online news aggregators (e.g., Google News). The entertainment industry was initially the fastest growing segment on the Internet.[citation needed] The Internet has enabled and accelerated new forms of personal interactions through instant messaging, Internet forums, and social networking. Online shopping has grown exponentially both for major retailers and small businesses and entrepreneurs, as it enables firms to extend their "bricks and mortar" presence to serve a larger market or even sell goods and services entirely online. Business-to-business and financial services on the Internet affect supply chains across entire industries.\n"
                "The Internet has no centralized governance in either technological implementation or policies for access and usage; each constituent network sets its own policies.[7] Only the overreaching definitions of the two principal name spaces in the Internet, the Internet Protocol address space and the Domain Name System (DNS), are directed by a maintainer organization, the Internet Corporation for Assigned Names and Numbers (ICANN). The technical underpinning and standardization of the core protocols is an activity of the Internet Engineering Task Force (IETF), a non-profit organization of loosely affiliated international participants that anyone may associate with by contributing technical expertise.[8]\n"
                "";

        str sError;
        file(x_pathSendFile).Save(sTest, out sError);
        rASSTR(sError);
    }

    // Init Out
    {
        ref<CNetLocatorIfaceGp> rNetLocator = 
            NewCommonLocator();

        rNetLocator->
            _x_pathLocalTempRoot = 
                x_pathUploadTemp;

        x_rNetSyncOut->x_rNetLocator = rNetLocator;

        x_rNetSyncOut->
            InitNetSyncOut(
                ref<CNetDriverSendatumIfaceGp>());
    }

    // Init In
    {
        ref<CNetLocatorIfaceGp> rNetLocator = 
            NewCommonLocator();

        rNetLocator->
            _x_pathLocalTempRoot = 
                x_pathDownloadTemp;

        x_rNetSyncIn->x_rNetLocator = rNetLocator;

        x_rNetSyncIn->
            InitNetSyncIn(
                ref<CNetDriverSendatumIfaceGp>());
    }
}

ref<CNetLocatorIfaceGp> CNetTestSyncGui::NewCommonLocator()
{
    ref<CNetLocatorIfaceGp> rNetLocator;

    rNetLocator->x_bEnableGrowCount =
        //false; // files AS THEY ARE!
        true; 

    rNetLocator->x_bParapack = 
        false; // simplify test

    /*
    rNetLocator->
        _x_slocDownloaderIdleStatus =
            TRANSLATE(
                "Request is[... UI-string-removed]");

    rNetLocator->
        _x_slocRecordsInUploadQueue =
            TRANSLATE(
                "To sen[... UI-string-removed]");
    */

    rNetLocator->
        x_sRemoteChannel = 
            "Test/NetSync/v01";

    rNetLocator->
        x_bAllowAutoDeleteOldFiles = 
            false;

    rNetLocator->
        x_tickStallRestartTimeout = 
            60000;

    rNetLocator->
        x_tickPollingTimeout = 
            1000;
            //oo; // external polling only

    /*
    rNetLocator->
        x_sServerDomainsOrIps =
            "[Logohere].com";
            */

    /*
    rNetLocator->
        x_rServerLogin = 
            rNet->GetNetLogin();
            */

    //ref<CNetDriverSendatumIfaceGp> rNetDriverSendatum;

    return rNetLocator;
}

void CNetTestSyncGui::SendFile(
        path pathSendFile)
{
    unidef unidefExtraHeader;
    unidefExtraHeader.DefineStr("AppTestExtra", "my cookie!");

    ref<CNetSyncProgressIfaceGp> rProgress;
    x_rNetSyncOut->
        StartFileSync(
            pathSendFile,
            pathSendFile.
                GetFolder().
                GetStripAtStart(
                    x_pathTop.Get().GetSlashed()), // sRestoreIntoSubdirs
            unidefExtraHeader,
            "", // sSecurityId
            ptr<CFileMultipartSplitIfaceGp>(),
            out rProgress);

    _m_utcLastSend = sys::GGetUtc();
}

void CNetTestSyncGui::SendFolder()
{
    this->x_rPathListTodoSend = 
        dir(x_pathSendFolder).
            GetFilesAndDirectoriesIncludingSubfolders(oo);

    if (this->x_rPathListTodoSend->GetCount() == 0)
    {
        ask("Empty source folder!");

        sys::GOpenExplorer(
            x_pathSendFolder,
            sys::E_ExplorerView_Tree,
            sys::E_ExplorerOpen_Folder);
    }
}

void CNetTestSyncGui::HandleNetTestSync()
{
    //
    // Add next file?
    //

    if (x_rPathListTodoSend->GetCount() > 0)
    {
        if (sys::GGetUtc() - _m_utcLastSend > 2000)
        {
            repeat(iSeekToFile, x_rPathListTodoSend->GetCount())
            {
                path pathSend = 
                    x_rPathListTodoSend->GetAt(0);
                x_rPathListTodoSend->RemoveAt(0);

                if (file(pathSend).IsExist())
                {
                    SendFile(pathSend);

                    break;
                }
            }
        }
    }

    //
    // I/o
    //

    x_rNetSyncIn->
        PollNetSyncIn(
            x_pathReceptionFolder,
            "*");

    // for ALL listed dirs
    //_m_ptrmapUiNetTestSyncGuiDir.InvalidateContent();
    //_m_ptrmapUiNetTestSyncGuiMain.InvalidateTreeState();
}

ref<UiContainer> CNetTestSyncGui::NewUiNetTestSyncGuiMain()
{
    InitNetTestSyncGui();

    ref<UiNetTestSyncGuiMain> rUi;

    rUi->m_rNetTestSyncGui = this;

    return rUi;
}

ref<UiContainer> CNetTestSyncGui::NewUiNetTestSyncGuiDir(
        path pathListFolder)
{
    ref<UiNetTestSyncGuiDir> rUi;

    rUi->m_rNetTestSyncGui = this;
    rUi->m_pathListFolder = pathListFolder;

    return rUi;
}

ref<UiContainer> CNetTestSyncGui::NewUiNetTestSyncGuiFile(
        path pathListedFile)
{
    ref<UiNetTestSyncGuiFile> rUi;

    rUi->m_rNetTestSyncGui = this;
    rUi->m_pathListedFile = pathListedFile;

    return rUi;
}
