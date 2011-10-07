#!/usr/bin/python
###############################################################################
# python scrip that allows for the generation of ast files. This is a great   #
# help as code for the abstract syntax tree is reppetetive and formulaic.     #
#                                                                             #
# author: Alex Norton, Zach Hoekstra                                          #
# date: 7/15/2011                                                             #
###############################################################################

visitcpp = open('visitor.cpp', 'w')
visithpp = open('visitor.hpp', 'w')
nodecpp = open('node.cpp', 'w')
nodehpp = open('node.hpp', 'w')

node_preamble = """
/*
 * node.hpp
 *
 * This file is automatically generated, do not change. If a change to this
 * file is needed, please change nodes.in instead.
 */

#ifndef NODE_HPP_INCLUDE
#define NODE_HPP_INCLUDE

#include <string>
#include <vector>

class visitor;

/* ************************************************************************** */
/* *** abstract __node classes ********************************************** */
/* ************************************************************************** */

class __node {
  public:

    __node(std::string name, int line = 0, int poss = 0) :
      _name(name), _line(line), _poss(poss) { }

    virtual ~__node() { };
    virtual void accept(visitor* v) = 0;

    inline int  line() const { return _line; }
    inline int& line()       { return _line; }
    inline int  poss() const { return _poss; }
    inline int& poss()       { return _poss; }

    inline std::string name() const { return _name; }

  protected:

    std::string _name;
    int _line;
    int _poss;
};

struct _i_expr : public __node {
    _i_expr(std::string name) : __node(name) { }
    virtual ~_i_expr() { }
};

struct _i_stmt : public __node {
    _i_stmt(std::string name) : __node(name) { }
    virtual ~_i_stmt() { }
};

struct _i_type : public __node {
    _i_type(std::string name) : __node(name) { }
    virtual ~_i_type() { }
};

class token : public __node {
  public:

    virtual ~token() { }
    virtual void accept(visitor* v);

    inline std::string  txt() const { return _txt; }
    inline std::string& txt()       { return _txt; }

    static token* get(std::string txt, int line, int poss);

  private:

    token(std::string txt, int line, int poss) :
      __node("token", line, poss), _txt(txt) { }

    std::string _txt;
};

/* ************************************************************************** */
/* *** node types *********************************************************** */
/* ************************************************************************** */

"""

nodecpp_preamble = """
/*
 * node.hpp
 *
 * This file is automatically generated, do not change. If a change to this
 * file is needed, please change nodes.in instead.
 */

#include <node.hpp>
#include <visitor.hpp>

token* token::get(std::string txt, int line, int poss) {
  return new token(txt, line, poss);
}

void token::accept(visitor* v) { v->token_v(this); }
"""

visitor_preamble = """
/*
 * visitor.h
 *
 * This file is automatically generated, do not change. If a change to this
 * file is needed, please change nodes.in instead.
 */

#ifndef VISITOR_H_INCLUDE
#define VISITOR_H_INCLUDE

#include <node.hpp>

struct visitor {
  public:

    visitor() { }
    virtual ~visitor() { }

    inline virtual void default_v(__node* node) { /* do nothing */}

    inline virtual void token_v(token* node) { default_v(node); }
"""

depth_preamble = """

struct depth_first_visitor : public visitor{
  public:

    depth_first_visitor() { }
    virtual ~depth_first_visitor() { }

    inline virtual void default_in (__node* node) { /* do nothing */ }
    inline virtual void default_out(__node* node) { /* do nothing */ }
    
    inline virtual void token_in (token* node) { default_in(node);  }
    inline virtual void token_out(token* node) { default_out(node); }
           virtual void token_v  (token* node);
"""

reverse_preamble = """

struct reverse_first_visitor : public visitor{
  public:

    reverse_first_visitor() { }
    virtual ~reverse_first_visitor() { }

    inline virtual void default_in (__node* node) { /* do nothing */ }
    inline virtual void default_out(__node* node) { /* do nothing */ }
    
    inline virtual void token_in (token* node) { default_in(node);  }
    inline virtual void token_out(token* node) { default_out(node); }
           virtual void token_v  (token* node);
"""

visitorcpp_preamble = """
/*
 * visitor.cpp
 *
 * This file is automatically generated, do not change. If a change to this
 * file is needed, please change nodes.in instead.
 */

#include <visitor.hpp>

void depth_first_visitor::token_v(token* node) {
  token_in(node);
  token_out(node);
}

void reverse_first_visitor::token_v(token* node) {
  token_out(node);
  token_in(node);
}

/* ************************************************************************** */
/* *** special ************************************************************** */
/* ************************************************************************** */
"""

visitcpp.write(visitorcpp_preamble)
visithpp.write(visitor_preamble)
nodecpp.write(nodecpp_preamble)
nodehpp.write(node_preamble)

def print_depth(node, ext):
  global visitcpp
  global visithpp
  
  params = []
  name = ''
  if ext:
    name = '%s_%s' % (node[1], ext)
  else:
    name = node[1]
  
  for elemen in node:
    params.append(elemen.split('|'))
  del params[0]
  del params[0]
  
  visithpp.write('    inline virtual void %s_in (%s* node) { default_in (node); }\n' % (name, name))
  visithpp.write('    inline virtual void %s_out(%s* node) { default_out(node); }\n' % (name, name))
  visithpp.write('           virtual void %s_v  (%s* node);\n'                       % (name, name))
  
  visitcpp.write('void depth_first_visitor::%s_v(%s* node) {\n' % (name, name))
  visitcpp.write('  %s_in (node);\n' % name)
  
  for param in params:
    if param[0].startswith('std::vector'):
      visitcpp.write('  for(auto iter = node->%s()->begin(); iter != node->%s()->end(); iter++)\n' % (param[1], param[1]))
      visitcpp.write('    (*iter)->accept(this);\n')
    else:
      visitcpp.write('  if(node->%s()) node->%s()->accept(this);\n' % (param[1], param[1]))
  
  visitcpp.write('  %s_out(node);\n' % name)
  visitcpp.write('}\n')

