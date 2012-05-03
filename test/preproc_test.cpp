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
 * Test for the Verilog preprocessor
 * 27/04/2012   Wei Song
 *
 *
 */

#include "src/preproc.h"

#include <boost/program_options.hpp>
#include <fstream>

namespace po = boost::program_options;
using std::string;
using std::vector;
using namespace VPPreProc;

int main(int argc, char** argv)
{

  string inpfile;
  string outpfile;
  std::ofstream of_handle;


  po::options_description preproc_opt("Preprocessor options");
  preproc_opt.add_options()
    ("help", "print usage")
    ("include,I", po::value<vector<string> >()->composing(), "include paths")
    ("macro,D", po::value<vector<string> >()->composing(), "macro defines")
    ("output,o", po::value<string>(), "output file")
    ;

  po::options_description input_opt;
  input_opt.add_options()
    ("input", po::value<string>(), "input file")
    ;

  po::options_description all_opt;
  all_opt.add(preproc_opt).add(input_opt);

  po::positional_options_description p;
  p.add("input", 1);

  po::variables_map vm;

  try {
    store(po::command_line_parser(argc, argv).options(all_opt).positional(p).run(), vm);
    notify(vm);
  } catch(exception& e) {
    cout << "Wrong command syntex! See usage using --help." << endl;
    return 1;
  }

  if(vm.count("help")) {        // print help information
    cout << "Verilog preprocessor: vpreproc [options] source_file" << endl;
    cout << preproc_opt << endl;
    return 1;
  }

  if(vm.count("input")) {       // fetch input file
    inpfile = vm["input"].as<string>();
  } else {
    cout << "No source file provided! See usage using --help." << endl;
    return 1;
  } 
  
  if(vm.count("output")) {
    outpfile = vm["output"].as<string>();
    if(outpfile != "") {
      of_handle.open(outpfile.c_str(), std::ios::out);
    }
  }

  VFileLineXs* filelinep = new VFileLineXs(NULL);
  VPreProcXs* preprocp = new VPreProcXs();
  filelinep->setPreproc(preprocp);
  preprocp->configure(filelinep);
  preprocp->openFile(inpfile);

  if(vm.count("macro")) {
    vector<string> mlist = vm["macro"].as<vector<string> >();
    vector<string>::iterator it, end;
    for(it=mlist.begin(), end=mlist.end(); it !=end; it++)
      preprocp->define(*it, "", "", true);
  }

  if(vm.count("include")) {
    vector<string> inclist = vm["include"].as<vector<string> >();
    vector<string>::iterator it, end;
    for(it=inclist.begin(), end=inclist.end(); it !=end; it++)
      preprocp->add_incr(*it);
  }

  while(true) {
    string rt = preprocp->getline();
    if(outpfile != "") {
      of_handle << rt;
    } else {
      cout << rt;
    }
    if(rt.size() == 0) break;
  }

  if(outpfile != "") 
    of_handle.close();  

  return 1;
}
