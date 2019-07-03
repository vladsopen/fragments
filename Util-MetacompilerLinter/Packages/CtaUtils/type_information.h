// type_information.h
// (see "AllParserIncludes.h")

// Generic Type Info

class CCommon;
class nullable_base;

class CClassId
{
public:
    enum EUniqueAddress {};
    CClassId(EUniqueAddress* peUniqueAddress);
    bool operator == (const CClassId& id) const;
private:
    EUniqueAddress* m_peUniqueAddress;
};

/*
class CErrorNullPointer : public CError
{
public:
    CErrorNullPointer(str sErrorId, sloc slocError);
};
*/

#define GENERIC_GetStaticClassId() \
    static CClassId GetStaticClassId() \
    { \
        static CClassId::EUniqueAddress s_sUniqueAddress; \
        return &s_EUniqueAddress; \
    }

#define GENERIC_ABSTRACT(CCommonClass, CSuper) \
private: \
    typedef CSuper super; \
public: \
    GENERIC_GetStaticClassId() \
    virtual CClassId GetObjectClassId() const { return GetStaticClassId(); } \
    virtual CCommon* TryCastToClass(CClassId id) \
    { \
        if (id == GetStaticClassId()) return this; \
        return super::TryCastToClass(id); \
    } \
    virtual str GetObjectClassName() const { return #CCommonClass; } \
    ~CCommonClass() {} // just to verify CCommonClass is matching client class name

#define GENERIC(CCommonClass, CSuper) \
    virtual void abstract_CCommon() {} \
    virtual CCommonClass* NewOfTheKind() const \
    { \
        CCommonClass* p = new CCommonClass; \
        return p; \
    } \
    GENERIC_ABSTRACT(CCommonClass, CSuper)

#define GENERIC_WITHOUT_NewOfTheKind(CCommonClass, CSuper) \
    virtual void abstract_CCommon() {} \
    GENERIC_ABSTRACT(CCommonClass, CSuper)

#define derive(CClass, p) \
    (nullable<CClass*>(). \
        AssignNullOrNonnullPointer((CClass*) \
            TryCast(CClass::GetStaticClassId(), p)))

#define iskindof(CClass, p) \
    (!!not_null(derive(CClass, p)))

//(CClass::TryCast(TryCast(p)) != 0)

#define iskindofobject(base_p, p) \
    (!!TryCast(base_p->GetObjectClassId(), p))

CCommon* TryCastImpl(CCommon* p);
CCommon* TryCast(CClassId id, CCommon* p);
CCommon* TryCast(CClassId id, const nullable_base& p);

