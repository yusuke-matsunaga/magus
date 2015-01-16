#ifndef ASTFUNCDECL_H
#define ASTFUNCDECL_H

/// @file AstFuncDecl.h
/// @brief AstFuncDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFuncDecl AstFuncDecl.h "AstFuncDecl.h"
/// @brief 関数宣言を表すクラス
//////////////////////////////////////////////////////////////////////
class AstFuncDecl :
  public AstStatement
{
  friend class YmslModule;

public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] type 型
  /// @param[in] param_list パラメータリスト
  /// @param[in] stmt 本体の文
  /// @param[in] loc ファイル位置
  AstFuncDecl(ShString name,
	      AstType* type,
	      AstParamList* param_list,
	      AstStatement* stmt,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFuncDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックス番号を返す．
  ymuint
  index() const;


public:
  //////////////////////////////////////////////////////////////////////
  // AstStatement の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  StmtType
  stmt_type() const;

  /// @brief 名前を得る．
  virtual
  ShString
  name() const;

  /// @brief 出力の型を返す．
  virtual
  const AstType*
  type() const;

  /// @brief パラメータリストの要素数を返す．
  ///
  /// kFuncDecl のみ有効
  virtual
  ymuint
  param_num() const;

  /// @brief パラメータリストの要素を返す．
  /// @param[in] pos 位置 ( 0 <= pos < param_num() )
  ///
  /// kFuncDecl のみ有効
  virtual
  const AstParam*
  param(ymuint pos) const;

  /// @brief 本体の文を返す．
  ///
  /// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
  virtual
  const AstStatement*
  stmt() const;

#if 0
  /// @brief 要素の生成と関数以外の参照解決を行う．
  /// @param[in] parent_scope 親のスコープ
  /// @param[in] type_mgr 型マネージャ
  virtual
  void
  phase1(YmslScope* parent_scope,
	 YmslTypeMgr* type_mgr);

  /// @brief 関数の参照解決を行う．
  /// @param[in] parent_scope 親のスコープ
  virtual
  void
  phase2(YmslScope* parent_scope);

  /// @brief 命令コードのサイズを計算する．
  virtual
  ymuint
  calc_size();

  /// @brief 命令コードを生成する．
  /// @param[in] driver ドライバ
  /// @param[in] code_list 命令コードの格納先
  /// @param[inout] addr 命令コードの現在のアドレス
  ///
  /// addr の値は更新される．
  virtual
  void
  compile(YmslDriver& driver,
	  YmslCodeList& code_list,
	  Ymsl_INT& addr);

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;
#endif


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // インデックス番号
  ymuint mIndex;

  // 出力の型
  AstType* mType;

  // パラメータリスト
  vector<AstParam*> mParamList;

  // 本体の文
  AstStatement* mStmt;

  // スコープ
  YmslScope* mScope;

};

END_NAMESPACE_YM_YMSL

#endif // ASTFUNCDECL_H
