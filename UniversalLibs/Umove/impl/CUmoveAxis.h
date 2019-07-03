// CUmoveAxis.h
#pragma once

//
// CUmoveAxis -
//
//   X or Y axis for Umove Vehicle movement
//
//ASSUME_IMPL_FOR_IFACE(CUmoveAxisImpl, CUmoveAxis)

class CUmoveAxis : public typeobject
{
public:
    CUmoveAxis();
    //NEW_LOCAL(CUmoveAxisImpl, CUmoveAxis)
    //NEW_GP(MyProject, CUmoveAxisImpl, CUmoveAxis)

// Constants

// Attributes

// Operations

    // x/y from point
    int GetAxisPoint(
            point pointFrom)
            vhook;

    // w/h from size
    int GetAxisSize(
            size sizeFrom)
            vhook;

// Derived Item Enumeration

    const typelist<CUmoveAxis> GetDerivedList();

// UI

protected:
    virtual int OnGetAxisPoint(
            point pointFrom)
            v1pure;
    virtual int OnGetAxisSize(
            size sizeFrom)
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // X
    class CUmoveAxisX : public CUmoveAxis
    {
    public:
        void flag_List_as_UmoveAxis();

    protected:

        virtual int OnGetAxisPoint(
                point pointFrom)
                return
                    pointFrom.x;

        virtual int OnGetAxisSize(
                size sizeFrom)
                return
                    sizeFrom.w;

    };

    // Y
    class CUmoveAxisY : public CUmoveAxis
    {
    public:
        void flag_List_as_UmoveAxis();

    protected:

        virtual int OnGetAxisPoint(
                point pointFrom)
                return
                    pointFrom.y;

        virtual int OnGetAxisSize(
                size sizeFrom)
                return
                    sizeFrom.h;

    };
