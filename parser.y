/* Reverse polish notation calculator */

%{
#include <iostream>
#include <vector>
#include <node.hpp>

extern int yylex();
extern __node* prog;

void yyerror(const char* msg) {
  std::cout << msg << std::endl;
}

%}

%union {
  token*                   token_t;
  program*                 program_t;
  main_c*                  main_t;

  std::vector<class_de*>*  claslist_t;
  class_de*                class_t;
  std::vector<formal*>*    formlist_t;
  formal*                  formal_t;
  std::vector<method_de*>* methlist_t;
  method_de*               method_t;
  std::vector<var_de*>*    varilist_t;
  var_de*                  vari_t;

  std::vector<_i_stmt*>*   stmtlist_t;
  std::vector<_i_expr*>*   exprlist_t;

  _i_expr*                 expr_t;
  _i_type*                 type_t;
  _i_stmt*                 stmt_t;
}

%token           IMPORT MEGGY PUBLIC STATIC MAIN STRING
%token           IF ELSE WHILE EXTENDS NEW RETURN
%token           SET_PIXEL GET_PIXEL DELAY CHECK_BUTTON TONE_START SET_AUX
%token           LPAREN RPAREN LBRACKET RBRACKET LCURLY RCURLY
%token           COMMA DOT SEMI PLUS MINUS MULT CAST BANG AND EQ LESS ASSIN
%token           INT BOOL BYTE CLASS COLOR BUTTON TONE VOID LENGTH
%token           TRUE FALSE ARRAY_DIM
%token <token_t> INT_LITERAL
%token <token_t> COLOR_LITERAL
%token <token_t> BUTTON_LITERAL
%token <token_t> TONE_LITERAL
%token <token_t> ID
%token <token_t> THIS

%type <program_t>    program
%type <main_t>       main

%type <claslist_t>   claslist
%type <class_t>      class
%type <formlist_t>   formlist
%type <formal_t>     formal
%type <methlist_t>   methlist
%type <method_t>     method
%type <varilist_t>   varilist
%type <vari_t>       vari

%type <exprlist_t>   exprlist
%type <stmtlist_t>   stmtlist
%type <type_t>       type_b

%type <expr_t>       expr
%type <type_t>       type
%type <stmt_t>       stmt

%right  ASSIN
%left   AND
%left   EQ
%left   LESS
%left   PLUS MINUS
%left   MULT
%right  CAST
%right  BANG
%left   DOT
%left   RPAREN LPAREN
%left   LBRACKET RBRACKET

%% /* Grammar rules and actions */

program:
    IMPORT MEGGY SEMI main claslist
    { prog = new program($4, $5); }
;

main:
    CLASS ID LCURLY PUBLIC STATIC VOID MAIN LPAREN STRING ARRAY_DIM ID RPAREN LCURLY stmt RCURLY RCURLY
    { $$ = new main_c($2, $11, $14); }
;

claslist:
    claslist class
    { $1->push_back($2); $$ = $1; }
  | /* epsilon */
    { $$ = new std::vector<class_de*>(); }
;

class:
    CLASS ID LCURLY varilist methlist RCURLY
    { $$ = new class_de($2, $4, $5); }
;

formlist:
    formlist COMMA formal
    { $1->push_back($3); $$ = $1; }
  | formal
    { $$ = new std::vector<formal*>(1, $1); }
;

formal:
    type ID
    { $$ = new formal($1, $2); }
;

methlist:
    methlist method
    { $1->push_back($2); $$ = $1; }
  | /* epsilon */
    { $$ = new std::vector<method_de*>(); }
;

method:
    PUBLIC type ID LPAREN formlist RPAREN LCURLY varilist stmtlist RETURN expr SEMI RCURLY
    { $$ = new method_de($2, $3, $5, $8, $9, $11); }
  | PUBLIC type ID LPAREN formlist RPAREN LCURLY varilist stmtlist RCURLY
    { $$ = new method_de($2, $3, $5, $8, $9, NULL); }
  | PUBLIC type ID LPAREN RPAREN LCURLY varilist stmtlist RETURN expr RCURLY
    { $$ = new method_de($2, $3, new std::vector<formal*>(), $7, $8, $10); }
  | PUBLIC type ID LPAREN RPAREN LCURLY varilist stmtlist RCURLY
    { $$ = new method_de($2, $3, new std::vector<formal*>(), $7, $8, NULL); }
;

