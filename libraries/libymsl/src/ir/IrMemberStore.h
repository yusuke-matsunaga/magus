#ifndef IRMEMBERSTORE_H
#define IRMEMBERSTORE_H

/// @file IrMemberStore.h
/// @brief IrMemberStore のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrMemberStore IrMemberStore.h "IrMemberStore.h"
/// @brief クラスメンバ用のストア命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrMemberStore :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] base ベースアドレス
  /// @param[in] var メンバ変数
  /// @param[in] val 値
  IrMemberStore(IrNode* base,
		const Var* var,
		IrNode* val);

  /// @brief デストラクタ
  virtual
  ~IrMemberStore();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

  /// @brief オブジェクトを指す式を返す．
  ///
  /// kOpMemberLoad, kOpMemberStore のみ有効
  virtual
  IrNode*
  obj_expr() const;

  /// @brief 変数を返す．
  ///
  /// kOpVarRef, kOpLoad, kOpStore, kOpMemberLoad, kOpMemberStore のみ有効
  virtual
  const Var*
  var() const;

  /// @brief 書き込む値を返す．
  virtual
  IrNode*
  store_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベースアドレス
  IrNode* mBase;

  // メンバ変数
  const Var* mVar;

  // 書き込む値
  IrNode* mStoreVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRMEMBERSTORE_H
