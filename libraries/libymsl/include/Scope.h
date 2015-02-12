#ifndef SCOPE_H
#define SCOPE_H

/// @file Scope.h
/// @brief Scope のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class Scope Scope.h "Scope.h"
/// @brief スコープを表すクラス
//////////////////////////////////////////////////////////////////////
class Scope
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent_scope 親のスコープ
  /// @param[in] global_scope グローバルスコープ
  /// @param[in] name 名前
  Scope(Scope* parent_scope,
	Scope* global_scope,
	ShString name = ShString());

  /// @brief デストラクタ
  ~Scope();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 自身の名前を返す．
  ///
  /// ShString() (空文字列)の場合もある．
  ShString
  name() const;

  /// @brief グローバルスコープを返す．
  Scope*
  global_scope() const;

  /// @brief 要素を追加する．
  /// @param[in] item 追加する要素
  void
  add(IrHandle* item);

  /// @brief 名前からハンドルを探す．
  /// @param[in] name 名前
  IrHandle*
  find(ShString name) const;

  /// @brief 名前からハンドルを探す．
  /// @param[in] name 名前
  ///
  /// こちらはこのスコープのみ探す．
  IrHandle*
  find_local(ShString name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 要求サイズ
  void
  alloc_table(ymuint req_size);

  /// @brief ハンドルを登録する．
  ///
  /// こちらはサイズチェックなし
  void
  _put(IrHandle* handle);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  Scope* mParentScope;

  // グローバルスコープ
  Scope* mGlobalScope;

  // 自身の名前
  ShString mName;

  // ハッシュサイズ
  ymuint mHashSize;

  // ハッシュ表を拡大するしきい値
  ymuint mNextLimit;

  // ハッシュ表
  IrHandle** mHashTable;

  // ハッシュの要素数
  ymuint mHashNum;

  // ここに属する変数のリスト
  vector<const Var*> mVarList;

};

END_NAMESPACE_YM_YMSL

#endif // SCOPE_H
