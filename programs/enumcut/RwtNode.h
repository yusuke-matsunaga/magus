#ifndef RWTNODE_H
#define RWTNODE_H

/// @file RwtNode.h
/// @brief RwtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class RwtNode RwtNode.h "RwtNode.h"
/// @brief rewriting 用のテンプレートを表すノード
/// ノードの種類は以下のとおり
/// - 定数0ノード
/// - 入力ノード
/// - ANDノード
/// - XORノード
//////////////////////////////////////////////////////////////////////
class RwtNode
{
  friend class RwtMgr;

private:

  /// @brief コンストラクタ
  RwtNode();

  /// @brief デストラクタ
  ~RwtNode();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// 定数0ノードの時に true を返す．
  bool
  is_const0() const;

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const;

  /// @brief ANDノードの時に true を返す．
  bool
  is_and() const;

  /// @brief XORノードの時に true を返す．
  bool
  is_xor() const;

  /// @brief 入力番号を返す．
  /// @note 入力ノードの時のみ意味を持つ．
  ymuint
  input_id() const;

  /// @brief ファンイン0のノードを返す．
  /// @note AND/XORノードの時のみ意味を持つ．
  const RwtNode*
  fanin0_node() const;

  /// @brief ファンイン1のノードを返す．
  /// @note AND/XORノードの時のみ意味を持つ．
  const RwtNode*
  fanin1_node() const;

  /// @brief ファンイン0の極性を返す．
  /// @note AND/XORノードの時のみ意味を持つ．
  bool
  fanin0_inv() const;

  /// @brief ファンイン1の極性を返す．
  /// @note AND/XORノードの時のみ意味を持つ．
  bool
  fanin1_inv() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // ノードの種類などを表す変数
  // 下位2ビットの意味は以下のとおり
  // - 00 : 定数0
  // - 01 : 入力
  // - 10 : AND
  // - 11 : XOR
  // 残りは入力番号
  ymuint32 mType;

  // ファンイン0のポインタ＋極性
  ympuint mFanin0;

  // ファンイン1のポインタ＋極性
  ympuint mFanin1;

};



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
RwtNode::RwtNode()
{
}

// @brief デストラクタ
inline
RwtNode::~RwtNode()
{
}

// @brief ID番号を返す．
inline
ymuint
RwtNode::id() const
{
  return mId;
}

// 定数0ノードの時に true を返す．
inline
bool
RwtNode::is_const0() const
{
  return (mType & 3U) == 0U;
}

// @brief 入力ノードの時に true を返す．
inline
bool
RwtNode::is_input() const
{
  return (mType & 3U) == 1U;
}

// @brief ANDノードの時に true を返す．
inline
bool
RwtNode::is_and() const
{
  return (mType & 3U) == 2U;
}

// @brief XORノードの時に true を返す．
inline
bool
RwtNode::is_xor() const
{
  return (mType & 3U) == 3U;
}

// @brief 入力番号を返す．
// @note 入力ノードの時のみ意味を持つ．
inline
ymuint
RwtNode::input_id() const
{
  return mType >> 2;
}

// @brief ファンイン0のノードを返す．
// @note AND/XORノードの時のみ意味を持つ．
inline
const RwtNode*
RwtNode::fanin0_node() const
{
  return reinterpret_cast<const RwtNode*>(mFanin0 & ~1UL);
}

// @brief ファンイン1のノードを返す．
// @note AND/XORノードの時のみ意味を持つ．
inline
const RwtNode*
RwtNode::fanin1_node() const
{
  return reinterpret_cast<const RwtNode*>(mFanin1 & ~1UL);
}

// @brief ファンイン0の極性を返す．
// @note AND/XORノードの時のみ意味を持つ．
inline
bool
RwtNode::fanin0_inv() const
{
  return static_cast<bool>(mFanin0 & 1UL);
}

// @brief ファンイン1の極性を返す．
// @note AND/XORノードの時のみ意味を持つ．
inline
bool
RwtNode::fanin1_inv() const
{
  return static_cast<bool>(mFanin1 & 1UL);
}

END_NAMESPACE_YM_NETWORKS

#endif // RWTNODE_H
