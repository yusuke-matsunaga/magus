#ifndef YMSLMODULE_H
#define YMSLMODULE_H

/// @file YmslModule.h
/// @brief YmslModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "SymDict.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslModule YmslModule.h "YmslModule.h"
/// @brief YMSL のモジュールを表すクラス
//////////////////////////////////////////////////////////////////////
class YmslModule
{
public:

  /// @brief コンストラクタ
  /// @param[in] name モジュール名
  YmslModule(ShString name);

  /// @brief デストラクタ
  ~YmslModule();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュール名を返す．
  ShString
  name() const;

  /// @brief トップレベルブロックを返す．
  AstBlock*
  toplevel_block() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュール名
  ShString mName;

  // グローバル辞書
  // 関数とグローバル変数が登録されている
  SymDict mGlobalDict;

  // トップレベルブロック
  AstBlock* mToplevelBlock;

  // ブロックスタック
  vector<AstBlock*> mBlockStack;

  // 関数のリスト
  vector<AstFuncDecl*> mFuncList;

  // グローバル変数のリスト
  vector<AstVarDecl*> mGlobalVarList;

  // トップレベルのローカル変数のリスト
  vector<AstVarDecl*> mLocalVarList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLMODULE_H
