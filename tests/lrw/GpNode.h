#ifndef GPNODE_H
#define GPNODE_H

/// @file GpNode.h
/// @brief GpNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

class GpNode;

//////////////////////////////////////////////////////////////////////
/// @class GpHandle GpNode.h "GpNode.h"
/// @brief GpNode + 否定属性の情報を持つハンドルクラス
//////////////////////////////////////////////////////////////////////
class GpHandle
{
  friend class GpMgr;

public:

  /// @brief 空のコンストラクタ
  GpHandle();

  /// @brief 普通のコンストラクタ
  explicit
  GpHandle(GpNode* node,
	   bool inv = false);

  /// @brief デストラクタ
  ~GpHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取り出すメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを取り出す．
  GpNode*
  node() const;

  /// @brief 否定属性を取り出す．
  bool
  inv() const;

  /// @brief 定数0を表しているとき true を返す．
  bool
  is_const0() const;

  /// @brief 定数1を表しているとき true を返す．
  bool
  is_const1() const;

  /// @brief 関数ベクタを取り出す．
  ymuint16
  func() const;

  /// @brief 等価比較演算子
  bool
  operator==(const GpHandle right) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 極性を入れ替える．
  GpHandle
  operator~() const;

  /// @brief 反転属性を取り除く
  /// @return 自分自身を返す．
  GpHandle
  cannonicalize();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  GpHandle(ympuint data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのポインタ + 否定属性
  ympuint mNodePol;

};


//////////////////////////////////////////////////////////////////////
/// @class GpNode GpNode.h "GpNode.h"
/// @brief genpat で用いられるパタンを表すノード
//////////////////////////////////////////////////////////////////////
class GpNode
{
  friend class GpMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] id ノード番号
  GpNode(ymuint id);

  /// @brief デストラクタ
  ~GpNode();


public:

  /// @brief ID 番号を返す．
  ymuint32
  id() const;

  /// @brief レベルを返す．
  ymuint32
  level() const;

  /// @brief サイズを返す．
  /// @note DAG の場合にはレベルより少ない数となる．
  ymuint32
  size() const;

  /// @brief このパタンが表している関数ベクタを返す．
  ymuint16
  func() const;

  /// @brief 入力ノードの時 true を返す．
  bool
  is_input() const;

  /// @brief 論理ノードの時 true を返す．
  bool
  is_logic() const;

  /// @brief AND ノードの時 true を返す．
  bool
  is_and() const;

  /// @brief XOR ノードの時 true を返す．
  bool
  is_xor() const;

  /// @brief 入力番号を返す．
  ymuint32
  input_id() const;

  /// @brief ファンインのハンドルを返す．
  /// @param[in] pos ファンイン位置番号 (0 or 1)
  GpHandle
  fanin(ymuint pos) const;

  /// @brief ファンインを返す．
  /// @param[in] pos ファンイン位置番号 (0 or 1)
  GpNode*
  fanin_node(ymuint pos) const;

  /// @brief ファンインの否定属性を返す．
  /// @param[in] pos ファンイン位置番号 (0 or 1)
  bool
  inv(ymuint pos) const;

  /// @brief ファンイン0のハンドルを返す．
  GpHandle
  fanin0() const;

  /// @brief ファンイン0を返す．
  GpNode*
  fanin0_node() const;

  /// @brief ファンイン0の否定属性を返す．
  bool
  inv0() const;

  /// @brief ファンイン1のハンドルを返す．
  GpHandle
  fanin1() const;

  /// @brief ファンイン1を返す．
  GpNode*
  fanin1_node() const;

  /// @brief ファンイン1の否定属性を返す．
  bool
  inv1() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // レベル
  ymuint32 mLevel;

  // ノード数
  ymuint32 mSize;

  // このパタンが表している関数ベクタ
  ymuint16 mFunc;

  // 入力番号 + α
  ymuint32 mInputId;

  // ファンイン
  GpHandle mFanin[2];

  // 構造ハッシュ用のリンク
  GpNode* mSlink;

};


//////////////////////////////////////////////////////////////////////
// GpHandle のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
GpHandle::GpHandle() :
  mNodePol(0UL)
{
}

// @brief 普通のコンストラクタ
inline
GpHandle::GpHandle(GpNode* node,
		   bool inv) :
  mNodePol(reinterpret_cast<ympuint>(node) | static_cast<ympuint>(inv))
{
}

