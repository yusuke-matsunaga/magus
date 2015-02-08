#ifndef IRVARREF_H
#define IRVARREF_H

/// @file IrVarRef.h
/// @brief IrVarRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrVarRef IrVarRef.h "IrVarRef.h"
/// @brief 変数参照を表すノード
//////////////////////////////////////////////////////////////////////
class IrVarRef :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] var 変数
  IrVarRef(const Var* var);

  /// @brief デストラクタ
  virtual
  ~IrVarRef();


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

  /// @brief 変数を返す．
  ///
  /// kOpVarRef, kMemberRef のみ有効
  virtual
  const Var*
  var() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数
  const Var* mVar;

};

END_NAMESPACE_YM_YMSL

#endif // IRVARREF_H
