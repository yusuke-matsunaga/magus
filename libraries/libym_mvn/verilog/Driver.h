#ifndef LIBYM_MVN_VERILOG_DRIVER_H
#define LIBYM_MVN_VERILOG_DRIVER_H

/// @file libym_mvn/verilog/Driver.h
/// @brief Driver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class Driver Driver.h "Driver.h"
/// @brief ドライバーを表すクラス
//////////////////////////////////////////////////////////////////////
class Driver
{
public:

  /// @brief 単純な形式のコンストラクタ
  /// @param[in] node 右辺のノード
  Driver(MvNode* node);

  /// @brief ビット指定形式のコンストラクタ
  /// @param[in] node 右辺のノード
  /// @param[in] index ビット指定位置
  Driver(MvNode* node,
	 ymuint index);

  /// @brief 範囲指定形式のコンストラクタ
  /// @param[in] node 右辺のノード
  /// @param[in] msb 範囲指定の MSB
  /// @param[in] lsb 範囲指定の LSB
  Driver(MvNode* node,
	 ymuint msb,
	 ymuint lsb);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  Driver(const Driver& src);

  /// @brief デストラクタ
  ~Driver();


public:

  /// @brief 右辺のノードを返す．
  MvNode*
  rhs_node() const;

  /// @brief 単純な形式の時 true を返す．
  bool
  is_simple() const;

  /// @brief ビット指定形式の時 true を返す．
  bool
  has_bitselect() const;

  /// @brief 範囲指定形式の時 true を返す．
  bool
  has_partselect() const;

  /// @brief ビット指定位置を返す．
  /// @note has_bitselect() == true の時のみ意味を持つ．
  ymuint
  index() const;

  /// @brief 範囲指定の MSB を返す．
  /// @note has_partselect() == true の時のみ意味を持つ．
  ymuint
  msb() const;

  /// @brief 範囲指定の LSB を返す．
  /// @note has_partselect() == true の時のみ意味を持つ．
  ymuint
  lsb() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 右辺のノード
  MvNode* mNode;

  // ビット指定位置 or 範囲指定の MSB
  ymuint32 mMsb;

  // 範囲指定の LSB
  ymuint32 mLsb;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 単純な形式のコンストラクタ
// @param[in] node 右辺のノード
inline
Driver::Driver(MvNode* node) :
  mNode(node),
  mMsb(0U),
  mLsb(0U)
{
}

// @brief ビット指定形式のコンストラクタ
// @param[in] node 右辺のノード
// @param[in] index ビット指定位置
inline
Driver::Driver(MvNode* node,
	       ymuint index) :
  mNode(node),
  mMsb((index << 1) | 1U),
  mLsb(0U)
{
}

// @brief 範囲指定形式のコンストラクタ
// @param[in] node 右辺のノード
// @param[in] msb 範囲指定の MSB
// @param[in] lsb 範囲指定の LSB
inline
Driver::Driver(MvNode* node,
	       ymuint msb,
	       ymuint lsb) :
  mNode(node),
  mMsb((msb << 1)),
  mLsb((lsb << 1) | 1U)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
Driver::Driver(const Driver& src) :
  mNode(src.mNode),
  mMsb(src.mMsb),
  mLsb(src.mLsb)
{
}

// @brief デストラクタ
inline
Driver::~Driver()
{
}

// @brief 右辺のノードを返す．
inline
MvNode*
Driver::rhs_node() const
{
  return mNode;
}

// @brief 単純な形式の時 true を返す．
inline
bool
Driver::is_simple() const
{
  return mMsb == 0U && mLsb == 0U;
}

// @brief ビット指定形式の時 true を返す．
inline
bool
Driver::has_bitselect() const
{
  return static_cast<bool>(mMsb & 1U);
}

// @brief 範囲指定形式の時 true を返す．
inline
bool
Driver::has_partselect() const
{
  return static_cast<bool>(mLsb & 1U);
}

// @brief ビット指定位置を返す．
// @note has_bitselect() == true の時のみ意味を持つ．
inline
ymuint
Driver::index() const
{
  return msb();
}

// @brief 範囲指定の MSB を返す．
// @note has_partselect() == true の時のみ意味を持つ．
inline
ymuint
Driver::msb() const
{
  return mMsb >> 1;
}

// @brief 範囲指定の LSB を返す．
// @note has_partselect() == true の時のみ意味を持つ．
inline
ymuint
Driver::lsb() const
{
  return mLsb >> 1;
}

END_NAMESPACE_YM_MVN_VERILOG

#endif // LIBYM_MVN_VERILOG_DRIVER_H
