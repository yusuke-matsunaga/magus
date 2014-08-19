
/// @file src/pt/PtAndOp.cc
/// @brief PtAndOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtAndOp.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] opr1, opr2 オペランド
PtAndOp::PtAndOp(const FileRegion& file_region,
		 PtNode* opr1,
		 PtNode* opr2) :
  PtBinaryOp(file_region, opr1, opr2)
{
}

// @brief デストラクタ
PtAndOp::~PtAndOp()
{
}

// @brief 型を返す．
PtNode::tType
PtAndOp::type() const
{
  return kAndOp;
}

// @brief 内容を表す文字列を返す．
string
PtAndOp::decompile() const
{
  return operand(0)->decompile() + " & " + operand(1)->decompile();
}

// @brief 対応した AIG を作る．
void
PtAndOp::gen_aig(AigMgr& aigmgr,
		 const vector<Aig>& bvar_array,
		 ymuint bw,
		 vector<Aig>& out_array)
{
  vector<Aig> tmp_array1(bw);
  vector<Aig> tmp_array2(bw);
  operand(0)->gen_aig(aigmgr, bvar_array, bw, tmp_array1);
  operand(1)->gen_aig(aigmgr, bvar_array, bw, tmp_array2);
  for (ymuint i = 0; i < bw; ++ i) {
    Aig a = tmp_array1[i];
    Aig b = tmp_array2[i];
    out_array[i] = aigmgr.make_and(a, b);
  }
}

END_NAMESPACE_YM_BB
