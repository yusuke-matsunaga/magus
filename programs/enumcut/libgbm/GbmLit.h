#ifndef GBMLIT_H
#define GBMLIT_H

/// @file GbmLit.h
/// @brief GbmLit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "logic/Literal.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmLit GbmLit.h "GbmLit.h"
/// @brief リテラルを表すクラス
///
/// 定数の場合もある．
//////////////////////////////////////////////////////////////////////
class GbmLit
{
public:

  /// @brief 空のコンストラクタ
  /// @note 値は不定
  GbmLit();

  /// @brief 変数と極性を指定するコンストラクタ
  /// @param[in] var_id 変数番号
  /// @param[in] inv 極性
  /// @note キャスト演算子でもある．
  GbmLit(VarId var_id,
	 bool inv = false);

  /// @brief リテラルを指定するコンストラクタ
  /// @param[in] lit リテラル
  /// @note キャスト演算子でもある．
  GbmLit(Literal lit);

  /// @brief 定数0を作る静的関数
  static
  GbmLit
  make_zero();

  /// @brief 定数1を作る静的関数
  static
  GbmLit
  make_one();

  /// @brief デストラクタ
  ~GbmLit();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラルを表している時 true を返す．
  bool
  is_lit() const;

  /// @brief リテラルを返す．
  /// @note is_lit() == false の時はエラーとなる．
  Literal
  literal() const;

  /// @brief 変数番号を返す．
  /// @note is_lit() == false の時はエラーとなる．
  VarId
  var_id() const;

  /// @brief 反転属性を返す．
  bool
  inv() const;

  /// @brief 定数0を表している時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1を表している時 true を返す．
  bool
  is_one() const;

  /// @brief 自身の極性を反転させる．
  /// @return 自身の参照を返す．
  const GbmLit&
  negate();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を直接指定するコンストラクタ
  /// @param[in] val 値
  /// @note make_zero()/make_one() で用いる．
  explicit
  GbmLit(ymuint val);

  /// @brief 値をセットする．
  /// @param[in] var_id 変数番号
  /// @param[in] inv 反転属性
  void
  set(VarId var_id,
      bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  // 0: 定数0
  // 1: 定数1
  // n: 変数番号 + 極性 を表す．
  ymuint32 mVal;

};


/// @brief GbmLit の内容をストリームに出力する．
ostream&
operator<<(ostream& s,
	   GbmLit lit);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 値は不定
inline
GbmLit::GbmLit()
{
  mVal = 0;
}

// @brief 変数を指定するコンストラクタ
// @param[in] var_id 変数番号
// @param[in] inv 反転属性
// @note キャスト演算子でもある．
inline
GbmLit::GbmLit(VarId var_id,
	       bool inv)
{
  set(var_id, inv);
}

// @brief リテラルを指定するコンストラクタ
// @param[in] lit リテラル
// @note キャスト演算子でもある．
inline
GbmLit::GbmLit(Literal lit)
{
  set(lit.varid(), lit.is_negative());
}

// @brief 値をセットする．
// @param[in] var_id 変数番号
// @param[in] inv 反転属性
inline
void
GbmLit::set(VarId var_id,
	    bool inv)
{
  mVal = ((var_id.val() + 1) << 1) | (static_cast<ymuint32>(inv) & 1U);
}

// @brief 値を直接指定するコンストラクタ
// @param[in] val 値
// @note make_zero()/make_one() で用いる．
inline
GbmLit::GbmLit(ymuint val)
{
  mVal = val;
}

// @brief 定数0を作る静的関数
inline
GbmLit
GbmLit::make_zero()
{
  return GbmLit(0);
}

// @brief 定数1を作る静的関数
inline
GbmLit
GbmLit::make_one()
{
  return GbmLit(1);
}

// @brief デストラクタ
inline
GbmLit::~GbmLit()
{
}

// @brief リテラルを表している時 true を返す．
inline
bool
GbmLit::is_lit() const
{
  return mVal >= 2;
}

// @brief リテラルを返す．
// @note is_lit() == false の時はエラーとなる．
inline
Literal
GbmLit::literal() const
{
  return Literal(var_id(), inv());
}

// @brief 反転属性を返す．
inline
bool
GbmLit::inv() const
{
  return static_cast<bool>(mVal & 1U);
}

// @brief 変数番号を返す．
// @note is_lit() == false の時はエラーとなる．
inline
VarId
GbmLit::var_id() const
{
  assert_cond( is_lit(), __FILE__, __LINE__);
  return VarId((mVal / 2) - 1);
}

// @brief 定数0を表している時 true を返す．
inline
bool
GbmLit::is_zero() const
{
  return mVal == 0U;
}

// @brief 定数1を表している時 true を返す．
inline
bool
GbmLit::is_one() const
{
  return mVal == 1U;
}

// @brief 自身の極性を反転させる．
// @return 自身の参照を返す．
inline
const GbmLit&
GbmLit::negate()
{
  mVal ^= 1U;
  return *this;
}

// @brief GbmLit の内容をストリームに出力する．
inline
ostream&
operator<<(ostream& s,
	   GbmLit lit)
{
  if ( lit.is_zero() ) {
    s << "CONST-0";
  }
  else if ( lit.is_one() ) {
    s << "CONST-1";
  }
  else {
    s << lit.literal();
  }
  return s;
}

END_NAMESPACE_YM

#endif // GBMLIT_H
