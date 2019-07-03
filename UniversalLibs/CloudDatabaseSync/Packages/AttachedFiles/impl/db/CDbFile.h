// CDbFile.h
#pragma once

//
// CDbFile - 
//
//   File attributes added to CDbNode base class
//
//ASSUME_IMPL_FOR_IFACE(CDbFileImpl, CDbFile)

class CDbFile : public dbobject
{
public:
    CDbFile();
    //~CDbFile();
    //NEW_LOCAL(CDbFileImpl, CDbFile)
    //NEW_GP(CloudSync, CDbFileImpl, CDbFile)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Original name
    str x_sFileName = ""
            xauto(Get, Set)
            xassert(value >= 0);

    // We store OS-independent dir as "Dir/Subdir", "MyDir", "", ...
    str x_sFileDir = ""
            xauto(Get, Set)
            xassert(value >= 0);

    // Size
    big x_nnFileBytes = 0
            xauto(Get, Set)
            xassert(value >= 0);

    // Upload progress tracker
    big x_nnFileUploadedBytes = -1
            xauto(Get, Set)
            xassert(value >= -1);

    // Timestamp
    moment x_momentFileLastModify = moment()
            xauto(Get, Set);

// UI

protected:
private:
    //bool _m_bOneTimeInitDbFileOk = false;
    //bool _m_bDbFileOpened = false;

    void _init_AttachToDbFile();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
