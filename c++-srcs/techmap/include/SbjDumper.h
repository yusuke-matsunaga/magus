#ifndef SBJDUMPER_H
#define SBJDUMPER_H

/// @file SbjDumper.h
/// @brief SbjDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"


BEGIN_NAMESPACE_SBJ

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
  static
  void
  dump(
    ostream& s,              ///< [in] 出力先のストリーム
    const SbjGraph& sbjgraph ///< [in] 対象のサブジェクトグラフ
  );

  /// @brief blif 形式で出力する．
  static
  void
  dump_blif(
    ostream& s,              ///< [in] 出力先のストリーム
    const SbjGraph& sbjgraph ///< [in] 対象のサブジェクトグラフ
  );

  /// @brief Verilog-HDL 形式で出力する関数
  static
  void
  dump_verilog(
    ostream& s,              ///< [in] 出力先のストリーム
    const SbjGraph& sbjgraph ///< [in] 対象のネットワーク
  );

};

END_NAMESPACE_SBJ

#endif // SBJDUMPER_H
