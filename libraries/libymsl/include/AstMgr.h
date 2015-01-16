#ifndef ASTMGR_H
#define ASTMGR_H

/// @file AstMgr.h
/// @brief AstMgr のヘッダファイル
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
/// @class AstMgr AstMgr.h "AstMgr.h"
/// @brief AST を管理するクラス
//////////////////////////////////////////////////////////////////////
class AstMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] debug デバッグフラグ
  AstMgr(bool debug = false);

  /// @brief デストラクタ
  ~AstMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ソースファイルを読み込む．
  /// @param[in] ido 入力データ
  /// @return 読み込みに成功したら true を返す．
  bool
  read_source(IDO& ido);

  /// @brief トップレベルのASTを返す．
  AstStatement*
  toplevel() const;


public:
  //////////////////////////////////////////////////////////////////////
  // bison から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードをセットする．
  /// @param[in] stmt_list ステートメントリスト
  /// @param[in] loc ファイル位置
  void
  set_root(AstStmtList* stmt_list,
	   const FileRegion& loc);

  /// @brief yylex とのインターフェイス
  /// @param[out] lval 値を格納する変数
  /// @param[out] lloc 位置情報を格納する変数
  /// @return 読み込んだトークンの id を返す．
  int
  scan(YYSTYPE& lval,
       FileRegion& lloc);


