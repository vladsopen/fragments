// CLogTypeImpl.h
#pragma once

#include "CLogTypeIface.h"

class CAppEventFloaterIfaceGp;

//
// CLogTypeImpl - see Iface
//

class CLogTypeImpl : public CLogTypeIface
{
public:
    CLogTypeImpl();
    DECLARE_NULL_TYPE(GetDerivedList);

// Constants

// Attributes

    // Floater title
    sloc GetLogCaption()
            vhook;

    // Window icon and color
    res GetLogSeverityIcon()
            vhook;

// Data Item Access

    // Optimized item search
    ptr<CAppEventFloaterIfaceGp> LookupByKey(
            type<CLogTypeIface> typeLogType);

// Operations

// Derived Item Enumeration

    const typelist<CLogTypeImpl> GetDerivedListImpl();

// UI

protected:
    virtual void OnLogEvent(
            sloc slocFriendlyMessage,
            str sTechnicalDetails)
            vcontinue;

    virtual const typelist<CLogTypeIface> OnGetDerivedList()
            ;

    virtual type<CLogTypeIface> OnAsLogTypeForDatabaseErrors()
            vnull;
    virtual type<CLogTypeIface> OnAsLogTypeForDemoError()
            vnull;
    virtual type<CLogTypeIface> OnAsLogTypeForProxyError()
            vnull;

    // Impl
    virtual sloc OnGetLogCaption()
            v1pure;
    virtual res OnGetLogSeverityIcon()
            v1pure;

private:
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // PatientIo / PatientEmbed errors
    class CLogTypeForDatabaseErrors : public CLogTypeImpl
    {
    public:
        void flag_List_as_LogTypeImpl();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(P e x L);

        virtual sloc OnGetLogCaption()
                return
                    TRANSLATE(
                        "Patient da[... UI-string-removed]");

        virtual res OnGetLogSeverityIcon()
                return
                    RES_STD_ERROR;
                    //RES_STD_WARNING;

        // you can override 'LogNewEvent' for this type here
        virtual void OnLogEvent(
                sloc slocFriendlyMessage,
                str sTechnicalDetails)
                do
                    __super::OnLogEvent(
                        slocFriendlyMessage,
                        sTechnicalDetails);
    };

    // Demo failures
    class CLogTypeForDemoError : public CLogTypeImpl
    {
    public:
        void flag_List_as_LogTypeImpl();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(D e m o);

        virtual sloc OnGetLogCaption()
                return
                    TRANSLATE(
                        "Demo mode [... UI-string-removed]");

        virtual res OnGetLogSeverityIcon()
                return
                    RES_STD_ERROR;
                    //RES_STD_WARNING;

        // you can override 'LogNewEvent' for this type here
        virtual void OnLogEvent(
                sloc slocFriendlyMessage,
                str sTechnicalDetails)
                do
                    __super::OnLogEvent(
                        slocFriendlyMessage,
                        sTechnicalDetails);
    };

    // Proxy failures
    class CLogTypeForProxyError : public CLogTypeImpl
    {
    public:
        void flag_List_as_LogTypeImpl();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(P r x y);

        virtual sloc OnGetLogCaption()
                return
                    TRANSLATE(
                        "Proxy debu[... UI-string-removed]");

        virtual res OnGetLogSeverityIcon()
                return
                    RES_STD_ERROR;
                    //RES_STD_WARNING;

        // you can override 'LogNewEvent' for this type here
        virtual void OnLogEvent(
                sloc slocFriendlyMessage,
                str sTechnicalDetails)
                do
                    __super::OnLogEvent(
                        slocFriendlyMessage,
                        sTechnicalDetails);
    };
