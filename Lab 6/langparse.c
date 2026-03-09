/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "lang.y"

//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
//

#include <iostream>
#include "lex.h"
#include "astnodes.h"
#include "cSymbolTable.h"

// Global flag to track semantic errors
static bool g_semanticErrorHappened = false;

// Macros for error handling
#define CHECK_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; } }
#define PROP_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; YYERROR; } }


#line 96 "langparse.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "langparse.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_TYPE_ID = 4,                    /* TYPE_ID  */
  YYSYMBOL_CHAR_VAL = 5,                   /* CHAR_VAL  */
  YYSYMBOL_INT_VAL = 6,                    /* INT_VAL  */
  YYSYMBOL_FLOAT_VAL = 7,                  /* FLOAT_VAL  */
  YYSYMBOL_LE = 8,                         /* LE  */
  YYSYMBOL_GE = 9,                         /* GE  */
  YYSYMBOL_AND = 10,                       /* AND  */
  YYSYMBOL_OR = 11,                        /* OR  */
  YYSYMBOL_EQUALS = 12,                    /* EQUALS  */
  YYSYMBOL_NOT_EQUALS = 13,                /* NOT_EQUALS  */
  YYSYMBOL_STRING_LIT = 14,                /* STRING_LIT  */
  YYSYMBOL_PROGRAM = 15,                   /* PROGRAM  */
  YYSYMBOL_PRINT = 16,                     /* PRINT  */
  YYSYMBOL_PRINTS = 17,                    /* PRINTS  */
  YYSYMBOL_WHILE = 18,                     /* WHILE  */
  YYSYMBOL_IF = 19,                        /* IF  */
  YYSYMBOL_ELSE = 20,                      /* ELSE  */
  YYSYMBOL_ENDIF = 21,                     /* ENDIF  */
  YYSYMBOL_STRUCT = 22,                    /* STRUCT  */
  YYSYMBOL_ARRAY = 23,                     /* ARRAY  */
  YYSYMBOL_RETURN = 24,                    /* RETURN  */
  YYSYMBOL_JUNK_TOKEN = 25,                /* JUNK_TOKEN  */
  YYSYMBOL_26_ = 26,                       /* '{'  */
  YYSYMBOL_27_ = 27,                       /* '}'  */
  YYSYMBOL_28_ = 28,                       /* ';'  */
  YYSYMBOL_29_ = 29,                       /* '['  */
  YYSYMBOL_30_ = 30,                       /* ']'  */
  YYSYMBOL_31_ = 31,                       /* ')'  */
  YYSYMBOL_32_ = 32,                       /* '('  */
  YYSYMBOL_33_ = 33,                       /* ','  */
  YYSYMBOL_34_ = 34,                       /* '='  */
  YYSYMBOL_35_ = 35,                       /* '.'  */
  YYSYMBOL_36_ = 36,                       /* '>'  */
  YYSYMBOL_37_ = 37,                       /* '<'  */
  YYSYMBOL_38_ = 38,                       /* '+'  */
  YYSYMBOL_39_ = 39,                       /* '-'  */
  YYSYMBOL_40_ = 40,                       /* '*'  */
  YYSYMBOL_41_ = 41,                       /* '/'  */
  YYSYMBOL_42_ = 42,                       /* '%'  */
  YYSYMBOL_YYACCEPT = 43,                  /* $accept  */
  YYSYMBOL_program = 44,                   /* program  */
  YYSYMBOL_block = 45,                     /* block  */
  YYSYMBOL_open = 46,                      /* open  */
  YYSYMBOL_close = 47,                     /* close  */
  YYSYMBOL_decls = 48,                     /* decls  */
  YYSYMBOL_decl = 49,                      /* decl  */
  YYSYMBOL_var_decl = 50,                  /* var_decl  */
  YYSYMBOL_struct_decl = 51,               /* struct_decl  */
  YYSYMBOL_array_decl = 52,                /* array_decl  */
  YYSYMBOL_func_decl = 53,                 /* func_decl  */
  YYSYMBOL_func_prefix = 54,               /* func_prefix  */
  YYSYMBOL_paramsspec = 55,                /* paramsspec  */
  YYSYMBOL_paramspec = 56,                 /* paramspec  */
  YYSYMBOL_stmts = 57,                     /* stmts  */
  YYSYMBOL_stmt = 58,                      /* stmt  */
  YYSYMBOL_func_call = 59,                 /* func_call  */
  YYSYMBOL_varref = 60,                    /* varref  */
  YYSYMBOL_varpart = 61,                   /* varpart  */
  YYSYMBOL_lval = 62,                      /* lval  */
  YYSYMBOL_params = 63,                    /* params  */
  YYSYMBOL_param = 64,                     /* param  */
  YYSYMBOL_expr = 65,                      /* expr  */
  YYSYMBOL_logic_and = 66,                 /* logic_and  */
  YYSYMBOL_equality = 67,                  /* equality  */
  YYSYMBOL_relational = 68,                /* relational  */
  YYSYMBOL_addit = 69,                     /* addit  */
  YYSYMBOL_term = 70,                      /* term  */
  YYSYMBOL_unary = 71,                     /* unary  */
  YYSYMBOL_fact = 72                       /* fact  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;


/* Second part of user prologue.  */
#line 46 "lang.y"

    int yyerror(const char *msg);

    cAstNode *yyast_root;
    cSymbol *g_current_func_sym = nullptr;
    cSymbol *g_current_func_display_sym = nullptr;
    cSymbol *g_current_ret_sym = nullptr;
    int g_current_func_line = 0;

