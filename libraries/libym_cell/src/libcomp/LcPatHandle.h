#ifndef LCPATHANDLE_H
#define LCPATHANDLE_H

/// @file LcPatHandle.h
/// @brief LcPatHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

class LcPatNode;

//////////////////////////////////////////////////////////////////////
/// @class LcPatHandle LcPatHandle.h "LcPatHandle.h"
/// @brief LcPatNode と極性を表すクラス
//////////////////////////////////////////////////////////////////////
class LcPatHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  explicit
  LcPatHandle(LcPatNode* node = NULL,
	      bool inv = false);

  /// @brief デストラクタ
  ~LcPatHandle();


public:

  /// @brief ノードを取り出す．
  LcPatNode*
  node() const;

  /// @brief 反転属性を取り出す．
  bool
  inv() const;

  /// @brief 値を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  void
  set(LcPatNode* node,
      bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタと反転属性を合わせたもの
  ympuint mData;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node ノード
// @param[in] inv 反転属性
inline
LcPatHandle::LcPatHandle(LcPatNode* node,
			 bool inv)
{
  set(node, inv);
}

// @brief デストラクタ
inline
LcPatHandle::~LcPatHandle()
{
}

// @brief ノードを取り出す．
inline
LcPatNode*
LcPatHandle::node() const
{
  return reinterpret_cast<LcPatNode*>(mData & ~1UL);
}

// @brief 反転属性を取り出す．
inline
bool
LcPatHandle::inv() const
{
  return static_cast<bool>(mData & 1UL);
}

// @brief 値を設定する．
// @param[in] node ノード
// @param[in] inv 反転属性
inline
void
LcPatHandle::set(LcPatNode* node,
		 bool inv)
{
  // bool に対する & 1UL は不必要だが念のため．
  mData = reinterpret_cast<ympuint>(node) | (static_cast<ympuint>(inv) & 1UL);
}

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCPATHANDLE_H
