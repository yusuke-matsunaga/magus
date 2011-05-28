#ifndef YM_MVN_MVNVERILOGWRITER_H
#define YM_MVN_MVNVERILOGWRITER_H

/// @file ym_networks/MvnVerilogWriter.h
/// @brief MvnVerilogWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnVerilogWriter MvnVerilogWriter.h "ym_networks/MvnVerilogWriter.h"
/// @brief Mvn の内容を Verilog-HDL 形式で出力するためのクラス
//////////////////////////////////////////////////////////////////////
class MvnVerilogWriter
{
public:

  /// @brief コンストラクタ
  MvnVerilogWriter();

  /// @brief デストラクタ
  ~MvnVerilogWriter();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を Verilog-HDL 形式で出力する
  /// @param[in] s 出力先のストリーム
  /// @param[in] mgr MvnMgr
  void
  operator()(ostream& s,
	     const MvnMgr& mgr);

  /// @brief 内容を Verilog-HDL 形式で出力する
  /// @param[in] s 出力先のストリーム
  /// @param[in] mgr MvnMgr
  /// @param[in] node_map ノードと Verilog 名の対応表
  void
  operator()(ostream& s,
	     const MvnMgr& mgr,
	     const MvnVlMap& node_map);

};

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNVERILOGWRITER_H
