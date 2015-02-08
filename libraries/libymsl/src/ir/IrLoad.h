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
  /// @param[in] addr アドレス
  IrLoad(IrNode* addr);

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

  /// @brief ロード/ストア対象のアドレスを得る．
  ///
  /// kOpLoad, kOpStore, kOpInc, kOpDec のみ有効
  virtual
  IrNode*
  address() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // アドレス
  IrNode* mAddress;

};

END_NAMESPACE_YM_YMSL

#endif // IRLOAD_H
