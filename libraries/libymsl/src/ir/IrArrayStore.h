#ifndef IRARRAYSTORE_H
#define IRARRAYSTORE_H

/// @file IrArrayStore.h
/// @brief IrArrayStore のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrArrayStore IrArrayStore.h "IrArrayStore.h"
/// @brief 配列用のストア命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrArrayStore :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] base ベースアドレス
  /// @param[in] offset オフセット
  /// @param[in] val 値
  IrArrayStore(IrNode* base,
	       IrNode* offset,
	       IrNode* val);

  /// @brief デストラクタ
  virtual
  ~IrArrayStore();


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

  /// @brief 配列本体の式を返す．
  ///
  /// kOpArrayLoad, kOpArrayStore のみ有効
  virtual
  IrNode*
  array_expr() const;

  /// @brief 配列のインデックスを返す．
  ///
  /// kOpArrayLoad, kOpArrayStore のみ有効
  virtual
  IrNode*
  array_index() const;

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

#endif // IRARRAYSTORE_H
