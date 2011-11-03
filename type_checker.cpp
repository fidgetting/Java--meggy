/*
 * type_checker.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: norton
 */

#include <type_checker.hpp>

#define seman(msg) throw semantic_exception(msg, node)
#define table (*_table)
#define dc(type, cast) dynamic_cast<type>(cast)

/* ************************************************************************** */
/* *** helper functions ***************************************************** */
/* ************************************************************************** */

void   type_checker::program_in(program* node) {
  table.push(table.global());
}

void  type_checker::program_out(program* node) {
  table.pop();
}

void  type_checker::class_de_in(class_de* node) {
  table.push(((named_STE*)table[node->name()->txt()])->g_scope());
}

void type_checker::class_de_out(class_de* node) {
  table.pop();
  _class = "";
}

void type_checker::method_de_in(method_de* node) {
  if(dc(method_STE*, table[node->name()->txt()]) != NULL)
      table.push(((method_STE*)table[node->name()->txt()])->g_scope());
  else seman("method not found in symbol table");

  table[node] = ((method_STE*)table[node->name()->txt()])->ret_t();
}

void type_checker::method_de_out(method_de* node) {
  table.pop();
}

/* ************************************************************************** */
/* *** worker functions ***************************************************** */
/* ************************************************************************** */

void type_checker::and_out(and_e* node) {
  if(table[node->l_expr()] != type::BOOL ||
      table[node->r_expr()] != type::BOOL)
    seman("invalid operands for && operator");
  table[node] = type::BOOL;
}

void type_checker::array_e_out(array_e* node) {
  if(table[node->idx()] != type::INT)
    seman("Index expression type for array must be INT");

  if(array_type* t = dc(array_type*, table[node->expr()]))
    table[node] = t->sub_type();
  else seman("array expression must be a type of array");
}

void type_checker::array_t_out(array_t* node) {
  // TODO
}

void type_checker::array_asn_out(array_asn_s* node) {
  if(table[node->idx()] != type::INT)
    seman("Index expression type for array reference must be INT");

  if(array_type* t = dc(array_type*, table[node->ref()])) {
    if(table[node->exp()] != t->sub_type())
      seman("Invalid expression type assigned into array");
  } else seman("array expression must be a type of array");
}

void type_checker::asn_out(asn_s* node) {
  if(!table[node->id()->txt()])
    seman(std::string("Undeclared variable ") + node->id()->txt());

  if(var_STE* v = dc(var_STE*, table[node->id()->txt()])) {
    if(v->g_type() != table[node->expr()])
      seman(std::string("Invalid expression type assigned to variable") +
          node->id()->txt());
  } else seman(node->id()->txt() + " must variable in assignment");
}

void type_checker::button_e_out(button_e* node) {
  table[node] = type::BUTTON;
}

void type_checker::byte_out(byte_t* node) {
  table[node] = type::BYTE;
}

void type_checker::call_e_out(call_e* node) {
  class_type* typ = dc(class_type*, table[node->expr()]);
  class_STE*  cste;
  method_STE* mste;

  if(typ && table[typ->name()] == NULL)
    seman(node->expr()->name() + " does not exist");
  if(!(cste = dc(class_STE*,  table[typ->name()])))
    seman("Receiver of method class must be a class type");

  if(cste->g_scope()->contains(node->id()->txt()) == NULL)
    seman(cste->name() + " node does not have method " + node->id()->txt());
  if(!(mste = dc(method_STE*, cste->g_scope()->contains(node->id()->txt()))))
    seman(cste->name() + " node does not have method " + node->id()->txt());
  if(mste->size() != node->args()->size())
    seman(cste->name() + "." + mste->name() + ": incorrect number of args");

  auto target = node->args()->rbegin();
  for(auto iter = mste->begin(); iter != mste->end(); iter++) {
    if(iter->second != table[*target])
      seman("Invalid argument type for method " + cste->name() + "." +
          mste->name());
    target++;
  }

  table[node] = mste->ret_t();
}

void type_checker::call_s_out(call_s* node) {
  class_type* typ = dc(class_type*, table[node->expr()]);
  class_STE*  cste;
  method_STE* mste;

  if(typ && table[typ->name()] == NULL)
    seman(node->expr()->name() + " does not exist");
  if(!(cste = dc(class_STE*,  table[typ->name()])))
    seman("Receiver of method class must be a class type");
  if(cste->g_scope()->contains(node->id()->txt()) == NULL)
    seman(cste->name() + " node does not have method " + node->id()->txt());
  if(!(mste = dc(method_STE*, cste->g_scope()->contains(node->id()->txt()))))
    seman(cste->name() + " node does not have method " + node->id()->txt());
  if(mste->size() != node->args()->size())
    seman(cste->name() + "." + mste->name() + ": incorrect number of args");

  auto target = node->args()->rbegin();
  for(auto iter = mste->begin(); iter != mste->end(); iter++) {
    if(iter->second != table[*target])
      seman("Invalid argument type for method " + cste->name() + "." +
          mste->name());
    target++;
  }

  table[node] = mste->ret_t();
}

