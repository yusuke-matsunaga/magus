#ifndef LUTMAP1_H
#define LUTMAP1_H

/// @file LutMap1.h
/// @brief LutMap1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_logic/TvFunc.h"
#include "GbmMgr.h"
#include "GbmLit.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LutMap1 LutMap1.h "LutMap1.h"
/// @brief LUTマクロに対するマッチングを行なう．
//////////////////////////////////////////////////////////////////////
class LutMap1
{
public:

  /// @brief コンストラクタ
  LutMap1();

  /// @brief デストラクタ
  ~LutMap1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッチングを行なう．
  bool
  matching(const TvFunc& f,
	   vector<bool>& conf_bits,
	   vector<GbmLit>& input_vars);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // LUT マクロを表すオブジェクト
  GbmMgr mMgr;

};

END_NAMESPACE_YM

#endif // LUTMAP1_H
