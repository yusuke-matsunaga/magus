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

class PatGen;

//////////////////////////////////////////////////////////////////////
/// @class PgNode PgNode.h "PgNode.h"
/// @brief パタン生成用のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class PgNode
{
  friend class PatGen;

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
/// @class PgHandle PgNode.h "PgNode.h"
/// @brief PgNode と極性を表すクラス
//////////////////////////////////////////////////////////////////////
class PgHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  explicit
  PgHandle(PgNode* node = NULL,
	   bool inv = false);

  /// @brief デストラクタ
  ~PgHandle();


public:

  /// @brief ノードを取り出す．
  PgNode*
  node() const;

  /// @brief 反転属性を取り出す．
  bool
  inv() const;

  /// @brief 値を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  void
  set(PgNode* node,
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

// @brief ノード番号を返す．
inline
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

// @brief コンストラクタ
// @param[in] node ノード
// @param[in] inv 反転属性
inline
PgHandle::PgHandle(PgNode* node,
		   bool inv)
{
  set(node, inv);
}

// @brief デストラクタ
inline
PgHandle::~PgHandle()
{
}

// @brief ノードを取り出す．
inline
PgNode*
PgHandle::node() const
{
  return reinterpret_cast<PgNode*>(mData & ~1UL);
}

// @brief 反転属性を取り出す．
inline
bool
PgHandle::inv() const
{
  return static_cast<bool>(mData & 1UL);
}

// @brief 値を設定する．
// @param[in] node ノード
// @param[in] inv 反転属性
inline
void
PgHandle::set(PgNode* node,
	      bool inv)
{
  // bool に対する & 1UL は不必要だが念のため．
  mData = reinterpret_cast<ympuint>(node) | (static_cast<ympuint>(inv) & 1UL);
}

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGNODE_H
