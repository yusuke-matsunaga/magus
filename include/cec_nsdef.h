#ifndef YM_CEC_CEC_NSDEF
#define YM_CEC_CEC_NSDEF

/// @file ym_cec/cec_nsdef.h
/// @brief cec の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"
#include "ym/ym_bnet.h"
#include "ym/SatBool3.h"


// 名前空間の定義
#define BEGIN_NAMESPACE_YM_CEC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsCec)

#define END_NAMESPACE_YM_CEC \
END_NAMESPACE(nsCec) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_CEC

/// @brief 組み合わせ回路の等価検証を行う関数
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
	  vector<SatBool3>& stats);

/// @brief 組み合わせ回路の等価検証を行う関数
void
check_ceq2(const BnNetwork& src_network1,
	   const BnNetwork& src_network2,
	   const vector<pair<ymuint, ymuint> >& iassoc,
	   const vector<pair<ymuint, ymuint> >& oassoc,
	   ymint log_level,
	   ostream* log_out,
	   const string& sat_type,
	   const string& sat_option,
	   ostream* sat_out,
	   ymuint sigsize,
	   vector<SatBool3>& stats);

END_NAMESPACE_YM_CEC

BEGIN_NAMESPACE_YM

using nsCec::check_ceq;

END_NAMESPACE_YM

#endif // YM_CEC_CEC_NSDEF
