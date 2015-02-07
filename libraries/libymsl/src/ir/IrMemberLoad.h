#ifndef IRMEMBERLOAD_H
#define IRMEMBERLOAD_H

/// @file IrMemberLoad.h
/// @brief IrMemberLoad のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrMemberLoad IrMemberLoad.h "IrMemberLoad.h"
/// @brief クラスメンバ用のロード命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrMemberLoad :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] base ベースアドレス
  /// @param[in] var メンバ変数
  IrMemberLoad(IrNode* base,
	       const Var* var);

  /// @brief デストラクタ
  virtual
  ~IrMemberLoad();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベースアドレス
  IrNode* mBase;

  // 変数
  const Var* mVar;

};

END_NAMESPACE_YM_YMSL

#endif // IRMEMBERLOAD_H
