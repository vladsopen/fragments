// CToken_DeclarationModifiers.h
// (see "CToken.h")
// (see "ParseDeclKeywords.cpp")
// (see "ParseQualifiedName.cpp")

//////////////////////////////////////////////////////////////////////////////////////
// CDeclModifier - all kinds of declaration keywords (some include arguments)

class CDeclModifier : public CToken
{
public:
    GENERIC(CDeclModifier, CToken);
    CDeclModifier();

// Parser logic

    bool IsSignalForBracedBodyToTerminatesDeclaraton();

// Name declaration logic

    // Checks for 'emum' scope rule
    bool IsClassBodyDeclaresInOuterScope();

protected:
    // derived classes should call it to support generic CDeclModifier chaining
    //virtual CToken* OnTryExtend();
    //virtual void OnTransform() {}
    virtual bool OnIsClassBodyDeclaresInOuterScope();
    virtual bool OnIsSignalForBracedBodyToTerminatesDeclaraton();
};

    ////////////////////////////////////////////////////////////////////////////////
    // 'const'

    class CConstKeyword : public CDeclModifier
    {
    public:
        GENERIC(CConstKeyword, CDeclModifier);

    protected:
        //virtual CToken* OnTryExtend();
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
                { return "const"; }

    private:
    };

    ////////////////////////////////////////////////////////////////////////////////
    // 'virtual'

    class CVirtualKeyword : public CDeclModifier
    {
    public:
        GENERIC(CVirtualKeyword, CDeclModifier);

    protected:
        //virtual CToken* OnTryExtend();
        //virtual void OnTransform() {}
        virtual void OnRegenerate(CWriter* pWriter);
        virtual const char* OnGetDefaultTextRepresentation()
                { return "virtual"; }

    private:
    };

    ////////////////////////////////////////////////////////////////////////////////
    // 'static'

    class CStaticKeyword : public CDeclModifier
    {
    public:
        GENERIC(CStaticKeyword, CDeclModifier);

    protected:
        //virtual CToken* OnTryExtend();
        //virtual void OnTransform() {}
        virtual void OnRegenerate(CWriter* pWriter);
        virtual const char* OnGetDefaultTextRepresentation()
                { return "static"; }

    private:
    };

    ////////////////////////////////////////////////////////////////////////////////
    // Templates

    class CTemplateKeywordAndArg : public CDeclModifier // 'template'<...>
    {
    public:
        GENERIC(CTemplateKeywordAndArg, CDeclModifier);
        CTemplateKeywordAndArg();

    protected:
        virtual CToken* OnTryExtend();
        virtual void OnTransform();
        virtual void OnRegenerate(CWriter* pWriter);
        virtual const char* OnGetDefaultTextRepresentation()
                { return "template"; }

    private:
        nullable<CTemplateArgList*> x_npTemplateArgList;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // Templates

    class CTypenameKeyword : public CDeclModifier // 'template'<...>
    {
    public:
        GENERIC(CTypenameKeyword, CDeclModifier);
        //CTypenameKeyword();

    protected:
        //virtual CToken* OnTryExtend();
        ////virtual void OnTransform() {}
        //virtual void OnRegenerate(CWriter* pWriter);
        virtual const char* OnGetDefaultTextRepresentation()
                { return "typename"; }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // Aggregates

    class CAggregateKeyword : public CDeclModifier  // 'class', 'enum', ...
    {
    public:
        GENERIC(CAggregateKeyword, CDeclModifier);

    protected:
        //virtual CToken* OnTryExtend(CTokenizer*) { return this; }
        //virtual void OnTransform() {}
    };

        class CClassStructUnionKeyword : public CAggregateKeyword // inner scope aggreg.
        {
        public:
            GENERIC(CClassStructUnionKeyword, CAggregateKeyword);

        protected:
            virtual CToken* OnTryExtend(CTokenizer*) { return this; }
            //virtual void OnTransform() {}
        };

            class CClassStructKeyword : public CClassStructUnionKeyword  // class-like
            {
            public:
                GENERIC(CClassStructKeyword, CClassStructUnionKeyword);

            protected:
                virtual CToken* OnTryExtend(CTokenizer*) { return this; }
                //virtual void OnTransform() {}
            };

                class CClassKeyword : public CClassStructKeyword  // 'class'
                {
                public:
                    GENERIC(CClassKeyword, CClassStructKeyword);

                protected:
                    virtual CToken* OnTryExtend(CTokenizer*) { return this; }
                    //virtual void OnTransform() {}
                    virtual const char* OnGetDefaultTextRepresentation()
                            { return "class"; }
                };

                class CStructKeyword : public CClassStructKeyword  // 'struct'
                {
                public:
                    GENERIC(CStructKeyword, CClassStructKeyword);

                protected:
                    virtual CToken* OnTryExtend(CTokenizer*) { return this; }
                    //virtual void OnTransform() {}
                    virtual const char* OnGetDefaultTextRepresentation()
                            { return "struct"; }
                };

                class CInterfaceKeyword : public CClassStructKeyword
                {
                public:
                    GENERIC(CInterfaceKeyword, CClassStructKeyword);

                protected:
                    virtual CToken* OnTryExtend(CTokenizer*) { return this; }
                    //virtual void OnTransform() {}
                    virtual const char* OnGetDefaultTextRepresentation()
                            { return "__interface"; }
                };

            class CUnionKeyword : public CClassStructUnionKeyword  // 'union'
            {
            public:
                GENERIC(CUnionKeyword, CClassStructUnionKeyword);

            protected:
                virtual CToken* OnTryExtend(CTokenizer*) { return this; }
                //virtual void OnTransform() {}
                virtual const char* OnGetDefaultTextRepresentation()
                        { return "union"; }
            };

        class CEnumKeyword : public CAggregateKeyword  // 'enum'
        {
        public:
            GENERIC(CEnumKeyword, CAggregateKeyword);

        protected:
            virtual CToken* OnTryExtend(CTokenizer*) { return this; }
            //virtual void OnTransform() {}
            virtual bool OnIsClassBodyDeclaresInOuterScope() { return true; }
            virtual const char* OnGetDefaultTextRepresentation()
                    { return "enum"; }
        };

    ////////////////////////////////////////////////////////////////////////////////
    // extern [""] [{ ... }]

    class CExternKeywordAndText : public CDeclModifier
    {
    public:
        GENERIC(CExternKeywordAndText, CDeclModifier);
        CExternKeywordAndText();

    protected:
        virtual CToken* OnTryExtend();
        virtual void OnTransform();
        virtual void OnRegenerate(CWriter* pWriter);
        virtual bool OnIsClassBodyDeclaresInOuterScope() { return true; }
        virtual bool OnIsSignalForBracedBodyToTerminatesDeclaraton() { return true; }
        virtual const char* OnGetDefaultTextRepresentation()
                { return "extern"; }

    private:
        nullable<CCharDoubleQuotedString*> m_npCharDoubleQuotedString;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // namespace [x] [{ ... }]

    class CNamespaceKeyword : public CDeclModifier
    {
    public:
        GENERIC(CNamespaceKeyword, CDeclModifier);
        //CNamespaceKeyword();

    protected:
        virtual CToken* OnTryExtend(CTokenizer*) { return this; }
        //virtual void OnTransform() {}
        virtual bool OnIsSignalForBracedBodyToTerminatesDeclaraton() { return true; }
        virtual const char* OnGetDefaultTextRepresentation()
                { return "namespace"; }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // typedef

    class CTypedefKeyword : public CDeclModifier  // 'typedef'
    {
    public:
        GENERIC(CTypedefKeyword, CDeclModifier);

[...]