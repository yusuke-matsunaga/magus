#ifndef IRLOAD_H
#define IRLOAD_H

/// @file IrLoad.h
/// @brief IrLoad のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrLoad IrLoad.h "IrLoad.h"
/// @brief ロード命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrLoad :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] var 変数
  IrLoad(const Var* var);

  /// @brief デストラクタ
  virtual
  ~IrLoad();


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
  /// kOpVarRef, kOpLoad, kOpStore のみ有効
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

#endif // IRLOAD_H
