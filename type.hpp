/*
 * type.hpp
 *
 *  Created on: Jun 13, 2011
 *      Author: norton
 */

#ifndef TYPE_HPP_INCLUDE
#define TYPE_HPP_INCLUDE

#include <node.hpp>

#include <map>
#include <string>
#include <iostream>

#define BOOL_S   1
#define BUTTON_S 1
#define BYTE_S   1
#define COLOR_S  1
#define INT_S    2
#define TONE_S   2
#define VOID_S   0
#define PTR_S    2

class type {
  protected:
    enum types {
      BOOL_t,
      BUTTON_t,
      BYTE_t,
      COLOR_t,
      INT_t,
      TONE_t,
      VOID_t,
      classType,
      arrayType,
    };

    static std::map<std::string, type*> class_types;
    static std::map<const type*, type*> array_types;

  public:
    static type* BOOL;
    static type* BUTTON;
    static type* BYTE;
    static type* COLOR;
    static type* INT;
    static type* TONE;
    static type* VOID;

    type(types type, int size) :
      _type(type), _size(size) { }
    virtual ~type() { }

    inline int         size()  const { return _size;  }

    static type* get_type(const std::string& name);
    static type* get_type(const type* t);
    static void create_class_type(const std::string& name);
    static void create_array_type(type* t);
    static type* instance(_i_type* t);
    static void clean();

    friend std::ostream& operator<<(std::ostream& ostr, type::types t);
    friend std::ostream& operator<<(std::ostream& ostr, const type& t);

  protected:

    types       _type;
    int         _size;
};

class class_type : public type {
  public:
    class_type(const std::string& name) :
      type(classType, PTR_S), _name(name) { }
    virtual ~class_type() { }

    inline std::string name() const { return _name; }

  protected:

    std::string _name;
};

class array_type : public type {
  public:
    array_type(type* sub) :
      type(type::arrayType, PTR_S), _sub(sub) { }
    virtual ~array_type() { }

    inline type* sub_type() const { return _sub; }
  protected:

    type* _sub;
};

std::ostream& operator<<(std::ostream& ostr, type::types t);
std::ostream& operator<<(std::ostream& ostr, const type& t);
std::ostream& operator<<(std::ostream& ostr, const class_type& t);
std::ostream& operator<<(std::ostream& ostr, const array_type& t);

#endif /* TYPE_HPP_ */
