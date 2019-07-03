// CGameFactoryIfaceGp.h
#pragma once

class CCallGameIfaceGp;
class CGameTypeIfaceGp;

//
// CGameFactoryIfaceGp -
//
//   Abstract base class to be implemented by all modules supplying
//     game implementations to Balance.
//

class CGameFactoryIfaceGp : public object
{
public:
    CGameFactoryIfaceGp();
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

// Operations

    // Return valid derived Game classes for all types you support
    ref<CCallGameIfaceGp> NewGameOrNull(
            type<CGameTypeIfaceGp> typeGameType)
            vhook;

// UI

protected:
    virtual ref<CCallGameIfaceGp> OnNewGameOrNull(
            type<CGameTypeIfaceGp> typeGameType)
            v1pure;
private:

    void _init_AttachToGameFactoryIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
