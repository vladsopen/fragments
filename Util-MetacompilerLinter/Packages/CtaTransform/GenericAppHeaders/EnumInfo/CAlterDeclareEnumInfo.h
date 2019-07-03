// CAlterDeclareEnumInfo.h

//
// CAlterDeclareEnumInfo - enum metadata and checks
//

class CAlterDeclareEnumInfo : public CDeclModifier
{
public:
    GENERIC(CAlterDeclareEnumInfo, CDeclModifier);
    CAlterDeclareEnumInfo();

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual const char* OnGetDefaultTextRepresentation();
    virtual bool OnIsSemicolonAtEnd();

private:
};