def print_reverse(node, ext):
  global visitcpp
  global visithpp
  
  params = []
  name = ''
  if ext:
    name = '%s_%s' % (node[1], ext)
  else:
    name = node[1]
  
  for element in node:
    params.append(element.split('|'))
  del params[0]
  del params[0]
  
  visithpp.write('    inline virtual void %s_in (%s* node) { default_in (node); }\n' % (name, name))
  visithpp.write('    inline virtual void %s_out(%s* node) { default_out(node); }\n' % (name, name))
  visithpp.write('           virtual void %s_v  (%s* node);\n'                       % (name, name))
  
  visitcpp.write('void reverse_first_visitor::%s_v(%s* node) {\n' % (name, name))
  visitcpp.write('  %s_out (node);\n' % name)
  
  for param in reversed(params):
    if param[0].startswith('std::vector'):
      visitcpp.write('  for(auto iter = node->%s()->rbegin(); iter != node->%s()->rend(); iter++)\n' % (param[1], param[1]))
      visitcpp.write('    (*iter)->accept(this);\n')
    else:
      visitcpp.write('  if(node->%s()) node->%s()->accept(this);\n' % (param[1], param[1]))
  
  visitcpp.write('  %s_in(node);\n' % name)
  visitcpp.write('}\n')
  

def print_visitor(node, ext):
  global visithpp

  name = ''  
  if ext:
    name = '%s_%s' % (node[1], ext)
  else:
    name = node[1]

 
  visithpp.write('    inline virtual void %s_v(%s* node) { default_v(node); }\n' % (name, name))  

def print_node(node, ext, typ):
  global nodecpp
  global nodehpp
  
  params = []
  width = 0
  class_name = ''
  if ext:
    class_name = '%s_%s' % (node[1], ext)
  else:
    class_name = node[1]
  
  for elemen in node:
    params.append(elemen.split('|'))
  del params[0]
  del params[0]
  
  for param in params:
    width = max(width, len(param[0]))
  
  # print the accept function
  nodecpp.write('void %s::accept(visitor* v) { v->%s_v(this); }\n' % (class_name, class_name))
  
  # print class start
  nodehpp.write('class %s : public %s {\n' % (class_name, typ))
  nodehpp.write('  public:\n')
  
  # print constructor
  nodehpp.write('    %s(' % class_name)
  for param in params:
    if param != params[0]:
      nodehpp.write(' ')
    nodehpp.write('%s* %s' % (param[0], param[1]))
    if param != params[-1]:
      nodehpp.write(',')
  nodehpp.write(') :\n')
  
  # print initializer list
  nodehpp.write('      %s("%s ")' % (typ, class_name))
  if len(params) != 0:
    nodehpp.write(', ')
  for param in params:
    nodehpp.write('_%s(%s)' % (param[1], param[1]))
    if param != params[-1]:
      nodehpp.write(',')
  nodehpp.write('{ }\n\n')
  
  nodehpp.write('    virtual ~%s();\n' % (class_name))
  nodehpp.write('    virtual void accept(visitor* v);\n')
  nodehpp.write('  \n')
  
  #print accessors
  for param in params:
    nodehpp.write('    inline %-*s  %s() const { return _%s; }\n' % (width, param[0] + '*' , param[1], param[1]))
    nodehpp.write('    inline %-*s %s()       { return _%s; }\n' % (width, param[0] + '*&', param[1], param[1]))
  nodehpp.write('  \n')
  nodehpp.write('  private:\n')
  
  #print instance variables
  for param in params:
    nodehpp.write('    %-*s* _%s;\n' % (width, param[0], param[1]))
  nodehpp.write('};\n\n')

def print_destruct(node, ext, typ):
  global nodecpp
  global nodehpp
  
  params = []
  name = ''
  if ext:
    name = '%s_%s' % (node[1], ext)
  else:
    name = node[1]
  
  for elemen in node:
    params.append(elemen.split('|'))
  del params[0]
  del params[0]
  
  nodecpp.write('%s::~%s() {\n' % (name, name))
  for param in params:
    if param[0].startswith('std::vector'):
      nodecpp.write('  for(auto iter = _%s->begin(); iter != _%s->end(); iter++)\n' % (param[1], param[1]))
      nodecpp.write('    delete *iter;\n')
    else:
      nodecpp.write('  delete _%s;\n' % param[1])
  nodecpp.write('}\n\n')

nodes = [line.split(' ') for line in open('nodes.in').read().split('\n')]
types = []

del nodes[-1]

for node in nodes:
  if node[0] == 'expr':
    types.append(['e', '_i_expr'])
  if node[0] == 'stmt':
    types.append(['s', '_i_stmt'])
  if node[0] == 'type':
    types.append(['t', '_i_type'])
  if node[0] == 'none':
    types.append([None, '__node'])

for node, ntype in map(None, nodes, types):
  print_node(node, ntype[0], ntype[1])
  print_visitor(node, ntype[0])

visithpp.write('};\n')
visithpp.write(depth_preamble)
nodecpp.write('\n\n')

for node, ntype in map(None, nodes, types):
  print_depth(node, ntype[0])
  print_destruct(node, ntype[0], ntype[1])

visithpp.write('};\n')
visithpp.write(reverse_preamble)

for node, ntype in map(None, nodes, types):
  print_reverse(node, ntype[0])

visithpp.write('};\n#endif\n\n')
nodehpp.write('\n#endif\n\n')


