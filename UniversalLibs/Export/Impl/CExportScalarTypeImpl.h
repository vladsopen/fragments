// CExportScalarTypeImpl.h
#pragma once

#include "CExportScalarTypeIfaceGp.h"

//
// CExportScalarTypeImpl - see iface
//

class CExportScalarTypeImpl : public CExportScalarTypeIfaceGp
{
public:
    CExportScalarTypeImpl();

// Constants

// Attributes

// Operations

// Derived Item Enumeration

    const typelist<CExportScalarTypeImpl> GetDerivedListImpl();

// UI

protected:
    virtual const typelist<CExportScalarTypeIfaceGp> OnGetDerivedList()
            ;
    virtual type<CExportScalarTypeIfaceGp> OnAsExportScalarTypeAsText()
            vnull;
    virtual type<CExportScalarTypeIfaceGp> OnAsExportScalarTypeAsFloatingPoint()
            vnull;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // General text
    class CExportScalarTypeAsText : public CExportScalarTypeImpl
    {
    public:
        void flag_List_as_ExportScalarTypeImpl();

    protected:

        virtual str OnGetExcelXmlCellDataType()
                return
                    "String";

        virtual bool OnIsValidExcelXmlDataForFormat(
                out str& out_sCheckIt)
                return
                    true;

    };

    // Floating point number
    class CExportScalarTypeAsFloatingPoint : public CExportScalarTypeImpl
    {
    public:
        void flag_List_as_ExportScalarTypeImpl();

    protected:

        virtual str OnGetExcelXmlCellDataType()
                return
                    "Number";

        virtual bool OnIsValidExcelXmlDataForFormat(
                out str& out_sCheckIt)
                ;
    };
