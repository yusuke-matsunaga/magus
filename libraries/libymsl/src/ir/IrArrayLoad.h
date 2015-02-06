#ifndef IRARRAYLOAD_H
#define IRARRAYLOAD_H

/// @file IrArrayLoad.h
/// @brief IrArrayLoad のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrArrayLoad IrArrayLoad.h "IrArrayLoad.h"
/// @brief 配列用のロード命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrArrayLoad :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] base ベースアドレス
  /// @param[in] offset オフセット
  IrArrayLoad(IrNode* base,
	      IrNode* offset);

  /// @brief デストラクタ
  virtual
  ~IrArrayLoad();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベースアドレス
  IrNode* mBase;

  // オフセット
  IrNode* mOffset;

};

END_NAMESPACE_YM_YMSL

#endif // IRARRAYLOAD_H
