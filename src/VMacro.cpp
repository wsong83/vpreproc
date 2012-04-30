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
 * Data structure to store macros
 * 30/04/2012   Wei Song
 *
 *
 */

#include "VMacro.h"

#include<boost/format.hpp>

using std::string;
using std::vector;

#include<list>;
using std::list;

#include<cctype>

using namespace VPPreProc;

VPPreProc::VMacro::VMacro(const string& nm, const string& formal, const string& v)
  : m_name(nm) {
  this->format(formal, v);
}

string VPPreProc::VMacro::get_value(const vector<string>& para) {

  assert(para.size() == m_PaNum);

  boost::format rv(value);
  for(unsigned int i=0; i<m_PaNum; i++)
    rv % para[i];

  return rv.str();
}

bool VPPreProc::VMacro::format_para(const string& pstr, vector<string>& plist) {

  assert(pstr.size() > 2 && pstr[0] == '(' && pstr[pstr.size() -1] == ')');

  // must in the format of (a, (4+5),8*4), using '(', ')', and ',' as delimiters
  plist.clear();
  unsigned int level = 0;
  unsigned int start;
  
  for(unsigned int i=1; i<pstr.size()-1; i++) {
    
    if(level == 0) { level++; start = i; } // the start of a parameter
    
    switch(pstr[i]) {
    case '(': level++; break;
    case ')': level--; break;
    case ',': if(level == 1) level--; break;
    }

    if(level == 0) plist.push_back(pstr.substr(start,i-start));
  }

  if(level == 1)
    plist.push_back(pstr.substr(start,pstr.size()-1-start));
  else
    return false;

  return plist.size() == m_paNum;
}

void VPPreProc::VMacro::format(const string& pstr, const string& vstr) {
  // extract the parameter first
  vector<string> plist;
  unsigned int level = 0;
  unsigned int start;
  
  for(unsigned int i=1; i<pstr.size()-1; i++) {
    
    if(level == 0) { level++; start = i; } // the start of a parameter
    
    switch(pstr[i]) {
    case '(': level++; break;
    case ')': level--; break;
    case ',': if(level == 1) level--; break;
    }

    if(level == 0) plist.push_back(pstr.substr(start,i-start));
  }
  
  if(level == 1)
    plist.push_back(pstr.substr(start,pstr.size()-1-start));
  else {
    valid = false;
    return;
  }

  // now start to format the vstr
  // first break it into a list of strings
  list<string> vslist;
  // 0, init; 1, escaped id; 2, simple id; 3, string; 4, simple number; 5, fixed-number
  unsigned int token_type = 0;  
  for(unsigned int i=0; i<vstr.size(); i++) {
    switch(token_type){
    case 0: {                   // init
      if(vstr[i] == '\\') {
        start = i;
        token_type = 1;
      } else if(isalpha(vstr[i]) || // simple id
                vstr[i] == '_' ||   // simple id
                vstr[i] == '`' ||   // macro name
                vstr[i] == '$'      // system function call
                ) {
        // simply id
        start = i;
        token_type = 2;
      } else if(vstr[i] == '\"') { // string
        start = i;
        token_type = 3;
      } else if(isdigit(vstr[i])) { // simple number
        start = i;
        token_type = 4;
      } else if(vstr[i] == '\'') { // fixed-number
        start = i;
        token_type = 5;
      } else {                  // single charactor token
        vslist.push_back(vstr.substr(i, 1));
      }
      break;
    }
    
