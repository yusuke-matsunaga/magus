
/// @file libym_mvn/MvnConcat.cc
/// @brief MvnConcat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConcat.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnConcat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] ibitwidth_array 入力のビット幅の配列
MvnConcat::MvnConcat(MvnModule* module,
		     const vector<ymuint>& ibitwidth_array) :
  MvnNode(module, ibitwidth_array.size(), 1)
{
  ymuint ni = ibitwidth_array.size();
  ymuint obitwidth = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint ibitwidth = ibitwidth_array[i];
    obitwidth += ibitwidth;
    set_ipin_bit_width(i, ibitwidth);
  }
  set_opin_bit_width(0, obitwidth);
}

// @brief デストラクタ
MvnConcat::~MvnConcat()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnConcat::type() const
{
  return kConcat;
}

// @brief concatenate ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] ibitwidth_array 入力のビット幅の配列
// @return 生成したノードを返す．
// @note 出力のビット幅は入力のビット幅の和となる．
MvnNode*
MvnMgr::new_concat(MvnModule* module,
		   const vector<ymuint>& ibitwidth_array)
{
  MvnNode* node = new MvnConcat(module, ibitwidth_array);
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