#line 212 "langparse.c"


#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   263

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  76
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  171

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   280


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    42,     2,     2,
      32,    31,    40,    38,    33,    39,    35,    41,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    28,
      37,    34,    36,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    29,     2,    30,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    26,     2,    27,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   110,   110,   116,   118,   121,   124,   127,   129,   131,
     133,   135,   137,   139,   142,   160,   180,   195,   211,   232,
     254,   277,   305,   333,   362,   391,   445,   447,   450,   453,
     455,   458,   460,   462,   464,   466,   468,   470,   472,   474,
     476,   479,   487,   495,   497,   499,   502,   515,   518,   520,
     523,   526,   528,   531,   533,   536,   538,   540,   543,   545,
     547,   549,   551,   554,   556,   558,   561,   563,   565,   567,
     570,   572,   575,   577,   579,   581,   583
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "TYPE_ID", "CHAR_VAL", "INT_VAL", "FLOAT_VAL", "LE", "GE", "AND", "OR",
  "EQUALS", "NOT_EQUALS", "STRING_LIT", "PROGRAM", "PRINT", "PRINTS",
  "WHILE", "IF", "ELSE", "ENDIF", "STRUCT", "ARRAY", "RETURN",
  "JUNK_TOKEN", "'{'", "'}'", "';'", "'['", "']'", "')'", "'('", "','",
  "'='", "'.'", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "'%'", "$accept",
  "program", "block", "open", "close", "decls", "decl", "var_decl",
  "struct_decl", "array_decl", "func_decl", "func_prefix", "paramsspec",
  "paramspec", "stmts", "stmt", "func_call", "varref", "varpart", "lval",
  "params", "param", "expr", "logic_and", "equality", "relational",
  "addit", "term", "unary", "fact", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-57)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-41)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -6,    -3,    19,   -57,   -57,   193,   -57,    24,     7,    59,
      38,    74,    82,    97,    -3,    12,     5,   -57,   193,   -57,
      61,    70,    73,   -57,    18,   166,   -57,   104,    53,   -57,
     103,   112,   -57,    35,   116,     5,   130,     5,     5,    28,
     120,   127,   125,   -57,   -57,     5,     5,   -57,    53,    64,
     150,    92,     9,    83,    77,   -57,   -57,   -57,   166,   -57,
     -57,   -57,   165,   167,    58,   -57,    -7,   -57,   143,   -57,
     -57,   -57,   -57,     5,   169,     5,   -57,    60,   -57,   163,
     -57,    17,   144,    23,    45,   148,    56,   172,   181,    50,
     -57,     5,   -57,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,   -57,   -57,   -57,   193,    69,
     121,   -57,    57,   -57,   -57,    66,   -57,     5,   161,   170,
     237,   237,   -57,   188,   171,   173,   -57,   150,    92,     9,
       9,    83,    83,    83,    83,    77,    77,   -57,   -57,   -57,
     193,   166,   -57,   193,   -57,   -57,   -57,   -57,   -57,   -57,
     -57,   204,   -57,   192,   196,   166,   -57,   193,   166,   237,
     174,   -57,   -57,   -57,   166,   -57,   226,   -57,   -57,   176,
     -57
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     5,     2,     0,     1,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,    38,     0,     8,
       0,     0,     0,    12,     0,     0,    30,     0,    47,    45,
       0,    13,    15,     0,    14,     0,     0,     0,     0,     0,
       0,     0,    46,    73,    74,     0,     0,    76,    75,     0,
      52,    54,    57,    62,    65,    69,    71,     7,     0,     9,
      11,    10,     0,     0,     0,    28,     0,    27,     0,     6,
       4,    29,    37,     0,     0,     0,    42,     0,    49,    50,
      25,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      70,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     3,    14,    19,     0,     0,
       0,    40,     0,    46,    43,     0,    41,     0,     0,     0,
       0,     0,    13,     0,     0,     0,    72,    51,    53,    55,
      56,    61,    60,    58,    59,    63,    64,    66,    67,    68,
       0,     0,    20,     0,    26,    44,    36,    48,    34,    35,
      33,     0,    16,     0,     0,     0,    21,     0,     0,     0,
       0,    18,    17,    22,     0,    24,     0,    31,    23,     0,
      32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -57,   -57,   199,    -9,   -56,   -36,   -17,   -20,   -57,   -57,
     -57,   -57,   -57,    96,   -12,     0,    -5,    22,   134,   -57,
     -57,   101,    -2,   122,   133,    32,    14,    33,   -39,   -57
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,    17,     5,    70,    18,    19,    20,    21,    22,
      23,    24,    66,    67,    25,    26,    47,    48,    29,    30,
      77,    78,    79,    50,    51,    52,    53,    54,    55,    56
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      27,    57,   105,    86,    65,    39,    58,    90,    42,     1,
      32,    43,    44,    27,    49,    40,    41,    96,    97,     6,
      27,    62,    63,     3,   109,    71,   110,    28,    91,    85,
     123,    62,     9,    81,    91,    83,    84,    45,    42,    33,
      28,    43,    44,    89,    46,    98,    99,    28,   118,    64,
      14,    15,    31,    27,   120,   108,    91,    85,    71,    62,
       9,    91,    34,   137,   138,   139,    76,    45,    91,    57,
      35,   112,   140,   115,    46,    91,   121,    91,    14,    15,
      28,   126,    73,    69,     3,   156,   107,   145,    74,    59,
      65,   116,    92,   117,   146,     3,   141,   142,    60,   163,
     143,    61,   165,    27,    94,    95,    36,   157,   168,   151,
     131,   132,   133,   134,    37,    27,    27,   102,   103,   104,
     150,   100,   101,    57,    62,    63,   129,   130,   155,    38,
      28,   158,    72,   135,   136,    27,    27,    75,    27,   -40,
      57,    71,    28,    28,    82,   164,    27,   166,    80,    87,
      27,    71,    27,    27,    27,    71,    88,    33,    71,    27,
      93,    27,    28,    28,    71,    28,    71,    68,    32,    42,
     106,   111,   113,    28,    91,   119,   122,    28,   124,    28,
      28,    28,    10,    11,    12,    13,    28,   125,    28,   148,
      16,   152,     3,    69,     7,   161,     8,     9,   149,   162,
       4,   153,   167,   154,   170,    68,   144,    42,   114,    10,
      11,    12,    13,   127,     0,    14,    15,    16,   147,     3,
      10,    11,    12,    13,   159,   160,   128,    68,    16,    42,
       3,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      42,     0,    10,    11,    12,    13,     0,   169,     0,     0,
      16,     0,     3,    10,    11,    12,    13,     0,     0,     0,
       0,    16,     0,     3
};

