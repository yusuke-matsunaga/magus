
/// @file MvnDff.cc
/// @brief MvnDff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MvnDff.h"
#include "networks/MvnMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnDff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] clock_pol クロックの極性
// @param[in] pol_array 非同期セット信号の極性情報を入れた配列
// @param[in] val_array 非同期セットの値を入れた配列
MvnDff::MvnDff(MvnModule* module,
	       ymuint clock_pol,
	       const vector<ymuint>& pol_array,
	       const vector<MvnNode*>& val_array) :
  MvnNodeBase(module, MvnNode::kDff, pol_array.size() + 2)
{
  ymuint np = pol_array.size();

  ymuint n1 = (np + 32) / 32;
  mPolArray = new ymuint32[n1];
  for (ymuint i = 0; i < n1; ++ i) {
    mPolArray[i] = 0UL;
  }
  mPolArray[0] |= (clock_pol & 1U);
  mValArray = new MvnNode*[np];
  for (ymuint i = 0; i < np; ++ i) {
    ymuint blk = (i + 1) / 32;
    ymuint sft = (i + 1) % 32;
    if ( pol_array[i] ) {
      mPolArray[blk] |= (1UL << sft);
    }
    mValArray[i] = val_array[i];
  }
}

// @brief デストラクタ
MvnDff::~MvnDff()
{
  delete [] mPolArray;
  delete [] mValArray;
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

// @brief 非同期セットの値を表す定数ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @note デフォルトの実装では NULL を返す．
const MvnNode*
MvnDff::control_val(ymuint pos) const
{
  return mValArray[pos];
}

// @brief 非同期セット/リセットタイプの FF ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] pol_array 非同期セット信号の極性情報を入れた配列
// @param[in] val_array 非同期セットの値を入れた配列
// @param[in] bit_width ビット幅
// @note control_array の要素数が非同期セット信号数となる．
MvnNode*
MvnMgr::new_dff(MvnModule* module,
		ymuint clock_pol,
		const vector<ymuint>& pol_array,
		const vector<MvnNode*>& val_array,
		ymuint bit_width)
{
  MvnDff* node = new MvnDff(module, clock_pol, pol_array, val_array);
  reg_node(node);

  ymuint np = pol_array.size();

  // データ入力
  node->_input(0)->mBitWidth = bit_width;
  // クロック入力
  node->_input(1)->mBitWidth = 1;
  // 非同期セット入力
  for (ymuint i = 0; i < np; ++ i) {
    node->_input(i + 2)->mBitWidth = 1;
  }
  // データ出力
  node->mBitWidth = bit_width;

  return node;
}

END_NAMESPACE_YM_NETWORKS_MVN
