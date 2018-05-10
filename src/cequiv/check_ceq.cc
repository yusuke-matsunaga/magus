
/// @file check_ceq.cc
/// @brief 組み合わせ回路の検証を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "cec_nsdef.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "FraigMgr.h"
#include "Bn2FraigConv.h"


BEGIN_NAMESPACE_YM_CEC

// 新しい関数
void
check_ceq(const BnNetwork& src_network1,
	  const BnNetwork& src_network2,
	  const vector<pair<ymuint, ymuint> >& iassoc,
	  const vector<pair<ymuint, ymuint> >& oassoc,
	  ymint log_level,
	  ostream* log_out,
	  const string& sat_type,
	  const string& sat_option,
	  ostream* sat_out,
	  ymuint sigsize,
	  vector<SatBool3>& stats)
{
  FraigMgr fraig_mgr(sigsize, sat_type, sat_option, sat_out);

  // オプションを設定する．
  fraig_mgr.set_loglevel(log_level);
  fraig_mgr.set_logstream(log_out);

  ymuint ni = iassoc.size();
  ymuint ni1 = src_network1.input_num();
  ymuint ni2 = src_network2.input_num();
  ASSERT_COND( ni1 == ni );
  ASSERT_COND( ni2 == ni );

  vector<FraigHandle> inputs(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    inputs[i] = fraig_mgr.make_input();
  }

  // src_network1 に対応する Fraig を作る．
  Bn2FraigConv conv1(fraig_mgr);
  {
    vector<FraigHandle> inputs1(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      inputs1[i] = inputs[iassoc[i].first];
    }
    conv1.convert(src_network1, inputs1);
  }

  // src_network2 に対応する Fraig を作る．
  Bn2FraigConv conv2(fraig_mgr);
  {
    vector<FraigHandle> inputs2(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      inputs2[i] = inputs[iassoc[i].second];
    }
    conv2.convert(src_network2, inputs2);
  }

  ymuint no = oassoc.size();
  ymuint no1 = src_network1.output_num();
  ymuint no2 = src_network2.output_num();
  ASSERT_COND( no1 == no );
  ASSERT_COND( no2 == no );

  stats.resize(no);
  for (ymuint i = 0; i < no; ++ i) {
    if ( log_level > 2 ) {
      (*log_out) << "Checking Output#" << (i + 1) << " / " << no << endl;
    }

    const BnNode* onode1 = src_network1.output(oassoc[i].first);
    FraigHandle h1 = conv1.get_handle(onode1->id());

    const BnNode* onode2 = src_network2.output(oassoc[i].second);
    FraigHandle h2 = conv2.get_handle(onode2->id());

    if ( h1 == h2 ) {
      stats[i] = SatBool3::True;
    }
    else {
      stats[i] = fraig_mgr.check_equiv(h1, h2);
    }
  }

  if ( log_level > 1 ) {
    fraig_mgr.dump_stats(*log_out);
  }
}

END_NAMESPACE_YM_CEC
