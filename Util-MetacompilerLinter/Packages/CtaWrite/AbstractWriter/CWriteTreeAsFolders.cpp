// CWriteTreeAsFolders.cpp
// (see "CWriteTree.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "CWriter.h"
#include "CWriteTree.h"
#include "VirtOs_dir.h"

CWriteTreeAsFolders::CWriteTreeAsFolders(
        path pathRoot)
: CWriteTree()
{
    ASSERT(pathRoot.Len() > 0);
    m_pathRoot = pathRoot;
}

CWriteTreeAsFolders::CTreeLevelAsDirectory::CTreeLevelAsDirectory(
        CWriteTreeAsFolders* pParentWriteTree,
        nullable<CTreeLevel*> npParentLevel)
: CTreeLevel(pParentWriteTree, npParentLevel)
{
}

str CWriteTreeAsFolders::CTreeLevelAsDirectory::OnExport()
{
    str sError;
    dir(GetFullPath().GetFolder()).CreateDir(out sError);
    return "";
}

path CWriteTreeAsFolders::CTreeLevelAsDirectory::MakeGoodPathName(str sAnyText)
{
    path pathGood =
        sAnyText.
            GetStripBoth().
            GetReplaced(path::GGetBadPathChars(), "`");
    if (pathGood.Len() == 0)
        return "_";
    else
        return pathGood;
}

path CWriteTreeAsFolders::GetRootPath()
{
    return m_pathRoot;
}

CWriteTreeAsFolders::CTreeLevel* CWriteTreeAsFolders::OnNewTreeLevel(
        nullable<CTreeLevel*> npParentLevel)
{
    return new CTreeLevelAsDirectory(this, npParentLevel);
}

[...]