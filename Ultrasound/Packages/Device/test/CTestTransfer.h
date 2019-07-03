// CTestTransfer.h
#pragma once

#include "CTestTransferBase.h"

class CUsbForEmulator;

//
// CTestTransfer - 
//
//   UT for online scanner firmware communication for raw audio input
//
//ASSUME_IMPL_FOR_IFACE(CTestTransferImpl, CTestTransfer)

class CTestTransfer : public CTestTransferBase
{
public:
    CTestTransfer();
    //NEW_LOCAL(CTestTransferImpl, CTestTransfer)
    //NEW_GP(MyProject, CTestTransferImpl, CTestTransfer)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

    void RunStressTest();

    void RunOnlineTest();

// UI

protected:
    virtual void OnHandleRawAudioSendToSinkForCluster(
            int iCluster,
            sbuf sbufRawAudio)
            ;
private:

    array<sbuf> m_asbufRawAudioSendToSinkForClusters;
    int m_iRandomBytePosition = 0;

    void CheckInputForAllClusters(
            ref<CUsbForEmulator> rUsb);

    void CheckClusterRawAudio(
            int iCluster,
            sbuf sbufCluster,
            ref<CUsbForEmulator> rUsb);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    virtual void OnTestClass();
};
