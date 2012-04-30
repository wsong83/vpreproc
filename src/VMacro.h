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
 * 27/04/2012   Wei Song
 *
 *
 */

#ifndef AVS_MACRO_H_
#define AVS_MACRO_H_

#include<string>
#include<vector>

namespace VPPreProc {

  class VMacro {
  public:
    // constructors
    VMacro(const std::string&, const std::string&, const std::string&);

    // helpers
    bool is_parameterized() const { return m_paNum > 0; }
    bool is_valid() const { return valid; }
    std::string get_value(const std::vector<string>& para = std::vector<string>()); /* get the value of a macro */
    std::string get_value(const std::string&); /* get the value of a macro */
    bool format_para(const std::string&, std::vector<string>&); /* format a string to normal parameter list */
    
    //data
    std::string m_name;         /* the name of the macro */
    unsigned int m_paNum;       /* number of parameters */
    std::string value;          /* formated macro value */
    
  private:
    // data
    bool valid;
    
    // helpers
    void format(const std::string&, const std::string&); /* format the formal and value to the boost formate formate */

  };

}

#endif
