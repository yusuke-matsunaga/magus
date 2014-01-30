
/// @file GbmBddEngine.cc
/// @brief GbmBddEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmBddEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmBddEngine
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr BddMgr
// @param[in] conf_num 設定変数の数
// @param[in] input_num 入力数
// @param[in] rep 関数の対称変数の代表番号を収める配列
GbmBddEngine::GbmBddEngine(BddMgr& mgr,
			   ymuint conf_num,
			   ymuint input_num,
			   const vector<ymuint>& rep) :
  mConfVarArray(conf_num),
  mInputNum(input_num),
  mRep(rep),
  mIorderVarArray(input_num * input_num)
{
}

// @brief デストラクタ
GbmBddEngine::~GbmBddEngine()
{
}

// @brief 対称性を考慮して初期解を作る．
void
GbmBddEngine::init_vars()
{
  // 最初に入力順を表す変数を確保する．
  for (ymuint i = 0; i < mInputNum; ++ i) {
    for (ymuint j = 0; j < mInputNum; ++ j) {
      VarId vid(i * mInputNum + j);
      mIorderVarArray[i * mInputNum + j] = vid;
      bool stat = mMgr.new_var(vid);
      assert_cond( stat, __FILE__, __LINE__);
    }
  }

  // 次に configuration bit 用の変数を確保する．
  ymuint base = mInputNum * mInputNum;
  for (ymuint i = 0; i < mConfVarArray.size(); ++ i) {
    VarId vid(base + i);
    mConvVarArray[i] = vid;
    bool stat = mMgr.new_var(vid);
    assert_cond( stat, __FILE__, __LINE__);
  }
}

// @brief 入力値を割り当てて解の候補を求める．
// @param[in] network 対象の LUT ネットワーク
// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
// @param[in] oid 出力のノード番号
// @param[in] oval 出力の値
// @param[out] model モデル
// @return 結果が空でなければ true を返し，model にその1つを収める．
bool
GbmBddEngine::make_bdd(const RcfNetwork& network,
		       ymuint bitpat,
		       ymuint oid,
		       bool oval,
		       vector<Bool3>& model)
{
}

// @brief SAT モデルから設定変数の割り当てを取り出す．
// @param[in] model SAT モデル
// @param[out] conf_bits 設定変数の割り当て
void
GbmBddEngine::get_conf_bits(const vector<Bool3>& model,
			    vector<bool>& conf_bits) const
{
  ymuint nc = mConfVarArray.size();
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = mConfVarArray[i];
    if ( model[vid.val()] == kB3True ) {
      conf_bits[i] = true;
    }
    else {
      conf_bits[i] = false;
    }
  }
}

// @brief SAT モデルから入力順を取り出す．
// @param[in] model SAT モデル
// @param[out] iorder 入力順
//             iorder[pos] に network の pos 番めの入力に対応した
//             関数の入力番号が入る．
void
GbmBddEngine::get_iorder(const vector<Bool3>& model,
			 vector<ymuint>& iorder) const
{
  ymuint ni = iorder.size();
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint base = i * ni;
    for (ymuint j = 0; j < ni; ++ j) {
      VarId vid = mIorderVarArray[base + j];
      if ( model[vid.val()] == kB3True ) {
	iorder[i] = j;
	break;
      }
    }
  }
}

END_NAMESPACE_YM
