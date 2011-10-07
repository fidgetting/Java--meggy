/*
 * scope.hpp
 *
 *  Created on: Jun 14, 2011
 *      Author: norton
 */

#ifndef SCOPE_HPP_INCLUDE
#define SCOPE_HPP_INCLUDE

#include <iostream>
#include <map>
#include <string>

class STE;

class scope {
  public:
    scope(scope* enclosing) :
      _enclosing(enclosing), _map() { }
    virtual ~scope();

    STE* lookup(const std::string& ste_name);
    STE* contains(const std::string& ste_name);

    void insert(STE* new_ste);

    int dot(std::ostream& ostr);

  protected:

    scope*                      _enclosing;
    std::map<std::string, STE*> _map;

};

#endif /* SCOPE_HPP_ */
