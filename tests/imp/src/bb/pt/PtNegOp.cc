
/// @file src/pt/PtNegOp.cc
/// @brief PtNegOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNegOp.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] opr1 オペランド
PtNegOp::PtNegOp(const FileRegion& file_region,
		 PtNode* opr1) :
  PtUnaryOp(file_region, opr1)
{
}

// @brief デストラクタ
PtNegOp::~PtNegOp()
{
}

// @brief 型を返す．
PtNode::tType
PtNegOp::type() const
{
  return kNegOp;
}

// @brief 内容を表す文字列を返す．
string
PtNegOp::decompile() const
{
  return " ~(" + operand(0)->decompile() + ")";
}

// @brief 対応した AIG を作る．
void
PtNegOp::gen_aig(AigMgr& aigmgr,
		 const vector<Aig>& bvar_array,
		 ymuint bw,
		 vector<Aig>& out_array)
{
  operand(0)->gen_aig(aigmgr, bvar_array, bw, out_array);
  for (ymuint i = 0; i < bw; ++ i) {
    out_array[i] = aigmgr.make_not(out_array[i]);
  }
}

END_NAMESPACE_YM_BB
