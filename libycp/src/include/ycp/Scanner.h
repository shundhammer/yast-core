/*---------------------------------------------------------------------\
|                                                                      |  
|                      __   __    ____ _____ ____                      |  
|                      \ \ / /_ _/ ___|_   _|___ \                     |  
|                       \ V / _` \___ \ | |   __) |                    |  
|                        | | (_| |___) || |  / __/                     |  
|                        |_|\__,_|____/ |_| |_____|                    |  
|                                                                      |  
|                               core system                            | 
|                                                        (C) SuSE GmbH |  
\----------------------------------------------------------------------/ 

   File:	Scanner.h

   Author:	Klaus Kaempf <kkaempf@suse.de>
		Mathias Kettner <kettner@suse.de>
   Maintainer:	Klaus Kaempf <kkaempf@suse.de>

/-*/
// -*- c++ -*-

/*
 * Interface to the flex generated scanner
 *
 */

#ifndef Scanner_h
#define Scanner_h

#ifndef __FLEX_LEXER_H
#include "FlexLexer.h"
#endif

#include "ycp/StaticDeclaration.h"
#include <stdio.h>
#include <string>

class TableEntry;
class SymbolTable;
#include "ycp/Type.h"
#include "ycp/y2log.h"

/// linked list for formal parameters (not a stack at all)
typedef struct formalparamstack {
    struct formalparamstack *next;	///< ptr to next formal parameter
    const char *name;			///< name of formal parameter
    constTypePtr type;			///< type of formal parameter
    unsigned int line;			///< line number of name token
} formalparam_t;

typedef union {
	bool bval;		// boolean
	long long ival;		// integer
	double fval;		// float
	const char *sval;	// string
	unsigned char *cval;	// bytecode
	char *pval;		// path
	char *yval;		// symbol
	const char *nval;	// name
	declaration_t *dval;	// builtin declaration
	TableEntry *tval;	// table entry
	formalparam_t *fpval;	// formal parameter chain
	void *val;		// any other value
} tokenValue;

/**
 * @short Scanner for scanning YCP syntax
 * @see Parser
 * This class is a filter. You give it a YCP text in form of
 * a string, a FILE *pointer or a unix file descriptor. Out of this
 * it produces a stream of tokens representing the lexical items
 * of the YCP text. This scanner class is based on yyFlexLexer, 
 * which is generated by flex++.
 */

class Scanner : public yyFlexLexer, public Logger
{
private:
    /**
     * Allocate this many bytes for a string. If a larger
     * string is encountered, the buffer size is at least doubled.
     * I don't believe, that the value of STRING_HUNK has a great
     * impact on speed. 1024 is probably a nice value for it.
     */
    static const int STRING_HUNK = 1024;

    /**
     * The name of the file being parsed. It is used for generating
     * nice error messages only. It is the empty string, if no filename
     * is available (e.g. while scanning from stdin).
     */
    string m_filename;

    /**
     * If the YCP text source is given in form of a string buffer,
     * this buffer is stored here. 0 otherwise.
     */
    const char *m_inputBuffer;

    /**
     * If the YCP text source is given in form of an open clib-level
     * file pointer, this variable hold it. Must be 0 otherwise.
     */
    FILE *m_inputFile;

    /**
     * If the YCP text source is given in form of a unix lowlevel file
     * descriptor, this variable holds the descriptor. Must be -1 otherwise,
     * since 0 is a valid file descriptor (stdin).
     */
    int m_inputFd;

    /**
     * Holds the value being scanned lastly.
     */
    tokenValue m_scannedValue;

    /**
     * Holds the type of the value being scanned lastly.
     */
    constTypePtr m_scannedType;

    /**
     * Holds the comment before the most recent token
     */
    std::string m_commentBefore;

    /**
     * Holds the line number of scanned_value
     */
    int m_lineNumber;

    /**
     * Used for string constant scanning
     */
    char *m_scandataBufferPtr;

    /**
     * Used for string constant scanning
     */
    char *m_scandataBuffer;

    /**
     * Used for string constant scanning
     */
    int m_scandataBufferSize;

    /**
     * Is true, if the input can be buffered, i.e. more than one
     * character may be read at once in order to gain performance.
     */
    bool m_buffered;

    // current symbol tables, used also in parser.yy
    SymbolTable *m_globalTable;
    SymbolTable *m_localTable;

    /**
     * This is a kludge rather than proper memory management.
     * Klaus, who owns the tables in various cases?
     */
    bool m_owningGlobal;
    bool m_owningLocal;

