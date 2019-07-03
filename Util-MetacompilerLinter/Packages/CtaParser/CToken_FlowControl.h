// CToken_FlowControl.h
// (see "CToken.h")
// (see "ParseExpressions.cpp")

//////////////////////////////////////////////////////////////////////////////////////
// CFlowControl - flow control operator (starts with a keyword)

class CFlowControl : public CToken
{
public:
    GENERIC_ABSTRACT(CFlowControl, CToken);

protected:
    //virtual CToken* OnTryExtend(CTokenizer*) { return this; }
    //virtual void OnTransform();

private:
};

    // CThrowOpOrModifier - depending on context can be either CThrowOp or CThrowModifier
    class CThrowOpOrModifier : public CToken
    {
    public:
        GENERIC(CThrowOpOrModifier, CToken);
        //CThrowOpOrModifier();

    protected:
        virtual CToken* OnTryExtend();
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
            { return "throw"; }
    };

        // CThrowOp
        class CThrowOp : public CFlowControl
        {
        public:
            GENERIC(CThrowOp, CFlowControl);
            CThrowOp();

            CExpressionList* x_pExpressionList;

        protected:
            virtual CToken* OnTryExtend();
            virtual void OnTransform();
            virtual void OnRegenerate(CWriter* pWriter);

        private:
        };

    // CWhileAlikeOp - all while-like flow control operators
    class CWhileAlikeOp : public CFlowControl
    {
    public:
        GENERIC(CWhileAlikeOp, CFlowControl);
        //CWhileAlikeOp();

    protected:
        //virtual CToken* OnTryExtend();
        virtual void OnTransform();
        //virtual void OnRegenerate(CWriter* pWriter);

    private:
    };

        // CWhileOp
        class CWhileOp : public CWhileAlikeOp
        {
        public:
            GENERIC(CWhileOp, CWhileAlikeOp);
            //CWhileOp();

        protected:
            //virtual CToken* OnTryExtend();
            //virtual void OnTransform();
            virtual void OnRegenerate(CWriter* pWriter);
            virtual const char* OnGetDefaultTextRepresentation()
                { return "while"; }

        private:
        };

        // CIfOp
        class CIfOp : public CWhileAlikeOp
        {
        public:
            GENERIC(CIfOp, CWhileAlikeOp);
            //CIfOp();

        protected:
            //virtual CToken* OnTryExtend();
            //virtual void OnTransform() {}
            virtual void OnRegenerate(CWriter* pWriter);
            virtual const char* OnGetDefaultTextRepresentation()
                { return "if"; }

        private:
        };

            // CElseOp
            class CElseOp : public CFlowControl
            {
            public:
                GENERIC(CElseOp, CFlowControl);
                //CElseOp();

            protected:
                //virtual CToken* OnTryExtend();
                //virtual void OnTransform() {}
                //virtual void OnRegenerate(CWriter* pWriter);
                virtual const char* OnGetDefaultTextRepresentation()
                    { return "else"; }
            };

        // CForOp
        class CForOp : public CWhileAlikeOp
        {
        public:
            GENERIC(CForOp, CWhileAlikeOp);
            //CForOp();

        protected:
            //virtual CToken* OnTryExtend();
            //virtual void OnTransform() {}
            virtual void OnRegenerate(CWriter* pWriter);
            virtual const char* OnGetDefaultTextRepresentation()
                { return "for"; }

        private:
            //CExpressionList* x_pExpressionList;
        };

        // CSwitchOp
        class CSwitchOp : public CWhileAlikeOp
        {
        public:
            GENERIC(CSwitchOp, CWhileAlikeOp);
            //CSwitchOp();

        protected:
            //virtual CToken* OnTryExtend();
            //virtual void OnTransform() {}
            //virtual void OnRegenerate(CWriter* pWriter);
            virtual const char* OnGetDefaultTextRepresentation()
                { return "switch"; }

        private:
        };

    // CDoOp
    class CDoOp : public CFlowControl
    {
    public:
        GENERIC(CDoOp, CFlowControl);
        //CDoOp();

    protected:
        //virtual CToken* OnTryExtend();
        //virtual void OnTransform() {}
        virtual void OnRegenerate(CWriter* pWriter);
        virtual const char* OnGetDefaultTextRepresentation()
            { return "do"; }

    private:
    };

    // CTryOp
    class CTryOp : public CFlowControl
    {
    public:
        GENERIC(CTryOp, CFlowControl);
        //CTryOp();

    protected:
        //virtual CToken* OnTryExtend();
        //virtual void OnTransform() {}
        //virtual void OnRegenerate(CWriter* pWriter);
        virtual const char* OnGetDefaultTextRepresentation()
            { return "try"; }

    private:
    };

        // CTryCstyleOp
        class CTryCstyleOp : public CTryOp
        {
        public:
            GENERIC(CTryCstyleOp, CTryOp);
        protected:
            //virtual void OnTransform() {}
            virtual const char* OnGetDefaultTextRepresentation()
                { return "__try"; }
        };

    // CCatchOp
    class CCatchOp : public CFlowControl
    {
    public:
        GENERIC(CCatchOp, CFlowControl);
        //CCatchOp();

    protected:
        //virtual CToken* OnTryExtend();
        //virtual void OnTransform() {}
        virtual void OnRegenerate(CWriter* pWriter);
        virtual const char* OnGetDefaultTextRepresentation()
            { return "catch"; }

    private:
    };

        // CExceptCstyleOp
        class CExceptCstyleOp : public CCatchOp
        {
        public:
            GENERIC(CExceptCstyleOp, CCatchOp);

        protected:

[...]