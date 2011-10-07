/*
 * scope.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: norton
 */

#include <scope.hpp>
#include <symtable.hpp>

scope::~scope() {
  for(auto iter = _map.begin(); iter != _map.end(); iter++) {
    delete iter->second;
  }
}

STE* scope::lookup(const std::string& ste_name) {
  if(_map.find(ste_name) != _map.end())
    return _map[ste_name];
  if(_enclosing != NULL)
    return _enclosing->lookup(ste_name);
  return NULL;
}

STE* scope::contains(const std::string& ste_name) {
  if(_map.find(ste_name) != _map.end())
    return _map[ste_name];
  return NULL;
}

void scope::insert(STE* new_ste) {
  _map[new_ste->name()] = new_ste;
}

int scope::dot(std::ostream& ostr) {
  int label = symtable::label_gen++;
  int field_label = 1;
  int target = 0;

  ostr << " " << label << "[label =\" <f0> Scope";
  for(auto iter = _map.begin(); iter != _map.end(); iter++)
    ostr << "| <f" << field_label++ << "> mDict\\[" << iter->first << "\\] ";
  ostr << "\"];";

  field_label = 1;
  for(auto iter = _map.begin(); iter != _map.end(); iter++) {
    target = lookup(iter->first)->dot(ostr);
    ostr << " " << label << ":<f" << field_label++ << "> -> " << target << ":<f0>;";
  }

  return label;
}
