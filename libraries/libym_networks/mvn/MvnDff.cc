
/// @file libym_networks/MvnDff.cc
/// @brief MvnDff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnDff.h"
#include "ym_networks/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnDff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] clock_pol クロックの極性
// @param[in] control_array 非同期セットの極性を入れた配列
MvnDff::MvnDff(MvnModule* module,
	       ymuint clock_pol,
	       const vector<ymuint>& control_array) :
  MvnNodeBase(module, MvnNode::kDff, (control_array.size() * 2) + 2, 1)
{
  ymuint np = control_array.size();

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
MvnDff::~MvnDff()
{
  delete [] mPolArray;
}

// @brief クロック信号の極性を得る．
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
// @note type() が kDff の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnDff::clock_pol() const
{
  return (mPolArray[0] & 1U);
}

// @brief 非同期セット信号の極性を得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
// @note type() が kDff の時のみ意味を持つ．
ymuint
MvnDff::control_pol(ymuint pos) const
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
MvnNode*
MvnMgr::new_dff(MvnModule* module,
		ymuint clock_pol,
		const vector<ymuint>& control_array,
		ymuint bit_width)
{
  MvnNode* node = new MvnDff(module, clock_pol, control_array);
  reg_node(node);

  ymuint np = control_array.size();

  // データ入力
  node->_input(0)->mBitWidth = bit_width;
  // クロック入力
  node->_input(1)->mBitWidth = 1;
  // 非同期セット入力
  for (ymuint i = 0; i < np; ++ i) {
    // コントロール
    node->_input(i * 2 + 2)->mBitWidth = 1;
    // データ
    node->_input(i * 2 + 3)->mBitWidth = bit_width;
  }
  // データ出力
  node->_output(0)->mBitWidth = bit_width;

  return node;
}

END_NAMESPACE_YM_MVN
