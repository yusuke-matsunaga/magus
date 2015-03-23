
/// @file GbmConfVarMgr.cc
/// @brief GbmConfVarMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmConfVarMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmConfVarMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmConfVarMgr::GbmConfVarMgr()
  mDebug(false)
{
}

// @brief デストラクタ
GbmConfVarMgr::~GbmConfVarMgr()
{
}

// @brief debug フラグを立てる
void
GbmConfVarMgr::debug_on()
{
  mDebug = true;
}

// @brief debug フラグを降ろす
void
GbmConfVarMgr::debug_off()
{
  mDebug = false;
}

// @brief debug フラグの値を得る．
bool
GbmConfVarMgr::debug() const
{
  return mDebug;
}

// @brief 設定変数を初期化する．
// @param[in] network 対象のネットワーク
void
GbmConfVarMgr::init_conf_vars(const RcfNetwork& network)
{
  ymuint nc = network.conf_num();
  mConfVarArray.clear();
  mConfVarArray.resize(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = new_var();
    mConfVarArray[i] = vid;
    if ( debug() ) {
      cout << "conf_bits[" << i << "] = " << vid << endl;
    }
  }
}

// @brief ノードに対応するリテラルを登録する．
// @param[in] id ノード番号
// @param[in] lit リテラル
void
GbmConfVarMgr::set_node_var(ymuint id,
			    GbmLit lit)
{
  ASSERT_COND( id < mNodeVarArray.size() );
  mNodeVarArray[id] = lit;
  if ( debug() ) {
    cout << "Node#" << id << ": " << lit << endl;
  }
}

// @brief SAT用の新しい変数を作る．
VarId
GbmConfVarMgr::new_var()
{
  return mSolver.new_var();
}

// @brief SAT モデルから設定変数の割り当てを取り出す．
// @param[in] model SAT モデル
// @param[out] conf_bits 設定変数の割り当て
void
GbmConfVarMgr::get_conf_bits(const vector<Bool3>& model,
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

END_NAMESPACE_YM
