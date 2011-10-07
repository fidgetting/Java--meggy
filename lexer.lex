%{
  #include <iostream>
  #include <node.hpp>
  #include <parser.hpp>

  int yyline = 0;
  int yyposs = 0;

#ifdef DEBUG
#define yyposs_inc yyposs += strlen(yytext); std::cout << yytext;
#else
#define yyposs_inc yyposs += strlen(yytext);
#endif

%}

num [0-9]
id  [a-zA-Z_]+[a-zA-Z0-9_]*
eol (\n|\r|\r\n)
ws  ([ \t])+

nonstar [^*]
nonstarslash [^*/]

arra_decl [\[]{ws}*[\]]
c_comment "/*"{nonstar}*("*"({nonstarslash}{nonstar}*)?)*"*/"
c_com "//".*$

%%

"import"      { yyposs_inc; return IMPORT;  }
"meggy.Meggy" { yyposs_inc; return MEGGY;   }
"public"      { yyposs_inc; return PUBLIC;  } 
"static"      { yyposs_inc; return STATIC;  } 
"main"        { yyposs_inc; return MAIN;    } 
"String"      { yyposs_inc; return STRING;  } 
"(byte)"      { yyposs_inc; return CAST;    } 
"return"      { yyposs_inc; return RETURN;  } 
"if"          { yyposs_inc; return IF;      } 
"else"        { yyposs_inc; return ELSE;    }
"while"       { yyposs_inc; return WHILE;   }
"true"        { yyposs_inc; return TRUE;    }
"false"       { yyposs_inc; return FALSE;   }
"this"        { yyposs_inc; return THIS;    }
"new"         { yyposs_inc; return NEW;     }
"length"      { yyposs_inc; return LENGTH;  }

"boolean"      { yyposs_inc; return BOOL;   }
"byte"         { yyposs_inc; return BYTE;   }
"class"        { yyposs_inc; return CLASS;  }
"int"          { yyposs_inc; return INT;    }
"void"         { yyposs_inc; return VOID;   }
"Meggy.Button" { yyposs_inc; return BUTTON; }
"Meggy.Color"  { yyposs_inc; return COLOR;  }
"Meggy.Tone"   { yyposs_inc; return TONE;   }

{arra_decl} { yyposs_inc; return ARRAY_DIM; }
{num}+ { yyposs_inc; yylval.token_t = token::get(yytext, yyline, yyposs); return INT_LITERAL; }
{id}   { yyposs_inc; yylval.token_t = token::get(yytext, yyline, yyposs); return ID;          }

"(" { yyposs_inc; return LPAREN;   }
")" { yyposs_inc; return RPAREN;   }
"[" { yyposs_inc; return LBRACKET; }
"]" { yyposs_inc; return RBRACKET; }
"{" { yyposs_inc; return LCURLY;   }
"}" { yyposs_inc; return RCURLY;   }

","  { yyposs_inc; return COMMA; }
"."  { yyposs_inc; return DOT;   }
";"  { yyposs_inc; return SEMI;  }
"+"  { yyposs_inc; return PLUS;  }
"*"  { yyposs_inc; return MULT;  }
"-"  { yyposs_inc; return MINUS; }
"!"  { yyposs_inc; return BANG;  }
"<"  { yyposs_inc; return LESS;  }
"="  { yyposs_inc; return ASSIN; }
"&&" { yyposs_inc; return AND;   }
"==" { yyposs_inc; return EQ;    }

"Meggy.setPixel"    { yyposs_inc; return SET_PIXEL;    }
"Meggy.getPixel"    { yyposs_inc; return GET_PIXEL;    }
"Meggy.delay"       { yyposs_inc; return DELAY;        }
"Meggy.checkButton" { yyposs_inc; return CHECK_BUTTON; }
"Meggy.toneStart"   { yyposs_inc; return TONE_START;   }
"Meggy.setAuxLEDs"  { yyposs_inc; return SET_AUX;      }

