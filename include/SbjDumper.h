#ifndef SBJDUMPER_H
#define SBJDUMPER_H

/// @file SbjDumper.h
/// @brief SbjDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjDumper SbjDumper.h "SbjDumper.h"
/// @brief SbjGraph の内容を出力するためのクラス
///
/// すべてクラスメソッドなのでクラスにする意味はあまりない．
//////////////////////////////////////////////////////////////////////
class SbjDumper
{
public:
  //////////////////////////////////////////////////////////////////////
  // 出力関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 独自形式で出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  static
  void
  dump(ostream& s,
       const SbjGraph& sbjgraph);

  /// @brief blif 形式で出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  static
  void
  dump_blif(ostream& s,
	    const SbjGraph& sbjgraph);

  /// @brief Verilog-HDL 形式で出力する関数
  /// @param[in] s 出力先のストリーム
  /// @param[in] sbjgraph 対象のネットワーク
  static
  void
  dump_verilog(ostream& s,
	       const SbjGraph& sbjgraph);

};

END_NAMESPACE_YM_SBJ

#endif // SBJDUMPER_H
