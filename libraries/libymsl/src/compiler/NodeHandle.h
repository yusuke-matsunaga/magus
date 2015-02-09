#ifndef NODEHANDLE_H
#define NODEHANDLE_H

/// @file NodeHandle.h
/// @brief NodeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "SymHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class NodeHandle NodeHandle.h "NodeHandle.h"
/// @brief ノードを保持する SymHandle
//////////////////////////////////////////////////////////////////////
class NodeHandle :
  public SymHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] node ノード
  NodeHandle(ShString name,
	     IrNode* node);

  /// @brief デストラクタ
  virtual
  ~NodeHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief ノードを返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  IrNode*
  node() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // ノード
  IrNode* mNode;

};

END_NAMESPACE_YM_YMSL

#endif // NODEHANDLE_H
