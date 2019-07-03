// CToken_FunctionStyleKeywords.h
// (see "CToken.h")
// (see "ParseExpressions.cpp")

////////////////////////////////////////////////////////////////////////////////
// Simple keyword-functions like 'this' or 'true'

class CBuiltinName : public CToken
{
public:
    GENERIC_ABSTRACT(CBuiltinName, CToken);

protected:
    //virtual CToken* OnTryExtend(CTokenizer*) { return this; }
    //virtual void OnTransform();

private:
};

    class CThisPointer : public CBuiltinName
    {
    public:
        GENERIC(CThisPointer, CBuiltinName);

    protected:
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
            { return "this"; }
    };

    /*
    class CTrueConstant : public CBuiltinName
    {
    public:
        GENERIC(CTrueConstant, CBuiltinName);

    protected:
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
            { return "true"; }
    };

    class CFalseConstant : public CBuiltinName
    {
    public:
        GENERIC(CFalseConstant, CBuiltinName);

    protected:
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
            { return "false"; }
    };
    */

    /*
    class CSuperClassSpec : public CBuiltinName
    {
    public:
        GENERIC(CSuperClassSpec, CBuiltinName);

    protected:
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
            { return "super"; }
    };
    */

/////////////////////////////////////////////////////////////////////////////////
// CFunctionStyleKeyword - 'switch' labels and member access

class CFunctionStyleKeyword : public CToken
{
public:
    GENERIC_ABSTRACT(CFunctionStyleKeyword, CToken);

protected:
    //virtual CToken* OnTryExtend(CTokenizer*) { return this; }
    //virtual void OnTransform();

private:
};

    // Unparened functions
    class CFunctionKeywordWithArg : public CFunctionStyleKeyword
    {
    public:
        GENERIC_ABSTRACT(CFunctionKeywordWithArg, CFunctionStyleKeyword);

    protected:
        //virtual CToken* OnTryExtend(CTokenizer*) { return this; }
        //virtual void OnTransform();

    private:
    };

        class CSizeofOp : public CFunctionKeywordWithArg
        {
        public:
            GENERIC(CSizeofOp, CFunctionKeywordWithArg);

        protected:
            virtual void OnTransform();
            virtual void OnRegenerate(CWriter* pWriter);
            virtual const char* OnGetDefaultTextRepresentation()
                { return "sizeof"; }
        };

        class CNewOp : public CFunctionKeywordWithArg
        {
        public:
            GENERIC(CNewOp, CFunctionKeywordWithArg);

        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "new"; }
        };

        class CDeleteOp : public CFunctionKeywordWithArg
        {
        public:
            GENERIC(CDeleteOp, CFunctionKeywordWithArg);

        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "delete"; }
        };

        class CAlignOfOp : public CFunctionKeywordWithArg
        {
        public:
            GENERIC(CAlignOfOp, CFunctionKeywordWithArg);

        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "__alignof"; }
        };

        class CTypeidOp : public CFunctionKeywordWithArg
        {
        public:
            GENERIC(CTypeidOp, CFunctionKeywordWithArg);

        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "typeid"; }
        };

        class CUuidofOp : public CFunctionKeywordWithArg
        {
        public:
            GENERIC(CUuidofOp, CFunctionKeywordWithArg);

        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "__uuidof"; }
        };

    // Template-like cast functions
    class CFunctionKeywordAsTemplate : public CFunctionStyleKeyword
    {
    public:
        GENERIC_ABSTRACT(CFunctionKeywordAsTemplate, CFunctionStyleKeyword);

    protected:
        //virtual CToken* OnTryExtend(CTokenizer*) { return this; }
        //virtual void OnTransform();

    private:
    };

        class CConstCast : public CFunctionKeywordAsTemplate
        {
        public:
            GENERIC(CConstCast, CFunctionKeywordAsTemplate);

        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "const_cast"; }
        };

        class CDynamicCast : public CFunctionKeywordAsTemplate
        {
        public:
            GENERIC(CDynamicCast, CFunctionKeywordAsTemplate);

        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "dynamic_cast"; }
        };

        class CReinterpretCast : public CFunctionKeywordAsTemplate
        {
        public:
            GENERIC(CReinterpretCast, CFunctionKeywordAsTemplate);

        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "reinterpret_cast"; }
        };

        class CStaticCast : public CFunctionKeywordAsTemplate
        {
        public:
            GENERIC(CStaticCast, CFunctionKeywordAsTemplate);

        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "static_cast"; }
        };

        class CTryCast : public CFunctionKeywordAsTemplate
        {
        public:
            GENERIC(CTryCast, CFunctionKeywordAsTemplate);

        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "__try_cast"; }
        };
