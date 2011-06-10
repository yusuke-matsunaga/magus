#ifndef YM_NETWORKS_MVNDUMPER_H
#define YM_NETWORKS_MVNDUMPER_H

/// @file ym_networks/MvnDumper.h
/// @brief MvnDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnDumper MvnDumper.h "ym_networks/MvnDumper.h"
/// @brief Mvn の内容を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class MvnDumper
{
public:

  /// @brief コンストラクタ
  MvnDumper();

  /// @brief デストラクタ
  ~MvnDumper();


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

};

END_NAMESPACE_YM_MVN

#endif // YM_NETWORKS_MVNDUMPER_H
