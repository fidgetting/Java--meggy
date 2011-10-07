/*
 * symtable.hpp
 *
 *  Created on: Jun 13, 2011
 *      Author: norton
 */

#ifndef SYMTABLE_HPP_INCLUDE
#define SYMTABLE_HPP_INCLUDE

#include <node.hpp>
#include <type.hpp>
#include <scope.hpp>
#include <visitor.hpp>

#include <map>
#include <string>
#include <vector>

class STE {
  public:

    STE(std::string& name) :
      _name(name) { }
    virtual ~STE() { }

    inline std::string  name() const { return _name; }
    inline std::string& name()       { return _name; }

    virtual int dot(std::ostream& ostr) = 0;

  protected:

    std::string _name;
};

class named_STE : public STE {
  public:
    named_STE(std::string name, scope* _scope) :
      STE(name), _size(0), _scope(_scope), _vars() { }
    virtual ~named_STE() { delete _scope; }

    inline scope* g_scope() const { return _scope; }
    inline int    size()      const { return _size;  }
    inline int&   size()            { return _size;  }

    type*  var(const std::string& name) const;
    type*& var(const std::string& name);

    inline bool has_v (const std::string& name) const
      { return _vars.find(name) != _vars.end(); }

  protected:
    int                          _size;
    scope*                       _scope;
    std::map<std::string, type*> _vars;
};

class class_STE : public named_STE {
  public:
    class_STE(std::string name, scope* _scope) :
      named_STE(name, new scope(_scope)) { }

    virtual int dot(std::ostream& ostr);
};

class method_STE : public named_STE {
  public:

    typedef std::map<std::string, type*> argsmap_t;
    typedef argsmap_t::              iterator               iterator;
    typedef argsmap_t::        const_iterator         const_iterator;
    typedef argsmap_t::      reverse_iterator       reverse_iterator;
    typedef argsmap_t::const_reverse_iterator const_reverse_iterator;

    method_STE(method_de* node, scope* enclosing) :
      named_STE(node->name()->txt(), new scope(enclosing)),
      _return(type::instance(node->type())), _params() { }
    virtual ~method_STE() { }

    inline type*  ret_t() const { return _return; }
    inline bool contains(const std::string & name) const
      { return _params.find(name) != _params.end(); }

    type*  operator[](const std::string& name) const;
    type*& operator[](const std::string& name);

    inline unsigned int  size() const { return _params.size(); }

    virtual int dot(std::ostream& ostr);

    inline               iterator begin()        { return _params.begin();  }
    inline               iterator end()          { return _params.end();    }
    inline         const_iterator begin() const  { return _params.begin();  }
    inline         const_iterator end()   const  { return _params.end();    }
    inline       reverse_iterator rbegin()       { return _params.rbegin(); }
    inline       reverse_iterator rend()         { return _params.rend();   }
    inline const_reverse_iterator rbegin() const { return _params.rbegin(); }
    inline const_reverse_iterator rend()   const { return _params.rend();   }

  protected:

    type*                        _return;
    std::map<std::string, type*> _params;

};

class var_STE : public STE {
  public:
    var_STE(std::string name, type* _type, char base, int offset) :
      STE(name), _type(_type), _base(base), _offset(offset) { }
    virtual ~var_STE() { }

    inline type* g_type()   const { return _type;   }
    inline char  g_base()   const { return _base;   }
    inline int   g_offset() const { return _offset; }

    virtual int dot(std::ostream& ostr);

  protected:

    type*  _type;
    char   _base;
    int    _offset;
};

class symtable {
  public:
    static int label_gen;

    symtable() :
      _global(new scope(NULL)), _stack(), _types() { }
    virtual ~symtable() { delete _global; }

    inline scope* current() const { return _stack.back(); }
    inline scope* global() { return _global; }
    inline void insert(STE* new_ste) { _stack.back()->insert(new_ste); }
    inline void push(scope* _scope) { _stack.push_back(_scope); }
    inline void pop() { _stack.pop_back(); }

    void insert_push(named_STE* new_ste);

    STE*   operator[](std::string name) { return _stack.back()->lookup(name); }
    type*& operator[](__node* node) { return _types[node]; }

    void dot(std::ostream& ostr);

  protected:
    scope*                   _global;
    std::vector<scope*>      _stack;
    std::map<__node*, type*> _types;
};

class sym_table_creator : public depth_first_visitor {
  public:
    sym_table_creator() :
      _table(new symtable()), _dup(false), _base('0'), _offset(0), _names() { }
    virtual ~sym_table_creator() { }

    inline symtable* table() const { return _table; }

    virtual void program_in (program* node);
    virtual void program_out(program* node);

    virtual void    formal_v(   formal* node);
    virtual void method_de_v(method_de* node);
    virtual void  class_de_v( class_de* node);
    virtual void    var_de_v(   var_de* node);

  protected:

    symtable*                      _table;
    bool                           _dup;
    char                           _base;
    int                            _offset;
    std::map<std::string, __node*> _names;

};

#endif /* SYMTABLE_HPP_ */





