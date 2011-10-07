/*
 * type_checker.hpp
 *
 *  Created on: Jun 14, 2011
 *      Author: norton
 */

#ifndef TYPE_CHECKER_HPP_INCLUDE
#define TYPE_CHECKER_HPP_INCLUDE

#include <visitor.hpp>
#include <node.hpp>
#include <symtable.hpp>

#include <exception>
#include <sstream>
#include <string>

class semantic_exception : public std::exception {
  public:
    semantic_exception(const std::string & msg, __node* n) :
      _msg(msg), _line(n->line()), _poss(n->poss()) { }
    virtual ~semantic_exception() throw() { }

    inline virtual const char* what() const throw() {
      std::ostringstream ostr;
      ostr << "[" << _line << "," << _poss << "] " << _msg;
      return ostr.str().c_str();
    }

  protected:
    std::string _msg;
    int _line;
    int _poss;
};

class type_checker : public depth_first_visitor {
  public:
    type_checker(symtable* table) :
      _table(table), _class("") { }
    virtual ~type_checker() { }

    virtual void    program_in(  program* node);
    virtual void   program_out(  program* node);
    virtual void   class_de_in( class_de* node);
    virtual void  class_de_out( class_de* node);
    virtual void  method_de_in(method_de* node);
    virtual void method_de_out(method_de* node);

    virtual void       and_out(        and_e* node);
    virtual void   array_e_out(      array_e* node);
    virtual void   array_t_out(      array_t* node);
    virtual void array_asn_out(  array_asn_s* node);
    virtual void       asn_out(        asn_s* node);
    virtual void  button_e_out(     button_e* node);
    virtual void      byte_out(       byte_t* node);
    virtual void    call_e_out(       call_e* node);
    virtual void    call_s_out(       call_s* node);
    virtual void   color_e_out(      color_e* node);
    virtual void     equal_out(      equal_e* node);
    virtual void     false_out(      false_e* node);
    virtual void        id_out(         id_e* node);
    virtual void        if_out(         if_s* node);
    virtual void     int_e_out(        int_e* node);
    virtual void    length_out(     length_e* node);
    virtual void        lt_out(         lt_e* node);
    virtual void     minus_out(      minus_e* node);
    virtual void       mul_out(        mul_e* node);
    virtual void       new_out(        new_e* node);
    virtual void new_array_out(  new_array_e* node);
    virtual void       not_out(        not_e* node);
    virtual void      plus_out(       plus_e* node);
    virtual void     while_out(      while_s* node);

    virtual void m_pixel_g_out(  m_pixel_g_e* node);
    virtual void m_pixel_s_out(    m_pixel_s* node);
    virtual void   m_delay_out(    m_delay_s* node);
    virtual void  m_button_out(   m_button_e* node);
    virtual void    m_tone_out(     m_tone_s* node);

  protected:
    symtable*   _table;
    std::string _class;

};


#endif /* TYPE_CHECKER_HPP_INCLUDE */
