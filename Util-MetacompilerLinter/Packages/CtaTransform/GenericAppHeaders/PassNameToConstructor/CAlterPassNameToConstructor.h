// CAlterPassNameToConstructor.h

#include "CAlterDeclaration.h"

//
// CAlterPassNameToConstructor - 
//
//   Inserts declared identifier name into constructor parameters.
//     see cta_::PassNameToConstructor to enable this for a typename.
//
//     CMyClass foo("foo", more);
//                  ^^^^^^
//

class CAlterPassNameToConstructor : public CAlterDeclaration
{
public:
    GENERIC(CAlterPassNameToConstructor, CAlterDeclaration);
    CAlterPassNameToConstructor();

// Constants 

// Attributes

// Members

protected:
    virtual void OnTransform();

private:
};