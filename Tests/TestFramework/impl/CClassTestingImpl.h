// CClassTestingImpl.h
#pragma once

#include "CClassTestingIfaceGp.h"
#include "VirtOs_CInterMem.h"

//
// CClassTestingImpl - see iface
//

class CClassTestingImpl : public CClassTestingIfaceGp
{
public:
    CClassTestingImpl();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

// UI

protected:
    virtual void OnRunClassTests(
            ref<CTestingOptionsIfaceGp> rTestingOptions)
            ;
    virtual ref<CTestingOptionsIfaceGp> OnGetRunningTestingOptions()
            ;
    virtual void On_internal_RestartVirtualTime()
            ;

private:

    // options we were started with
    ref<CTestingOptionsIfaceGp> m_rTestingOptions;

    // use us once only!
    bool _m_ClassTestingUsed = false;

    // class filter
    str m_sSkipTests;
    str m_sOnlyTest;

    // Selective profining
    str m_sProfileClass;

    str m_sTimingLog;

    // for delayed recursive calls
    static array< ref<CClassTestingImpl> > s_aRescheduledClassTesting;

    // never repeat
    //static map<classinfo, unused> s_mapTestedClasses;
    static map<classinfo, unused> s_mapDelayedClasses;
    //map<classinfo, unused> _m_mapTestedClasses;

    // the shared data
    struct DSharedTestState
    {
        static const int C_nMaxBytes = 100 * 1000;
        static const int C_tickLock = 1000;
        char _m_acTested[C_nMaxBytes + 1];
    };
    static interstruct<DSharedTestState> s_interstructSharedTestState;

    // LOCAL_TEST() caller
    void RunProjectLocalTestWithinClassTests(
            ref<CTestingOptionsIfaceGp> rTestingOptions);

    // For every project
    void RunSingleProjectLocalTestWithinClassTests(
            ref<CProject> rProject);

    // Main loop
    void TestAllRegisteredClasses(
            draw dcDisplay);

    // Called for every mathing classinfo
    bool TestByClassInfo(
            classinfo classinfoTest,
            draw dcDisplay);

    // Config reset
    void CleanupProjectAfterTest(
            ref<CTestingOptionsIfaceGp> rTestingOptions);

    // for delayed recursive calls
    void ScheduleDelayedTestRun(
            ref<CClassTestingImpl> rClassTesting);
    void CallDelayedTests();

    // Screen indicator
    void DisplayTestProgress(
            draw dcDisplay,
            str sMessage);

    // Abortion check
    bool NeedToStop();

    // Pretest cleanup
    void DeleteTestFiles();

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