public:
  //////////////////////////////////////////////////////////////////////
  // 抽象構文木の部品クラスを作る関数
  //
  // これらのオブジェクトは全て mAlloc 上に確保される．
  // このクラスのデストラクタが全て開放するので個別に
  // 開放する必要はない．
  //////////////////////////////////////////////////////////////////////

  /// @brief import 用のモジュール記述を作る
  /// @param[in] module モジュール名
  /// @param[in] alias エイリアス名
  /// @param[in] loc ファイル位置
  AstModule*
  new_Module(AstSymbol* module,
	     AstSymbol* alias,
	     const FileRegion& loc);

  /// @brief import 文を作る．
  /// @param[in] module_list モジュールのリスト
  /// @param[in] loc ファイル位置
  AstStatement*
  new_Import(AstModuleList* module_list,
	     const FileRegion& loc);

  /// @brief enum 定義を作る．
  /// @param[in] name 型名
  /// @param[in] const_list 定数リスト
  /// @param[in] loc ファイル位置
  AstStatement*
  new_EnumDecl(AstSymbol* name,
	       AstEnumConstList* const_list,
	       const FileRegion& loc);

  /// @brief enum 定数を作る．
  /// @param[in] name 定数名
  /// @param[in] expr 値を表す式
  /// @param[in] loc ファイル位置
  AstEnumConst*
  new_EnumConst(AstSymbol* name,
		AstExpr* expr,
		const FileRegion& loc);

  /// @brief 変数宣言を作る．
  /// @param[in] name 変数名
  /// @param[in] type 型
  /// @param[in] init_expr 初期化式
  /// @param[in] loc ファイル位置
  AstStatement*
  new_VarDecl(AstSymbol* name,
	      AstType* type,
	      AstExpr* init_expr,
	      const FileRegion& loc);

  /// @brief パラメータ宣言を作る．
  /// @param[in] name 変数名
  /// @param[in] type 型
  /// @param[in] init_expr 初期化式
  /// @param[in] loc ファイル位置
  AstParam*
  new_Param(AstSymbol* name,
	    AstType* type,
	    AstExpr* init_expr,
	    const FileRegion& loc);

  /// @brief 関数宣言を作る．
  /// @param[in] name 変数名
  /// @param[in] type 型
  /// @param[in] param_list パラメータリスト
  /// @param[in] stmt 本体の文
  /// @param[in] loc ファイル位置
  AstStatement*
  new_FuncDecl(AstSymbol* name,
	       AstType* type,
	       AstParamList* param_list,
	       AstStatement* stmt,
	       const FileRegion& loc);

  /// @brief 代入文を作る．
  /// @param[in] stmt_type 文の種類
  /// @param[in] left 左辺
  /// @param[in] right 右辺
  /// @param[in] loc ファイル位置
  AstStatement*
  new_Assignment(StmtType stmt_type,
		 AstExpr* left,
		 AstExpr* right,
		 const FileRegion& loc);

  /// @brief if 文を作る．
  /// @param[in] expr 条件式
  /// @param[in] then_stmt 条件が成り立った時実行される文
  /// @param[in] else_stmt 条件が成り立たなかった時実行される文
  /// @param[in] loc ファイル位置
  AstStatement*
  new_If(AstExpr* expr,
	 AstStatement* then_stmt,
	 AstStatement* else_stmt,
	 const FileRegion& loc);

  /// @brief for 文を作る．
  /// @param[in] init 初期化文
  /// @param[in] cond 条件式
  /// @param[in] next 増加文
  /// @param[in] stmt 本体の文
  /// @param[in] loc ファイル位置
  AstStatement*
  new_For(AstStatement* init,
	  AstExpr* cond,
	  AstStatement* next,
	  AstStatement* stmt,
	  const FileRegion& loc);

  /// @brief while 文を作る．
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体の文
  /// @param[in] loc ファイル位置
  AstStatement*
  new_While(AstExpr* cond,
	    AstStatement* stmt,
	    const FileRegion& loc);

  /// @brief do-while 文を作る．
  /// @param[in] stmt 本体
  /// @param[in] cond 条件式
  /// @param[in] loc ファイル位置
  AstStatement*
  new_DoWhile(AstStatement* stmt,
	      AstExpr* cond,
	      const FileRegion& loc);

  /// @brief switch 文を作る．
  /// @param[in] expr 条件式
  /// @param[in] case_list caseリスト
  /// @param[in] loc ファイル位置
  AstStatement*
  new_Switch(AstExpr* expr,
	     AstCaseList* case_list,
	     const FileRegion& loc);

  /// @brief case-item を作る．
  /// @param[in] label ラベル
  /// @param[in] stmt 本体の文
  /// @param[in] loc ファイル位置
  AstCaseItem*
  new_CaseItem(AstExpr* label,
	       AstStatement* stmt,
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
  /// @param[in] stmt_list 本体の文
  /// @param[in] loc ファイル位置
  AstStatement*
  new_BlockStmt(AstStmtList* stmt_list,
		const FileRegion& loc);

  /// @brief 式文を作る．
  /// @param[in] expr 式
  /// @param[in] loc ファイル位置
  AstStatement*
  new_ExprStmt(AstExpr* expr,
	       const FileRegion& loc);

  /// @brief 単項演算式を作る．
  /// @param[in] op 演算子のタイプ
  /// @param[in] left オペランド
  /// @param[in] loc ファイル位置
  AstExpr*
  new_UniOp(ExprType op,
	    AstExpr* left,
	    const FileRegion& loc);

  /// @brief 二項演算式を作る．
  /// @param[in] op 演算子のタイプ
  /// @param[in] left, right オペランド
  AstExpr*
  new_BinOp(ExprType op,
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
  new_ArrayRef(AstExpr* id,
	       AstExpr* index,
	       const FileRegion& loc);

  /// @brief 関数呼び出しを作る．
  /// @param[in] id 関数名
  /// @param[in] expr_list 引数のリスト
  /// @param[in] loc ファイル位置
  AstExpr*
  new_FuncCall(AstExpr* id,
	       AstExprList* expr_list,
	       const FileRegion& loc);

  /// @brief true 定数式を作る．
  /// @param[in] loc ファイル位置
  AstExpr*
  new_TrueConst(const FileRegion& loc);

  /// @brief false 定数式を作る．
  /// @param[in] loc ファイル位置
  AstExpr*
  new_FalseConst(const FileRegion& loc);

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

  /// @brief プライマリを作る．
  /// @param[in] symbol_list 変数名
  /// @param[in] loc ファイル位置
  AstExpr*
  new_Primary(AstSymbolList* symbol_list,
	      const FileRegion& loc);

  /// @brief プリミティブ型を作る．
  /// @param[in] type 型
  /// @param[in] loc ファイル位置
  AstType*
  new_PrimType(TypeId type,
	       const FileRegion& loc);

  /// @brief 名前付きの型を作る．
  /// @param[in] type_name 型名
  AstType*
  new_NamedType(AstSymbol* type_name);

  /// @brief array 型を作る．
  /// @param[in] elem_type 要素の型
  /// @param[in] loc ファイル位置
  AstType*
  new_ArrayType(AstType* elem_type,
		const FileRegion& loc);

  /// @brief set 型を作る．
  /// @param[in] elem_type 要素の型
  /// @param[in] loc ファイル位置
  AstType*
  new_SetType(AstType* elem_type,
	      const FileRegion& loc);

  /// @brief map 型を作る．
  /// @param[in] key_type キーの型
  /// @param[in] elem_type 要素の型
  /// @param[in] loc ファイル位置
  AstType*
  new_MapType(AstType* key_type,
	      AstType* elem_type,
	      const FileRegion& loc);

  /// @brief シンボルを作る．
  /// @param[in] str シンボル名
  /// @param[in] loc ファイル位置
  AstSymbol*
  new_Symbol(ShString str,
	     const FileRegion& loc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 字句解析器
  YmslScanner* mScanner;

  // トップレベルの AST
  AstToplevel* mToplevel;

  // デバッグフラグ
  bool mDebug;

};

END_NAMESPACE_YM_YMSL

#endif // ASTMGR_H
