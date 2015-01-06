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
#include "YmUtils/ShString.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_YMSL

union YYSTYPE;
class YmslScanner;

//////////////////////////////////////////////////////////////////////
/// @class YmslParser YmslParser.h "YmslParser.h"
/// @brief YMSL のモジュールを表すクラス
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

  /// @brief 一つのソースファイルを読み込む．
  /// @param[in] ido 入力データ
  /// @param[in] module モジュール
  /// @return 読み込みに成功したら true を返す．
  bool
  read_source(IDO& ido,
	      YmslModule* module);

#if 0
  /// @brief トップレベルブロックを返す．
  AstBlock*
  toplevel_block() const;

  /// @brief 関数のリストを返す．
  const vector<AstFuncDecl*>&
  function_list() const;

  /// @brief グローバル変数のリストを返す．
  const vector<AstVarDecl*>&
  global_var_list() const;

  /// @brief 関数を探す．
  /// @param[in] name 関数名
  ///
  /// 見つからなければ NULL を返す．
  AstFuncDecl*
  find_function(ShString name) const;

  /// @brief 変数を探す．
  /// @param[in] name 変数名
  ///
  /// 見つからなければ NULL を返す．
  AstVarDecl*
  find_var(ShString name) const;
#endif


public:
  //////////////////////////////////////////////////////////////////////
  // bison から用いられる関数
  //////////////////////////////////////////////////////////////////////
#if 0
  /// @brief 現在のブロックを返す．
  AstBlock*
  cur_block() const;

  /// @brief 新しいブロックを作りスタックに積む．
  /// @return 新しいブロックを返す．
  AstBlock*
  push_new_block();

  /// @brief ブロックをスタックから取り去る．
  void
  pop_block();

  /// @brief インポートする．
  /// @param[in] module_name モジュール名
  /// @param[in] alias_name エイリアス名
  void
  import(AstSymbol* module_name,
	 AstSymbol* alias_name);

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
#endif

  /// @brief yylex とのインターフェイス
  /// @param[out] lval 値を格納する変数
  /// @param[out] lloc 位置情報を格納する変数
  /// @return 読み込んだトークンの id を返す．
  int
  scan(YYSTYPE& lval,
       FileRegion& lloc,
       AstMgr& mgr);

#if 0
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
  /// @param[in] global グローバル変数の時 true にするフラグ
  /// @param[in] loc ファイル位置
  AstVarDecl*
  new_VarDecl(AstSymbol* name,
	      AstValueType* type,
	      AstExpr* init_expr,
	      bool global,
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
  /// @param[in] token トークン
  /// @param[in] left 左辺
  /// @param[in] right 右辺
  AstStatement*
  new_Assignment(TokenType token,
		 AstPrimary* left,
		 AstExpr* right);

  /// @brief if 文を作る．
  /// @param[in] if_list IfBlock のリスト
  /// @param[in] loc ファイル位置
  AstStatement*
  new_If(AstIfBlock* if_list,
	 const FileRegion& loc);

  /// @brief if blockを作る．
  /// @param[in] prev 直前の要素
  /// @param[in] cond 条件式
  /// @param[in] block 本体
  /// @param[in] loc ファイル位置
  AstIfBlock*
  new_IfBlock(AstIfBlock* prev,
	      AstExpr* cond,
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
  /// @param[in] prev 直前の要素
  /// @param[in] label ラベル
  /// @param[in] block 本体
  /// @param[in] loc ファイル位置
  AstCaseItem*
  new_CaseItem(AstCaseItem* prev,
	       AstExpr* label,
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

  /// @brief ITE演算式を作る．
  /// @param[in] opr1, opr2, opr3 オペランド
  AstExpr*
  new_IteOp(AstExpr* opr1,
	    AstExpr* opr2,
	    AstExpr* opr3);

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
  new_VarExpr(AstSymbol* symbol,
	      const FileRegion& loc);

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

  /// @brief 左辺のプライマリを作る．
  /// @param[in] symbol 変数名
  AstPrimary*
  new_Primary(AstSymbol* symbol);

  /// @brief void型を作る．
  /// @param[in] loc ファイル位置
  AstValueType*
  new_VoidType(const FileRegion& loc);

  /// @brief boolean型を作る．
  /// @param[in] loc ファイル位置
  AstValueType*
  new_BooleanType(const FileRegion& loc);

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

  /// @brief ラベルを作る．
  /// @param[in] code_list 命令コードを格納する配列
  /// @param[in] name ラベル名
  YmslLabel*
  new_label(YmslCodeList& code_list,
	    ShString name = ShString());
#endif


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 字句解析器
  YmslScanner* mScanner;

  // モジュール
  YmslModule* mModule;

  // デバッグフラグ
  bool mDebug;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLPARSER_H