static const yytype_int16 yycheck[] =
{
       5,    18,    58,    39,    24,    14,    18,    46,     3,    15,
       3,     6,     7,    18,    16,     3,     4,     8,     9,     0,
      25,     3,     4,    26,    31,    25,    33,     5,    11,     1,
      86,     3,     4,    35,    11,    37,    38,    32,     3,    32,
      18,     6,     7,    45,    39,    36,    37,    25,    31,    31,
      22,    23,    28,    58,    31,    64,    11,     1,    58,     3,
       4,    11,     3,   102,   103,   104,    31,    32,    11,    86,
      32,    73,   108,    75,    39,    11,    31,    11,    22,    23,
      58,    31,    29,    27,    26,   141,    28,    30,    35,    28,
     110,    31,    28,    33,    28,    26,   108,    28,    28,   155,
     109,    28,   158,   108,    12,    13,    32,   143,   164,   121,
      96,    97,    98,    99,    32,   120,   121,    40,    41,    42,
     120,    38,    39,   140,     3,     4,    94,    95,   140,    32,
     108,   143,    28,   100,   101,   140,   141,    34,   143,    27,
     157,   141,   120,   121,    14,   157,   151,   159,    32,    29,
     155,   151,   157,   158,   159,   155,    29,    32,   158,   164,
      10,   166,   140,   141,   164,   143,   166,     1,     3,     3,
       3,    28,     3,   151,    11,    31,    28,   155,     6,   157,
     158,   159,    16,    17,    18,    19,   164,     6,   166,    28,
      24,     3,    26,    27,     1,     3,     3,     4,    28,     3,
       1,    30,    28,    30,    28,     1,   110,     3,    74,    16,
      17,    18,    19,    91,    -1,    22,    23,    24,   117,    26,
      16,    17,    18,    19,    20,    21,    93,     1,    24,     3,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,    -1,    16,    17,    18,    19,    -1,    21,    -1,    -1,
      24,    -1,    26,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    24,    -1,    26
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    15,    44,    26,    45,    46,     0,     1,     3,     4,
      16,    17,    18,    19,    22,    23,    24,    45,    48,    49,
      50,    51,    52,    53,    54,    57,    58,    59,    60,    61,
      62,    28,     3,    32,     3,    32,    32,    32,    32,    46,
       3,     4,     3,     6,     7,    32,    39,    59,    60,    65,
      66,    67,    68,    69,    70,    71,    72,    49,    57,    28,
      28,    28,     3,     4,    31,    50,    55,    56,     1,    27,
      47,    58,    28,    29,    35,    34,    31,    63,    64,    65,
      32,    65,    14,    65,    65,     1,    48,    29,    29,    65,
      71,    11,    28,    10,    12,    13,     8,     9,    36,    37,
      38,    39,    40,    41,    42,    47,     3,    28,    46,    31,
      33,    28,    65,     3,    61,    65,    31,    33,    31,    31,
      31,    31,    28,    47,     6,     6,    31,    66,    67,    68,
      68,    69,    69,    69,    69,    70,    70,    71,    71,    71,
      48,    57,    28,    46,    56,    30,    28,    64,    28,    28,
      58,    57,     3,    30,    30,    57,    47,    48,    57,    20,
      21,     3,     3,    47,    57,    47,    57,    28,    47,    21,
      28
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    43,    44,    45,    45,    46,    47,    48,    48,    49,
      49,    49,    49,    49,    50,    50,    51,    52,    52,    53,
      53,    53,    53,    53,    53,    54,    55,    55,    56,    57,
      57,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    59,    59,    60,    60,    60,    61,    62,    63,    63,
      64,    65,    65,    66,    66,    67,    67,    67,    68,    68,
      68,    68,    68,    69,    69,    69,    70,    70,    70,    70,
      71,    71,    72,    72,    72,    72,    72
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     4,     3,     1,     1,     2,     1,     2,
       2,     2,     1,     2,     2,     2,     5,     6,     6,     3,
       4,     5,     6,     7,     6,     3,     3,     1,     1,     2,
       1,     7,     9,     5,     5,     5,     4,     2,     1,     3,
       2,     4,     3,     3,     4,     1,     1,     1,     3,     1,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       2,     1,     3,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: PROGRAM block  */
#line 111 "lang.y"
                                { 
                                    (yyval.program_node) = new cProgramNode((yyvsp[0].block_node));
                                    yyast_root = (yyval.program_node);
                                    YYACCEPT;
                                }
#line 1413 "langparse.c"
    break;

  case 3: /* block: open decls stmts close  */
#line 117 "lang.y"
                                { (yyval.block_node) = new cBlockNode(static_cast<cDeclsNode*>((yyvsp[-2].ast_node)), (yyvsp[-1].stmts_node)); }
#line 1419 "langparse.c"
    break;

  case 4: /* block: open stmts close  */
#line 119 "lang.y"
                                { (yyval.block_node) = new cBlockNode(nullptr, (yyvsp[-1].stmts_node)); }
#line 1425 "langparse.c"
    break;

  case 5: /* open: '{'  */
#line 122 "lang.y"
                                { g_symbolTable.IncreaseScope(); }
#line 1431 "langparse.c"
    break;

  case 6: /* close: '}'  */
#line 125 "lang.y"
                                { g_symbolTable.DecreaseScope(); }
#line 1437 "langparse.c"
    break;

  case 7: /* decls: decls decl  */
#line 128 "lang.y"
                                { static_cast<cDeclsNode*>((yyvsp[-1].ast_node))->Insert(static_cast<cDeclNode*>((yyvsp[0].ast_node))); (yyval.ast_node) = (yyvsp[-1].ast_node); }
#line 1443 "langparse.c"
    break;

  case 8: /* decls: decl  */
#line 130 "lang.y"
                                { (yyval.ast_node) = new cDeclsNode(static_cast<cDeclNode*>((yyvsp[0].ast_node))); }
#line 1449 "langparse.c"
    break;

  case 9: /* decl: var_decl ';'  */
#line 132 "lang.y"
                                { (yyval.ast_node) = (yyvsp[-1].ast_node); }
#line 1455 "langparse.c"
    break;

  case 10: /* decl: array_decl ';'  */
#line 134 "lang.y"
                            { (yyval.ast_node) = (yyvsp[-1].ast_node); }
#line 1461 "langparse.c"
    break;

  case 11: /* decl: struct_decl ';'  */
#line 136 "lang.y"
                            { (yyval.ast_node) = (yyvsp[-1].ast_node); }
#line 1467 "langparse.c"
    break;

  case 12: /* decl: func_decl  */
#line 138 "lang.y"
                            { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 1473 "langparse.c"
    break;

  case 13: /* decl: error ';'  */
#line 140 "lang.y"
                            {  }
#line 1479 "langparse.c"
    break;

  case 14: /* var_decl: TYPE_ID IDENTIFIER  */
#line 143 "lang.y"
                                    { 
                                        // Declaration with a TYPE_ID token (built-in or previously
                                        // created type symbol). $1 is a cSymbol* for the type.
                                        cSymbol *typesym = (yyvsp[-1].symbol);
                                        
                                        // Check for duplicate declaration
                                        if (g_symbolTable.FindLocal(*(yyvsp[0].str_val)) != nullptr)
                                        {
                                            SemanticParseError("Symbol " + *(yyvsp[0].str_val) + " already defined in current scope");
                                        }
                                        
                                        cSymbol *declsym = new cSymbol(*(yyvsp[0].str_val));
                                        g_symbolTable.Insert(declsym);
                                        (yyval.ast_node) = new cVarDeclNode(typesym, declsym);
                                        delete (yyvsp[0].str_val);
                                        CHECK_ERROR();
                                    }
#line 1501 "langparse.c"
    break;

  case 15: /* var_decl: IDENTIFIER IDENTIFIER  */
#line 161 "lang.y"
                                    {
                                        // Declaration where the type is given by an identifier
                                        // (e.g., a previously declared struct type).
                                        cSymbol *typesym = g_symbolTable.Find(*(yyvsp[-1].str_val));
                                        if (typesym == nullptr) typesym = new cSymbol(*(yyvsp[-1].str_val));
                                        
                                        // Check for duplicate declaration
                                        if (g_symbolTable.FindLocal(*(yyvsp[0].str_val)) != nullptr)
                                        {
                                            SemanticParseError("Symbol " + *(yyvsp[0].str_val) + " already defined in current scope");
                                        }
                                        
                                        cSymbol *declsym = new cSymbol(*(yyvsp[0].str_val));
                                        g_symbolTable.Insert(declsym);
                                        (yyval.ast_node) = new cVarDeclNode(typesym, declsym);
                                        delete (yyvsp[-1].str_val);
                                        delete (yyvsp[0].str_val);
                                        CHECK_ERROR();
                                    }
#line 1525 "langparse.c"
    break;

  case 16: /* struct_decl: STRUCT open decls close IDENTIFIER  */
#line 181 "lang.y"
                                { 
                                    // Check for duplicate declaration
                                    if (g_symbolTable.FindLocal(*(yyvsp[0].str_val)) != nullptr)
                                    {
                                        SemanticParseError("Symbol " + *(yyvsp[0].str_val) + " already defined in current scope");
                                    }
                                    
                                    // Create a symbol for the struct type name and insert
                                    cSymbol *typesym = new cSymbol(*(yyvsp[0].str_val));
                                    g_symbolTable.Insert(typesym);
                                    (yyval.ast_node) = new cStructDeclNode(static_cast<cDeclsNode*>((yyvsp[-2].ast_node)), typesym);
                                    delete (yyvsp[0].str_val);
                                    CHECK_ERROR();
                                }
#line 1544 "langparse.c"
    break;

  case 17: /* array_decl: ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER  */
#line 196 "lang.y"
                                { 
                                    cSymbol *typesym = (yyvsp[-4].symbol);
                                    
                                    // Check for duplicate declaration
                                    if (g_symbolTable.FindLocal(*(yyvsp[0].str_val)) != nullptr)
                                    {
                                        SemanticParseError("Symbol " + *(yyvsp[0].str_val) + " already defined in current scope");
                                    }
                                    
                                    cSymbol *namesym = new cSymbol(*(yyvsp[0].str_val));
                                    (yyval.ast_node) = new cArrayDeclNode((yyvsp[-2].int_val), typesym, namesym);
                                    g_symbolTable.Insert(namesym);
                                    delete (yyvsp[0].str_val);
                                    CHECK_ERROR();
                                }
#line 1564 "langparse.c"
    break;

  case 18: /* array_decl: ARRAY IDENTIFIER '[' INT_VAL ']' IDENTIFIER  */
#line 212 "lang.y"
                                {
                                    // array of a user-defined type
                                    cSymbol *typesym = g_symbolTable.Find(*(yyvsp[-4].str_val));
                                    if (typesym == nullptr) typesym = new cSymbol(*(yyvsp[-4].str_val));
                                    
                                    // Check for duplicate declaration
                                    if (g_symbolTable.FindLocal(*(yyvsp[0].str_val)) != nullptr)
                                    {
                                        SemanticParseError("Symbol " + *(yyvsp[0].str_val) + " already defined in current scope");
                                    }
                                    
                                    cSymbol *namesym = new cSymbol(*(yyvsp[0].str_val));
                                    (yyval.ast_node) = new cArrayDeclNode((yyvsp[-2].int_val), typesym, namesym);
                                    g_symbolTable.Insert(namesym);
                                    delete (yyvsp[-4].str_val);
                                    delete (yyvsp[0].str_val);
                                    CHECK_ERROR();
                                }
#line 1587 "langparse.c"
    break;

  case 19: /* func_decl: func_prefix ')' ';'  */
#line 233 "lang.y"
                                {
                                    // prototype with no args
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cStmtsNode *knownBody = nullptr;
                                    cFuncDeclNode *knownDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (knownDecl != nullptr && knownDecl->IsDefined()) knownBody = knownDecl->GetBody();
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    cFuncDeclNode *funcDecl = new cFuncDeclNode(rettype, funcsym, nullptr);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    (yyval.ast_node) = new cFuncNode(rettype, displaySym, nullptr, nullptr, knownBody);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
#line 1613 "langparse.c"
    break;

  case 20: /* func_decl: func_prefix paramsspec ')' ';'  */
#line 255 "lang.y"
                                {
                                    // prototype with args
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cArgsNode *args = new cArgsNode(static_cast<cDeclsNode*>((yyvsp[-2].ast_node)));
                                    cStmtsNode *knownBody = nullptr;
                                    cFuncDeclNode *knownDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (knownDecl != nullptr && knownDecl->IsDefined()) knownBody = knownDecl->GetBody();
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    cFuncDeclNode *funcDecl = new cFuncDeclNode(rettype, funcsym, args);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    (yyval.ast_node) = new cFuncNode(rettype, displaySym, args, nullptr, knownBody);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
#line 1640 "langparse.c"
    break;

  case 21: /* func_decl: func_prefix ')' open stmts close  */
#line 278 "lang.y"
                                {
                                    // definition with no args and no decls
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cFuncDeclNode *prevDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    bool alreadyDefined = (prevDecl != nullptr && prevDecl->IsDefined());
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    cFuncDeclNode *funcDecl = new cFuncDeclNode(rettype, funcsym, nullptr, true);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    cStmtsNode *body = static_cast<cStmtsNode*>((yyvsp[-1].stmts_node));
                                    if (prevDecl != nullptr) prevDecl->SetBody(body);
                                    else funcDecl->SetBody(body);
                                    if (alreadyDefined)
                                    {
                                        SemanticParseError(funcsym->GetName() + " already has a definition");
                                    }
                                    (yyval.ast_node) = new cFuncNode(rettype, displaySym, nullptr, nullptr, body);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
#line 1672 "langparse.c"
    break;

  case 22: /* func_decl: func_prefix ')' open decls stmts close  */
#line 306 "lang.y"
                                {
                                    // definition with no args
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cFuncDeclNode *prevDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    bool alreadyDefined = (prevDecl != nullptr && prevDecl->IsDefined());
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    cFuncDeclNode *funcDecl = new cFuncDeclNode(rettype, funcsym, nullptr, true);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    cStmtsNode *body = static_cast<cStmtsNode*>((yyvsp[-1].stmts_node));
                                    if (prevDecl != nullptr) prevDecl->SetBody(body);
                                    else funcDecl->SetBody(body);
                                    if (alreadyDefined)
                                    {
                                        SemanticParseError(funcsym->GetName() + " already has a definition");
                                    }
                                    (yyval.ast_node) = new cFuncNode(rettype, displaySym, nullptr, static_cast<cDeclsNode*>((yyvsp[-2].ast_node)), body);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
#line 1704 "langparse.c"
    break;

  case 23: /* func_decl: func_prefix paramsspec ')' open decls stmts close  */
#line 334 "lang.y"
                                {
                                    // definition with args and decls
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cArgsNode *args = new cArgsNode(static_cast<cDeclsNode*>((yyvsp[-5].ast_node)));
                                    cFuncDeclNode *prevDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    bool alreadyDefined = (prevDecl != nullptr && prevDecl->IsDefined());
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    cFuncDeclNode *funcDecl = new cFuncDeclNode(rettype, funcsym, args, true);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    cStmtsNode *body = static_cast<cStmtsNode*>((yyvsp[-1].stmts_node));
                                    if (prevDecl != nullptr) prevDecl->SetBody(body);
                                    else funcDecl->SetBody(body);
                                    if (alreadyDefined)
                                    {
                                        SemanticParseError(funcsym->GetName() + " already has a definition");
                                    }
                                    (yyval.ast_node) = new cFuncNode(rettype, displaySym, args, static_cast<cDeclsNode*>((yyvsp[-2].ast_node)), body);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
#line 1737 "langparse.c"
    break;

  case 24: /* func_decl: func_prefix paramsspec ')' open stmts close  */
#line 363 "lang.y"
                                {
                                    // definition with args and no decls
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cArgsNode *args = new cArgsNode(static_cast<cDeclsNode*>((yyvsp[-4].ast_node)));
                                    cFuncDeclNode *prevDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    bool alreadyDefined = (prevDecl != nullptr && prevDecl->IsDefined());
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    cFuncDeclNode *funcDecl = new cFuncDeclNode(rettype, funcsym, args, true);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    cStmtsNode *body = static_cast<cStmtsNode*>((yyvsp[-1].stmts_node));
                                    if (prevDecl != nullptr) prevDecl->SetBody(body);
                                    else funcDecl->SetBody(body);
                                    if (alreadyDefined)
                                    {
                                        SemanticParseError(funcsym->GetName() + " already has a definition");
                                    }
                                    (yyval.ast_node) = new cFuncNode(rettype, displaySym, args, nullptr, body);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
#line 1770 "langparse.c"
    break;

  case 25: /* func_prefix: TYPE_ID IDENTIFIER '('  */
#line 392 "lang.y"
                                {
                                    // Create a function symbol in the current (enclosing)
                                    // scope, then open a new scope for parameters.
                                    std::string funcName(*(yyvsp[-1].str_val));
                                    cSymbol *outer = g_symbolTable.Find(funcName);
                                    
                                    // Check if function name already exists in current scope
                                    cSymbol *existing = g_symbolTable.FindLocal(funcName);
                                    
                                    if (existing != nullptr && existing->GetDecl() != nullptr)
                                    {
                                        // Symbol exists - check if it's a function
                                        if (existing->GetDecl()->IsFunc())
                                        {
                                            // It's already a function, reuse it
                                            g_current_func_sym = existing;
                                            cFuncDeclNode *decl = dynamic_cast<cFuncDeclNode*>(existing->GetDecl());
                                            g_current_func_display_sym = (decl != nullptr) ? decl->GetDisplaySym() : existing;
                                        }
                                        else
                                        {
                                            // It's not a function, error
                                            SemanticParseError(funcName + " previously defined as other than a function");
                                            g_current_func_sym = existing;  // Continue parsing
                                            g_current_func_display_sym = existing;
                                        }
                                    }
                                    else if (existing != nullptr)
                                    {
                                        // Symbol exists but no decl yet, treat as new
                                        g_current_func_sym = existing;
                                        g_current_func_display_sym = existing;
                                    }
                                    else
                                    {
                                        // New function, create symbol
                                        g_current_func_sym = new cSymbol(funcName);
                                        g_symbolTable.Insert(g_current_func_sym);

                                        // Match historical XML behavior for nested shadowing where
                                        // an outer non-function symbol has the same name.
                                        if (outer != nullptr && outer->GetDecl() != nullptr && !outer->GetDecl()->IsFunc())
                                            g_current_func_display_sym = outer;
                                        else
                                            g_current_func_display_sym = g_current_func_sym;
                                    }
                                    
                                    g_current_ret_sym = (yyvsp[-2].symbol);
                                    g_current_func_line = yylineno;
                                    g_symbolTable.IncreaseScope();
                                    (yyval.ast_node) = nullptr;
                                    delete (yyvsp[-1].str_val);
                                }
#line 1828 "langparse.c"
    break;

  case 26: /* paramsspec: paramsspec ',' paramspec  */
#line 446 "lang.y"
                                { static_cast<cDeclsNode*>((yyvsp[-2].ast_node))->Insert(static_cast<cDeclNode*>((yyvsp[0].ast_node))); (yyval.ast_node) = (yyvsp[-2].ast_node); }
#line 1834 "langparse.c"
    break;

  case 27: /* paramsspec: paramspec  */
#line 448 "lang.y"
                            { (yyval.ast_node) = new cDeclsNode(static_cast<cDeclNode*>((yyvsp[0].ast_node))); }
#line 1840 "langparse.c"
    break;

  case 28: /* paramspec: var_decl  */
#line 451 "lang.y"
                                    { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 1846 "langparse.c"
    break;

  case 29: /* stmts: stmts stmt  */
#line 454 "lang.y"
                                { (yyvsp[-1].stmts_node)->Insert((yyvsp[0].stmt_node)); (yyval.stmts_node) = (yyvsp[-1].stmts_node); }
#line 1852 "langparse.c"
    break;

  case 30: /* stmts: stmt  */
#line 456 "lang.y"
                            { (yyval.stmts_node) = new cStmtsNode((yyvsp[0].stmt_node)); }
#line 1858 "langparse.c"
    break;

  case 31: /* stmt: IF '(' expr ')' stmts ENDIF ';'  */
#line 459 "lang.y"
                                { (yyval.stmt_node) = new cIfNode((yyvsp[-4].expr_node), (yyvsp[-2].stmts_node)); }
#line 1864 "langparse.c"
    break;

  case 32: /* stmt: IF '(' expr ')' stmts ELSE stmts ENDIF ';'  */
#line 461 "lang.y"
                                { (yyval.stmt_node) = new cIfNode((yyvsp[-6].expr_node), (yyvsp[-4].stmts_node), (yyvsp[-2].stmts_node)); }
#line 1870 "langparse.c"
    break;

  case 33: /* stmt: WHILE '(' expr ')' stmt  */
#line 463 "lang.y"
                                { (yyval.stmt_node) = new cWhileNode((yyvsp[-2].expr_node), static_cast<cStmtNode*>((yyvsp[0].stmt_node))); }
#line 1876 "langparse.c"
    break;

  case 34: /* stmt: PRINT '(' expr ')' ';'  */
#line 465 "lang.y"
                                { (yyval.stmt_node) = new cPrintNode((yyvsp[-2].expr_node)); }
#line 1882 "langparse.c"
    break;

  case 35: /* stmt: PRINTS '(' STRING_LIT ')' ';'  */
#line 467 "lang.y"
                                { (yyval.stmt_node) = new cPrintsNode((yyvsp[-2].str_val)); }
#line 1888 "langparse.c"
    break;

  case 36: /* stmt: lval '=' expr ';'  */
#line 469 "lang.y"
                                { (yyval.stmt_node) = new cAssignNode(static_cast<cExprNode*>((yyvsp[-3].expr_node)), (yyvsp[-1].expr_node)); }
#line 1894 "langparse.c"
    break;

  case 37: /* stmt: func_call ';'  */
#line 471 "lang.y"
                            { (yyval.stmt_node) = static_cast<cStmtNode*>((yyvsp[-1].ast_node)); }
#line 1900 "langparse.c"
    break;

  case 38: /* stmt: block  */
#line 473 "lang.y"
                            { (yyval.stmt_node) = static_cast<cStmtNode*>((yyvsp[0].block_node)); }
#line 1906 "langparse.c"
    break;

  case 39: /* stmt: RETURN expr ';'  */
#line 475 "lang.y"
                                     { (yyval.stmt_node) = new cReturnNode((yyvsp[-1].expr_node)); }
#line 1912 "langparse.c"
    break;

  case 40: /* stmt: error ';'  */
#line 477 "lang.y"
                            {}
#line 1918 "langparse.c"
    break;

  case 41: /* func_call: IDENTIFIER '(' params ')'  */
#line 480 "lang.y"
                                    { 
                                        cSymbol *fs = g_symbolTable.Find(*(yyvsp[-3].str_val));
                                        if (fs == nullptr) fs = new cSymbol(*(yyvsp[-3].str_val));
                                        cParamsNode *p = static_cast<cParamsNode*>((yyvsp[-1].ast_node));
                                        (yyval.ast_node) = new cFuncCallNode(fs, p);
                                        delete (yyvsp[-3].str_val);
                                    }
#line 1930 "langparse.c"
    break;

  case 42: /* func_call: IDENTIFIER '(' ')'  */
#line 488 "lang.y"
                            { 
                                        cSymbol *fs = g_symbolTable.Find(*(yyvsp[-2].str_val));
                                        if (fs == nullptr) fs = new cSymbol(*(yyvsp[-2].str_val));
                                        (yyval.ast_node) = new cFuncCallNode(fs, nullptr);
                                        delete (yyvsp[-2].str_val);
                            }
#line 1941 "langparse.c"
    break;

  case 43: /* varref: varref '.' varpart  */
#line 496 "lang.y"
                                { static_cast<cVarExprNode*>((yyvsp[-2].expr_node))->AddPart((yyvsp[0].symbol)); (yyval.expr_node) = (yyvsp[-2].expr_node); }
#line 1947 "langparse.c"
    break;

  case 44: /* varref: varref '[' expr ']'  */
#line 498 "lang.y"
                            { static_cast<cVarExprNode*>((yyvsp[-3].expr_node))->AddPart(static_cast<cExprNode*>((yyvsp[-1].expr_node))); (yyval.expr_node) = (yyvsp[-3].expr_node); }
#line 1953 "langparse.c"
    break;

  case 45: /* varref: varpart  */
#line 500 "lang.y"
                            { (yyval.expr_node) = new cVarExprNode((yyvsp[0].symbol)); }
#line 1959 "langparse.c"
    break;

  case 46: /* varpart: IDENTIFIER  */
#line 503 "lang.y"
                                { 
                                    // Resolve identifier to symbol in current scopes
                                    cSymbol *found = g_symbolTable.Find(*(yyvsp[0].str_val));
                                    if (found != nullptr) {
                                        (yyval.symbol) = found;
                                    }
                                    else {
                                        (yyval.symbol) = new cSymbol(*(yyvsp[0].str_val));
                                    }
                                    delete (yyvsp[0].str_val);
                                }
#line 1975 "langparse.c"
    break;

  case 47: /* lval: varref  */
#line 516 "lang.y"
                                { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 1981 "langparse.c"
    break;

  case 48: /* params: params ',' param  */
#line 519 "lang.y"
                                { static_cast<cParamsNode*>((yyvsp[-2].ast_node))->Insert(static_cast<cExprNode*>((yyvsp[0].ast_node))); (yyval.ast_node) = (yyvsp[-2].ast_node); }
#line 1987 "langparse.c"
    break;

  case 49: /* params: param  */
#line 521 "lang.y"
                            { (yyval.ast_node) = new cParamsNode(static_cast<cExprNode*>((yyvsp[0].ast_node))); }
#line 1993 "langparse.c"
    break;

  case 50: /* param: expr  */
#line 524 "lang.y"
                                { (yyval.ast_node) = (yyvsp[0].expr_node); }
#line 1999 "langparse.c"
    break;

  case 51: /* expr: expr OR logic_and  */
#line 527 "lang.y"
                                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode(OR), (yyvsp[0].expr_node)); }
#line 2005 "langparse.c"
    break;

  case 52: /* expr: logic_and  */
#line 529 "lang.y"
                            { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 2011 "langparse.c"
    break;

  case 53: /* logic_and: logic_and AND equality  */
#line 532 "lang.y"
                                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode(AND), (yyvsp[0].expr_node)); }
#line 2017 "langparse.c"
    break;

  case 54: /* logic_and: equality  */
#line 534 "lang.y"
                            { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 2023 "langparse.c"
    break;

  case 55: /* equality: equality EQUALS relational  */
#line 537 "lang.y"
                                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode(EQUALS), (yyvsp[0].expr_node)); }
#line 2029 "langparse.c"
    break;

  case 56: /* equality: equality NOT_EQUALS relational  */
#line 539 "lang.y"
                                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode(NOT_EQUALS), (yyvsp[0].expr_node)); }
#line 2035 "langparse.c"
    break;

  case 57: /* equality: relational  */
#line 541 "lang.y"
                            { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 2041 "langparse.c"
    break;

  case 58: /* relational: relational '>' addit  */
#line 544 "lang.y"
                                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode('>'), (yyvsp[0].expr_node)); }
#line 2047 "langparse.c"
    break;

  case 59: /* relational: relational '<' addit  */
#line 546 "lang.y"
                                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode('<'), (yyvsp[0].expr_node)); }
#line 2053 "langparse.c"
    break;

  case 60: /* relational: relational GE addit  */
#line 548 "lang.y"
                                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode(GE), (yyvsp[0].expr_node)); }
