#ifndef NETWORKS_BDNNODEHANDLE_H
#define NETWORKS_BDNNODEHANDLE_H

/// @file YmNetworks/BdnNodeHandle.h
/// @brief BdnNodeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnNodeHandle BdnNode.h "YmNetworks/BdnNode.h"
/// @brief BdnNode へのポインタと極性を合わせたデータを表すクラス
/// @sa BdnNode
//////////////////////////////////////////////////////////////////////
class BdnNodeHandle
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  /// @note エラー値に設定される．
  BdnNodeHandle();

  /// @brief 内容を設定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  explicit
  BdnNodeHandle(BdnNode* node,
		bool inv = false);

  /// @brief デストラクタ
  ~BdnNodeHandle();

  /// @brief 内容を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  void
  set(BdnNode* node,
      bool inv);

  /// @brief エラー値を返す．
  static
  BdnNodeHandle
  make_error();

  /// @brief 定数0を返す．
  static
  BdnNodeHandle
  make_zero();

  /// @brief 定数1を返す．
  static
  BdnNodeHandle
  make_one();


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取り出すメンバ間数
  //////////////////////////////////////////////////////////////////////

  /// @brief 極性を否定したハンドルを返す．
  BdnNodeHandle
  operator~() const;

  /// @brief ノードを得る．
  BdnNode*
  node() const;

  /// @brief 極性を得る．
  /// @return 反転している時 true を返す．
  bool
  inv() const;

  /// @brief エラー値を表している時 true を返す．
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
  operator==(BdnNodeHandle right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  BdnNodeHandle(ympuint data);


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
BdnNodeHandle::BdnNodeHandle() :
  mData(0UL)
{
}

// @brief 内容を設定したコンストラクタ
// @param[in] node ノード
// @param[in] inv 反転している時に true とするフラグ
inline
BdnNodeHandle::BdnNodeHandle(BdnNode* node,
			     bool inv) :
  mData(reinterpret_cast<ympuint>(node) | inv)
{
}

// @brief 内容を直接指定したコンストラクタ
inline
BdnNodeHandle::BdnNodeHandle(ympuint data) :
  mData(data)
{
}

// @brief デストラクタ
inline
BdnNodeHandle::~BdnNodeHandle()
{
}

// @brief 内容を設定する．
// @param[in] node ノード
// @param[in] inv 反転している時に true とするフラグ
inline
void
BdnNodeHandle::set(BdnNode* node,
		   bool inv)
{
  mData = reinterpret_cast<ympuint>(node) | inv;
}

// @brief エラー値を返す．
inline
BdnNodeHandle
BdnNodeHandle::make_error()
{
  return BdnNodeHandle(static_cast<ympuint>(0UL));
}

// @brief 定数0を返す．
inline
BdnNodeHandle
BdnNodeHandle::make_zero()
{
  return BdnNodeHandle(static_cast<ympuint>(2UL));
}

// @brief 定数1を返す．
inline
BdnNodeHandle
BdnNodeHandle::make_one()
{
  return BdnNodeHandle(static_cast<ympuint>(3UL));
}

// @brief 極性を否定したハンドルを返す．
inline
BdnNodeHandle
BdnNodeHandle::operator~() const
{
  ympuint val = mData;
  if ( val != 0UL ) {
    val ^= 1UL;
  }
  return BdnNodeHandle(val);
}

// @brief ノードを得る．
inline
BdnNode*
BdnNodeHandle::node() const
{
  return reinterpret_cast<BdnNode*>(mData & ~3UL);
}

// @brief 極性を得る．
// @return 反転している時 true を返す．
inline
bool
BdnNodeHandle::inv() const
{
  return static_cast<bool>(mData & 1UL);
}

// @brief エラー値を表している時 true を返す．
inline
bool
BdnNodeHandle::is_error() const
{
  return mData == 0UL;
}

// @brief 定数0を指しているとき true を返す．
inline
bool
BdnNodeHandle::is_zero() const
{
  return mData == 2UL;
}

// @brief 定数1を指しているとき true を返す．
inline
bool
BdnNodeHandle::is_one() const
{
  return mData == 3UL;
}

// @brief 定数を指しているとき true を返す．
inline
bool
BdnNodeHandle::is_const() const
{
  return (mData & 2UL) == 2UL;
}

// @brief 等価比較演算子
inline
bool
BdnNodeHandle::operator==(BdnNodeHandle right) const
{
  return mData == right.mData;
}

inline
bool
operator!=(BdnNodeHandle left,
	   BdnNodeHandle right)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_NETWORKS_BDN

#endif // NETWORKS_BDNNODE_H
