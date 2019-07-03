// CLogTypeIface.h
#pragma once

//
// CLogTypeIface -
//
//   Application error logs.
//
//ASSUME_IMPL_FOR_IFACE(CLogTypeImpl, CLogTypeIface)

class CLogTypeIface : public typeobject
{
public:
    CLogTypeIface();
    NEW_LOCAL(CLogTypeImpl, CLogTypeIface)
    //NEW_GP(Ralarm, CLogTypeImpl, CLogTypeIface)

// Constants

// Attributes

// Operations

    // Call this to add a log entry
    void LogJob(
            sloc slocFriendlyMessage,
            str sTechnicalDetails)
            vhook;

// Derived Item Enumeration

    const typelist<CLogTypeIface> GetDerivedList()
            vhook;

    type<CLogTypeIface> AsLogTypeForDatabaseErrors()
            vhook;
    type<CLogTypeIface> AsLogTypeForSenderErrors()
            vhook;
    type<CLogTypeIface> AsLogTypeForReceiverErrors()
            vhook;

// UI

protected:
    virtual void OnLogJob(
            sloc slocFriendlyMessage,
            str sTechnicalDetails)
            v1pure;
    virtual const typelist<CLogTypeIface> OnGetDerivedList()
            v1pure;
    virtual type<CLogTypeIface> OnAsLogTypeForDatabaseErrors()
            v1pure;
    virtual type<CLogTypeIface> OnAsLogTypeForSenderErrors()
            v1pure;
    virtual type<CLogTypeIface> OnAsLogTypeForReceiverErrors()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
