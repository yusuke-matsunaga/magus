
/// @file YmslParser.cc
/// @brief YmslParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslParser.h"
#include "YmslScanner.h"

#include "YmslAstUniOp.h"
#include "YmslAstBinOp.h"
#include "YmslAstInt.h"
#include "YmslAstFloat.h"
#include "YmslAstString.h"
#include "YmslAstSymbol.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslParser
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslParser::YmslParser()
{
  mScanner = NULL;
  mRoot = NULL;
}

// @brief デストラクタ
YmslParser::~YmslParser()
{
  // 念の為
  delete mScanner;
}

// @brief 読み込む．
// @param[in] ido 入力データ
// @return 成功したら true を返す．
bool
YmslParser::read(IDO& ido)
{
  mScanner = new YmslScanner(ido);

  return true;
}

// @brief yylex とのインターフェイス
// @param[out] lvalp 値を格納する変数
// @param[out] llocp 位置情報を格納する変数
// @return 読み込んだトークンの id を返す．
int
YmslParser::yylex(YmslAst*& lval,
		  FileRegion& lloc)
{
  int id = mScanner->read_token(lloc);

  switch ( id ) {
  case SYMBOL:
    lval = new_AstSymbol(mScanner->cur_string(), lloc);
    break;

  case STRING:
    lval = new_AstString(mScanner->cur_string(), lloc);
    break;

  case INT_NUM:
    lval = new_AstInt(mScanner->cur_int(), lloc);
    break;

  case FLOAT_NUM:
    lval = new_AstFloat(mScanner->cur_float(), lloc);
    break;

  default:
    lval = NULL;
    break;
  }

  return id;
}

// @brief + 式を作る．
// @param[in] left, right オペランド
YmslAst*
YmslParser::new_AstPlus(YmslAst* left,
			YmslAst* right)
{
  return new YmslAstPlus(left, right);
}

// @brief - 式を作る．
// @param[in] left, right オペランド
YmslAst*
YmslParser::new_AstMinus(YmslAst* left,
			 YmslAst* right)
{
  return new YmslAstMinus(left, right);
}

// @brief * 式を作る．
// @param[in] left, right オペランド
YmslAst*
YmslParser::new_AstMult(YmslAst* left,
			YmslAst* right)
{
  return new YmslAstMult(left, right);
}

// @brief / 式を作る．
// @param[in] left, right オペランド
YmslAst*
YmslParser::new_AstDiv(YmslAst* left,
		       YmslAst* right)
{
  return new YmslAstDiv(left, right);
}

// @brief % 式を作る．
// @param[in] left, right オペランド
YmslAst*
YmslParser::new_AstMod(YmslAst* left,
		       YmslAst* right)
{
  return new YmslAstMod(left, right);
}

// @brief 識別子式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstSymbol(const char* val,
			  const FileRegion& loc)
{
  return new YmslAstSymbol(val, loc);
}

// @brief 整数定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstInt(int val,
		       const FileRegion& loc)
{
  return new YmslAstInt(val, loc);
}

// @brief 浮動小数点定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstFloat(double val,
			 const FileRegion& loc)
{
  return new YmslAstFloat(val, loc);
}

// @brief 文字列定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstString(const char* val,
			  const FileRegion& loc)
{
  return new YmslAstString(val, loc);
}

END_NAMESPACE_YM_YMSL
