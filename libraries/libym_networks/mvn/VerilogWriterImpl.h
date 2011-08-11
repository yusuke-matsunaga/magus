#ifndef VERILOGWRITERIMPL_H
#define VERILOGWRITERIMPL_H

/// @file VerilogWriterImpl.h
/// @brief VerilogWriterImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @class VerilogWriterImpl VerilogWriterImpl.h
/// @brief MvnVerilogWriter の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class VerilogWriterImpl
{
public:

  /// @brief コンストラクタ
  VerilogWriterImpl();

  /// @brief デストラクタ
  ~VerilogWriterImpl();


public:

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] mgr MvnMgr
  void
  dump(ostream& s,
       const MvnMgr& mgr);

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] mgr MvnMgr
  /// @param[in] node_map ノードと Verilog 名の対応表
  void
  dump(ostream& s,
       const MvnMgr& mgr,
       const MvnVlMap& node_map);

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // VERILOGWRITERIMPL_H
