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
    //NEW_GP(CustomApps, CLogTypeImpl, CLogTypeIface)

// Constants

// Attributes

// Operations

    // Call this to add a log entry
    void LogEvent(
            sloc slocFriendlyMessage,
            str sTechnicalDetails)
            vhook;

// Derived Item Enumeration

    const typelist<CLogTypeIface> GetDerivedList()
            vhook;

    type<CLogTypeIface> AsLogTypeForDatabaseErrors()
            vhook;
    type<CLogTypeIface> AsLogTypeForDemoError()
            vhook;
    type<CLogTypeIface> AsLogTypeForProxyError()
            vhook;

// UI

protected:
    virtual void OnLogEvent(
            sloc slocFriendlyMessage,
            str sTechnicalDetails)
            v1pure;
    virtual const typelist<CLogTypeIface> OnGetDerivedList()
            v1pure;
    virtual type<CLogTypeIface> OnAsLogTypeForDatabaseErrors()
            v1pure;
    virtual type<CLogTypeIface> OnAsLogTypeForDemoError()
            v1pure;
    virtual type<CLogTypeIface> OnAsLogTypeForProxyError()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
