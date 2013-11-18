#ifndef GBMVAR_H
#define GBMVAR_H

/// @file GbmVar.h
/// @brief GbmVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_logic/VarId.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmVar GbmVar.h "GbmVar.h"
/// @brief 変数を表すクラス
///
/// 定数の場合もある．
//////////////////////////////////////////////////////////////////////
class GbmVar
{
public:

  /// @brief 空のコンストラクタ
  /// @note 値は不定
  GbmVar();

  /// @brief 変数を指定するコンストラクタ
  /// @note キャスト演算子でもある．
  GbmVar(VarId var_id);

  /// @brief 定数0を作る静的関数
  static
  GbmVar
  make_zero();

  /// @brief 定数1を作る静的関数
  static
  GbmVar
  make_one();

  /// @brief デストラクタ
  ~GbmVar();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を表している時 true を返す．
  bool
  is_var() const;

  /// @brief 変数番号を返す．
  /// @note is_var() == false の時はエラーとなる．
  VarId
  var_id() const;

  /// @brief 定数0を表している時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1を表している時 true を返す．
  bool
  is_one() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を直接指定するコンストラクタ
  /// @param[in] val 値
  /// @note make_zero()/make_one() で用いる．
  explicit
  GbmVar(ymuint val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  // 0: 定数0
  // 1: 定数1
  // n: 変数( n -  2) を表す．
  ymuint32 mVal;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 値は不定
inline
GbmVar::GbmVar()
{
  mVal = 0;
}

// @brief 変数を指定するコンストラクタ
// @note キャスト演算子でもある．
inline
GbmVar::GbmVar(VarId var_id)
{
  mVal = var_id.val() + 2;
}

// @brief 値を直接指定するコンストラクタ
// @param[in] val 値
// @note make_zero()/make_one() で用いる．
inline
GbmVar::GbmVar(ymuint val)
{
  mVal = val;
}

// @brief 定数0を作る静的関数
inline
GbmVar
GbmVar::make_zero()
{
  return GbmVar(0);
}

// @brief 定数1を作る静的関数
inline
GbmVar
GbmVar::make_one()
{
  return GbmVar(1);
}

// @brief デストラクタ
inline
GbmVar::~GbmVar()
{
}

// @brief 変数を表している時 true を返す．
inline
bool
GbmVar::is_var() const
{
  return mVal >= 2;
}

// @brief 変数番号を返す．
// @note is_var() == false の時はエラーとなる．
inline
VarId
GbmVar::var_id() const
{
  assert_cond( is_var(), __FILE__, __LINE__);
  return VarId(mVal - 2);
}

// @brief 定数0を表している時 true を返す．
inline
bool
GbmVar::is_zero() const
{
  return mVal == 0;
}

// @brief 定数1を表している時 true を返す．
inline
bool
GbmVar::is_one() const
{
  return mVal == 1;
}

END_NAMESPACE_YM

#endif // GBMVAR_H
