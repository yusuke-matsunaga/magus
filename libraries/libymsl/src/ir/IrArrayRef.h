#ifndef IRARRAYREF_H
#define IRARRAYREF_H

/// @file IrArrayRef.h
/// @brief IrArrayRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrArrayRef IrArrayRef.h "IrArrayRef.h"
/// @brief 配列参照を表すノード
//////////////////////////////////////////////////////////////////////
class IrArrayRef :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] base ベースアドレス
  /// @param[in] offset オフセット
  IrArrayRef(IrNode* base,
	     IrNode* offset);

  /// @brief デストラクタ
  virtual
  ~IrArrayRef();


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
  /// kOpArrayRef のみ有効
  virtual
  IrNode*
  array_expr() const;

  /// @brief 配列のインデックスを返す．
  ///
  /// kOpArrayRef のみ有効
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

#endif // IRARRAYREF_H
