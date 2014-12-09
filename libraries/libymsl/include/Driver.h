#ifndef DRIVER_H
#define DRIVER_H

/// @file Driver.h
/// @brief Driver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmslDict.h"
#include "YmUtils/IDO.h"
#include "YmUtils/FileRegion.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_YMSL

union YYSTYPE;
class YmslScanner;

//////////////////////////////////////////////////////////////////////
/// @class Driver Driver.h "Driver.h"
/// @brief YMSL 用の構文解析器
//////////////////////////////////////////////////////////////////////
class Driver
{
public:

  /// @brief コンストラクタ
  Driver();

  /// @brief デストラクタ
  ~Driver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込む．
  /// @param[in] ido 入力データ
  /// @return 成功したら true を返す．
  bool
  read(IDO& ido);

  /// @brief トップレベルブロックを返す．
  AstBlock*
  toplevel_block() const;


public:
  //////////////////////////////////////////////////////////////////////
  // yacc/bison で用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief yylex とのインターフェイス
  /// @param[out] lval 値を格納する変数
  /// @param[out] lloc 位置情報を格納する変数
  /// @return 読み込んだトークンの id を返す．
  int
  yylex(YYSTYPE& lval,
	FileRegion& lloc);

  /// @brief 現在のブロックを返す．
  AstBlock*
  cur_block();

  /// @brief 新しいブロックを作りスタックに積む．
  /// @return 新しいブロックを返す．
  AstBlock*
  push_new_block();

  /// @brief ブロックをスタックから取り去る．
  void
  pop_block();

  /// @brief 関数を追加する．
  void
  add_function(AstFuncDecl* funcdecl);

  /// @brief グローバル変数を追加する．
  /// @param[in] vardecl 変数宣言
  void
  add_global_var(AstVarDecl* vardecl);

  /// @brief 現在のブロックに変数を追加する．
  void
  add_local_var(AstVarDecl* vardecl);

  /// @brief 現在のブロックに statement を追加する．
  void
  add_statement(AstStatement* stmt);


public:
  //////////////////////////////////////////////////////////////////////
  // 抽象構文木の部品クラスを作る関数
  //
  // これらのオブジェクトは全て mAlloc 上に確保される．
  // このクラスのデストラクタが全て開放するので個別に
  // 開放する必要はない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数宣言を作る．
  /// @param[in] name 変数名
  /// @param[in] type 型
  /// @param[in] init_expr 初期化式
  /// @param[in] loc ファイル位置
  AstVarDecl*
  new_VarDecl(AstSymbol* name,
	      AstValueType* type,
	      AstExpr* init_expr,
	      const FileRegion& loc);

  /// @brief 関数宣言を作る．
  /// @param[in] name 変数名
  /// @param[in] type 型
  /// @param[in] param_list パラメータリスト
  /// @param[in] block 本体のブロック
  /// @param[in] loc ファイル位置
  AstFuncDecl*
  new_FuncDecl(AstSymbol* name,
	       AstValueType* type,
	       AstVarDecl* param_list,
	       const FileRegion& loc);

  /// @brief 代入文を作る．
  /// @param[in] left 左辺
  /// @param[in] right 右辺
  AstStatement*
  new_Assignment(AstExpr* left,
		 AstExpr* right);

  /// @brief if 文を作る．
  /// @param[in] top 先頭の if ブロック
  /// @param[in] elif_list elif ブロックリスト
  /// @param[in] else_block else ブロック
  /// @param[in] loc ファイル位置
  AstStatement*
  new_If(AstIfBlock* top,
	 AstIfBlock* elif_list,
	 AstIfBlock* else_block,
	 const FileRegion& loc);

  /// @brief if blockを作る．
  /// @param[in] cond 条件式
  /// @param[in] block 本体
  /// @param[in] loc ファイル位置
  AstIfBlock*
  new_IfBlock(AstExpr* cond,
	      AstBlock* block,
	      const FileRegion& loc);

  /// @brief for 文を作る．
  /// @param[in] init 初期化文
  /// @param[in] cond 条件式
  /// @param[in] next 増加文
  /// @param[in] block 本体
  /// @param[in] loc ファイル位置
  AstStatement*
  new_For(AstStatement* init,
	  AstExpr* cond,
	  AstStatement* next,
	  AstBlock* block,
	  const FileRegion& loc);

  /// @brief while 文を作る．
  /// @param[in] cond 条件式
  /// @param[in] block 本体
  /// @param[in] loc ファイル位置
  AstStatement*
  new_While(AstExpr* cond,
	    AstBlock* block,
	    const FileRegion& loc);

