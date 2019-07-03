// CAlterQx.h

//
// CAlterQx - SQL syntax and ORM validity check and parameter binding
//

class CAlterQx : public CToken
{
public:
    GENERIC(CAlterQx, CToken);
    CAlterQx();

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual const char* OnGetDefaultTextRepresentation();
    virtual bool OnIsSemicolonAtEnd();

private:
    CExpression* m_pExpression;
    str m_sQxClass;

    str m_sUniqueNewQx;

    bool VerifyNoArguments(
            str sFlagName,
            str sArgument);
};





