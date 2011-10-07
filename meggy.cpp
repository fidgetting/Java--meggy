/*
 * meggy.cpp
 *
 *  Created on: Jun 3, 2011
 *      Author: norton
 */

#include <visitor.hpp>
#include <basic_visitor.hpp>
#include <node.hpp>
#include <type.hpp>
#include <symtable.hpp>
#include <type_checker.hpp>

#include <stdio.h>
#include <iostream>
#include <fstream>

extern int yyparse(void);
extern FILE* yyin;

program* prog;

int main(int argc, char** argv) {
  std::string fname(argv[1]);
  std::ofstream ast_ostr((fname + ".ast").c_str());
  std::ofstream sym_ostr((fname + ".sym").c_str());
  line_visitor      line;
  dot_visitor       dot(ast_ostr);
  sym_table_creator symvisitor;
  type_checker      typevisit(symvisitor.table());

  /* read open and read the relevant file */
  yyin = fopen(argv[1], "r");
  if(!yyin) {
    printf("couldn't open file for reading\n");
    return -1;
  }
  yyparse();
  fclose(yyin);

  if(!prog)
    return -1;

  prog->accept(&line);
  prog->accept(&dot);
  prog->accept(&symvisitor);
  //prog->accept(&typevisit);

  symvisitor.table()->dot(sym_ostr);

  ast_ostr.close();
  sym_ostr.close();
  type::clean();
  delete symvisitor.table();
  delete prog;
  return 0;
}