  /// @brief do-while 文を作る．
  /// @param[in] block 本体
  /// @param[in] cond 条件式
  /// @param[in] loc ファイル位置
  AstStatement*
  new_DoWhile(AstBlock* block,
	      AstExpr* cond,
	      const FileRegion& loc);

  /// @brief switch 文を作る．
  /// @param[in] expr 条件式
  /// @param[in] case_list caseリスト
  /// @param[in] loc ファイル位置
  AstStatement*
  new_Switch(AstExpr* expr,
	     AstCaseItem* case_list,
	     const FileRegion& loc);

  /// @brief case-item を作る．
  /// @param[in] label ラベル
  /// @param[in] block 本体
  /// @param[in] loc ファイル位置
  AstCaseItem*
  new_CaseItem(AstExpr* label,
	       AstBlock* block,
	       const FileRegion& loc);

  /// @brief goto 文を作る．
  /// @param[in] label ラベル
  /// @param[in] loc ファイル位置
  AstStatement*
  new_Goto(AstSymbol* label,
	   const FileRegion& loc);

  /// @brief ラベルを作る．
  /// @param[in] label ラベル
  /// @param[in] loc ファイル位置
  AstStatement*
  new_Label(AstSymbol* label,
	    const FileRegion& loc);

  /// @brief break 文を作る．
  /// @param[in] loc ファイル位置
  AstStatement*
  new_Break(const FileRegion& loc);

  /// @brief continue 文を作る．
  /// @param[in] loc ファイル位置
  AstStatement*
  new_Continue(const FileRegion& loc);

  /// @brief return 文を作る．
  /// @param[in] expr 値
  /// @param[in] loc ファイル位置
  AstStatement*
  new_Return(AstExpr* expr,
		const FileRegion& loc);

  /// @brief ブロック文を作る．
  /// @param[in] block 本体
  /// @param[in] loc ファイル位置
  AstStatement*
  new_BlockStmt(AstBlock* block,
		const FileRegion& loc);

  /// @brief 式文を作る．
  /// @param[in] expr 式
  AstStatement*
  new_ExprStmt(AstExpr* expr);

  /// @brief 単項演算式を作る．
  /// @param[in] op 演算子のトークン
  /// @param[in] left オペランド
  /// @param[in] loc ファイル位置
  AstExpr*
  new_UniOp(TokenType op,
	    AstExpr* left,
	    const FileRegion& loc);

  /// @brief 二項演算式を作る．
  /// @param[in] op 演算子のトークン
  /// @param[in] left, right オペランド
  AstExpr*
  new_BinOp(TokenType op,
	    AstExpr* left,
	    AstExpr* right);

  /// @brief 配列参照を作る．
  /// @param[in] id 配列名
  /// @param[in] index インデックス
  /// @param[in] loc ファイル位置
  AstExpr*
  new_ArrayRef(AstSymbol* id,
	       AstExpr* index,
	       const FileRegion& loc);

  /// @brief 関数呼び出しを作る．
  /// @param[in] id 関数名
  /// @param[in] expr_list 引数のリスト
  /// @param[in] loc ファイル位置
  AstExpr*
  new_FuncCall(AstSymbol* id,
	       AstExpr* expr_list,
	       const FileRegion& loc);

  /// @brief 識別子式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstExpr*
  new_VarExpr(AstSymbol* symbol);

  /// @brief 整数定数式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstExpr*
  new_IntConst(int val,
	       const FileRegion& loc);

  /// @brief 浮動小数点定数式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstExpr*
  new_FloatConst(double val,
		 const FileRegion& loc);

  /// @brief 文字列定数を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstExpr*
  new_StringConst(const char* val,
		  const FileRegion& loc);

  /// @brief 整数型を作る．
  /// @param[in] loc ファイル位置
  AstValueType*
  new_IntType(const FileRegion& loc);

  /// @brief 浮動小数点型を作る．
  /// @param[in] loc ファイル位置
  AstValueType*
  new_FloatType(const FileRegion& loc);

  /// @brief 文字列型を作る．
  /// @param[in] loc ファイル位置
  AstValueType*
  new_StringType(const FileRegion& loc);

  /// @brief ユーザー定義型を作る．
  /// @param[in] type_name 型名
  AstValueType*
  new_UserType(AstSymbol* type_name);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 字句解析器
  YmslScanner* mScanner;

  // グローバル辞書
  // 関数とグローバル変数が登録されている
  YmslDict mGlobalDict;

  // トップレベルブロック
  AstBlock* mToplevelBlock;

  // ブロックスタック
  vector<AstBlock*> mBlockStack;

};

END_NAMESPACE_YM_YMSL


#endif // DRIVER_H
