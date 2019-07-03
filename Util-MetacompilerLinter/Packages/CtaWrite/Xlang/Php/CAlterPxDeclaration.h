// CAlterPxDeclaration.h
#pragma once

#include "CAlterDeclaration.h"

//
// CAlterPxDeclaration - 
//
//   Transform declaration to PHP initialization
//

class CAlterPxDeclaration : public CAlterDeclaration
{
public:
    GENERIC(CAlterPxDeclaration, CAlterDeclaration);
    CAlterPxDeclaration();

// Constants 

// Attributes

// Members

protected:
    virtual void OnTransform();

private:
};