#
# Makefile.am for core/libycp/src/include/ycp
#

pkgincludedir = $(includedir)/ycp

# these go to $(pkgincludedir)

pkginclude_HEADERS =					\
							\
	Xmlcode.h					\
	YCPBoolean.h YCPByteblock.h			\
	YCPElement.h YCPFloat.h				\
	YCPInteger.h YCPList.h				\
	YCPMap.h YCPPath.h				\
	YCPString.h YCPSymbol.h YCPTerm.h		\
	YCPValue.h YCPVoid.h toString.h			\
	YCPExternal.h					\
	Type.h TypePtr.h				\
	YCPBuiltinBoolean.h YCPBuiltinFloat.h		\
	YCPBuiltinByteblock.h YCPBuiltinSymbol.h	\
	YCPBuiltinInteger.h YCPBuiltinList.h		\
	YCPBuiltinMap.h	YCPBuiltinMisc.h		\
	YCPBuiltinPath.h YCPBuiltinString.h		\
	YCPBuiltinTerm.h YCPBuiltinVoid.h		\
	YCPBuiltinMultiset.h				\
	StaticDeclaration.h				\
	YCode.h	YCodePtr.h				\
	YCPCode.h					\
	YCPCodeCompare.h				\
	Bytecode.h Import.h Point.h			\
	YExpression.h YStatement.h YBlock.h		\
	SymbolTable.h Parser.h				\
	YSymbolEntry.h					\
	y2log.h ycpless.h pathsearch.h			\
	y2string.h					\
	ExecutionEnvironment.h

# do not install Scanner.h to avoid "flex" package dependency
EXTRA_DIST=Scanner.h

#<INSTALL-HEADER-TARGET>

# Automatically inserted by insert_install_target
# DO NOT REMOVE THE TAGS!

# Install header files only if different to version already installed -
# prevent unnecessary installations to keep make dependency changes
# and thus rebuilds for dependent packages as low as possible.

install-pkgincludeHEADERS: $(pkginclude_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(pkgincludedir)
	@list='$(pkginclude_HEADERS)';							\
	for p in $$list; do								\
		if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; 			\
		if ! cmp -s $$d$$p $(DESTDIR)$(pkgincludedir)/$$p; then			\
			echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(pkgincludedir)/$$p";	\
			$(INSTALL_DATA) $$d$$p $(DESTDIR)$(pkgincludedir)/$$p;		\
	  	fi									\
	done

#</INSTALL-HEADER-TARGET>
