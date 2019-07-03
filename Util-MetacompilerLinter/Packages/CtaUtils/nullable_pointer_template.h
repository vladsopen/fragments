// nullable_pointer_template.h
// (see "AllParserIncludes.h")

// Safe NULL pointer value
//   nullable<CClass*> must be IsNull()-checked before GetValidPointer(),
//   otherwise GetValidPointer() throws error.

class CCommon;

extern void GReportFatal(
            const char* szError);

class nullable_base
{
public:
    CCommon* GetPointerOrNull() const 
    {
        return m_p;
    }

protected:
    mutable CCommon* m_p;

    nullable_base() {} // used only as a base for nullable<> template
};

template<class PTR>
class nullable : public nullable_base
{
public:
    nullable(PTR p = 0)
    {
        m_p = p;
    }

    nullable(const nullable& n)
    {
        m_p = n.m_p;
    }

    nullable& operator = (const nullable& n)
    {
        m_p = n.m_p;
        return *this;
    }

    nullable& operator = (PTR p)
    {
        if (!p)
        {
            FAIL("nullable is being assigned NULL");

            GReportFatal(
                "Null pointer passed to nullable::AssignValidPointer()");
            //throw (CErrorNullPointer(
            //    "/CTA:NLAS/",
            //    "null pointer passed to nullable::AssignValidPointer()"));
        }

        m_p = (CCommon*) p;
        return *this;
    }

    const nullable& AssignNullOrNonnullPointer(PTR p) const 
    {
        m_p = (PTR) p;
        return *this;
    }

    const nullable& MakeNull() const 
    {
        m_p = 0;
        return *this;
    }

    bool IsNull() const 
    {
        return m_p == 0;
    }

    PTR GetValidPointer(
            str sContextInfo = "")
            const 
    {
        if (m_p == 0)
        {
            FAIL("nullable is not valid as assumed");

            GReportFatal(
                "Internal nullable pointer is null in GetValidPointer(" + sContextInfo + ")");

            //throw (CErrorNullPointer(
            //    "/CTA:NLUS/",
            //    "nullable pointer is null in GetValidPointer(). " + 
            //        sContextInfo));
        }

        return (PTR) m_p;
    }

    PTR DefaultIfNull(PTR p) const 
    {
        if (!m_p)
            return p;
        return (PTR) m_p;
    }

    PTR GetPointerOrNull() const 
    {
        return (PTR) m_p;
    }

    bool operator == (PTR p) const 
    {
        return m_p == p;
    }

    bool operator != (PTR p) const 
    {
        return m_p != p;
    }

    bool operator == (const nullable& p) const 
    {
        return m_p == p.m_p;
    }

    bool operator != (const nullable& p) const 
    {
        return m_p != p.m_p;
    }
};

// Safe dereferencing:
//  nullable<CData*> npData;
//  if not_null(CData* pData = npData)
//      ...
//  else
//      error;
#undef not_null // hide Isolation version
#define not_null(pInit) (pInit .GetPointerOrNull())

