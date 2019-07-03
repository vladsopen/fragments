// CAlterCtaMacro.cpp

[...]

            //
            // Select the argument type
            //

            if (sName == "Write")
            {
                m_sWrite += sArg;
            }
            else
            if (sName == "Find")
            {
                m_sFind += sArg;
            }
            else
            if (sName == "Replace")
            {
                m_textReplace.AddWord(sArg);
            }
            else
            if (sName == "MustMatch")
            {
                m_sMustMatch += sArg;
            }
            else
            if (sName == "ErrorIfMatch")
            {
                m_sErrorIfMatch += sArg;
            }
            else
            if (sName == "NameHook") // SEMIPUBLIC()
            {
                GetTokenizer()->ReportError(
                    errn(0xAE01),
                    str() + 
                        "using of " + sName + " is not approved here",
                    pString);

                HandleInternalSemipublicMacro(
                    pString,
                    sArg,
                    pathSource,
                    GetTokenizer()->
                        GetNameHookMap());
            }
            else
            if (sName == "InternalPrefixHook")
            {
                GetTokenizer()->ReportError(
                    errn(0xAE02),
                    str() + 
                        "using of " + sName + " is not approved here",
                    pString);

                HandleInternalSemipublicMacro(
                    pString,
                    sArg,
                    pathSource,
                    GetTokenizer()->
                        GetInternalPrefixHookMap());
            }
            else
            if (sName == "IncludeOnlyFrom") // INCLUDABILITY()
            {
                /*
                GetTokenizer()->ReportError(
                    errn(0xAE03),
                    str() + 
                        "using of " + sName + " is not approved here",
                    pString);
                    */

                m_bIncludeOnlyFrom = true;
                m_bForceExecuteInRegenerate = true;
            }
            else
            if (sName == "AllowPath")
            {
                str sMatchPath = sArg;
                if (sMatchPath.FindPos(':') != -1 
                    || 
                    sMatchPath.FindPos('\\') != -1)
                {
                    GetTokenizer()->ReportError(
                        errn(0xC5C5),
                        str() + 
                            "non-portable path in AllowPath '" + 
                                sMatchPath + "'",
                        pString);
                }

                m_textAllowPath.
                    AddWord(
                        sMatchPath);
            }
            else
[...]