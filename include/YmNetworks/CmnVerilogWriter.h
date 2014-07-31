#ifndef NETWORKS_CMNVERILOGWRITER_H
#define NETWORKS_CMNVERILOGWRITER_H

/// @file YmNetworks/CmnVerilogWriter.h
/// @brief CmnVerilogWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/cmn.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

class VerilogWriterImpl;

//////////////////////////////////////////////////////////////////////
/// @class CmnVerilogWriter CmnVerilogWriter.h "YmNetworks/CmnVerilogWriter.h"
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
  /// @param[in] mgr 対象のネットワーク
  void
  operator()(ostream& s,
	     const CmnMgr& mgr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  VerilogWriterImpl* mImpl;

};

END_NAMESPACE_YM_NETWORKS_CMN

#endif // NETWORKS_CMNVERILOGWRITER_H
