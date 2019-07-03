// CAlterGenericThis.h
// (see "CAlterGenericThis.cpp")

// CAlterGenericThis - 'this' with ref<> support

class CAlterGenericThis : public CThisPointer
{
public:
    GENERIC(CAlterGenericThis, CThisPointer);
    CAlterGenericThis();

// Content

protected:
    //virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    //virtual const char* OnGetDefaultTextRepresentation();
    //virtual bool OnIsSemicolonEnabledAtExpressionEnd();
    //virtual bool OnIsAlwaysLastInExpressionList();

private:
};