type:
    type ARRAY_DIM
    { $$ = new array_t($1); }
  | type_b
    { $$ = $1;              }
;

type_b:
    BOOL
    { $$ = new bool_t();    }
  | BUTTON
    { $$ = new button_t();  }
  | BYTE
    { $$ = new byte_t();    }
  | INT
    { $$ = new int_t();     }
  | VOID
    { $$ = new void_t();    }
  | COLOR
    { $$ = new color_t();   }
  | TONE
    { $$ = new tone_t();    }
  | ID
    { $$ = new class_t($1); }
;

varilist:
    varilist vari
    { $1->push_back($2); $$ = $1; }
  | /* epsilon */
    { $$ = new std::vector<var_de*>(); }
;

vari:
    type ID SEMI
    { $$ = new var_de($1, $2); }
;

stmtlist:
    stmt stmtlist
    { $2->push_back($1); $$ = $2; }
  | /* epsilon */
    { $$ = new std::vector<_i_stmt*>(); }
;

stmt:
    LCURLY stmtlist RCURLY
    { $$ = new block_s($2); }
  | SET_PIXEL LPAREN expr COMMA expr COMMA expr RPAREN SEMI
    { $$ = new m_pixel_s($3, $5, $7); }
  | TONE_START LPAREN expr COMMA expr RPAREN SEMI
    { $$ = new m_tone_s($3, $5); }
  | SET_AUX LPAREN expr RPAREN SEMI
    { $$ = new m_aux_s($3); }
  | DELAY LPAREN expr RPAREN SEMI
    { $$ = new m_delay_s($3); }
  | expr DOT ID LPAREN exprlist RPAREN SEMI
    { $$ = new call_s($1, $3, $5); }
  | expr DOT ID LPAREN RPAREN SEMI
    { $$ = new call_s($1, $3, new std::vector<_i_expr*>()); }
  | IF LPAREN expr RPAREN stmt ELSE stmt
    { $$ = new if_s($3, $5, $7); }
  | WHILE LPAREN expr RPAREN stmt
    { $$ = new while_s($3, $5); }
  | ID ASSIN expr SEMI
    { $$ = new asn_s($1, $3); }
  | expr LBRACKET expr RBRACKET ASSIN expr SEMI
    { $$ = new array_asn_s($1, $3, $6); }
;

exprlist:
    exprlist COMMA expr
    { $1->push_back($3); $$ = $1; }
  | expr
    { $$ = new std::vector<_i_expr*>(1, $1); }
;

expr:
    expr AND expr
    { $$ = new and_e($1, $3); }
  | expr EQ expr
    { $$ = new equal_e($1, $3); } 
  | expr LESS expr
    { $$ = new lt_e($1, $3); }
  | expr PLUS expr
    { $$ = new plus_e($1, $3); }
  | expr MINUS expr
    { $$ = new minus_e($1, $3); }
  | expr MULT expr
    { $$ = new mul_e($1, $3); }
  | expr LBRACKET expr RBRACKET
    { $$ = new array_e($1, $3); }
  | expr DOT LENGTH
    { $$ = new length_e($1); }
  | expr DOT ID LPAREN exprlist RPAREN
    { $$ = new call_e($1, $3, $5); }
  | expr DOT ID LPAREN RPAREN
    { $$ = new call_e($1, $3, new std::vector<_i_expr*>()); }
  | GET_PIXEL LPAREN expr COMMA expr RPAREN
    { $$ = new m_pixel_g_e($3, $5); }
  | CHECK_BUTTON LPAREN expr RPAREN
    { $$ = new m_button_e($3); }
  | CAST expr
    { $$ = new cast_e(new byte_t(), $2); }
  | INT_LITERAL
    { $$ = new int_e($1); }
  | COLOR_LITERAL
    { $$ = new color_e($1); }
  | BUTTON_LITERAL
    { $$ = new button_e($1); }
  | TONE_LITERAL
    { $$ = new tone_e($1); }
  | TRUE
    { $$ = new true_e(); }
  | FALSE
    { $$ = new false_e(); }
  | ID
    { $$ = new id_e($1); }
  | THIS
    { $$ = new this_e(); }
  | NEW type_b LBRACKET expr RBRACKET
    { $$ = new new_array_e($2, $4); }
  | NEW ID LPAREN RPAREN
    { $$ = new new_e($2); }
  | BANG expr
    { $$ = new not_e($2); }
  | LPAREN expr RPAREN
    { $$ = $2; }
;

%%




