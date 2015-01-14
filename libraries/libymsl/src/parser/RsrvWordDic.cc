
/// @file RsrvWordDic.cc
/// @brief RsrvWordDic の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "RsrvWordDic.h"


BEGIN_NAMESPACE_YM_YMSL

#include "grammer.hh"

BEGIN_NONAMESPACE

// 表を作るためのデータ構造
struct STpair {
  const char* mStr;
  TokenType mTok;
};

// 予約語のデータの配列
static
STpair init_data[] = {
  { ":",        COLON      },
  { ";",        SEMI       },
  { ",",        COMMA      },
  { ".",        DOT        },
  { "?",        QST        },
  { "++",       PLUSPLUS   },
  { "--",       MINUSMINUS },
  { "=",        EQ         },
  { "+=",       EQPLUS     },
  { "-=",       EQMINUS    },
  { "*=",       EQMULT     },
  { "/=",       EQDIV      },
  { "%=",       EQMOD      },
  { "<<=",      EQLSHIFT   },
  { ">>=",      EQRSHIFT   },
  { "&=",       EQBITAND   },
  { "|=",       EQBITOR    },
  { "^=",       EQBITXOR   },
  { "(",        LP         },
  { ")",        RP         },
  { "{",        LCB        },
  { "}",        RCB        },
  { "[",        LBK        },
  { "]",        RBK        },
  { "+",        PLUS       },
  { "-",        MINUS      },
  { "*",        MULT       },
  { "/",        DIV        },
  { "%",        MOD        },
  { "not",      LOGNOT     },
  { "and",      LOGAND     },
  { "or",       LOGOR      },
  { "~",        BITNEG     },
  { "&",        BITAND     },
  { "|",        BITOR      },
  { "^",        BITXOR     },
  { "==",       EQEQ       },
  { "!=",       NOTEQ      },
  { "<",        LT         },
  { ">",        GT         },
  { "<=",       LE         },
  { ">=",       GE         },

  { "array",    ARRAY      },
  { "as",       AS         },
  { "boolean",  BOOLEAN    },
  { "break",    BREAK      },
  { "case",     CASE       },
  { "continue", CONTINUE   },
  { "default",  DEFAULT    },
  { "do",       DO         },
  { "elif",     ELIF       },
  { "else",     ELSE       },
  { "enum",     ENUM       },
  { "float",    FLOAT      },
  { "for",      FOR        },
  { "function", FUNCTION   },
  { "goto",     GOTO       },
  { "if",       IF         },
  { "import",   IMPORT     },
  { "int",      INT        },
  { "map",      MAP        },
  { "return",   RETURN     },
  { "set",      SET        },
  { "string",   STRING     },
  { "switch",   SWITCH     },
  { "var",      VAR        },
  { "void",     VOID       },
  { "while",    WHILE      },
  { "_int_",    INT_VAL    },
  { "_dummy_",  DUMMY      },
  { "_error_",  ERROR      },
  { "_float_",  FLOAT_VAL  },
  { "_string_", STRING_VAL },
  { "_symbol_", SYMBOL     },
};

// 文字列からのハッシュ関数
ymuint
hash_func1(const char* str)
{
  ymuint h = 0;
  ymuint c;
  for ( ; (c = static_cast<ymuint>(*str)); ++ str) {
    h = h * 37 + c;
  }
  return h;
}

END_NONAMESPACE


// コンストラクタ
RsrvWordDic::RsrvWordDic()
{
  mSize = sizeof(init_data) / sizeof(STpair);
  mCellArray = new Cell[mSize];
  mTable1 = new Cell*[mSize];
  mTable2 = new Cell*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable1[i] = NULL;
    mTable2[i] = NULL;
  }
  for (ymuint i = 0; i < mSize; ++ i) {
    STpair& p = init_data[i];
    Cell* cell = &mCellArray[i];
    cell->mStr = p.mStr;
    cell->mTok = p.mTok;
    if ( isascii(p.mStr[0]) ) {
      ymuint pos1 = hash_func1(p.mStr) % mSize;
      cell->mLink1 = mTable1[pos1];
      mTable1[pos1] = cell;
    }
    ymuint pos2 = p.mTok % mSize;
    cell->mLink2 = mTable2[pos2];
    mTable2[pos2] = cell;
  }
}

// デストラクタ
RsrvWordDic::~RsrvWordDic()
{
  delete [] mCellArray;
  delete [] mTable1;
  delete [] mTable2;
}

// str が予約語ならそのトークン番号を返す．
// 上記以外ならば SYMBOL を返す．
TokenType
RsrvWordDic::token(const char* str) const
{
  ymuint pos = hash_func1(str) % mSize;
  for (Cell* cell = mTable1[pos]; cell; cell = cell->mLink1) {
    if ( strcmp(str, cell->mStr) == 0 ) {
      return cell->mTok;
    }
  }
  return SYMBOL;
}

// @brief トークン番号から文字列を返す関数
const char*
RsrvWordDic::str(TokenType token) const
{
  ymuint pos = token % mSize;
  for (Cell* cell = mTable2[pos]; cell; cell = cell->mLink2) {
    if ( token == cell->mTok ) {
      return cell->mStr;
    }
  }
  return NULL;
}

END_NAMESPACE_YM_YMSL
