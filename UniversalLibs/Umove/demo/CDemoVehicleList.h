// CDemoVehicleList.h
#pragma once

class CDemoVehicle;

//
// CDemoVehicleList - 
//
//   A list of CDemoVehicle objects. Just for demo.
//     In real app you may have a list of markers to move 
//     or just a single thumb or separator as a Umove Vehicle.
//
//ASSUME_IMPL_FOR_IFACE(CDemoVehicleListImpl, CDemoVehicleList)

class CDemoVehicleList : public object
{
public:
    CDemoVehicleList();
    //NEW_LOCAL(CDemoVehicleListImpl, CDemoVehicleList)
    //NEW_GP(MyProject, CDemoVehicleListImpl, CDemoVehicleList)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // Current selection
    //link<CDemoVehicle> x_linkSelection
    //        xauto(Get);
    //void x_linkSelection_Set(link<CDemoVehicle> value); // UI update

    // Main test vehicle
    ref<CDemoVehicle> GetGuineaPigVehicle();

// array<> Publishing

    int GetVehicleCount()
        { return m_aDemoVehicle.GetCount(); }
    // void SetCount(int nNewSize)
    //     { return m_aDemoVehicle.SetCount(nNewSize); }
    // ref<CDemoVehicle> GetAt(int iAt)
    //     { return m_aDemoVehicle.GetAt(iAt); }
    // void SetAt(int iAt, ref<CDemoVehicle> value)
    //     { m_aDemoVehicle.SetAt(iAt, value); }
    void AddVehicle(ref<CDemoVehicle> value)
        { m_aDemoVehicle.Add(value); }
    // void InsertAt(int iAt, ref<CDemoVehicle> value)
    //     { m_aDemoVehicle.InsertAt(iAt, value); }
    // void RemoveAt(int iAt, int nCount = 1)
    //     { m_aDemoVehicle.RemoveAt(iAt, nCount); }
    // void RemoveAll()
    //     { m_aDemoVehicle.RemoveAll(); }
    bool IterateVehicles(out iter& out_i, out ref<CDemoVehicle>& out_value)
        { return m_aDemoVehicle.Iterate(out out_i, out out_value); }
    // see also IterateBackwards()

// Operations

    // Test Init
    void RenderDemoVehicles();

// UI

protected:
private:
    array< ref<CDemoVehicle> > m_aDemoVehicle;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
