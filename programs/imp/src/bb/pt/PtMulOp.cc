
/// @file src/pt/PtMulOp.cc
/// @brief PtMulOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtMulOp.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] opr1, opr2 オペランド
PtMulOp::PtMulOp(const FileRegion& file_region,
		 PtNode* opr1,
		 PtNode* opr2) :
  PtBinaryOp(file_region, opr1, opr2)
{
}

// @brief デストラクタ
PtMulOp::~PtMulOp()
{
}

// @brief 型を返す．
PtNode::tType
PtMulOp::type() const
{
  return kMulOp;
}

// @brief 内容を表す文字列を返す．
string
PtMulOp::decompile() const
{
  return operand(0)->decompile() + " * " + operand(1)->decompile();
}

// @brief 対応した AIG を作る．
void
PtMulOp::gen_aig(AigMgr& aigmgr,
		 const vector<Aig>& bvar_array,
		 ymuint bw,
		 vector<Aig>& out_array)
{
  ASSERT_COND( out_array.size() == bw );
  vector<Aig> tmp_array1(bw);
  vector<Aig> tmp_array2(bw);
  operand(0)->gen_aig(aigmgr, bvar_array, bw, tmp_array1);
  operand(1)->gen_aig(aigmgr, bvar_array, bw, tmp_array2);

  vector<Aig> pp_array(bw * bw);
  for (ymuint i = 0; i < bw; ++ i) {
    for (ymuint j = 0; j < bw; ++ j) {
      pp_array[i * bw + j] = aigmgr.make_and(tmp_array1[j], tmp_array2[i]);
    }
  }

  vector<list<Aig> > tmp_array(bw);
  for (ymuint i = 0; i < bw; ++ i) {
    for (ymuint j = 0; j <= i; ++ j) {
      Aig tmp = pp_array[(i - j) * bw + j];
      if ( !tmp.is_zero() ) {
	tmp_array[i].push_back(tmp);
      }
    }
  }
  for ( ; ; ) {
    bool changed = false;
    for (ymuint i = 0; i < bw; ++ i) {
      list<Aig>& tmp_list = tmp_array[i];
      if ( tmp_list.size() < 3 ) {
	continue;
      }
      changed = true;
      vector<Aig> tmp3(3);
      Aig p1 = tmp3[0] = tmp_list.front();
      tmp_list.pop_front();
      Aig p2 = tmp3[1] = tmp_list.front();
      tmp_list.pop_front();
      Aig p3 = tmp3[2] = tmp_list.front();
      tmp_list.pop_front();
      Aig tmp = aigmgr.make_xor(tmp3);
      tmp_list.push_back(tmp);
      if ( i < bw - 1 ) {
	tmp3[0] = aigmgr.make_and(p1, p2);
	tmp3[1] = aigmgr.make_and(p1, p3);
	tmp3[2] = aigmgr.make_and(p2, p3);
	tmp = aigmgr.make_or(tmp3);
	tmp_array[i + 1].push_back(tmp);
      }
    }
    if ( !changed ) {
      break;
    }
  }

  vector<Aig> tmp3(3);
  Aig cin = aigmgr.make_zero();
  for (ymuint i = 0; i < bw; ++ i) {
    list<Aig>& tmp_list = tmp_array[i];
    list<Aig>::iterator p = tmp_list.begin();
    Aig a;
    Aig b;
    if ( p == tmp_list.end() ) {
      a = aigmgr.make_zero();
      b = aigmgr.make_zero();
    }
    else {
      a = *p;
      ++ p;
      if ( p == tmp_list.end() ) {
	b = aigmgr.make_zero();
      }
      else {
	b = *p;
      }
    }
    tmp3[0] = a;
    tmp3[1] = b;
    tmp3[2] = cin;
    out_array[i] = aigmgr.make_xor(tmp3);
    tmp3[0] = aigmgr.make_and(a, b);
    tmp3[1] = aigmgr.make_and(a, cin);
    tmp3[2] = aigmgr.make_and(b, cin);
    cin = aigmgr.make_or(tmp3);
  }
}

END_NAMESPACE_YM_BB
