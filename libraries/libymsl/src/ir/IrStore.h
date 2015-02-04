#ifndef IRSTROE_H
#define IRSTROE_H

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
  /// @param[in] base ベースアドレス
  /// @param[in] offset オフセット
  /// @param[in] val 値
  IrStore(IrNode* base,
	  IrNode* offset,
	  IrNode* val);

  /// @brief デストラクタ
  virtual
  ~IrStore();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ベースアドレスを返す．
  virtual
  IrNode*
  base() const;

  /// @brief オフセットを返す．
  virtual
  IrNode*
  offset() const;

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

  // オフセット
  IrNode* mOffset;

  // 書き込む値
  IrNode* mStoreVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRSTORE_H
