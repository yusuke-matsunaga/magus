#ifndef SASIMPLE_H
#define SASIMPLE_H

/// @file SaSimple.h
/// @brief SaSimple のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SaBase.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SaSimple SaSimple.h "SaSimple.h"
/// @brief 単純な解析器
//////////////////////////////////////////////////////////////////////
class SaSimple :
  public SaBase
{
public:

  /// @brief コンストラクタ
  SaSimple();

  /// @brief デストラクタ
  virtual
  ~SaSimple();


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
  // 内部もしくは継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  capture(SatReason creason,
	  vector<Literal>& learnt);

};

END_NAMESPACE_YM_SAT

#endif // SASIMPLE_H
