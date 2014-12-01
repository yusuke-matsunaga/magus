
/// @file YmslParser.cc
/// @brief YmslParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslParser.h"
#include "YmslScanner.h"

#include "YmslAstList.h"
#include "YmslAstVarDecl.h"
#include "YmslAstFuncDecl.h"
#include "YmslAstFuncCall.h"
#include "YmslAstAssignment.h"
#include "YmslAstIf.h"
#include "YmslAstElif.h"
#include "YmslAstFor.h"
#include "YmslAstWhile.h"
#include "YmslAstDoWhile.h"
#include "YmslAstSwitch.h"
#include "YmslAstCaseItem.h"
#include "YmslAstGoto.h"
#include "YmslAstLabel.h"
#include "YmslAstBreak.h"
#include "YmslAstContinue.h"
#include "YmslAstReturn.h"
#include "YmslAstBlock.h"
#include "YmslAstUniOp.h"
#include "YmslAstBinOp.h"
#include "YmslAstInt.h"
#include "YmslAstFloat.h"
#include "YmslAstString.h"
#include "YmslAstSymbol.h"
#include "YmslAstIntType.h"
#include "YmslAstFloatType.h"
#include "YmslAstStringType.h"
#include "YmslAstUserType.h"


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
  int yyparser(YmslParser&);

  mScanner = new YmslScanner(ido);

  int stat = yyparse(*this);

  delete mScanner;
  mScanner = NULL;

  return (stat == 0);
}

// @brief 構文木の根のノードを返す．
YmslAst*
YmslParser::root() const
{
  return mRoot;
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
  RsrvWordDic dic;

  switch ( id ) {
  case SYMBOL:
    lval = new_AstSymbol(mScanner->cur_string(), lloc);
    break;

  case STRING_VAL:
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

// @brief ルートを設定する．
void
YmslParser::set_root(YmslAst* root)
{
  mRoot = root;
}

// @brief リストを作る．
YmslAst*
YmslParser::new_AstList()
{
  return new YmslAstList();
}

// @brief 変数宣言を作る．
// @param[in] id 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstVarDecl(YmslAst* id,
			   YmslAst* type,
			   YmslAst* init_expr,
			   const FileRegion& loc)
{
  return new YmslAstVarDecl(id, type, init_expr, loc);
}

// @brief 関数宣言を作る．
// @param[in] id 変数名
// @param[in] type 型
// @param[in] param_list パラメータリスト
// @param[in] statement_list 本体のリスト
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstFuncDecl(YmslAst* id,
			    YmslAst* type,
			    YmslAst* param_list,
			    YmslAst* statement_list,
			    const FileRegion& loc)
{
  return new YmslAstFuncDecl(id, type, param_list, statement_list, loc);
}

// @brief 代入文を作る．
// @param[in] left 左辺
// @param[in] right 右辺
YmslAst*
YmslParser::new_AstAssignment(YmslAst* left,
			      YmslAst* right)
{
  return new YmslAstAssignment(left, right);
}

// @brief if 文を作る．
// @param[in] cond 条件式
// @param[in] then_list then 文
// @param[in] elif_list elif ブロックリスト
// @param[in] else_list else 文
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstIf(YmslAst* cond,
		      YmslAst* then_list,
		      YmslAst* elif_list,
		      YmslAst* else_list,
		      const FileRegion& loc)
{
  return new YmslAstIf(cond, then_list, elif_list, else_list, loc);
}

// @brief elif 文を作る．
// @param[in] cond 条件式
// @param[in] body_list 本文
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstElif(YmslAst* cond,
			YmslAst* body_list,
			const FileRegion& loc)
{
  return new YmslAstElif(cond, body_list, loc);
}

// @brief for 文を作る．
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] body 本文
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstFor(YmslAst* init,
		       YmslAst* cond,
		       YmslAst* next,
		       YmslAst* body,
		       const FileRegion& loc)
{
  return new YmslAstFor(init, cond, next, body, loc);
}

// @brief while 文を作る．
// @param[in] cond 条件式
// @param[in] body 本文
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstWhile(YmslAst* cond,
			 YmslAst* body,
			 const FileRegion& loc)
{
  return new YmslAstWhile(cond, body, loc);
}

// @brief do-while 文を作る．
// @param[in] body 本文
// @param[in] cond 条件式
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstDoWhile(YmslAst* body,
			   YmslAst* cond,
			   const FileRegion& loc)
{
  return new YmslAstDoWhile(body, cond, loc);
}

// @brief switch 文を作る．
// @param[in] cond 条件式
// @param[in] case_list caseリスト
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstSwitch(YmslAst* body,
			  YmslAst* case_list,
			  const FileRegion& loc)
{
  return new YmslAstSwitch(body, case_list, loc);
}

