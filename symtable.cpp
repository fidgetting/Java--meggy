/*
 * symtable.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: norton
 */

#include <symtable.hpp>

#define ptrs(ptr) (unsigned long)ptr

int symtable::label_gen = 0;

/* ************************************************************************** */
/* *** STE functions ******************************************************** */
/* ************************************************************************** */

type*  named_STE::var(const std::string& name) const {
  if(_vars.find(name) != _vars.end())
    return _vars.find(name)->second;
  return NULL;
}

type*& named_STE::var(const std::string& name) {
  return _vars[name];
}

int class_STE::dot(std::ostream& ostr) {
  int label = symtable::label_gen++;
  int target = 0;

  ostr << "  " << label << "[label=\"";
  ostr << " <f0> class_STE" <<
      " | <f1> mName = " << _name <<
      " | <f2> mSize = " << _size <<
      " | <f3> mScope \"];";

  target = _scope->dot(ostr);
  ostr << "  " << label << ":<f3> -> " << target << ":<f0>;";

  return label;
}

type*  method_STE::operator[](const std::string& name) const {
  if(_params.find(name) != _params.end())
    return _params.find(name)->second;
  return NULL;
}

type*& method_STE::operator[](const std::string& name) {
  return _params[name];
}

int method_STE::dot(std::ostream& ostr) {
  int label = symtable::label_gen++;
  int target = 0;

  ostr << "  " << label << "[label=\"";
  ostr << " <f0> method_STE" <<
      " | <f1> mName = " << _name <<
      " | <f2> mSigniture = ( ";

  for(auto iter = _params.begin(); iter != _params.end(); iter++) {
    ostr << *(iter->second) << " ";
  }

  ostr << ") returns " << *_return <<
      " | <f3> mSize = " << _size <<
      " | <f4> mScope \"];";

  target = _scope->dot(ostr);
  ostr << "  " << label << ":<f4> -> " << target << ":<f0>;";

  return label;
}

int var_STE::dot(std::ostream& ostr) {
  int label = symtable::label_gen++;

  ostr << "  " << label << "[label=\"";
  ostr << " <f0> var_STE" <<
      " | <f1> mName = " << _name <<
      " | <f2> mType = " << _type <<
      " | <f3> mBase = " << _base <<
      " | <f4> mOffset = " << _offset << "\"];";

  return label;
}

/* ************************************************************************** */
/* *** symbol table functions *********************************************** */
/* ************************************************************************** */

void symtable::insert_push(named_STE* new_ste) {
  this->insert(new_ste);
  this->push(new_ste->g_scope());
}

void symtable::dot(std::ostream& ostr) {
  ostr << "digraph SymTable {\n" <<
      "  graph [rankdir=\"LR\"];\n" <<
      "  node [shape=record];";
  _global->dot(ostr);
  ostr << "}";
}

/* ************************************************************************** */
/* *** visitor functions **************************************************** */
/* ************************************************************************** */

void sym_table_creator::program_in(program* node) {
  _table->push(_table->global());
}

void sym_table_creator::program_out(program* node) {
  if(_dup) {
    std::cerr << "ERROR: erros found while building symbol table" << std::endl;
    //exit(-1);
  }
  _table->pop();
}

void sym_table_creator::formal_v(formal* node) {
  _table->insert(
      new var_STE(
          node->name()->txt(),
          type::instance(node->type()),
          _base,
          _offset));
}

void sym_table_creator::method_de_v(method_de* node) {
  if(_table->current()->contains(node->name()->txt()) != NULL) {
    _dup = true;
    std::cerr << "[" << node->line() << "," << node->poss() << "]" <<
        " Redfined symbol " << node->name()->txt() << std::endl;
  }

  method_STE* ste = new method_STE(node, _table->current());
  _table->insert_push(ste);
  _offset = 3;
  _base = 'Y';

  for(auto iter = node->parm()->begin(); iter != node->parm()->end(); iter++) {
    if(ste->contains((*iter)->name()->txt())) {
      _dup = true;
      std::cerr << "[" << (*iter)->line() << "," << (*iter)->poss() << "]" <<
          " Redfined symbol " << node->name()->txt() << std::endl;
    }
    (*ste)[(*iter)->name()->txt()] = type::instance((*iter)->type());
    (*iter)->accept(this);
    _offset += (*ste)[(*iter)->name()->txt()]->size();
  }

  for(auto iter = node->vars()->begin(); iter != node->vars()->end(); iter++) {
    if(ste->contains((*iter)->name()->txt())) {
      _dup = true;
      std::cerr << "[" << (*iter)->line() << "," << (*iter)->poss() << "]" <<
          " Redfined symbol " << node->name()->txt() << std::endl;
    }
    (*ste)[(*iter)->name()->txt()] = type::instance((*iter)->type());
    (*iter)->accept(this);
    _offset += (*ste)[(*iter)->name()->txt()]->size();
  }

  //ste->size() = _offset - 1;
  _table->pop();
}

void sym_table_creator::class_de_v(class_de* node) {
  if(_names.find(node->name()->txt()) != _names.end()) {
    _dup = true;
    std::cerr << "[" << node->line() << "," << node->poss() << "]" <<
        " Redfined symbol " << node->name()->txt() << std::endl;
  }
  _names[node->name()->txt()] = node;

  class_STE* ste = new class_STE(node->name()->txt(), _table->current());
  _table->insert_push(ste);
  _offset = 0;
  _base = 'Z';
  type::create_class_type(ste->name());

  for(auto iter = node->vars()->begin(); iter != node->vars()->end(); iter++) {
    if(ste->has_v((*iter)->name()->txt())) {
      _dup = true;
      std::cerr << "[" << (*iter)->line() << "," << (*iter)->poss() << "]" <<
          " Redfined symbol " << node->name()->txt() << std::endl;
    }

    (*iter)->accept(this);
    _offset += type::instance((*iter)->type())->size();
  }

  ste->size() = _offset;

  for(auto iter = node->meth()->begin(); iter != node->meth()->end(); iter++) {
    (*iter)->accept(this);
  }

  _table->pop();
}

void sym_table_creator::var_de_v(var_de* node) {
  _table->insert(
      new var_STE(
          node->name()->txt(),
          type::instance(node->type()),
          _base,
          _offset));
}





