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
 * A wrapper of the Verilog-PreProcessor of Verilog Perl tool 3.314
 * 27/04/2012   Wei Song
 *
 *
 */

#include "preproc.h"

using namespace VPPreProc;

#//**********************************************************************
#// Overrides error handling virtual functions to invoke callbacks

void VPPreProc::VFileLineXs::error(const string& msg) {
    static string holdmsg; holdmsg = msg;
    m_vPreprocp->call(NULL, 1,"error",holdmsg.c_str());
}

#//**********************************************************************
#// VPreProcXs functions

VPPreProc::VPreProcXs::~VPreProcXs() {
    for (deque<VFileLineXs*>::iterator it=m_filelineps.begin(); it!=m_filelineps.end(); ++it) {
	delete *it;
    }
    
    // delete the macro database
    for (map<string, VMacro*>::iterator it=db.begin(); it!=db.end(); ++it) {
      delete it->second;
    }
}

#//**********************************************************************
#// Overrides of virtual functions to invoke callbacks

void VPPreProc::VPreProcXs::comment(string cmt) {
  // nothing
}
void VPPreProc::VPreProcXs::include(string filename) {
  //
}
void VPPreProc::VPreProcXs::undef(string define) {
  //
}
void VPPreProc::VPreProcXs::undefineall() {
  //
}
void VPPreProc::VPreProcXs::define(string define, string value, string params) {
  //
}
bool VPPreProc::VPreProcXs::defExists(string define) {
  //
}
string VPPreProc::VPreProcXs::defParams(string define) {
  string paramStr;
  return paramStr;
}
string VPPreProc::VPreProcXs::defValue(string define) {
    string valueStr;
    return valueStr;
}
string VPPreProc::VPreProcXs::defSubstitute(string subs) {
    string outStr;
    return outStr;
}