    /**
     * list of predefined namespace which have been auto-imported
     *   Y2Namespace is non-const since the block might get evaluated (constructor)
     */
    std::list<std::pair<std::string, Y2Namespace *> > m_autoimport_predefined;

public:
    /**
     * Creates a new scanner that scans from an open clib-level
     * file descriptor that has been opened with fopen(filename, "r").
     * You have to close the file yourself afterwards.
     * @param inputfile the open file
     * @param filename If you have the name of the file in hand here,
     * tell it, it makes nicer error messages. Give 0 otherwise.
     */
    Scanner (FILE *inputfile, const char *filename);

    /**
     * Creates a new scanner that scans from a zero terminated constant C string.
     * Your buffer is left untouched. We don't make a copy of it, so ist must
     * be valid all the time this class is used.
     * @param inputbuffer Pointer to the string.
     */
    Scanner(const char *inputbuffer);

    /**
     * Creates a new scanner that scans from an open unix
     * lowlevel file descriptor. You have to close the descriptor 
     * yourself afterwards
     * @param input_fd the file descriptor
     * @param filename If you have the name of the file in hand here,
     * tell it, it makes nicer error messages. Give 0 otherwise.
     */
    Scanner(int input_fd, const char *filename);

    /**
     * Cleans up
     */
    ~Scanner();

    /**
     * Makes the scanner use buffering, i.e. read more than
     * one character at once.
     */
    void setBuffered();

    /**
     * Initialize tables for global and local symbols.
     *
     * If gTable and lTable are set, they're used instead of local
     * ones. This is used for include files using the symbol tables
     * of the including block.
     * see: Parser::parse()
     */
    void initTables (SymbolTable *globalTable, SymbolTable *localTable);

    /**
     * return current globalTable.
     * used by parser.
     */
    SymbolTable *globalTable () const;

    /**
     * return current localTable.
     * used by parser.
     */
    SymbolTable *localTable () const;

    /**
     * Scans and returns the next token. Return 0, in case of EOF.
     * The value of the scanned token is saved in @ref #scanned_value and
     * can be retrieved with @ref #scannedValue. The implementation of
     * this function is generated by flex++.
     */
    int yylex();

    /**
     * Overriden from @ref yyFlexLexer. The flex scanner uses this
     * function to get the next input characters. Our implementation
     * always returns just one character. Too much lookahead would result
     * in blocking and deadlock in a protocol situation.
     * FIXME: when reading from file read max_size characters
     * @param buf Buffer where the input is to be stored in
     * @param max_size size of this buffer
     * @return the number of new input character. 0 on EOF.
     */
    int LexerInput( char* buf, int max_size );

    /**
     * Is called by the flex lexer, if a scan error occurs.
     * Calls @ref #logError.
     */
    void LexerError( const char* msg );

    /**
     * Gets the value of the latest scanned token. Returns
     * 0, if that token does not represent a proper value.
     */
    tokenValue scannedValue() const;

    /**
     * Gets the value of the latest scanned token. Returns
     * 0, if that token does not represent a proper value.
     */
    constTypePtr scannedType() const;

    std::string commentBefore () const;

    /**
     * Gets the line number of the latest scanned token.
     */
    int lineNumber() const;

    /**
     * Gets the currently scanned file
     */
    string filename() const;

    /**
     * Is called by @ref #LexerError.
     * Is also called by yyerror for error reporting. It reports
     * the error via y2log and also reports the filename, if available
     * and the linenumber.
     * @param msg the plain error message
     */
    void logError (const char *loginfo, int lineno, ...) __attribute__ ((format (printf, 2, 4)));

    /**
     * logs a warning.
     */
    void logWarning (const char *loginfo, int lineno, ...) __attribute__ ((format (printf, 2, 4)));

    /**
     * get list of predefined namespaces which have been autoloaded by the scanner
     */
    const std::list<std::pair<std::string, Y2Namespace *> > & autoimport_predefined() const { return m_autoimport_predefined; };

    /**
     * strdup via new, so delete [] can be used safely
     */
    static char *doStrdup (const char *s);

    /**
     *
     */
    void closeInput ();
    
private:
    /**
     * Used in the rules of the scanner to define the value
     * of a token.
     */
    void setScannedToken (const tokenValue & value, constTypePtr type);

    void setCommentBefore (const string & comment_before);

    /**
     * Internal helper function that deals with strings of arbitrary
     * length
     */
    char *extend_scanbuffer (int size);

public:

    virtual void error(string error);
    virtual void warning(string warning);

};

#endif // Scanner_h
