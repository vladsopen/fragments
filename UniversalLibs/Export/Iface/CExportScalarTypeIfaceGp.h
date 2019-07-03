// CExportScalarTypeIfaceGp.h
#pragma once

//
// CExportScalarTypeIfaceGp -
//
//   Defines Excel-like scalar value types.
//     Value formatting should be defined separately within CExportGridScalarIfaceGp
//     properties.
//
ASSUME_IMPL_FOR_IFACE(CExportScalarTypeImpl, CExportScalarTypeIfaceGp)

class CExportScalarTypeIfaceGp : public typeobject
{
public:
    CExportScalarTypeIfaceGp();
    //NEW_LOCAL(CExportScalarTypeImpl, CExportScalarTypeIfaceGp)
    NEW_GP(SysCommon, CExportScalarTypeImpl, CExportScalarTypeIfaceGp)

// Constants

// Attributes

// Operations

    // <Cell ss:Formula="=3/7"><Data ss:Type="Number">0.42857142857142855</Data></Cell>
    //                                        ^^^^^^
    str GetExcelXmlCellDataType()
            vhook;

    // Verify if the data can be put in a cell of this format
    //   (usually for numbers where Excel faults out on non digits)
    bool IsValidExcelXmlDataForFormat(
            out str& out_sCheckIt)
            vhook;

// Derived Item Enumeration

    const typelist<CExportScalarTypeIfaceGp> GetDerivedList()
            vhook;

    type<CExportScalarTypeIfaceGp> AsExportScalarTypeAsText()
            vhook;
    type<CExportScalarTypeIfaceGp> AsExportScalarTypeAsFloatingPoint()
            vhook;

// UI

protected:
    virtual str OnGetExcelXmlCellDataType()
            v1pure;
    virtual bool OnIsValidExcelXmlDataForFormat(
            out str& out_sCheckIt)
            v1pure;

    virtual const typelist<CExportScalarTypeIfaceGp> OnGetDerivedList()
            v1pure;
    virtual type<CExportScalarTypeIfaceGp> OnAsExportScalarTypeAsText()
            v1pure;
    virtual type<CExportScalarTypeIfaceGp> OnAsExportScalarTypeAsFloatingPoint()
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
