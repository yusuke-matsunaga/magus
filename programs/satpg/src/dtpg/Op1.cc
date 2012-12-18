
/// @file atpg/src/dtpg/Op1.cc
/// @brief Op1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "Op1.h"
#include "FaultMgr.h"
#include "DtpgNode.h"
#include "DtpgFault.h"
#include "TvMgr.h"
#include "TestVector.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
// クラス Op1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Op1::Op1(FaultMgr& fmgr,
	 TvMgr& tvmgr,
	 bool skip) :
  mFaultMgr(fmgr),
  mTvMgr(tvmgr),
  mSkip(skip)
{
}

// @brief デストラクタ
Op1::~Op1()
{
}

// @brief テストパタンが見つかった場合に呼ばれる関数
void
Op1::set_detected(DtpgFault* f,
		  const vector<DtpgNode*>& input_list,
		  const vector<Bool3>& model)
{
  TestVector* tv = mTvMgr.new_vector();
  tv->init();
  for (vector<DtpgNode*>::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    DtpgNode* node = *p;
    VarId idx = node->gvar();
    ymuint iid = node->input_id();
    // 今のところ model には 0 か 1 しか設定されていないはず．
    if ( model[idx.val()] == kB3True ) {
      tv->set_val(iid, kVal1);
    }
    else if ( model[idx.val()] == kB3False ) {
      tv->set_val(iid, kVal0);
    }
  }
  f->set_skip();
  mFaultMgr.set_status(f->safault(), kFsDetected);
}

// @brief 検出不能のときに呼ばれる関数
void
Op1::set_untestable(DtpgFault* f)
{
  if ( mSkip ) {
    f->set_skip();
  }
  mFaultMgr.set_status(f->safault(), kFsUntestable);
}

END_NAMESPACE_YM_SATPG_DTPG
