#ifndef IRSTORE_H
#define IRSTORE_H

/// @file IrStore.h
/// @brief IrStore のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrStore IrStore.h "IrStore.h"
/// @brief ストア命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrStore :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] addr アドレス
  /// @param[in] val 値
  IrStore(IrHandle* addr,
	  IrNode* val);

  /// @brief デストラクタ
  virtual
  ~IrStore();


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
  IrHandle*
  address() const;

  /// @brief 書き込む値を返す．
  virtual
  IrNode*
  store_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // アドレス
  IrHandle* mAddress;

  // 書き込む値
  IrNode* mStoreVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRSTORE_H
