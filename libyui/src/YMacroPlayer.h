/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:		YMacroPlayer.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YMacroPlayer_h
#define YMacroPlayer_h

#include <string>

class YWidget;
class YBlock;

class YMacroPlayer
{
public:

    /**
     * Constructor
     **/
    YMacroPlayer( const string & macroFileName );

    /**
     * Destructor
     **/
    virtual ~YMacroPlayer();

    /**
     * Report error status
     **/
    bool error() const { return _error; }

    /**
     * Returns "true" if the execution of the entire macro is finished.
     * Undefined on error, so check for error() first!
     **/
    bool finished();

    /**
     * Return the result of the next macro block and increment the internal block
     * counter. Returns YCPNull() on any previous error or if finished.
     * Check for error() or finished() before calling this!
     **/
    YCPValue evaluateNextBlock();

    /**
     * Rewind macro execution - prepare to redo from start
     **/
    void rewind();

protected:

    /**
     * Read and parse a macro file. Sets the internal "error" status.
     **/
    void readMacroFile( const string & macroFileName );

    /**
     * Clear error status
     **/
    void clearError()	{ _error = false; }

    /**
     * Set error status
     **/
    void setError()	{ _error = true; }

    /**
     * The parsed macro
     **/
    YBlockPtr _macro;

    /**
     * Error status
     **/
    bool _error;

    /**
     * Number of the next macro block to execute
     **/
    int _nextBlockNo;
};

#endif // YMacroPlayer_h