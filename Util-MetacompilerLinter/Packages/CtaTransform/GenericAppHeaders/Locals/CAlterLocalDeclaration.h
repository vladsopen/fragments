// CAlterLocalDeclaration.h
// (see "CAlterLocalDeclaration.cpp")

// CAlterLocalDeclaration - checks local declarations in Generic classes

class CAlterLocalDeclaration : public CAlterDeclaration
{
public:
    GENERIC(CAlterLocalDeclaration, CAlterDeclaration);
    CAlterLocalDeclaration();

// Content

// Operations

    // Declaration init checker
    static str GReportIfDeclarationMissingInitializer(
            CDeclaration* pDeclaration,
            CTransformContext* pTransformContext);

protected:
    virtual void OnTransform();

private:

    // Check declaraion pattern
    void CheckForBadDeclarations();
};