#ifndef NETWORKS_BDNCONSTNODEHANDLE_H
#define NETWORKS_BDNCONSTNODEHANDLE_H

/// @file YmNetworks/BdnConstNodeHandle.h
/// @brief BdnConstNodeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnConstNodeHandle BdnNode.h "YmNetworks/BdnNode.h"
/// @brief BdnNode へのポインタと極性を合わせたデータを表すクラス
/// @sa BdnNode
//////////////////////////////////////////////////////////////////////
class BdnConstNodeHandle
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  BdnConstNodeHandle();

  /// @brief 内容を設定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  explicit
  BdnConstNodeHandle(const BdnNode* node,
		     bool inv = false);

  /// @brief デストラクタ
  ~BdnConstNodeHandle();

  /// @brief 内容を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  void
  set(const BdnNode* node,
      bool inv);

  /// @brief エラーハンドルを返す．
  static
  BdnConstNodeHandle
  make_error();

  /// @brief 定数0を返す．
  static
  BdnConstNodeHandle
  make_zero();

  /// @brief 定数1を返す．
  static
  BdnConstNodeHandle
  make_one();


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取り出すメンバ間数
  //////////////////////////////////////////////////////////////////////

  /// @brief 極性を否定したハンドルを返す．
  BdnConstNodeHandle
  operator~() const;

  /// @brief ノードを得る．
  const BdnNode*
  node() const;

  /// @brief 極性を得る．
  /// @return 反転している時 true を返す．
  bool
  inv() const;

  /// @brief エラーハンドルを指しているとき true を返す．
  bool
  is_error() const;

  /// @brief 定数0を指しているとき true を返す．
  bool
  is_zero() const;

  /// @brief 定数1を指しているとき true を返す．
  bool
  is_one() const;

  /// @brief 定数を指しているとき true を返す．
  bool
  is_const() const;

  /// @brief 等価比較演算子
  bool
  operator==(BdnConstNodeHandle right) const;


private:

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  BdnConstNodeHandle(ympuint data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタ＋極性を収めたデータ本体
  ympuint mData;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
BdnConstNodeHandle::BdnConstNodeHandle() :
  mData(0UL)
{
}

// @brief 内容を設定したコンストラクタ
// @param[in] node ノード
// @param[in] inv 反転している時に true とするフラグ
inline
BdnConstNodeHandle::BdnConstNodeHandle(const BdnNode* node,
				       bool inv) :
  mData(reinterpret_cast<ympuint>(node) | inv)
{
}

// @brief 内容を直接指定したコンストラクタ
inline
BdnConstNodeHandle::BdnConstNodeHandle(ympuint data) :
  mData(data)
{
}

// @brief デストラクタ
inline
BdnConstNodeHandle::~BdnConstNodeHandle()
{
}

// @brief 内容を設定する．
// @param[in] node ノード
// @param[in] inv 反転している時に true とするフラグ
inline
void
BdnConstNodeHandle::set(const BdnNode* node,
			bool inv)
{
  mData = reinterpret_cast<ympuint>(node) | inv;
}

// @brief エラーハンドルを返す．
inline
BdnConstNodeHandle
BdnConstNodeHandle::make_error()
{
  return BdnConstNodeHandle(static_cast<ympuint>(0UL));
}

// @brief 定数0を返す．
inline
BdnConstNodeHandle
BdnConstNodeHandle::make_zero()
{
  return BdnConstNodeHandle(static_cast<ympuint>(2UL));
}

// @brief 定数1を返す．
inline
BdnConstNodeHandle
BdnConstNodeHandle::make_one()
{
  return BdnConstNodeHandle(static_cast<ympuint>(3UL));
}

// @brief 極性を否定したハンドルを返す．
inline
BdnConstNodeHandle
BdnConstNodeHandle::operator~() const
{
  ympuint val = mData;
  if ( val != 0UL ) {
    val ^= 1UL;
  }
  return BdnConstNodeHandle(val);
}

// @brief ノードを得る．
inline
const BdnNode*
BdnConstNodeHandle::node() const
{
  return reinterpret_cast<BdnNode*>(mData & ~1UL);
}

// @brief 極性を得る．
// @return 反転している時 true を返す．
inline
bool
BdnConstNodeHandle::inv() const
{
  return static_cast<bool>(mData & 1UL);
}

// @brief エラーハンドルを指しているとき true を返す．
inline
bool
BdnConstNodeHandle::is_error() const
{
  return mData == 0UL;
}

// @brief 定数0を指しているとき true を返す．
inline
bool
BdnConstNodeHandle::is_zero() const
{
  return mData == 2UL;
}

// @brief 定数1を指しているとき true を返す．
inline
bool
BdnConstNodeHandle::is_one() const
{
  return mData == 3UL;
}

// @brief 定数を指しているとき true を返す．
inline
bool
BdnConstNodeHandle::is_const() const
{
  return (mData & ~1UL) == 0UL;
}

// @brief 等価比較演算子
inline
bool
BdnConstNodeHandle::operator==(BdnConstNodeHandle right) const
{
  return mData == right.mData;
}

inline
bool
operator!=(BdnConstNodeHandle left,
	   BdnConstNodeHandle right)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_NETWORKS_BDN

#endif // NETWORKS_BDNCONSTNODEHANDLE_H
