// CUiBody.h
// (see "CUiBody.cpp")

//
// CUiBody - Ui() method code tree internal representation
//

class CUiBody : public CCommon
{
public:
    GENERIC(CUiBody, CCommon);
    CUiBody();

    void ParseUiMethodStatementList(
            CStatementList* pOriginalStatementList,
            out int& out_nUiFound);

private:

    bool m_bFirstBranch;
    map<CToken*, unused> m_mapTransformedToken;

    str GetUiClassName(
            CDeclaration* pDeclaration,
            CUiBranch* pUiBranch);

    // Called when another 'rUi'-declaration detected
    void HandleUiBlockDeclaration(
            CDeclaration* pDeclaration);

    // Determines 'rUi'-delaration type and appends init expression if necessary
    void TransformInitializer(
            CDeclaration* pDeclaration,
            CUiBranch* pUiBranch);

    // Replace 'ref<UiClass>' to 'UiClass*'
    void TransformRefToPtr(
            CDeclaration* pDeclaration,
            CUiBranch* pUiBranch);

    // cta_UI()... intro generated before the compound
    void InsertInitialCaption(
            CDeclaration* pDeclaration,
            CStatementList* pParentStatementList,
            CUiBranch* pUiBranch);

    // Lock/unlock 'rUi' identifier
    void InsertUndefUnlockingUi(
            CStatementList* pStatementList,
            int iUndefPos,
            str sUndefComment,
            int iRedefinePosAfterUndefInserted,
            str sRedefineComment,
            str sFinalizerCode,
            CUiBranch* pUiBranch);

    // Look for rUi->On...
    void TransformAllUiEventHandlers(
            CDeclaration* pDeclaration,
            CStatementList* pStatementList,
            CUiBranch* pUiBranch);

    // Insert update block just once
    void EnsureUiUpdateBlockBracing(
            CDeclaration* pDeclaration,
            int iBeforeStatement,
            CUiBranch* pUiBranch);

    // rUi->On...
    void HandlePossibleUiEventHandler(
            CDeclaration* pDeclaration,
            CExpression* pScanExpression,
            int iStatement,
            CUiBranch* pUiBranch);

    // Legacy cta_UI() blocks
    void HandlePossibleApi2SubBlock(
            CDeclaration* pDeclaration,
            CExpression* pScanExpression,
            int iStatement,
            CUiBranch* pUiBranch);

    // Repetitive retransform prevention
    bool QueryTransformPermission(
            CToken* pToken);
};