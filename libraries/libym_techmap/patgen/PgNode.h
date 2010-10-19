#ifndef LIBYM_TECHMAP_PATGEN_PGNODE_H
#define LIBYM_TECHMAP_PATGEN_PGNODE_H

/// @file libym_techmap/patgen/PgNode.h
/// @brief PgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

class PgNodeMgr;

//////////////////////////////////////////////////////////////////////
/// @class PgNode PgNode.h "PgNode.h"
/// @brief パタン生成用のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class PgNode
{
  friend class PgNodeMgr;

private:

  /// @brief コンストラクタ
  PgNode();

  /// @brief デストラクタ
  ~PgNode();


public:

  /// @brief ノード番号を返す．
  ymuint
  id() const;

  /// @brief 入力の時 true を返す．
  bool
  is_input() const;

  /// @brief AND の時 true を返す．
  bool
  is_and() const;

  /// @brief XOR の時 true を返す．
  bool
  is_xor() const;

  /// @brief 入力の時に入力番号を返す．
  ymuint
  input_id() const;

  /// @brief AND/XOR の時にファンインのノードを返す．
  /// @param[in] pos 位置 ( 0 or 1 )
  PgNode*
  fanin(ymuint pos) const;

  /// @brief AND/XOR の時にファンインの極性を返す．
  /// @param[in] pos 位置 ( 0 or 1 )
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  fanin_inv(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号
  ymuint32 mId;

  // ノードの種類 ( 2bit )
  //  + 入力番号 ( 30bit)
  // or
  //  + ファンインの極性 ( 1bit x 2)
  ymuint32 mType;

  // ファンインのノード
  PgNode* mFanin[2];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード番号を返す．
ymuint
PgNode::id() const
{
  return mId;
}

// @brief 入力の時 true を返す．
inline
bool
PgNode::is_input() const
{
  return ((mType & 3U) == 0U);
}

// @brief AND の時 true を返す．
inline
bool
PgNode::is_and() const
{
  return ((mType & 3U) == 2U);
}

// @brief XOR の時 true を返す．
inline
bool
PgNode::is_xor() const
{
  return ((mType & 3U) == 3U);
}

// @brief 入力の時に入力番号を返す．
inline
ymuint
PgNode::input_id() const
{
  return mType >> 2;
}

// @brief AND/XOR の時にファンインのノードを返す．
// @param[in] pos 位置 ( 0 or 1 )
inline
PgNode*
PgNode::fanin(ymuint pos) const
{
  return mFanin[pos];
}

// @brief AND/XOR の時にファンインの極性を返す．
// @param[in] pos 位置 ( 0 or 1 )
// @retval true 反転あり
// @retval false 反転なし
inline
bool
PgNode::fanin_inv(ymuint pos) const
{
  return static_cast<bool>((mType >> (pos + 2)) & 1U);
}

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGNODE_H
