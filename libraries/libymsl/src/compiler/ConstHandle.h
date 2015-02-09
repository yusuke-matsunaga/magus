#ifndef CONSTHANDLE_H
#define CONSTHANDLE_H

/// @file ConstHandle.h
/// @brief ConstHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class ConstHandle ConstHandle.h "ConstHandle.h"
/// @brief 定数値を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class ConstHandle :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] const_node 定数ノード
  ConstHandle(ShString name,
	      IrNode* const_node);

  /// @brief デストラクタ
  virtual
  ~ConstHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief 種類を返す．
  virtual
  HandleType
  handle_type() const;

  /// @brief 定数ノードを返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  IrNode*
  constant() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 定数ノード
  IrNode* mConstNode;

};

END_NAMESPACE_YM_YMSL

#endif // CONSTHANDLE_H
