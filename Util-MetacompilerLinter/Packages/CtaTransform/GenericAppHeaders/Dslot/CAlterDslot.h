// CAlterDslot.h

//
// CAlterDslot - debug(<Project>, <key>) translator
//

class CAlterDslot : public CToken
{
public:
    GENERIC(CAlterDslot, CToken);
    CAlterDslot();

// Operations

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual const char* OnGetDefaultTextRepresentation();
    //virtual bool OnIsSemicolonEnabledAtExpressionEnd();
    /*
    virtual bool OnSupportMagicStrGeneratorScanner(
            CMagicStrGenerator* pMagicStrGenerator,
            CWriter* pWriter);
            */

private:

    bool m_bRecognizedDebug;
    CCharDoubleQuotedString* m_pKeyString;
    CExpression* m_pKeyStringExpression;
};

    