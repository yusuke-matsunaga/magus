#ifndef YMSLPARSER_H
#define YMSLPARSER_H

/// @file YmslParser.h
/// @brief YmslParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/IDO.h"
#include "YmUtils/FileRegion.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslParser YmslParser.h "YmslParser.h"
/// @brief YMSL 用の構文解析器
//////////////////////////////////////////////////////////////////////
class YmslParser
{
public:

  /// @brief コンストラクタ
  YmslParser();

  /// @brief デストラクタ
  ~YmslParser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込む．
  /// @param[in] ido 入力データ
  /// @return 成功したら true を返す．
  bool
  read(IDO& ido);

  /// @brief 構文木の根のノードを返す．
  YmslAst*
  root() const;


public:
  //////////////////////////////////////////////////////////////////////
  // yacc/bison で用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief yylex とのインターフェイス
  /// @param[out] lvalp 値を格納する変数
  /// @param[out] llocp 位置情報を格納する変数
  /// @return 読み込んだトークンの id を返す．
  int
  yylex(YmslAst*& lval,
	FileRegion& lloc);

  /// @brief リストを作る．
  YmslAst*
  new_AstList();

  /// @brief ルートを設定する．
  void
  set_root(YmslAst* root);

  /// @brief 変数宣言を作る．
  /// @param[in] id 変数名
  /// @param[in] type 型
  /// @param[in] init_expr 初期化式
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstVarDecl(YmslAst* id,
		 YmslAst* type,
		 YmslAst* init_expr,
		 const FileRegion& loc);

  /// @brief 関数宣言を作る．
  /// @param[in] id 変数名
  /// @param[in] type 型
  /// @param[in] param_list パラメータリスト
  /// @param[in] statement_list 本体のリスト
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstFuncDecl(YmslAst* id,
		  YmslAst* type,
		  YmslAst* param_list,
		  YmslAst* statement_list,
		  const FileRegion& loc);

  /// @brief 代入文を作る．
  /// @param[in] left 左辺
  /// @param[in] right 右辺
  YmslAst*
  new_AstAssignment(YmslAst* left,
		    YmslAst* right);

  /// @brief if 文を作る．
  /// @param[in] cond 条件式
  /// @param[in] then_list then 文
  /// @param[in] elif_list elif ブロックリスト
  /// @param[in] else_list else 文
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstIf(YmslAst* cond,
	    YmslAst* then_list,
	    YmslAst* elif_list,
	    YmslAst* else_list,
	    const FileRegion& loc);

  /// @brief elif 文を作る．
  /// @param[in] cond 条件式
  /// @param[in] body_list 本文
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstElif(YmslAst* cond,
	      YmslAst* body_list,
	      const FileRegion& loc);

  /// @brief for 文を作る．
  /// @param[in] init 初期化文
  /// @param[in] cond 条件式
  /// @param[in] next 増加文
  /// @param[in] body 本文
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstFor(YmslAst* init,
	     YmslAst* cond,
	     YmslAst* next,
	     YmslAst* body,
	     const FileRegion& loc);

  /// @brief while 文を作る．
  /// @param[in] cond 条件式
  /// @param[in] body 本文
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstWhile(YmslAst* cond,
	       YmslAst* body,
	       const FileRegion& loc);

  /// @brief do-while 文を作る．
  /// @param[in] body 本文
  /// @param[in] cond 条件式
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstDoWhile(YmslAst* body,
		 YmslAst* cond,
		 const FileRegion& loc);

  /// @brief switch 文を作る．
  /// @param[in] cond 条件式
  /// @param[in] case_list caseリスト
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstSwitch(YmslAst* body,
		YmslAst* case_list,
		const FileRegion& loc);

  /// @brief case-item を作る．
  /// @param[in] label ラベル
  /// @param[in] statement_list 本体のリスト
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstCaseItem(YmslAst* label,
		  YmslAst* statment_list,
		  const FileRegion& loc);

  /// @brief goto 文を作る．
  /// @param[in] label ラベル
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstGoto(YmslAst* label,
	      const FileRegion& loc);

  /// @brief ラベルを作る．
  /// @param[in] label ラベル
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstLabel(YmslAst* label,
	       const FileRegion& loc);

  /// @brief break 文を作る．
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstBreak(const FileRegion& loc);

  /// @brief continue 文を作る．
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstContinue(const FileRegion& loc);

  /// @brief return 文を作る．
  /// @param[in] expr 値
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstReturn(YmslAst* expr,
		const FileRegion& loc);

  /// @brief 単項演算式を作る．
  /// @param[in] op 演算子のトークン
  /// @param[in] left オペランド
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstUniOp(TokenType op,
	       YmslAst* left,
	       const FileRegion& loc);

  /// @brief 二項演算式を作る．
  /// @param[in] op 演算子のトークン
  /// @param[in] left, right オペランド
  YmslAst*
  new_AstBinOp(TokenType op,
	       YmslAst* left,
	       YmslAst* right);

  /// @brief 配列参照を作る．
  /// @param[in] id 配列名
  /// @param[in] index インデックス
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstArrayRef(YmslAst* id,
		  YmslAst* index,
		  const FileRegion& loc);

  /// @brief 関数呼び出しを作る．
  /// @param[in] id 関数名
  /// @param[in] param_list 引数のリスト
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstFuncCall(YmslAst* id,
		  YmslAst* param_list,
		  const FileRegion& loc);

  /// @brief 識別子式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstSymbol(const char* val,
		const FileRegion& loc);

  /// @brief 文字列定数式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstString(const char* val,
		const FileRegion& loc);

  /// @brief 整数定数式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstInt(int val,
	     const FileRegion& loc);

  /// @brief 浮動小数点定数式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstFloat(double val,
	       const FileRegion& loc);

  /// @brief 文字列型を作る．
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstStringType(const FileRegion& loc);

  /// @brief 整数型を作る．
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstIntType(const FileRegion& loc);

  /// @brief 浮動小数点型を作る．
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstFloatType(const FileRegion& loc);

  /// @brief ユーザー定義型を作る．
  /// @param[in] type_name 型名
  YmslAst*
  new_AstUserType(YmslAst* type_name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  YmslScanner* mScanner;

  // 構文木の根
  YmslAst* mRoot;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLPARSER_H