// @brief 内容を直接指定したコンストラクタ
inline
GpHandle::GpHandle(ympuint data) :
  mNodePol(data)
{
}

// @brief デストラクタ
inline
GpHandle::~GpHandle()
{
}

// @brief ノードを取り出す．
inline
GpNode*
GpHandle::node() const
{
  return reinterpret_cast<GpNode*>(mNodePol & ~1UL);
}

// @brief 否定属性を取り出す．
inline
bool
GpHandle::inv() const
{
  return static_cast<bool>(mNodePol & 1UL);
}

// @brief 定数0を表しているとき true を返す．
inline
bool
GpHandle::is_const0() const
{
  return node() == NULL && !inv();
}

// @brief 定数1を表しているとき true を返す．
inline
bool
GpHandle::is_const1() const
{
  return node() == NULL && inv();
}

// @brief 関数ベクタを取り出す．
inline
ymuint16
GpHandle::func() const
{
  ymuint16 ans = 0U;
  if ( node() != NULL ) {
    ans = node()->func();
  }
  if ( inv() ) {
    ans = ~ans & 0xFFFF;
  }
  return ans;
}

// @brief 等価比較演算子
inline
bool
GpHandle::operator==(const GpHandle right) const
{
  return mNodePol == right.mNodePol;
}

// @brief 極性を入れ替える．
inline
GpHandle
GpHandle::operator~() const
{
  return GpHandle(mNodePol ^ 1UL);
}

// @brief 反転属性を取り除く
// @return 自分自身を返す．
inline
GpHandle
GpHandle::cannonicalize()
{
  mNodePol &= ~1UL;
  return *this;
}


//////////////////////////////////////////////////////////////////////
// GpNode のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 入力ノードを表す
inline
GpNode::GpNode(ymuint id) :
  mId(id),
  mSlink(NULL)
{
}

// デストラクタ
inline
GpNode::~GpNode()
{
}

// ID 番号を返す．
inline
ymuint32
GpNode::id() const
{
  return mId;
}

// @brief レベルを返す．
inline
ymuint32
GpNode::level() const
{
  return mLevel;
}

// @brief サイズを返す．
inline
ymuint32
GpNode::size() const
{
  return mSize;
}

// このパタンが表している関数ベクタを返す．
inline
ymuint16
GpNode::func() const
{
  return mFunc;
}

// 入力ノードの時 true を返す．
inline
bool
GpNode::is_input() const
{
  return static_cast<bool>((mInputId & 3U) == 0U);
}

// @brief 論理ノードの時 true を返す．
inline
bool
GpNode::is_logic() const
{
  return static_cast<bool>((mInputId & 2U) == 2U);
}

// @brief AND ノードの時 true を返す．
inline
bool
GpNode::is_and() const
{
  return static_cast<bool>((mInputId & 3U) == 2U);
}

// @brief XOR ノードの時 true を返す．
inline
bool
GpNode::is_xor() const
{
  return static_cast<bool>((mInputId & 3U) == 3U);
}

// 入力番号を返す．
inline
ymuint32
GpNode::input_id() const
{
  return (mInputId >> 4);
}

// @brief ファンインのハンドルを返す．
// @param[in] pos ファンイン位置番号 (0 or 1)
inline
GpHandle
GpNode::fanin(ymuint pos) const
{
  return mFanin[pos];
}

// ファンインを返す．
inline
GpNode*
GpNode::fanin_node(ymuint pos) const
{
  return fanin(pos).node();
}

// ファンインの否定属性を返す．
inline
bool
GpNode::inv(ymuint pos) const
{
  return fanin(pos).inv();
}

// @brief ファンイン0のハンドルを返す．
inline
GpHandle
GpNode::fanin0() const
{
  return mFanin[0];
}

// ファンイン0を返す．
inline
GpNode*
GpNode::fanin0_node() const
{
  return fanin0().node();
}

// ファンイン0の否定属性を返す．
inline
bool
GpNode::inv0() const
{
  return fanin0().inv();
}

// @brief ファンイン0のハンドルを返す．
inline
GpHandle
GpNode::fanin1() const
{
  return mFanin[1];
}

// ファンイン1を返す．
inline
GpNode*
GpNode::fanin1_node() const
{
  return fanin1().node();
}

// ファンイン0の否定属性を返す．
inline
bool
GpNode::inv1() const
{
  return fanin1().inv();
}

END_NAMESPACE_YM

#endif // GPNODE_H