#line 2059 "langparse.c"
    break;

  case 61: /* relational: relational LE addit  */
#line 550 "lang.y"
                                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode(LE), (yyvsp[0].expr_node)); }
#line 2065 "langparse.c"
    break;

  case 62: /* relational: addit  */
#line 552 "lang.y"
                            { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 2071 "langparse.c"
    break;

  case 63: /* addit: addit '+' term  */
#line 555 "lang.y"
                                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode('+'), (yyvsp[0].expr_node)); }
#line 2077 "langparse.c"
    break;

  case 64: /* addit: addit '-' term  */
#line 557 "lang.y"
                            { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode('-'), (yyvsp[0].expr_node)); }
#line 2083 "langparse.c"
    break;

  case 65: /* addit: term  */
#line 559 "lang.y"
                            { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 2089 "langparse.c"
    break;

  case 66: /* term: term '*' unary  */
#line 562 "lang.y"
                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode('*'), (yyvsp[0].expr_node)); }
#line 2095 "langparse.c"
    break;

  case 67: /* term: term '/' unary  */
#line 564 "lang.y"
                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode('/'), (yyvsp[0].expr_node)); }
#line 2101 "langparse.c"
    break;

  case 68: /* term: term '%' unary  */
#line 566 "lang.y"
                { (yyval.expr_node) = new cBinaryExprNode((yyvsp[-2].expr_node), new cOpNode('%'), (yyvsp[0].expr_node)); }
