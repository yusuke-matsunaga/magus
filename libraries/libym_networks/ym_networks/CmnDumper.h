#ifndef YM_NETWORKS_CMNDUMPER_H
#define YM_NETWORKS_CMNDUMPER_H

/// @file ym_networks/CmnDumper.h
/// @brief CmnDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/cmn_nsdef.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnDumper CmnDumper.h "ym_networks/CmnDumper.h"
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
  dump(ostream& s,
       const CmnMgr& lngraph);

  /// @relates CmnMgr
  /// @brief 内容をシミュレーション可能な Verilog-HDL 形式で出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] cngraph 対象のネットワーク
  void
  dump_verilog(ostream& s,
	       const CmnMgr& cngraph);

};

END_NAMESPACE_YM_CMN

#endif // YM_NETWORKS_CMNDUMPER_H