"Meggy.Color.DARK"   { yyposs_inc; yylval.token_t = token::get("0 DARK", yyline, yyposs);   return COLOR_LITERAL; }
"Meggy.Color.RED"    { yyposs_inc; yylval.token_t = token::get("1 RED", yyline, yyposs);    return COLOR_LITERAL; }
"Meggy.Color.ORANGE" { yyposs_inc; yylval.token_t = token::get("2 ORANGE", yyline, yyposs); return COLOR_LITERAL; } 
"Meggy.Color.YELLOW" { yyposs_inc; yylval.token_t = token::get("3 YELLOW", yyline, yyposs); return COLOR_LITERAL; }
"Meggy.Color.GREEN"  { yyposs_inc; yylval.token_t = token::get("4 GREEN", yyline, yyposs);  return COLOR_LITERAL; }
"Meggy.Color.BLUE"   { yyposs_inc; yylval.token_t = token::get("5 BLUE", yyline, yyposs);   return COLOR_LITERAL; }
"Meggy.Color.VIOLET" { yyposs_inc; yylval.token_t = token::get("6 VIOLET", yyline, yyposs); return COLOR_LITERAL; }
"Meggy.Color.WHITE"  { yyposs_inc; yylval.token_t = token::get("7 WHITE", yyline, yyposs);  return COLOR_LITERAL; }

"Meggy.Button.A"     { yyposs_inc; yylval.token_t = token::get("Button_A", yyline, yyposs);     return BUTTON_LITERAL; }
"Meggy.Button.B"     { yyposs_inc; yylval.token_t = token::get("Button_B", yyline, yyposs);     return BUTTON_LITERAL; }
"Meggy.Button.Up"    { yyposs_inc; yylval.token_t = token::get("Button_Up", yyline, yyposs);    return BUTTON_LITERAL; }
"Meggy.Button.Down"  { yyposs_inc; yylval.token_t = token::get("Button_Down", yyline, yyposs);  return BUTTON_LITERAL; }
"Meggy.Button.Left"  { yyposs_inc; yylval.token_t = token::get("Button_Left", yyline, yyposs);  return BUTTON_LITERAL; }
"Meggy.Button.Right" { yyposs_inc; yylval.token_t = token::get("Button_Right", yyline, yyposs); return BUTTON_LITERAL; }

"Meggy.Tone.C3"  { yyposs_inc; yylval.token_t = token::get("61157 C3",  yyline, yyposs); return TONE_LITERAL; }
"Meggy.Tone.Cs3" { yyposs_inc; yylval.token_t = token::get("57724 Cs3", yyline, yyposs); return TONE_LITERAL; }
"Meggy.Tone.D3"  { yyposs_inc; yylval.token_t = token::get("54485 D3",  yyline, yyposs); return TONE_LITERAL; }
"Meggy.Tone.Ds3" { yyposs_inc; yylval.token_t = token::get("51427 Ds3", yyline, yyposs); return TONE_LITERAL; }
"Meggy.Tone.E3"  { yyposs_inc; yylval.token_t = token::get("48541 E3",  yyline, yyposs); return TONE_LITERAL; }
"Meggy.Tone.F3"  { yyposs_inc; yylval.token_t = token::get("45816 F3",  yyline, yyposs); return TONE_LITERAL; }
"Meggy.Tone.Fs3" { yyposs_inc; yylval.token_t = token::get("43243 Fs3", yyline, yyposs); return TONE_LITERAL; }
"Meggy.Tone.G3"  { yyposs_inc; yylval.token_t = token::get("40816 G3",  yyline, yyposs); return TONE_LITERAL; }
"Meggy.Tone.Gs3" { yyposs_inc; yylval.token_t = token::get("38526 Gs3", yyline, yyposs); return TONE_LITERAL; }
"Meggy.Tone.A3"  { yyposs_inc; yylval.token_t = token::get("36363 A3",  yyline, yyposs); return TONE_LITERAL; }
"Meggy.Tone.As3" { yyposs_inc; yylval.token_t = token::get("34323 As3", yyline, yyposs); return TONE_LITERAL; }

{c_comment} { yyposs_inc; }
{c_com}     { yyposs_inc; }
{ws}        { yyposs_inc; }
{eol}       {
#ifdef DEBUG
  std::cout << std::endl;
#endif 
  yyposs = 0; yyline++;
}
. { std::cout << "Illegal character: " << yytext << std::endl; }

%%

int yywrap(void) {
  return 1;
}

