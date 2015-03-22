
/// @file DtpgSatBaseS.cc
/// @brief DtpgSatBaseS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatBaseS.h"

#include "TpgNode.h"
#include "TpgFault.h"
#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
DtpgSatBaseS::DtpgSatBaseS(const string& sat_type,
			   const string& sat_option,
			   ostream* sat_outp,
			   const TpgNetwork& network,
			   BackTracer& bt,
			   DetectOp& dop,
			   UntestOp& uop) :
  DtpgSat(sat_type, sat_option, sat_outp, network, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatBaseS::~DtpgSatBaseS()
{
}

// @brief テスト生成を行なう．
// @param[in] flist 対象の故障リスト
void
DtpgSatBaseS::run_multi(const vector<TpgFault*>& flist)
{
  for (vector<TpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    TpgFault* f = *p;
    run_single(f);
  }
}

END_NAMESPACE_YM_SATPG
