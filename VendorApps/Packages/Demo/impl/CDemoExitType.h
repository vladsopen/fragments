// CDemoExitType.h
#pragma once

class CDemoImpl;

//
// CDemoExitType -
//
//   Chooses what kind of result we generate after sampler emulation
//
//ASSUME_IMPL_FOR_IFACE(CDemoExitTypeImpl, CDemoExitType)

class CDemoExitType : public typeobject
{
public:
    CDemoExitType();
    //NEW_LOCAL(CDemoExitTypeImpl, CDemoExitType)
    //NEW_GP(CustomApps, CDemoExitTypeImpl, CDemoExitType)
    DECLARE_NULL_TYPE(GetDerivedList);

// Constants

// Attributes

    // For debug UI
    str GetDemoExitDescription()
            vhook;

// Operations

    // Apply selected logic
    void MakeDemoExit(
            ref<CDemoImpl> rDemo,
            out str& out_sError)
            vhook;

// Derived Item Enumeration

    const typelist<CDemoExitType> GetDerivedList();

// UI

    // (o) Select / Setup _____
    ref<UiContainer> NewUiExitTypeSetup(
            ref<CDemoImpl> rDemo)
            vhook;

protected:
    virtual str OnGetDemoExitDescription()
            v1pure;
    virtual void OnMakeDemoExit(
            ref<CDemoImpl> rDemo,
            out str& out_sError)
            v1pure;
    virtual ref<UiContainer> OnNewUiExitTypeSetup(
            ref<CDemoImpl> rDemo)
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // Respond to Command=Create
    class CDemoExitTypeIsCreateNew : public CDemoExitType
    {
    public:
        void flag_List_as_DemoExitType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(N e w);

        virtual str OnGetDemoExitDescription()
                return
                    "Create new sample files (first revision)";

        virtual void OnMakeDemoExit(
                ref<CDemoImpl> rDemo,
                out str& out_sError)
                ;

        virtual ref<UiContainer> OnNewUiExitTypeSetup(
                ref<CDemoImpl> rDemo)
                return
                    rDemo->
                        NewUiDemoExitSetupCreate();
    };

    // Update to new version on Command=Open
    class CDemoExitTypeIsUpdatedVersion : public CDemoExitType
    {
    public:
        void flag_List_as_DemoExitType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(U p d V);

        virtual str OnGetDemoExitDescription()
                return
                    "Update to new revision";

        virtual void OnMakeDemoExit(
                ref<CDemoImpl> rDemo,
                out str& out_sError)
                ;

        virtual ref<UiContainer> OnNewUiExitTypeSetup(
                ref<CDemoImpl> rDemo)
                return
                    rDemo->
                        NewUiDemoExitSetupUpdate();
    };

    // Dont change anything
    class CDemoExitTypeIsNothing : public CDemoExitType
    {
    public:
        void flag_List_as_DemoExitType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(N o p);

        virtual str OnGetDemoExitDescription()
                return
                    "Change nothing";

        virtual void OnMakeDemoExit(
                ref<CDemoImpl> rDemo,
                out str& out_sError)
                ;

        virtual ref<UiContainer> OnNewUiExitTypeSetup(
                ref<CDemoImpl> rDemo)
                return
                    rDemo->
                        NewUiDemoExitSetupNop();
    };

    // Exit with error
    class CDemoExitTypeIsFail : public CDemoExitType
    {
    public:
        void flag_List_as_DemoExitType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(F a i l);

        virtual str OnGetDemoExitDescription()
                return
                    "Exit with error";

        virtual void OnMakeDemoExit(
                ref<CDemoImpl> rDemo,
                out str& out_sError)
                ;

        virtual ref<UiContainer> OnNewUiExitTypeSetup(
                ref<CDemoImpl> rDemo)
                return
                    rDemo->
                        NewUiDemoExitSetupFail();
    };
