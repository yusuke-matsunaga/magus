
/// @file CmnPort.cc
/// @brief CmnPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/CmnPort.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

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

END_NAMESPACE_YM_NETWORKS_CMN
