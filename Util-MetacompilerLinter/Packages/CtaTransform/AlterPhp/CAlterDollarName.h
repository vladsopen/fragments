// CAlterDollarName.h
// (see "CAlterDollarName.cpp")

#include "CAlterQualifiedName.h"

//
// CAlterDollarName - 
//
//   PHP: transforms $xVar to cvL reference
//

class CAlterDollarName : public CAlterQualifiedName
{
public:
    GENERIC(CAlterDollarName, CAlterQualifiedName);
    CAlterDollarName();

    // Prefix verification
    static bool IsDollarName(
            str sName);

protected:
    virtual void OnTransform();

private:
    void ApplyTransform();

    void TransformDollarExecutable(
            CExpression* pParentExpression,
            int iNameIndexInParent);

    void TransformDollarParameters(
            CExpression* pParentExpression,
            int iNameIndexInParent);

    CToken* CreateArrayExpressionWithMagicStr(
            str sBefore,
            str sName,
            str sAfter);
};