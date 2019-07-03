// CTestingOptionsIfaceGp.h
#pragma once

class CProject;

//
// CTestingOptionsIfaceGp - 
//
//   ClassTest runtime options
//

class CTestingOptionsIfaceGp : public object
{
public:
    CTestingOptionsIfaceGp();
    // this should not have an Impl 
    //   which would force load dll all the time!
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Where we called from? (see also x_bOnlyRunTestsForOneProject)
    ptr<CProject> _x_pTestInitiatingProject
            xauto(Get, Set);

    // For Retail mode demo tests
    bool x_bFakeVisualTests = false
            xauto(Get, Set);

    // We run either all initially or separate project upon loading
    bool x_bOnlyRunTestsForOneProject = false
            xauto(Get, Set);

    // Need to disable it 
    bool x_bDumpConverageForMultipleProjects = true
            xauto(Get, Set);

// Operations

// UI

protected:
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
