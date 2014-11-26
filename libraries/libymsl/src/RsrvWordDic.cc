
/// @file RsrvWordDic.cc
/// @brief RsrvWordDic の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "RsrvWordDic.h"


BEGIN_NAMESPACE_YM_YMSL

BEGIN_NONAMESPACE

// 表を作るためのデータ構造
struct STpair {
  const char* mStr;
  TokenType mTok;
};

// 予約語のデータの配列
static
STpair init_data[] = {
  { "if",       IF       },
  { "else",     ELSE     },
  { "elif",     ELIF     },
  { "for",      FOR      },
  { "while",    WHILE    },
  { "do",       DO       },
  { "goto",     GOTO     },
  { "break",    BREAK    },
  { "continue", CONTINUE },
  { "function", FUNCTION },
  { "var",      VAR      }
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
  mTable = new Cell*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
  for (ymuint i = 0; i < mSize; ++ i) {
    STpair& p = init_data[i];
    Cell* cell = &mCellArray[i];
    cell->mStr = p.mStr;
    cell->mTok = p.mTok;
    ymuint pos1 = hash_func1(p.mStr) % mSize;
    cell->mLink = mTable[pos1];
    mTable[pos1] = cell;
  }
}

// デストラクタ
RsrvWordDic::~RsrvWordDic()
{
  delete [] mCellArray;
  delete [] mTable;
}

// str が予約語ならそのトークン番号を返す．
// 上記以外ならば SYMBOL を返す．
TokenType
RsrvWordDic::token(const char* str) const
{
  ymuint pos = hash_func1(str) % mSize;
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( strcmp(str, cell->mStr) == 0 ) {
      return cell->mTok;
    }
  }
  return SYMBOL;
}

// @brief トークン番号から文字列を返す関数
const char*
token2str(TokenType token)
{
  static const char* table[] = {
    /// @brief コロン(:)
    ":",
    /// @brief セミコロン(;)
    ";",
    /// @brief コンマ(,)
    ",",
    /// @brief ドット(.)
    ".",

    /// @brief プラス(+)
    "+",
    /// @brief マイナス(-)
    "-",
    /// @brief かける(*)
    "*",
    /// @brief わる(/)
    "/",
    /// @brief 余り(%)
    "%",
    /// @brief ++
    "++",
    /// @brief --
    "--",

    /// @brief logical not(!)
    "!",
    /// @brief logical and(&&)
    "&&",
    /// @brief logical or(||)
    "||",
    /// @brief 等しい(==)
    "==",
    /// @brief 等しくない(!=)
    "!=",
    /// @brief 小なり(<)
    "<",
    /// @brief 大なり(>)
    ">",
    /// @brief 小なりイコール(<=)
    "<=",
    /// @brief 大なりイコール(>=)
    ">=",

    /// @brief bitwise not(~)
    "~",
    /// @brief bitwise and(&)
    "&",
    /// @brief bitwise or(|)
    "|",
    /// @brief bitwise xor(^)
    "^",

    /// @brief 代入 (=)
    "=",

    /// @brief 左括弧( ( )
    "(",
    /// @brief 右括弧( ) )
    ")",
    /// @brief 左中括弧( { )
    "{",
    /// @brief 右中括弧( } )
    "}",
    /// @brief 左角括弧( [ )
    "[",
    /// @brief 右角括弧( ] )
    "]",

    /// @brief 'if'
    "if",
    /// @brief 'else'
    "else",
    /// @brief 'elif'
    "elif",
    /// @brief 'for'
    "for",
    /// @brief 'while'
    "while",
    /// @brief 'do'
    "do",
    /// @brief 'goto'
    "goto",
    /// @brief 'break'
    "break",
    /// @brief 'continue'
    "continue",

    /// @brief 'function'
    "function",
    /// @brief 'return'
    "return",
    /// @brief 'var'
    "var",

    /// @brief シンボル
    "__symbol__",
    /// @brief 整数値
    "__int_num__",
    /// @brief 浮動小数点数値
    "__float_num__",
    /// @brief 文字列リテラル
    "__string__",
    /// @brief エラー
    "__error__",
    /// @brief ファイルの末尾
    "__end__"
  };

  return table[token];
}

END_NAMESPACE_YM_YMSL
