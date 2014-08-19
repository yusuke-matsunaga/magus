
/// @file src/pt/PtModOp.cc
/// @brief PtModOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtModOp.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] opr1, opr2 オペランド
PtModOp::PtModOp(const FileRegion& file_region,
		 PtNode* opr1,
		 PtNode* opr2) :
  PtBinaryOp(file_region, opr1, opr2)
{
}

// @brief デストラクタ
PtModOp::~PtModOp()
{
}

// @brief 型を返す．
PtNode::tType
PtModOp::type() const
{
  return kModOp;
}

// @brief 内容を表す文字列を返す．
string
PtModOp::decompile() const
{
  return operand(0)->decompile() + " % " + operand(1)->decompile();
}

// @brief 対応した AIG を作る．
void
PtModOp::gen_aig(AigMgr& aigmgr,
		 const vector<Aig>& bvar_array,
		 ymuint bw,
		 vector<Aig>& out_array)
{
  vector<Aig> tmp_array1(bw);
  vector<Aig> tmp_array2(bw);
  operand(0)->gen_aig(aigmgr, bvar_array, bw, tmp_array1);
  operand(1)->gen_aig(aigmgr, bvar_array, bw, tmp_array2);
  ymuint b = 0;
  ymuint c = 0;
  bool error = false;
  for (ymuint i = 0; i < bw; ++ i) {
    if ( tmp_array2[i].is_one() ) {
      ++ c;
      if ( c > 1 ) {
	error = true;
	break;
      }
      b = i;
    }
    else if ( !tmp_array2[i].is_zero() ) {
      error = true;
      break;
    }
  }
  if ( error ) {
    cerr << "Sorry, cannot handle" << endl;
    for (ymuint i = 0; i < bw; ++ i) {
      cerr << "tmp_array2[" << i << "] = " << tmp_array2[i] << endl;
    }
  }
  else {
    // ここまできたら 0 〜 (b - 1) が1
    //                b 〜 が0 のパタン
    for (ymuint i = 0; i < bw; ++ i) {
      if ( i < b ) {
	out_array[i] = tmp_array1[i];
      }
      else {
	out_array[i] = aigmgr.make_zero();
      }
    }
  }
}

END_NAMESPACE_YM_BB
