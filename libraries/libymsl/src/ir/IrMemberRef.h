#ifndef IRMEMBERREF_H
#define IRMEMBERREF_H

/// @file IrMemberRef.h
/// @brief IrMemberRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrMemberRef IrMemberRef.h "IrMemberRef.h"
/// @brief クラスメンバ参照を表すノード
//////////////////////////////////////////////////////////////////////
class IrMemberRef :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] base ベースアドレス
  /// @param[in] var メンバ変数
  IrMemberRef(IrNode* base,
	      const Var* var);

  /// @brief デストラクタ
  virtual
  ~IrMemberRef();


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
  /// kOpMemberRef のみ有効
  virtual
  IrNode*
  obj_expr() const;

  /// @brief 変数を返す．
  ///
  /// kOpVarRef, kOpMemberRef のみ有効
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

#endif // IRMEMBERREF_H
