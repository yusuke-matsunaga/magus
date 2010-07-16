
/// @file libym_mvn/MvConst.cc
/// @brief MvConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvConst.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
// @param[in] val 値
MvConst::MvConst(MvModule* module,
		 ymuint bit_width,
		 const vector<ymuint32>& val) :
  MvNode(module, 0, 1),
  mVal(val)
{
  set_bit_width(_output(0), bit_width);
}

// @brief デストラクタ
MvConst::~MvConst()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvConst::type() const
{
  return kConst;
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kConst の時のみ意味を持つ．
void
MvConst::const_value(vector<ymuint32>& val) const
{
  val = mVal;
}

END_NAMESPACE_YM_MVN
