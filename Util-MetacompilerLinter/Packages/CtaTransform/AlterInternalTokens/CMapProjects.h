// CMapProjects.h

//
// CMapProjects - locates project names for tokens
//

class CMapProjects : public CCommon
{
public:
    GENERIC(CMapProjects, CCommon);
    CMapProjects();

// Constants

// Attributes

// Operations

    // Find/cache
    str FindProjectNameForSourceFile(
            path pathSource,
            str sAddInfoOnError,
            CToken* pReportError);

protected:

private:

    map<path, str> m_mapCachePathToProject;

    // non caching
    str DoFindProjectNameForSourceFile(
            path pathSource,
            str sAddInfoOnError,
            CToken* pReportError);
};

