
/// @file src/pt/PtUminusOp.cc
/// @brief PtUminusOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtUminusOp.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] opr1 オペランド
PtUminusOp::PtUminusOp(const FileRegion& file_region,
		       PtNode* opr1) :
  PtUnaryOp(file_region, opr1)
{
}

// @brief デストラクタ
PtUminusOp::~PtUminusOp()
{
}

// @brief 型を返す．
PtNode::tType
PtUminusOp::type() const
{
  return kUminusOp;
}

// @brief 内容を表す文字列を返す．
string
PtUminusOp::decompile() const
{
  return " - (" + operand(0)->decompile() + ")";
}

// @brief 対応した AIG を作る．
void
PtUminusOp::gen_aig(AigMgr& aigmgr,
		    const vector<Aig>& bvar_array,
		    ymuint bw,
		    vector<Aig>& out_array)
{
  vector<Aig> tmp_array(bw);
  operand(0)->gen_aig(aigmgr, bvar_array, bw, tmp_array);
  Aig cin = aigmgr.make_one();
  for (ymuint i = 0; i < bw; ++ i) {
    out_array[i] = aigmgr.make_xor(tmp_array[i], cin);
    cin = aigmgr.make_and(tmp_array[i], cin);
  }
}

END_NAMESPACE_YM_BB
