
/// @file libym_mvn/MvDff.cc
/// @brief MvDff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvDff.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvDff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
// @param[in] np 非同期セット入力ピン数
MvDff::MvDff(MvModule* module,
	     ymuint bit_width,
	     ymuint np) :
  MvNode(module, np + 2, 1)
{
  // データ入力
  set_ipin_bit_width(0, bit_width);
  // クロック入力
  set_ipin_bit_width(1, 1);
  // 非同期セット入力
  for (ymuint i = 0; i < np; ++ i) {
    set_ipin_bit_width(i + 2, 1);
  }
  // データ出力
  set_opin_bit_width(0, bit_width);
}

// @brief デストラクタ
MvDff::~MvDff()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvDff::type() const
{
  return kDff;
}


//////////////////////////////////////////////////////////////////////
// クラス MvLatch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvLatch::MvLatch(MvModule* module,
		 ymuint bit_width) :
  MvNode(module, 2, 1)
{
  // データ入力
  set_ipin_bit_width(0, bit_width);
  // イネーブル入力
  set_ipin_bit_width(1, 1);
  // データ出力
  set_opin_bit_width(0, bit_width);
}

// @brief デストラクタ
MvLatch::~MvLatch()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvLatch::type() const
{
  return kLatch;
}

END_NAMESPACE_YM_MVN
