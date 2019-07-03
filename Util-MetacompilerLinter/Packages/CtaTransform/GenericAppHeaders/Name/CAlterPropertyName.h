// CAlterPropertyName.h
// (see "CAlterPropertyName.cpp")

//
// CAlterPropertyName - transforms x-property name to allow controlled access
//   to its methods and data
//

class CAlterPropertyName : public CAlterQualifiedName
{
public:
    GENERIC(CAlterPropertyName, CAlterQualifiedName);
    CAlterPropertyName();

    // Prefix verification
    static bool IsXpropertyName(
            str sName);
    static bool IsXpropertyNameExposable(
            str sName);

protected:
    virtual void OnTransform();

private:
    void ApplyTransform();

    bool TryComplexAssignmentTransform(
            CCharAssignmentToken* pCharAssignmentToken,
            CUnqualifiedName* pRootUnqualifiedName,
            CExpression* pParentExpression,
            int iNameIndexInParent);
};