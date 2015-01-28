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
  /// @param[in] parent 親のスコープ
  /// @param[in] name 名前
  Scope(Scope* parent,
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

  /// @brief 子のスコープを追加する．
  /// @param[in] item 追加する要素
  void
  add_scope(Scope* item);

  /// @brief 関数を追加する．
  /// @param[in] item 追加する要素
  void
  add_function(Function* item);

  /// @brief 変数を追加する．
  /// @param[in] item 追加する要素
  void
  add_var(Var* item);

  /// @brief 型を追加する．
  /// @param[in] item 追加する要素
  void
  add_type(Type* item);

  /// @brief ラベルを追加する．
  /// @param[in] item 追加する要素
  void
  add_label(Label* item);

  /// @brief 名前からハンドルを探す．
  /// @param[in] name 名前
  SymHandle*
  find(ShString name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 要求サイズ
  void
  alloc_table(ymuint req_size);

  /// @brief ハンドルを登録する．
  void
  put(SymHandle* handle);

  /// @brief ハンドルを登録する．
  ///
  /// こちらはサイズチェックなし
  void
  _put(SymHandle* handle);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  Scope* mParent;

  // 自身の名前
  ShString mName;

  // ハッシュサイズ
  ymuint mHashSize;

  // ハッシュ表を拡大するしきい値
  ymuint mNextLimit;

  // ハッシュ表
  SymHandle** mHashTable;

  // ハッシュの要素数
  ymuint mHashNum;

};

END_NAMESPACE_YM_YMSL

#endif // SCOPE_H
