
/// @file RandHasGen.cc
/// @brief RandHashGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "RandHashGen.h"
#include "XorFunc.h"
#include "utils/RandCombiGen.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス RandHashGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RandHashGen::RandHashGen()
{
}

// @brief デストラクタ
RandHashGen::~RandHashGen()
{
}

// @brief ランダムにハッシュ関数を作る．
// @param[in] input_num 入力数
// @param[in] output_num 出力数
// @param[in] max_degree
InputFunc*
RandHashGen::gen_func(ymuint input_num,
		      ymuint output_num,
		      ymuint max_degree)
{
  vector<vector<ymuint> > f1_vect(output_num);

  RandCombiGen rcg1(input_num, output_num);
  rcg1.generate(mRandGen);

  for (ymuint opos = 0; opos < output_num; ++ opos) {
    vector<ymuint>& var_vect = f1_vect[opos];
    ymuint pos0 = rcg1.elem(opos);
    var_vect.push_back(pos0);

    if ( max_degree > 1 ) {
      ymuint mask = (1U << (max_degree - 1)) - 1;
      ymuint bit_pat = mRandGen.int32() % mask;
      ymuint nbit = 0;
      for (ymuint k = 0; k < max_degree; ++ k) {
	if ( bit_pat & (1U << k) ) {
	  ++ nbit;
	}
      }
      if ( nbit > 0 ) {
	RandCombiGen rcg2(input_num - 1, nbit);
	vector<ymuint> pos_list;
	pos_list.reserve(input_num - 1);
	for (ymuint k = 0; k < input_num; ++ k) {
	  if ( k != pos0 ) {
	    pos_list.push_back(k);
	  }
	}
	rcg2.generate(mRandGen);
	for (ymuint k = 0; k < nbit; ++ k) {
	  ymuint pos = pos_list[rcg2.elem(k)];
	  var_vect.push_back(pos);
	}
      }
    }
  }
  return new XorFunc(f1_vect);
}

END_NAMESPACE_YM_IGF
