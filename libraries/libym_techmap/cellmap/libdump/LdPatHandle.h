#ifndef LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDPATHANDLE_H
#define LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDPATHANDLE_H

/// @file libym_techmap/cellmap/libdump/LdPatHandle.h
/// @brief LdPatHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"


BEGIN_NAMESPACE_YM_CELLMAP_LIBDUMP

class LdPatNode;

//////////////////////////////////////////////////////////////////////
/// @class LdPatHandle LdPatHandle.h "LdPatHandle.h"
/// @brief LdPatNode と極性を表すクラス
//////////////////////////////////////////////////////////////////////
class LdPatHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  explicit
  LdPatHandle(LdPatNode* node = NULL,
	      bool inv = false);

  /// @brief デストラクタ
  ~LdPatHandle();


public:

  /// @brief ノードを取り出す．
  LdPatNode*
  node() const;

  /// @brief 反転属性を取り出す．
  bool
  inv() const;

  /// @brief 値を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  void
  set(LdPatNode* node,
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
LdPatHandle::LdPatHandle(LdPatNode* node,
			 bool inv)
{
  set(node, inv);
}

// @brief デストラクタ
inline
LdPatHandle::~LdPatHandle()
{
}

// @brief ノードを取り出す．
inline
LdPatNode*
LdPatHandle::node() const
{
  return reinterpret_cast<LdPatNode*>(mData & ~1UL);
}

// @brief 反転属性を取り出す．
inline
bool
LdPatHandle::inv() const
{
  return static_cast<bool>(mData & 1UL);
}

// @brief 値を設定する．
// @param[in] node ノード
// @param[in] inv 反転属性
inline
void
LdPatHandle::set(LdPatNode* node,
		 bool inv)
{
  // bool に対する & 1UL は不必要だが念のため．
  mData = reinterpret_cast<ympuint>(node) | (static_cast<ympuint>(inv) & 1UL);
}

END_NAMESPACE_YM_CELLMAP_LIBDUMP

#endif // LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDPATHANDLE_H