void type_checker::color_e_out(color_e* node) {
  table[node] = type::COLOR;
}

void type_checker::equal_out(equal_e* node) {
  if(table[node->l_expr()] != table[node->r_expr()])
    seman("Operands to == operator must be of same type");
  table[node] = type::BOOL;
}

void type_checker::false_out(false_e* node) {
  table[node] = type::BOOL;
}

void type_checker::id_out(id_e* node) {
  if(dc(var_STE*, table[node->literal()->txt()]) == NULL)
    seman("Undeclared variable " + node->literal()->txt());
  table[node] = dc(var_STE*, table[node->literal()->txt()])->g_type();
}

void type_checker::if_out(if_s* node) {
  if(table[node->expr()] != type::BOOL)
    seman("Invalid condition type for if statement");
}

void type_checker::int_e_out(int_e* node) {
  table[node] = type::INT;
}

void type_checker::length_out(length_e* node) {
  if(!table[node->expr()]->is_array())
    seman("Operator length called on non-array type");
  table[node] = type::INT;
}

void type_checker::lt_out(lt_e* node) {
  if(table[node->l_expr()] != table[node->r_expr()])
    seman("Operands to < operator must be of same type");
  if(table[node->l_expr()] != type::BYTE && table[node->l_expr()] != type::INT)
    seman("Operands to < operator must be int or byte");
  table[node] = type::BOOL;
}

void type_checker::minus_out(minus_e* node) {
  if(table[node->l_expr()] != table[node->r_expr()])
    seman("Operands to - operator must be of same type");
  if(table[node->l_expr()] != type::INT || table[node->r_expr()] != type::BYTE)
    seman("Operands to - operator must be INT or BYTE");
  table[node] = type::INT;
}

void type_checker::mul_out(mul_e* node) {
  if(table[node->l_expr()] != type::BYTE || table[node->r_expr()] != type::BYTE)
    seman("Operands to * oeprator must be BYTE");
  table[node] = type::INT;
}

void type_checker::new_out(new_e* node) {
  if(dc(class_STE*, table[node->literal()->txt()]) == NULL)
    seman("Undeclared class type in new operator");
  table[node] = type::get_type(node->literal()->txt());
}

void type_checker::new_array_out(new_array_e* node) {
  if(table[node->expr()] != type::INT)
    seman("Index expression type for array reference must be INT");
  if(type::get_type(type::instance(node->type())) == NULL)
    type::create_array_type(type::instance(node->type()));
  table[node] = type::get_type(type::instance(node->type()));
}

void type_checker::not_out(not_e* node) {
  if(table[node->expr()] != type::BOOL)
    seman("Invalid operand type for operator !");
  table[node] = type::BOOL;
}

void type_checker::plus_out(plus_e* node) {
  if(table[node->l_expr()] != table[node->r_expr()])
    seman("Operands to + operator must be of same type");
  if(table[node->l_expr()] != type::INT || table[node->r_expr()] != type::BYTE)
    seman("Operands to + operator must be INT or BYTE");
  table[node] = type::INT;
}

void type_checker::while_out(while_s* node) {
  if(table[node->expr()] != type::BOOL)
    seman("Invalid condition type for while statement");
}

void type_checker::m_pixel_g_out(m_pixel_g_e* node) {
  if(table[node->x_expr()] != type::BYTE || table[node->y_expr()] != type::BYTE)
    seman("Invalid argument type for method MeggyGetPixel");
  table[node] = type::COLOR;
}

void type_checker::m_pixel_s_out(m_pixel_s* node) {
  if(table[node->x_expr()] != type::BYTE ||
     table[node->y_expr()] != type::BYTE ||
     table[node->color()]  != type::COLOR )
    seman("Invalid argument type for method MeggySetPixel");
}

void type_checker::m_delay_out(m_delay_s* node) {
  if(table[node->expr()] != type::INT)
    seman("Invalid argument type for method MeggyDelay");
}

void type_checker::m_button_out(m_button_e* node) {
  if(table[node->expr()] != type::BUTTON)
    seman("Invalid argument type for method MeggyCheckButton");
  table[node] = type::BOOL;
}

void type_checker::m_tone_out(m_tone_s* node) {
  if(table[node->dura()] != type::INT || table[node->tone()] != type::TONE)
    seman("Invalid argument type for method MeggyToneStart");
}

