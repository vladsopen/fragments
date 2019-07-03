// CWriteTree.h
// (see "CWriteTree.cpp",
//      "CWriteTree_CTreeLevel.cpp",
//      "CWriteTreeAsFolders.cpp",
//      "CWriteTreeAsText.cpp")

//
// CWriteTree - tree-based output stream for token hierarchy
//

class CWriteTree : public CWriter
{
public:
    GENERIC_ABSTRACT(CWriteTree, CWriter);
    CWriteTree();

// Stream Control

    str ProduceResult();

protected:
    virtual void OnWriteTokenStr(
            CToken* pToken,
            str sNextFragment,
            ELineNumbering eLineNumbering);
    virtual void OnWriteTokenMultiLineStr(
            CToken* pToken,
            str sNextFragment,
            ELineNumbering eLineNumbering);
    virtual void OnWriteTokenZeroTerminatedString(
            CToken* pToken,
            const char* szNextFragment,
            ELineNumbering eLineNumbering);
    virtual void OnWriteTokenBuffer(
            CToken* pToken,
            const char* pcNextFragment,
            int nLength,
            ELineNumbering eLineNumbering);
    virtual void OnWriteCosmeticNewLine();
    virtual void OnWriteDebugText(str sAnyText);
    virtual void OnWriteDebugSubBranch(
            str sBranchKey,
            str sBranchIndexKey,
            str sAnyText);
    virtual void OnHandleTokenStart(CToken* pToken);
    virtual void OnHandleTokenEnd(CToken* pToken);
    virtual int OnGetActualCurrentOutputLineForDebug();

    class CTreeLevel;
    CTreeLevel* NewTreeLevel(nullable<CTreeLevel*> npParentLevel);

protected:

    // Tree Level
    class CTreeLevel : public CCommon
    {
    public:
        GENERIC_WITHOUT_NewOfTheKind(CTreeLevel, CCommon);
        CTreeLevel(
                CWriteTree* pParentWriteTree,
                nullable<CTreeLevel*> npParentLevel);

        void SetText(str sText);
        str GetText();
        str GetFormattedNumberInParent();

        CTreeLevel* AddNewSubLevel();
        int GetSublevelCount();

        nullable<CTreeLevel*> GetParentLevel();
        CWriteTree* GetParentWriteTree();

        str Export();

    protected:
        virtual str OnExport() = 0;

    private:
        CWriteTree* m_pParentWriteTree;
        nullable<CTreeLevel*> m_npParentLevel;
        array<CTreeLevel*> m_aSubLevel;
        str m_sText;
    };

// new virtuals
protected:
    virtual CTreeLevel* OnNewTreeLevel(nullable<CTreeLevel*> npParentLevel) = 0;

private:

    void AppendTextToBranchName(str sAppend);
    void CreateSubBranch(str sInitialText);
    void GoUp();

    nullable<CTreeLevel*> m_npRootLevel;
    nullable<CTreeLevel*> m_npCurrentLevel;

    void EnsureRootLevelCreated();
};

//
// CWriteTreeAsFolders
//

class CWriteTreeAsFolders : public CWriteTree
{
public:
    GENERIC_WITHOUT_NewOfTheKind(CWriteTreeAsFolders, CWriteTree);
    CWriteTreeAsFolders(
        path pathRoot);

// Path

    path GetRootPath();

protected:

    class CTreeLevelAsDirectory : public CTreeLevel
    {
    public:
        CTreeLevelAsDirectory(
            CWriteTreeAsFolders* pParentWriteTree,
            nullable<CTreeLevel*> npParentLevel);

    protected:
        virtual str OnExport();

    private:
        path GetFullPath();
        static path MakeGoodPathName(str sAnyText);
    };

    virtual CTreeLevel* OnNewTreeLevel(nullable<CTreeLevel*> npParentLevel);

private:
    path m_pathRoot;
};

//
// CWriteTreeAsFolders
//

class CWriteTreeAsText : public CWriteTree
{
public:
    GENERIC(CWriteTreeAsText, CWriteTree);
    CWriteTreeAsText();

protected:

    class CTreeLevelAsString : public CTreeLevel
    {
    public:
        GENERIC_WITHOUT_NewOfTheKind(CTreeLevelAsString, CTreeLevel);
        CTreeLevelAsString(
            CWriteTreeAsText* pParentWriteTree,
            nullable<CTreeLevel*> npParentLevel);

    protected:
        virtual str OnExport();
    };

    virtual CTreeLevel* OnNewTreeLevel(nullable<CTreeLevel*> npParentLevel);

private:
};