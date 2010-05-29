#ifndef MAGUS_EQUIV_EQUIV_NSDEF_H
#define MAGUS_EQUIV_EQUIV_NSDEF_H

/// @file magus/equiv/equiv_nsdef.h
/// @brief equiv2 の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: equiv_nsdef.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "magus.h"
#include "ym_bnet/BNetwork.h"
#include "ym_sat/Bool3.h"


// 名前空間の定義
#define BEGIN_NAMESPACE_MAGUS_EQUIV \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsEquiv)

#define END_NAMESPACE_MAGUS_EQUIV \
END_NAMESPACE(nsEquiv) \
END_NAMESPACE_MAGUS


BEGIN_NAMESPACE_MAGUS_EQUIV
  
// 新しい関数
void
check_equiv(const BNetwork& src_network1,
	    const BNetwork& src_network2,
	    const vector<pair<ymuint32, ymuint32> >& iassoc,
	    const vector<pair<ymuint32, ymuint32> >& oassoc,
	    ymint log_level,
	    ostream* log_out,
	    const string& sat_type,
	    const string& sat_option,
	    ostream* sat_out,
	    ymuint sigsize,
	    vector<Bool3>& stats);

END_NAMESPACE_MAGUS_EQUIV

#endif // MAGUS_EQUIV_EQUIV_NSDEF_H
