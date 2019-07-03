// CSyncTypeImpl.h
#pragma once

#include "CSyncTypeIface.h"

//
// CSyncTypeImpl - see Iface
//

class CSyncTypeImpl : public CSyncTypeIface
{
public:
    CSyncTypeImpl();
    DECLARE_NULL_TYPE(GetDerivedList);

// Constants

// Attributes

    // short marker
    str GetLogLetter()
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CSyncTypeImpl> GetDerivedListImpl();

// UI

protected:
    SEE_ALSO(CSyncTypeIface) // F12-lookup

    // ... your virtuals from Iface here

    virtual const typelist<CSyncTypeIface> OnGetDerivedList()
            ;
    virtual str OnGetLogLetter()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // Built-in local-only constants
    class CSyncTypeForFactory : public CSyncTypeImpl
    {
    public:
        void flag_List_as_SyncTypeImpl();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(F a c t);

        virtual str OnGetLogLetter()
                return
                    "F";
    };

    // A record to be sent to the network
    class CSyncTypeForLocalOut : public CSyncTypeImpl
    {
    public:
        void flag_List_as_SyncTypeImpl();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(L o c S);

        virtual str OnGetLogLetter()
                return
                    "L";
    };

    // Network incoming download record
    class CSyncTypeForNetIn : public CSyncTypeImpl
    {
    public:
        void flag_List_as_SyncTypeImpl();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(N e I n);

        virtual str OnGetLogLetter()
                return
                    "N";
    };
