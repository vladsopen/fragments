// CDeviceImpl.cpp

#include "CProject.h"
#include "CDeviceImpl.h"
#include "CConnection.h"
#include "CUcomEnumeratorIfaceGp.h"
#include "CAppEventFloaterIfaceGp.h"
#include "CUcomDeviceIfaceGp.h"
#include "CUcomDetectionCallback.h"
#include "CSessionIface.h"
#include "CPatientLinkIface.h"
#include "CPolling.h"
#include "CProcedureIface.h"
#include "CKeyboardSimulator.h"
#include "CGlobalConfigIface.h"
#include "CUcomDriverEnumeratorForEmulatorIface.h"
#include "VirtOs_CAppGp.h"
#include "VirtOs_CGlobalUi.h"
#include "CNetLink.h"

CDeviceImpl::CDeviceImpl()
{
    _init_AttachProperties();
}

void CDeviceImpl::HandleRedetectTimer()
{
    bool bNeedRedetect = false;

    bool bNetInput = 
        x_rNetLink->
            IsNetInputReplacingRealDevice();

    if (bNetInput != m_bNetInput)
    {
        m_bNetInput = bNetInput;
        bNeedRedetect = true;
    }

    if (!x_rConnection->x_bPosturalDetected ||
        x_rConnection->x_slocPosturalConnectionError != "" ||
        m_bConnectedToEmulator != 
            CProject::GGetIt().x_rGlobalConfig->
                _x_bDeviceSignalEmulator)
    {
        if (!bNetInput)
        {
            bNeedRedetect = true;
        }
    }

    if (bNeedRedetect)
    {
        HandleNeedAutoRedetect();
    }
}

void CDeviceImpl::OnRedetectDevice()
{
    static int s_nDetectionCount = 0;
    s_nDetectionCount++;
    debug("Device./show.RedetectTry:/4e").
        FloatBlink("Detecting... [" + Str(s_nDetectionCount) + "]");

    // start redetect timer
    _m_ontimerHandleRedetect.
        SetTimeoutInTicks(
            C_tickMinAutoRedetectAfterFailure,
            this);

    // End previous connection
    x_rConnection->EndPosturalConnection();
    this->x_rConnection = null();
    _init_AttachProperties();

    // start
    x_rConnection->x_bPosturalScanned = true;
    x_rConnection->x_slocPosturalConnectionError = 
        "Connection aborted. /ERR:NMNI/";

    //
    // Input from network?
    //

    bool bNetInput = 
        x_rNetLink->
            IsNetInputReplacingRealDevice();

    ref<CGlobalConfigIface> rGlobalConfig = 
        CProject::GGetIt().x_rGlobalConfig;
    if (rGlobalConfig->_x_bDeviceSignalEmulator
        ||
        bNetInput)
    {
        rGlobalConfig->_x_bDeviceSignalEmulator = true;

        // Emulator replaces all default enumerators here
        _m_rUcomEnumerator->
            AddUcomDriverEnumerator(
                ref<CUcomDriverEnumeratorForEmulatorIface>(),
                CUcomEnumeratorIfaceGp::E_AddEnumerator_ReplaceAll);

        m_bConnectedToEmulator = true;
    }
    else
    {
        m_bConnectedToEmulator = false;
    }

    int nDevices = _m_rUcomEnumerator->GetMatchingUcomDeviceCount();
    if (nDevices > 1)
    {
        ReportDeviceWarning(
            TRANSLATE(
                "Multiple p[... UI-string-removed]");
    }

    x_rConnection->x_bPosturalDetected = (nDevices >= 1);

    rASSERT(nDevices == 1 || !x_rConnection->x_bPosturalDetected);

    ref<CUcomDeviceIfaceGp> rNewUcomDevice;
[...]
}

