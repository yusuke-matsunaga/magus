
/// @file libym_networks/cmn/CmnPort.cc
/// @brief CmnPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnPort.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
// クラス CmnPort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] bw ビット数
// @param[in] bit_array 対応する入出力ノードの配列
CmnPort::CmnPort(const string& name,
		 ymuint bw) :
  mName(name),
  mBitWidth(bw)
{
}

// @brief デストラクタ
CmnPort::~CmnPort()
{
}

END_NAMESPACE_YM_CMN
