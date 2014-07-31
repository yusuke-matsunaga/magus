#ifndef NPNNODE_H
#define NPNNODE_H

/// @file NpnNode.h
/// @brief NpnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "NpnXform.h"
#include "NpnHandle.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class NpnNode NpnNode.h "NpnNode.h"
/// @brief Npn-DAG のノード
//////////////////////////////////////////////////////////////////////
class NpnNode
{
  friend class NpnNodeMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] id ノード番号
  NpnNode(ymuint id);


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief このノードが表す関数ベクタを返す．
  ymuint16
  func() const;

  /// @brief このノードが表す関数のサポートベクタを返す．
  ymuint
  sup_vect() const;

  /// @brief このノードが表す関数の XOR サポートベクタを返す．
  ymuint
  xorsup_vect() const;

  /// @brief 定数ノードの時 true を返す．
  bool
  is_const() const;

  /// @brief 入力ノードの時 true を返す．
  bool
  is_input() const;

  /// @brief 論理ノードの時 true を返す．
  bool
  is_logic() const;

  /// @brief ANDノードの時 true を返す．
  bool
  is_and() const;

  /// @brief XORノードの時 true を返す．
  bool
  is_xor() const;

  /// @brief ファンイン0のハンドルを返す．
  NpnHandle
  fanin0() const;

  /// @brief ファンイン1のハンドルを返す．
  NpnHandle
  fanin1() const;

  /// @brief 同位体変換のリストを返す．
  const vector<NpnXform>&
  ident_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号 + ノードタイプ(下位2ビット)
  ymuint32 mId;

  // サポートベクタ
  ymuint8 mSupVect;

  // XORサポートベクタ
  ymuint8 mXorSupVect;

  // このノードが表している関数ベクタ
  ymuint16 mFunc;

  // ファンインのハンドル
  NpnHandle mFanin[2];

  // 同位体変換のリスト
  vector<NpnXform> mIdentList;

  // 構造ハッシュ用のリンク
  NpnNode* mSlink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ノード番号
inline
NpnNode::NpnNode(ymuint id) :
  mId(id << 2)
{
}

// @brief ID番号を返す．
inline
ymuint
NpnNode::id() const
{
  return mId >> 2;
}

// @brief このノードが表す関数ベクタを返す．
inline
ymuint16
NpnNode::func() const
{
  return mFunc;
}

// @brief このノードが表す関数のサポートベクタを返す．
inline
ymuint
NpnNode::sup_vect() const
{
  return mSupVect;
}

// @brief このノードが表す関数の XOR サポートベクタを返す．
inline
ymuint
NpnNode::xorsup_vect() const
{
  return mXorSupVect;
}

// @brief 定数ノードの時 true を返す．
inline
bool
NpnNode::is_const() const
{
  return (mId & 3U) == 0U;
}

// @brief 入力ノードの時 true を返す．
inline
bool
NpnNode::is_input() const
{
  return (mId & 3U) == 1U;
}

// @brief 論理ノードの時 true を返す．
inline
bool
NpnNode::is_logic() const
{
  return (mId & 2U) == 2U;
}

// @brief ANDノードの時 true を返す．
inline
bool
NpnNode::is_and() const
{
  return (mId & 3U) == 2U;
}

// @brief XORノードの時 true を返す．
inline
bool
NpnNode::is_xor() const
{
  return (mId & 3U) == 3U;
}

// @brief ファンイン0のハンドルを返す．
inline
NpnHandle
NpnNode::fanin0() const
{
  return mFanin[0];
}

// @brief ファンイン1のハンドルを返す．
inline
NpnHandle
NpnNode::fanin1() const
{
  return mFanin[1];
}

// @brief 同位体変換のリストを返す．
inline
const vector<NpnXform>&
NpnNode::ident_list() const
{
  return mIdentList;
}

END_NAMESPACE_YM

#endif // NPNNODE_H
