#ifndef LUT443MATCH_H
#define LUT443MATCH_H

/// @file Lut443Match.h
/// @brief Lut443Match のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmMgr.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Lut443Match Lut443Match.h "Lut443Match.h"
/// @brief 4-4-3 の LUT ネットワークに対するマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class Lut443Match
{
public:

  /// @brief コンストラクタ
  Lut443Match();

  /// @brief デストラクタ
  ~Lut443Match();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた関数を LUT ネットワークで実現できるか調べる．
  /// @param[in] func 対象の関数
  bool
  match(const TvFunc& func);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // LUT ネットワークを管理するクラス
  GbmMgr mMgr;

  // A0 の出力
  GbmNodeHandle mA0Root;

  // A1 の出力
  GbmNodeHandle mA1Root;

  // A2 の出力
  GbmNodeHandle mA2Root;

  // A3 の出力
  GbmNodeHandle mA3Root;

  // B0 の出力
  GbmNodeHandle mB0Root;

  // B1 の出力
  GbmNodeHandle mB1Root;

  // B2 の出力p
  GbmNodeHandle mB2Root;

  // C1 の出力
  GbmNodeHandle mC1Root;

  // C2 の出力
  GbmNodeHandle mC2Root;

};

END_NAMESPACE_YM

#endif // LUT443MATCH_H
