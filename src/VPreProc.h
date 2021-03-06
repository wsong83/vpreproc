/*
 * Copyright (c) 2011-2012 Wei Song <songw@cs.man.ac.uk> 
 *    Advanced Processor Technologies Group, School of Computer Science
 *    University of Manchester, Manchester M13 9PL UK
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

/* 
 * Embed the Verilog-Perl verilog preprocessor to AVS
 * 22/05/2012   Wei Song
 *
 *
 */
// -*- C++ -*-
//*************************************************************************
//
// Copyright 2000-2012 by Wilson Snyder.  This program is free software;
// you can redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License Version 2.0.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//*************************************************************************
/// \file
/// \brief Verilog::Preproc: Preprocess verilog code
///
/// Authors: Wilson Snyder
///
/// Code available from: http://www.veripool.org/verilog-perl
///
//*************************************************************************

#ifndef _VPREPROC_H_
#define _VPREPROC_H_ 1

#include <string>
#include <map>
#include <iostream>
#include "VFileLine.h"

namespace VPPreProc {

/// Generic opaque pointer to VPreProcImp implementation class.
struct VPreProcOpaque {
    virtual ~VPreProcOpaque() {}
};
class VDefine;

//**********************************************************************
// VPreProc
/// Verilog Preprocessor.
////
/// This defines a preprocessor.  Functions are virtual so users can override them.
/// After creating, call openFile(), then getline() in a loop.  The class will to the rest...

class VPreProc {
public:
    VPreProc();
    void configure(VFileLine* filelinep);
    virtual ~VPreProc();

    // STATE
private:
    int		m_keepComments;
    int		m_keepWhitespace;
    bool	m_lineDirectives;
    bool	m_pedantic;
    bool	m_synthesis;

public:
    // CONSTANTS
    enum MiscConsts {
 	DEFINE_RECURSION_LEVEL_MAX = 1000,	// How many `def substitutions before an error
 	INCLUDE_DEPTH_MAX = 500,		// How many `includes deep before an error
	STREAM_DEPTH_LEVEL_MAX = 2000,		// How many streams deep (sometimes `def deep) before an error
	//					// Set more than DEFINE_RECURSION_LEVEL_MAX or INCLUDE_DEPTH_MAX
 	NEWLINES_VS_TICKLINE = 20		// Use `line in place of this many newlines
    };

    // ACCESSORS
    /// Insert given file into this point in input stream
    bool openFile(std::string filename, VFileLine* filelinep=NULL);
    void debug(int level);	///< Set debugging level
    std::string getall(size_t approx_chunk);	///< Return all lines, or at least approx_chunk bytes. (Null if done.)
    std::string getline();		///< Return next line/lines. (Null if done.)
    bool isEof();		///< Return true on EOF.
    void insertUnreadback(std::string text);

    VFileLine* fileline();	///< File/Line number for last getline call

    // The default behavior is to pass all unknown `defines right through.
    // This lets the user determine how to report the errors.  It also nicely
    // allows `celldefine and such to remain in the output stream.

    // CONTROL METHODS
    // These options control how the parsing proceeds
    int  keepComments() { return m_keepComments; }
    void keepComments(int flag) { m_keepComments=flag; }	// Return comments, 0=no, 1=yes, 2=callback
    int  keepWhitespace() { return m_keepWhitespace; }
    void keepWhitespace(int flag) { m_keepWhitespace=flag; }	// Return extra whitespace
    bool lineDirectives() { return m_lineDirectives; }
    void lineDirectives(bool flag) { m_lineDirectives=flag; }	// Insert `line directives
    bool pedantic() { return m_pedantic; }
    void pedantic(bool flag) { m_pedantic=flag; }	// Obey standard; Don't substitute `error
    bool synthesis() { return m_synthesis; }
    void synthesis(bool flag) { m_synthesis=flag; }	// Ignore translate off

    // CALLBACK METHODS
    // This probably will want to be overridden for given child users of this class.
    virtual void comment(std::string cmt) = 0;		///< Comment detected (if keepComments==2)
    virtual void include(std::string filename) = 0;	///< Request a include file be processed
    virtual void define(std::string name, std::string value, std::string params) = 0; ///< `define without any parameters
    virtual void undef(std::string name) = 0;		///< Remove a definition
    virtual void undefineall() = 0;			///< Remove all non-command-line definitions
    virtual bool defExists(std::string name) = 0;	///< Return true if define exists
    virtual std::string defParams(std::string name) = 0;	///< Return parameter list if define exists
    virtual std::string defValue(std::string name) = 0;	///< Return value of given define (should exist)
    virtual std::string defSubstitute(std::string substitute) = 0;	///< Return value to substitute for given post-parameter value

    // UTILITIES
    void error(std::string msg) { fileline()->error(msg); }	///< Report a error
    void fatal(std::string msg) { fileline()->fatal(msg); }	///< Report a fatal error

private:
    VPreProcOpaque*	m_opaquep;	///< Pointer to parser's implementation data.
};

}

#endif // Guard
