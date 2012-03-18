
/// @file src/pt/PtSllOp.cc
/// @brief PtSllOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtSllOp.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] opr1, opr2 オペランド
PtSllOp::PtSllOp(const FileRegion& file_region,
		 PtNode* opr1,
		 PtNode* opr2) :
  PtBinaryOp(file_region, opr1, opr2)
{
}

// @brief デストラクタ
PtSllOp::~PtSllOp()
{
}

// @brief 型を返す．
PtNode::tType
PtSllOp::type() const
{
  return kSllOp;
}

// @brief 内容を表す文字列を返す．
string
PtSllOp::decompile() const
{
  return operand(0)->decompile() + " << " + operand(1)->decompile();
}

// @brief 対応した AIG を作る．
void
PtSllOp::gen_aig(AigMgr& aigmgr,
		 const vector<Aig>& bvar_array,
		 ymuint bw,
		 vector<Aig>& out_array)
{
  vector<Aig> tmp_array1(bw);
  vector<Aig> tmp_array2(bw);
  operand(0)->gen_aig(aigmgr, bvar_array, bw, tmp_array1);
  operand(1)->gen_aig(aigmgr, bvar_array, bw, tmp_array2);
  Aig cin = aigmgr.make_zero();
  vector<Aig> tmp_array3(3);
  for (ymuint i = 0; i < bw; ++ i) {
    Aig a = tmp_array1[i];
    Aig b = tmp_array2[i];
    tmp_array3[0] = a;
    tmp_array3[1] = b;
    tmp_array3[2] = cin;
    out_array[i] = aigmgr.make_xor(tmp_array3);
    tmp_array3[0] = aigmgr.make_and(a, b);
    tmp_array3[1] = aigmgr.make_and(a, cin);
    tmp_array3[2] = aigmgr.make_and(b, cin);
    cin = aigmgr.make_or(tmp_array3);
  }
}

END_NAMESPACE_YM_BB
