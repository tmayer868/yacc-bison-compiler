/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_INTERPRETER_TAB_H_INCLUDED
# define YY_YY_INTERPRETER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FLOAT = 258,
    SEPERATOR = 259,
    SHORTHAND = 260,
    NUM_F = 261,
    VARNAME = 262,
    ADD = 263,
    SUB = 264,
    MULT = 265,
    DIV = 266,
    EQUALS = 267,
    LPAREN = 268,
    RPAREN = 269,
    WS = 270,
    EOL = 271,
    GREATER = 272,
    LESS = 273,
    LOGEQUAL = 274,
    GEQUAL = 275,
    LEQUAL = 276,
    EOP = 277,
    BOL = 278,
    BOP = 279,
    WHILE = 280,
    IF = 281,
    END = 282,
    RCURLY = 283,
    LCURLY = 284,
    PRINT = 285,
    MAX = 286,
    ROOTS = 287,
    HIGH = 288,
    LOW = 289,
    POINTS = 290,
    EXP = 291,
    LOG = 292,
    POW = 293,
    SIN = 294,
    ASIN = 295,
    COS = 296,
    ACOS = 297,
    TAN = 298,
    ATAN = 299,
    RAISE = 300,
    DEF = 301,
    FUNCNAME = 302
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 64 "interpreter.y" /* yacc.c:1909  */

	double iVal;
	char* sVal;
	Dual *dVal;
	int lVal;//logical values
	node *nVal;


#line 111 "interpreter.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INTERPRETER_TAB_H_INCLUDED  */
