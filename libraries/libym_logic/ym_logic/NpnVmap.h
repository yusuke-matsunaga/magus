#ifndef YM_LOGIC_NPNVMAP_H
#define YM_LOGIC_NPNVMAP_H

/// @file ym_logic/NpnVmap.h
/// @brief NpnVmap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/npn_nsdef.h"
#include "ym_logic/Pol.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// @class NpnVmap NpnVmap.h "ym_logic/NpnVmap.h"
/// @ingroup NpnGroup
/// @brief 変数の変換を表すクラス
///
/// @sa NpnMap, NpnMapM, tPol
//////////////////////////////////////////////////////////////////////
class NpnVmap
{
public:

  /// @brief 空のコンストラクタ．
  /// @note 内容は不定
  NpnVmap();

  /// @brief 変数番号と極性を指定したコンストラクタ
  /// @param[in] pos 変数番号
  /// @param[in] pol 極性
  explicit
  NpnVmap(ymuint pos,
	  tPol pol = kPolPosi);

  /// @brief 不正な値を返すクラス関数
  static
  NpnVmap
  invalid();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変換先の変数番号を得る．
  ymuint
  pos() const;

  /// @brief 極性を得る．
  tPol
  pol() const;

  /// @brief 不正な値の時に true を返す．
  bool
  is_invalid() const;

  /// @brief 等価比較演算
  bool
  operator==(const NpnVmap& right) const;

  /// @brief 非等価比較演算
  bool
  operator!=(const NpnVmap& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号と極性をパックしたもの
  ymuint8 mPosPol;

};

//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ．
// @note 内容は不定
inline
NpnVmap::NpnVmap() :
  mPosPol(0xFF)
{
}

// @brief 変数番号と極性を指定したコンストラクタ
// @param[in] pos 変数番号
// @param[in] pol 極性
inline
NpnVmap::NpnVmap(ymuint pos,
		 tPol pol) :
  mPosPol((pos << 1) | static_cast<ymuint8>(pol))
{
}

// @brief 不正な値を返すクラス関数
inline
NpnVmap
NpnVmap::invalid()
{
  // 実はデフォルトコンストラクタを呼ぶだけ．
  // おもに読みやすさのために用意した関数
  return NpnVmap();
}

// @brief 変換先の変数番号を得る．
inline
ymuint
NpnVmap::pos() const
{
  return mPosPol >> 1;
}

// @brief 極性を得る．
inline
tPol
NpnVmap::pol() const
{
  return static_cast<tPol>(mPosPol & 1U);
}

// @brief 不正な値の時に true を返す．
inline
bool
NpnVmap::is_invalid() const
{
  return mPosPol == 0xFF;
}

// @brief 等価比較演算
inline
bool
NpnVmap::operator==(const NpnVmap& right) const
{
  return mPosPol == right.mPosPol;
}

// @brief 非等価比較演算
inline
bool
NpnVmap::operator!=(const NpnVmap& right) const
{
  return !operator==(right);
}

END_NAMESPACE_YM_NPN

#endif // YM_LOGIC_NPNVMAP_H
