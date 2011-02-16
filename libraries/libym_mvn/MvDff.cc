
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
// @param[in] control_array 非同期セットの極性と値を入れた配列
MvDff::MvDff(MvModule* module,
	     ymuint bit_width,
	     ymuint np,
	     const vector<ymuint32>& control_array) :
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

  ymuint n1 = (np + 31) / 32;
  mPolArray = new ymuint32[n1];
  for (ymuint i = 0; i < n1; ++ i) {
    mPolArray[i] = 0UL;
  }
  ymuint n2 = (bit_width + 31) / 32;
  mValArray = new ymuint32[np * n2];
  for (ymuint i = 0; i < np * n2; ++ i) {
    mValArray[i] = 0UL;
  }
  for (ymuint i = 0; i < np; ++ i) {
    ymuint src_offset = i * (n2 + 1);
    ymuint blk = (i + 31) / 32;
    ymuint sft = i % 32;
    if ( control_array[i * (n2 + 1)] ) {
      mPolArray[blk] |= (1UL << sft);
    }
    ymuint offset = i * n2;
    for (ymuint j = 0; j < n2; ++ j) {
      mValArray[j + offset] = control_array[j + src_offset + 1];
    }
  }
}

// @brief デストラクタ
MvDff::~MvDff()
{
  delete [] mPolArray;
  delete [] mValArray;
}

// @brief ノードの種類を得る．
MvNode::tType
MvDff::type() const
{
  return kDff;
}

// @brief 非同期セット信号の極性を得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
// @note type() が kDff の時のみ意味を持つ．
ymuint
MvDff::control_pol(ymuint pos) const
{
  ymuint blk = pos / 32;
  ymuint sft = pos % 32;
  return (mPolArray[blk] >> sft) & 1U;
}

// @brief 非同期セット信号のセット値を得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @param[out] val 値を格納するベクタ
// @note type() が kDff の時のみ意味を持つ．
void
MvDff::control_value(ymuint pos,
		     vector<ymuint32>& val) const
{
  ymuint nw = (output(0)->bit_width() + 31) / 32;
  val.resize(nw);
  ymuint offset = pos * nw;
  for (ymuint i = 0; i < nw; ++ i) {
    val[i] = mValArray[i + offset];
  }
}

// @brief 非同期セット/リセットタイプの FF ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] np 非同期セットの制御信号数
// @param[in] control_array 非同期セットの極性と値を入れた配列
// @param[in] bit_width ビット幅
MvNode*
MvMgr::new_dff(MvModule* module,
	       ymuint np,
	       const vector<ymuint32>& control_array,
	       ymuint bit_width)
{
  MvNode* node = new MvDff(module, bit_width, np, control_array);
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
