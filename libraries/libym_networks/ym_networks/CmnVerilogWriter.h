#ifndef YM_NETWORKS_CMNVERILOGWRITER_H
#define YM_NETWORKS_CMNVERILOGWRITER_H

/// @file ym_networks/CmnVerilogWriter.h
/// @brief CmnVerilogWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/cmn.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class CmnVerilogWriter CmnVerilogWriter.h "ym_networks/CmnVerilogWriter.h"
/// @brief CmnMgr の内容を Verilog-HDL 形式で出力するクラス
//////////////////////////////////////////////////////////////////////
class CmnVerilogWriter
{
public:

  /// @brief コンストラクタ
  CmnVerilogWriter();

  /// @brief デストラクタ
  ~CmnVerilogWriter();


public:

  /// @relates CmnMgr
  /// @brief 内容をシミュレーション可能な Verilog-HDL 形式で出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] cngraph 対象のネットワーク
  void
  operator()(ostream& s,
	     const CmnMgr& cngraph);

};

END_NAMESPACE_YM_NETWORKS

#endif // YM_NETWORKS_CMNVERILOGWRITER_H
