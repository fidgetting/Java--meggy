/*
 * type.cpp
 *
 *  Created on: Jun 13, 2011
 *      Author: norton
 */

#include <type.hpp>

#define dyn_test(type, var) if(dynamic_cast<type>(var) != NULL)

type* type::BOOL =   new type(type::BOOL_t,   BOOL_S  );
type* type::BUTTON = new type(type::BUTTON_t, BUTTON_S);
type* type::BYTE =   new type(type::BYTE_t,   BYTE_S  );
type* type::COLOR =  new type(type::COLOR_t,  COLOR_S );
type* type::INT =    new type(type::INT_t,    INT_S   );
type* type::TONE =   new type(type::TONE_t,   TONE_S  );
type* type::VOID =   new type(type::VOID_t,   VOID_S  );

std::map<std::string, type*> type::class_types;
std::map<const type*, type*> type::array_types;

type* type::get_type(const std::string& name) {
  if(class_types.find(name) != class_types.end())
    return class_types[name];
  return NULL;
}

type* type::get_type(const type* t) {
  if(array_types.find(t) != array_types.end())
    return array_types[t];
  return NULL;
}

void type::create_class_type(const std::string& name) {
  if(class_types.find(name) == class_types.end())
    class_types[name] = new class_type(name);
}

void type::create_array_type(type* t) {
  if(array_types.find(t) == array_types.end())
    array_types[t] = new array_type(t);
}

type* type::instance(_i_type* t) {
  dyn_test(bool_t*,   t)
    return BOOL;
  dyn_test(button_t*, t)
    return BUTTON;
  dyn_test(byte_t*,   t)
    return BYTE;
  dyn_test(color_t*,  t)
    return COLOR;
  dyn_test(int_t*,    t)
    return INT;
  dyn_test(tone_t*,   t)
    return TONE;
  dyn_test(void_t*,   t)
    return VOID;
  dyn_test(array_t*,  t) {
    type* i_t = instance(dynamic_cast<array_t*>(t)->type());
    if(get_type(i_t) == NULL)
      create_array_type(i_t);
    return get_type(i_t);
  }

  if(get_type(dynamic_cast<class_t*>(t)->name()->txt()) == NULL) {
    std::string i_t = dynamic_cast<class_t*>(t)->name()->txt();
    create_class_type(i_t);
    return get_type(i_t);
  }
  return get_type(dynamic_cast<class_t*>(t)->name()->txt());
}

void type::clean() {
  delete BOOL;
  delete BUTTON;
  delete BYTE;
  delete COLOR;
  delete INT;
  delete TONE;
  delete VOID;
  for(auto iter = class_types.begin(); iter != class_types.end(); iter++)
    delete iter->second;
  for(auto iter = array_types.begin(); iter != array_types.end(); iter++)
      delete iter->second;
}

std::ostream& operator<<(std::ostream& ostr, type::types t) {
  switch(t) {
    case type::BOOL_t:    ostr << "BOOL";   break;
    case type::BUTTON_t:  ostr << "BUTTON"; break;
    case type::BYTE_t:    ostr << "BYTE";   break;
    case type::COLOR_t:   ostr << "COLOR";  break;
    case type::INT_t:     ostr << "INT";    break;
    case type::TONE_t:    ostr << "TONE";   break;
    case type::VOID_t:    ostr << "VOID";   break;
    case type::classType:
    case type::arrayType:
    default: ostr << "ERROR"; break;
  }
  return ostr;
}

std::ostream& operator<<(std::ostream& ostr, const type& t) {
  return ostr << t._type;
}

std::ostream& operator<<(std::ostream& ostr, const class_type& t) {
  return ostr << t.name();
}

std::ostream& operator<<(std::ostream& ostr, const array_type& t) {
  return ostr << t.sub_type();
}

