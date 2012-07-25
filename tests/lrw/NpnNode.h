#ifndef NPNNODE_H
#define NPNNODE_H

/// @file NpnNode.h
/// @brief NpnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "NpnXform.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class NpnHandle NpnNode.h "NpnNode.h"
/// @brief NpnNode と NPN変換の組を表すクラス
//////////////////////////////////////////////////////////////////////
class NpnHandle
{
  friend class NpnNode;
  friend class NpnNodeMgr;

private:

  /// @brief コンストラクタ
  NpnHandle();

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] id ノード番号
  /// @param[in] xf NPN変換
  NpnHandle(ymuint id,
	    NpnXform xf);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  /// @param[in] id ノード番号
  /// @param[in] xf NPN変換
  void
  set(ymuint id,
      NpnXform xf);

  /// @brief ノード番号を返す．
  ymuint
  node_id() const;

  /// @brief NPN変換を返す．
  NpnXform
  npn_xform() const;

  /// @brief 出力の反転属性を得る．
  bool
  oinv() const;

  /// @brief ハッシュ用の値を返す．
  ymuint
  hash() const;

  /// @brief 出力を反転したハンドルを返す．
  NpnHandle
  operator~() const;

  /// @brief NPN変換を施す．
  /// @param[in] xf 変換
  /// @return 結果を返す．
  NpnHandle
  operator*(NpnXform xf) const;

  /// @brief 等価比較
  friend
  bool
  operator==(NpnHandle left,
	     NpnHandle right);

  /// @brief 大小比較
  friend
  bool
  operator<(NpnHandle left,
	    NpnHandle right);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  NpnHandle(ymuint32 data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  ymuint32 mData;

};

/// @brief 等価比較
/// @param[in] left, right オペランド
bool
operator==(NpnHandle left,
	   NpnHandle right);

/// @brief 非等価比較
/// @param[in] left, right オペランド
bool
operator!=(NpnHandle left,
	   NpnHandle right);

/// @brief 大小比較 ( < )
/// @param[in] left, right オペランド
bool
operator<(NpnHandle left,
	  NpnHandle right);

/// @brief 大小比較 ( > )
/// @param[in] left, right オペランド
bool
operator>(NpnHandle left,
	  NpnHandle right);

/// @brief 大小比較 ( <= )
/// @param[in] left, right オペランド
bool
operator<=(NpnHandle left,
	   NpnHandle right);

/// @brief 大小比較 ( >= )
/// @param[in] left, right オペランド
bool
operator>=(NpnHandle left,
	   NpnHandle right);

/// @brief ストリーム出力
ostream&
operator<<(ostream& s,
	   NpnHandle handle);


//////////////////////////////////////////////////////////////////////
/// @class NpnNode NpnNode.h "NpnNode.h"
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

  /// @brief ボリュームを返す．
  ymuint
  volume() const;

  /// @brief このノードが表す関数ベクタを返す．
  ymuint16
  func() const;

  /// @brief このノードが表す関数のサポートを返す．
  /// @note サポートは 0 から 15 までの符号なし整数
  /// @note i ビット目が 1 なら i 番めの変数を含んでいる．
  ymuint
  support() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号 + ノードタイプ(下位2ビット)
  ymuint32 mId;

  // ボリューム
  ymuint8 mVolume;

  // サポート
  ymuint8 mSupport;

  // このノードが表している関数ベクタ
  ymuint16 mFunc;

  // ファンイン
  NpnHandle mFanin[2];

  // 構造ハッシュ用のリンク
  NpnNode* mSlink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
NpnHandle::NpnHandle()
{
}

// @brief 内容を指定したコンストラクタ
// @param[in] id ノード番号
// @param[in] xf NPN変換
inline
NpnHandle::NpnHandle(ymuint id,
		     NpnXform xf)
{
  set(id, xf);
}

// @brief 内容を直接指定したコンストラクタ
inline
NpnHandle::NpnHandle(ymuint32 data) :
  mData(data)
{
}

// @brief 内容を設定する．
// @param[in] id ノード番号
// @param[in] xf NPN変換
inline
void
NpnHandle::set(ymuint id,
	       NpnXform xf)
{
  mData = (id << 10) | xf.data();
}

// @brief ノード番号を返す．
inline
ymuint
NpnHandle::node_id() const
{
  return mData >> 10;
}

// @brief NPN変換を返す．
inline
NpnXform
NpnHandle::npn_xform() const
{
  return NpnXform(mData & 1023U);
}

// @brief 出力の反転属性を得る．
inline
bool
NpnHandle::oinv() const
{
  return static_cast<bool>(mData & 1U);
}

// @brief 出力を反転したハンドルを返す．
inline
NpnHandle
NpnHandle::operator~() const
{
  return NpnHandle(mData ^ 1U);
}

// @brief NPN変換を施す．
// @param[in] xf 変換
// @return 結果を返す．
inline
NpnHandle
NpnHandle::operator*(NpnXform xf) const
{
  ymuint id = node_id();
  NpnXform xf0 = npn_xform();
  return NpnHandle(id, xf0 * xf);
}

// @brief ハッシュ用の値を返す．
inline
ymuint
NpnHandle::hash() const
{
  return mData;
}

// @brief 等価比較
inline
bool
operator==(NpnHandle left,
	   NpnHandle right)
{
  return left.mData == right.mData;
}

// @brief 非等価比較
inline
bool
operator!=(NpnHandle left,
	   NpnHandle right)
{
  return !operator==(left, right);
}

// @brief 大小比較 ( < )
inline
bool
operator<(NpnHandle left,
	  NpnHandle right)
{
  return left.mData < right.mData;
}

// @brief 大小比較 ( > )
inline
bool
operator>(NpnHandle left,
	  NpnHandle right)
{
  return operator<(right, left);
}

// @brief 大小比較 ( <= )
inline
bool
operator<=(NpnHandle left,
	   NpnHandle right)
{
  return !operator<(right, left);
}

// @brief 大小比較 ( >= )
inline
bool
operator>=(NpnHandle left,
	   NpnHandle right)
{
  return !operator<(left, right);
}

// @brief ストリーム出力
inline
ostream&
operator<<(ostream& s,
	   NpnHandle handle)
{
  ymuint id = handle.node_id();
  NpnXform xf = handle.npn_xform();
  s << xf << ": NODE#" << id;
  return s;
}

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

// @brief ボリュームを返す．
inline
ymuint
NpnNode::volume() const
{
  return mVolume;
}

// @brief このノードが表す関数ベクタを返す．
inline
ymuint16
NpnNode::func() const
{
  return mFunc;
}

// @brief このノードが表す関数のサポートを返す．
// @note サポートは 0 から 15 までの符号なし整数
// @note i ビット目が 1 なら i 番めの変数を含んでいる．
inline
ymuint
NpnNode::support() const
{
  return mSupport;
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

END_NAMESPACE_YM

#endif // NPNNODE_H
