#ifndef NETWORKS_CMNDUMPER_H
#define NETWORKS_CMNDUMPER_H

/// @file networks/CmnDumper.h
/// @brief CmnDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/cmn.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnDumper CmnDumper.h "networks/CmnDumper.h"
/// @brief CmnMgr の内容を出力するクラス
//////////////////////////////////////////////////////////////////////
class CmnDumper
{
public:

  /// @brief コンストラクタ
  CmnDumper();

  /// @brief デストラクタ
  ~CmnDumper();


public:

  /// @brief 内容を s に出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] lngraph 対象のネットワーク
  void
  operator()(ostream& s,
	     const CmnMgr& lngraph);

};

END_NAMESPACE_YM_NETWORKS_CMN

#endif // NETWORKS_CMNDUMPER_H
