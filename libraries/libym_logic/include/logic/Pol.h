#ifndef LOGIC_POL_H
#define LOGIC_POL_H

/// @file logic/Pol.h
/// @brief tPol の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup LogicGroup 論理式/論理関数を扱うクラス
///
/// 論理式(ファクタードフォーム)を扱うためのクラスライブラリ
/// C++ の機能を活かして論理演算子のオーバーロードを使っている．


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @ingroup LogicGroup
/// @brief 極性を表す型
//////////////////////////////////////////////////////////////////////
enum tPol {
  /// @brief 肯定(正極性)
  kPolPosi = 0,
  /// @brief 否定(負極性)
  kPolNega = 1
};

/// @ingroup LogicGroup
/// @{

/// @brief 2つの極性の合成
/// @param[in] pol1, pol2 2つの極性
/// @return pol1 と pol2 を合成したもの
inline
tPol
operator*(tPol pol1,
	  tPol pol2)
{
  return tPol( int(pol1) ^ int(pol2) );
}

/// @brief 極性の否定
/// @param[in] pol 入力の極性
/// @return pol の否定
inline
tPol
operator~(tPol pol)
{
  return operator*(pol, kPolNega);
}

/// @brief ostream に対する書出し
/// @param[in] s 出力ストリーム
/// @param[in] pol 極性
/// @return s をそのまま返す．
inline
ostream&
operator<<(ostream& s,
	   tPol pol)
{
  return s << (( pol == kPolPosi ) ? "Posi" : "Nega");
}

/// @}

END_NAMESPACE_YM

#endif // LOGIC_POL_H