// @brief case-item を作る．
// @param[in] label ラベル
// @param[in] statement_list 本体のリスト
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstCaseItem(YmslAst* label,
			    YmslAst* statement_list,
			    const FileRegion& loc)
{
  return new YmslAstCaseItem(label, statement_list, loc);
}

// @brief goto 文を作る．
// @param[in] label ラベル
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstGoto(YmslAst* label,
			const FileRegion& loc)
{
  return new YmslAstGoto(label, loc);
}

// @brief ラベルを作る．
// @param[in] label ラベル
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstLabel(YmslAst* label,
			 const FileRegion& loc)
{
  return new YmslAstLabel(label, loc);
}

// @brief break 文を作る．
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstBreak(const FileRegion& loc)
{
  return new YmslAstBreak(loc);
}

// @brief continue 文を作る．
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstContinue(const FileRegion& loc)
{
  return new YmslAstContinue(loc);
}

// @brief return 文を作る．
// @param[in] expr 値
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstReturn(YmslAst* expr,
			  const FileRegion& loc)
{
  return new YmslAstReturn(expr, loc);
}

// @brief ブロックを作る．
// @param[in] statement_list 文のリスト
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstBlock(YmslAst* statement_list,
			 const FileRegion& loc)
{
  return new YmslAstBlock(statement_list, loc);
}

// @brief 単項演算式を作る．
// @param[in] op 演算子のトークン
// @param[in] left オペランド
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstUniOp(TokenType op,
			 YmslAst* left,
			 const FileRegion& loc)
{
  switch ( op ) {
  case PLUS: return new YmslAstUniPlus(left, loc);
  case MINUS: return new YmslAstUniMinus(left, loc);
  case BITNEG: return new YmslAstBitNeg(left, loc);
  case LOGNOT: return new YmslAstLogNot(left, loc);
  default: ASSERT_NOT_REACHED;
  }
  return NULL;
}

// @brief 二項演算式を作る．
// @param[in] op 演算子のトークン
// @param[in] left, right オペランド
YmslAst*
YmslParser::new_AstBinOp(TokenType op,
			 YmslAst* left,
			 YmslAst* right)
{
  switch ( op ) {
  case PLUS: return new YmslAstPlus(left, right);
  case MINUS: return new YmslAstMinus(left, right);
  case MULT: return new YmslAstMult(left, right);
  case DIV: return new YmslAstDiv(left, right);
  case MOD: return new YmslAstMod(left, right);
  case BITAND: return new YmslAstBitAnd(left, right);
  case BITOR: return new YmslAstBitOr(left, right);
  case BITXOR: return new YmslAstBitXor(left, right);
  case LOGAND: return new YmslAstLogAnd(left, right);
  case LOGOR: return new YmslAstLogOr(left, right);
  case EQEQ: return new YmslAstEqEq(left, right);
  case NOTEQ: return new YmslAstNotEq(left, right);
  case LT: return new YmslAstLt(left, right);
  case GT: return new YmslAstGt(left, right);
  case LE: return new YmslAstLe(left, right);
  case GE: return new YmslAstGe(left, right);
  default: ASSERT_NOT_REACHED;
  }
  return NULL;
}

// @brief 配列参照を作る．
// @param[in] id 配列名
// @param[in] index インデックス
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstArrayRef(YmslAst* id,
			    YmslAst* index,
			    const FileRegion& loc)
{
  return NULL;
}

// @brief 関数呼び出しを作る．
// @param[in] id 関数名
// @param[in] param_list 引数のリスト
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstFuncCall(YmslAst* id,
			    YmslAst* param_list,
			    const FileRegion& loc)
{
  return new YmslAstFuncCall(id, param_list, loc);
}

// @brief 識別子式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstSymbol(const char* val,
			  const FileRegion& loc)
{
  return new YmslAstSymbol(ShString(val), loc);
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

// @brief 文字列型を作る．
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstStringType(const FileRegion& loc)
{
  return new YmslAstStringType(loc);
}

// @brief 整数型を作る．
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstIntType(const FileRegion& loc)
{
  return new YmslAstIntType(loc);
}

// @brief 浮動小数点型を作る．
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstFloatType(const FileRegion& loc)
{
  return new YmslAstFloatType(loc);
}

// @brief ユーザー定義型を作る．
// @param[in] type_name 型名
// @param[in] loc ファイル位置
YmslAst*
YmslParser::new_AstUserType(YmslAst* type_name)
{
  return new YmslAstUserType(type_name);
}

END_NAMESPACE_YM_YMSL
