// CJobFilterType.h
#pragma once

class CJobStateImpl;
class CJobFilterSwitch;

//
// CJobFilterType -
//
//   Job list filters
//
//ASSUME_IMPL_FOR_IFACE(CJobFilterTypeImpl, CJobFilterType)

class CJobFilterType : public typeobject
{
public:
    CJobFilterType();
    //NEW_LOCAL(CJobFilterTypeImpl, CJobFilterType)
    //NEW_GP(Ralarm, CJobFilterTypeImpl, CJobFilterType)
    DECLARE_NULL_TYPE(GetDerivedList);

// Constants

// Attributes

    // For menu prefix string
    sloc GetFilterName(
            type<CJobFilterSwitch> typeSwitch)
            vhook;

    // Long descr
    //sloc GetFilterTip()
    //        vhook;

    // Do the filter
    bool IsJobMatchingFilter(
            ref<CJobStateImpl> rJobState,
            type<CJobFilterSwitch> typeSwitch)
            vhook;

// Operations

// Derived Item Enumeration

    const typelist<CJobFilterType> GetDerivedList();

// UI

protected:
    virtual sloc OnGetFilterName(
            type<CJobFilterSwitch> typeSwitch)
            v1pure;
    virtual bool OnIsJobMatchingFilter(
            ref<CJobStateImpl> rJobState,
            type<CJobFilterSwitch> typeSwitch)
            v1pure;
private:

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};

    // In/Out
    class CJobFilterTypeIsIncoming : public CJobFilterType
    {
    public:
        void flag_List_as_JobFilterType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(I n O t);

        virtual sloc OnGetFilterName(
                type<CJobFilterSwitch> typeSwitch)
                return
                    typeSwitch == type<CJobFilterSwitchIsOnNormal>()?
                        TRANSLATE(
                            "Received",[... UI-string-removed]);

        virtual bool OnIsJobMatchingFilter(
                ref<CJobStateImpl> rJobState,
                type<CJobFilterSwitch> typeSwitch)
                return
                    typeSwitch == type<CJobFilterSwitchIsOnNormal>() &&
                    rJobState->x_bDownloadJob?
                        true
                    :
                    typeSwitch == type<CJobFilterSwitchIsOnInverse>() &&
                    rJobState->x_bUploadJob?
                        true
                    :
                    typeSwitch == type<CJobFilterSwitchIsOff>()?
                        true
                        :
                        false;
                    /*
                    typeSwitch == type<CJobFilterSwitchIsOnNormal>() &&
                    rJobState->x_typeJobDirectionImpl ==
                        type<CJobDirectionIsIn>()?
                        true
                    :
                    typeSwitch == type<CJobFilterSwitchIsOnInverse>() &&
                    rJobState->x_typeJobDirectionImpl ==
                        type<CJobDirectionIsOut>()?
                        true
                    :
                    typeSwitch == type<CJobFilterSwitchIsOff>()?
                        true
                        :
                        false;
                    */

    };

    // Complete/In progress
    class CJobFilterTypeIsComplete : public CJobFilterType
    {
    public:
        void flag_List_as_JobFilterType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(C m p l);

        virtual sloc OnGetFilterName(
                type<CJobFilterSwitch> typeSwitch)
                return
                    typeSwitch == type<CJobFilterSwitchIsOnNormal>()?
                        TRANSLATE(
                            "Complete",[... UI-string-removed]");

        virtual bool OnIsJobMatchingFilter(
                ref<CJobStateImpl> rJobState,
                type<CJobFilterSwitch> typeSwitch)
                return
                    typeSwitch == type<CJobFilterSwitchIsOnNormal>() &&
                    !rJobState->IsJobInProgress()?
                        true
                    :
                    typeSwitch == type<CJobFilterSwitchIsOnInverse>() &&
                    rJobState->IsJobInProgress()?
                        true
                    :
                    typeSwitch == type<CJobFilterSwitchIsOff>()?
                        true
                        :
                        false;

    };

    // Has reply
    class CJobFilterTypeIsReplied : public CJobFilterType
    {
    public:
        void flag_List_as_JobFilterType();

    protected:

        virtual permanentid OnGetPermanentTypeId()
                return
                    PERMANENT_TYPE_ID(R e p l);

        virtual sloc OnGetFilterName(
                type<CJobFilterSwitch> typeSwitch)
                return
                    typeSwitch == type<CJobFilterSwitchIsOnNormal>()?
                        TRANSLATE(
                            "Has reply"[... UI-string-removed]");

        virtual bool OnIsJobMatchingFilter(
                ref<CJobStateImpl> rJobState,
                type<CJobFilterSwitch> typeSwitch)
                return
                    typeSwitch == type<CJobFilterSwitchIsOnNormal>() &&
                    rJobState->x_nRepliesToJob > 0?
                        true
                    :
                    typeSwitch == type<CJobFilterSwitchIsOnInverse>() &&
                    rJobState->x_nRepliesToJob == 0?
                        true
                    :
                    typeSwitch == type<CJobFilterSwitchIsOff>()?
                        true
                        :
                        false;

    };