#line 2107 "langparse.c"
    break;

  case 69: /* term: unary  */
#line 568 "lang.y"
                            { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 2113 "langparse.c"
    break;

  case 70: /* unary: '-' unary  */
#line 571 "lang.y"
                { (yyval.expr_node) = new cBinaryExprNode(new cIntExprNode(0), new cOpNode('-'), (yyvsp[0].expr_node)); }
#line 2119 "langparse.c"
    break;

  case 71: /* unary: fact  */
#line 573 "lang.y"
                { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 2125 "langparse.c"
    break;

  case 72: /* fact: '(' expr ')'  */
#line 576 "lang.y"
                                { (yyval.expr_node) = (yyvsp[-1].expr_node); }
#line 2131 "langparse.c"
    break;

  case 73: /* fact: INT_VAL  */
#line 578 "lang.y"
                            { (yyval.expr_node) = new cIntExprNode((yyvsp[0].int_val)); }
#line 2137 "langparse.c"
    break;

  case 74: /* fact: FLOAT_VAL  */
#line 580 "lang.y"
                            { (yyval.expr_node) = new cFloatExprNode((yyvsp[0].float_val)); }
#line 2143 "langparse.c"
    break;

  case 75: /* fact: varref  */
#line 582 "lang.y"
                            { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 2149 "langparse.c"
    break;

  case 76: /* fact: func_call  */
#line 584 "lang.y"
                            { (yyval.expr_node) = static_cast<cExprNode*>((yyvsp[0].ast_node)); }
#line 2155 "langparse.c"
    break;


#line 2159 "langparse.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 586 "lang.y"


// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}
// Function that gets called when a semantic error happens
void SemanticParseError(std::string error)
{
    std::cout << "ERROR: " << error << " near line " 
              << yylineno << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}
