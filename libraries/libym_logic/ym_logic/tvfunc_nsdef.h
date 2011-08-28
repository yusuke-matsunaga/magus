#ifndef YM_LOGIC_TVFUNC_NSDEF_H
#define YM_LOGIC_TVFUNC_NSDEF_H

/// @file ym_logic/tvfunc_nsdef.h
/// @brief TvFunc パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief TvFunc 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_TVFUNC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsTvFunc)

/// @brief TvFunc 用の名前空間の終了
#define END_NAMESPACE_YM_TVFUNC \
END_NAMESPACE(nsTvFunc) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_TVFUNC

//////////////////////////////////////////////////////////////////////
// クラスの前方参照
//////////////////////////////////////////////////////////////////////

class TvFunc;


//////////////////////////////////////////////////////////////////////
// 定数
//////////////////////////////////////////////////////////////////////

/// @brief 扱う最大の入力数
/// 特に根拠はないが，これなら Walsh 係数が 32 ビット整数で収まる．
/// あと真理値表ベースの手法ではこれくらいが限度
static
const ymuint kTvFuncMaxNi = 20;

END_NAMESPACE_YM_TVFUNC

BEGIN_NAMESPACE_YM

using nsTvFunc::TvFunc;
using nsTvFunc::kTvFuncMaxNi;

END_NAMESPACE_YM

#endif // YM_LOGIC_TVFUNC_NSDEF_H
