/*
 * basic_visitor.hpp
 *
 *  Created on: Jun 7, 2011
 *      Author: norton
 */

#ifndef BASIC_VISITOR_HPP_INCLUDE
#define BASIC_VISITOR_HPP_INCLUDE

#include <visitor.hpp>

#include <iostream>

class line_visitor : public reverse_first_visitor {
  public:

    line_visitor() :
      line(0), poss(0) { }
    virtual ~line_visitor() { }

    virtual void default_in(__node* node);
    virtual void token_in(token* node);

  private:
    int line;
    int poss;
};

class dot_visitor : public depth_first_visitor {
  public:

    dot_visitor(std::ostream& ostr) :
      count(0), ostr(ostr), node_stack() { }
    virtual ~dot_visitor() { }

    virtual void default_in (__node* node);
    virtual void default_out(__node* node);

  private:

    int              count;
    std::ostream&    ostr;
    std::vector<int> node_stack;
};

#endif /* BASIC_VISITOR_HPP_INCLUDE */
