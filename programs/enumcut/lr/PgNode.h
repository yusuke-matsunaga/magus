#ifndef PGNODE_H
#define PGNODE_H

/// @file PgNode.h
/// @brief PgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class PgNode PgNode.h "PgNode.h"
/// @brief RwtNode を生成するための元データ
/// ほぼ RwtNode ど同様のインターフェイスを持つ．
/// 違いはハッシュ表のためのリンクポインタを持つということ．
//////////////////////////////////////////////////////////////////////
class PgNode
{
  friend class RwtPatGen;

private:

  /// @brief コンストラクタ
  PgNode();

  /// @brief デストラクタ
  ~PgNode();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 型の情報を返す．
  ymuint
  type() const;

  /// @brief 定数0ノードの時に true を返す．
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

  /// @brief 論理関数を返す．
  TvFunc
  function() const;

  /// @brief ファンイン0の情報をエンコードした値を返す．
  /// @note AND/XORノードの時のみ意味を持つ．
  ymuint32
  fanin0() const;

  /// @brief ファンイン1の情報をエンコードした値を返す．
  /// @note AND/XORノードの時のみ意味を持つ．
  ymuint32
  fanin1() const;

  /// @brief ファンイン0のノード番号を返す．
  /// @note AND/XORノードの時のみ意味を持つ．
  ymuint
  fanin0_id() const;

  /// @brief ファンイン1のノード番号を返す．
  /// @note AND/XORノードの時のみ意味を持つ．
  ymuint
  fanin1_id() const;

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

  // 論理関数
  TvFunc mFunc;

  // ノードの種類などを表す変数
  // 下位2ビットの意味は以下のとおり
  // - 00 : 定数0
  // - 01 : 入力
  // - 10 : AND
  // - 11 : XOR
  // 残りは入力番号
  ymuint32 mType;

  // ファンイン0のID番号＋極性
  ymuint32 mFanin0;

  // ファンイン1のID番号＋極性
  ymuint32 mFanin1;

  // ハッシュ表のためのリンクポインタ
  PgNode* mLink;

};



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
PgNode::PgNode()
{
}

// @brief デストラクタ
inline
PgNode::~PgNode()
{
}

// @brief ID番号を返す．
inline
ymuint
PgNode::id() const
{
  return mId;
}

// @brief 型の情報を返す．
inline
ymuint
PgNode::type() const
{
  return mType;
}

// @brief 定数0ノードの時に true を返す．
inline
bool
PgNode::is_const0() const
{
  return (mType & 3U) == 0U;
}

// @brief 入力ノードの時に true を返す．
inline
bool
PgNode::is_input() const
{
  return (mType & 3U) == 1U;
}

// @brief ANDノードの時に true を返す．
inline
bool
PgNode::is_and() const
{
  return (mType & 3U) == 2U;
}

// @brief XORノードの時に true を返す．
inline
bool
PgNode::is_xor() const
{
  return (mType & 3U) == 3U;
}

// @brief 入力番号を返す．
// @note 入力ノードの時のみ意味を持つ．
inline
ymuint
PgNode::input_id() const
{
  return mType >> 2;
}

// @brief 論理関数を返す．
inline
TvFunc
PgNode::function() const
{
  return mFunc;
}

// @brief ファンイン0の情報をエンコードした値を返す．
// @note AND/XORノードの時のみ意味を持つ．
inline
ymuint32
PgNode::fanin0() const
{
  return mFanin0;
}

// @brief ファンイン1の情報をエンコードした値を返す．
// @note AND/XORノードの時のみ意味を持つ．
inline
ymuint32
PgNode::fanin1() const
{
  return mFanin1;
}

// @brief ファンイン0のノード番号を返す．
// @note AND/XORノードの時のみ意味を持つ．
inline
ymuint
PgNode::fanin0_id() const
{
  return mFanin0 >> 1;
}

// @brief ファンイン1のノード番号を返す．
// @note AND/XORノードの時のみ意味を持つ．
inline
ymuint
PgNode::fanin1_id() const
{
  return mFanin1 >> 1;
}

// @brief ファンイン0の極性を返す．
// @note AND/XORノードの時のみ意味を持つ．
inline
bool
PgNode::fanin0_inv() const
{
  return static_cast<bool>(mFanin0 & 1UL);
}

// @brief ファンイン1の極性を返す．
// @note AND/XORノードの時のみ意味を持つ．
inline
bool
PgNode::fanin1_inv() const
{
  return static_cast<bool>(mFanin1 & 1UL);
}

END_NAMESPACE_YM_NETWORKS

#endif // PGNODE_H
