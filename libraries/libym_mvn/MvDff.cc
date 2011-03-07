
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
// @param[in] clock_pol クロックの極性
// @param[in] control_array 非同期セットの極性を入れた配列
// @param[in] bit_width ビット幅
MvDff::MvDff(MvModule* module,
	     ymuint clock_pol,
	     const vector<ymuint>& control_array,
	     ymuint bit_width) :
  MvNode(module, control_array.size() + 2, 1)
{
  ymuint np = control_array.size();

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

  ymuint n1 = (np + 32) / 32;
  mPolArray = new ymuint32[n1];
  for (ymuint i = 0; i < n1; ++ i) {
    mPolArray[i] = 0UL;
  }
  mPolArray[0] |= (clock_pol & 1U);
  for (ymuint i = 0; i < np; ++ i) {
    ymuint blk = (i + 1) / 32;
    ymuint sft = (i + 1) % 32;
    if ( control_array[i] ) {
      mPolArray[blk] |= (1UL << sft);
    }
  }
}

// @brief デストラクタ
MvDff::~MvDff()
{
  delete [] mPolArray;
}

// @brief ノードの種類を得る．
MvNode::tType
MvDff::type() const
{
  return kDff;
}

// @brief クロック信号の極性を得る．
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
// @note type() が kDff の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvDff::clock_pol() const
{
  return (mPolArray[0] & 1U);
}

// @brief 非同期セット信号の極性を得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
// @note type() が kDff の時のみ意味を持つ．
ymuint
MvDff::control_pol(ymuint pos) const
{
  ymuint blk = (pos + 1) / 32;
  ymuint sft = (pos + 1) % 32;
  return (mPolArray[blk] >> sft) & 1U;
}

// @brief 非同期セット/リセットタイプの FF ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] control_array 非同期セットの極性を入れた配列
// @param[in] bit_width ビット幅
// @note control_array の要素数が非同期セット信号数となる．
MvNode*
MvMgr::new_dff(MvModule* module,
	       ymuint clock_pol,
	       const vector<ymuint>& control_array,
	       ymuint bit_width)
{
  MvNode* node = new MvDff(module, clock_pol, control_prray, bit_width);
  reg_node(node);

  return node;
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

// @brief ラッチノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
MvNode*
MvMgr::new_latch(MvModule* module,
		 ymuint bit_width)
{
  MvNode* node = new MvLatch(module, bit_width);
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
