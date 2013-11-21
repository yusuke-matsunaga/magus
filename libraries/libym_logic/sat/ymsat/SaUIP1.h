#ifndef LIBYM_LOGIC_SAT_YMSAT_ANALYZER_SAUIP1_H
#define LIBYM_LOGIC_SAT_YMSAT_ANALYZER_SAUIP1_H

/// @file libym_logic/sat/ymsat/analyzer/SaUIP1.h
/// @brief SaUIP1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SaBase.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SaUIP1 SaUIP1.h "SaUIP1.h"
/// @brief first UIP ヒューリスティックを用いた解析器
//////////////////////////////////////////////////////////////////////
class SaUIP1 :
  public SaBase
{
public:

  /// @brief コンストラクタ
  SaUIP1();

  /// @brief デストラクタ
  virtual
  ~SaUIP1();


public:
  //////////////////////////////////////////////////////////////////////
  // SatAnalyer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 解析を行う．
  /// @param[in] creason 矛盾を起こした原因
  /// @param[out] learnt 学習された節を表すリテラルのベクタ
  /// @return バックトラックレベル
  virtual
  int
  analyze(SatReason creason,
	  vector<Literal>& learnt);


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部もしくは継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  capture(SatReason creason,
	  vector<Literal>& learnt);

};

END_NAMESPACE_YM_SAT

#endif // LIBYM_LOGIC_SAT_YMSAT_ANALYZER_SAUIP1_H
