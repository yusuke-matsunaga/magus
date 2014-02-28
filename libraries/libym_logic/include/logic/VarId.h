#ifndef LOGIC_VARID_H
#define LOGIC_VARID_H

/// @file logic/VarId.h
/// @brief VarId の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "utils/IDO.h"
#include "utils/ODO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class VarId VarId.h "logic/VarId.h"
/// @ingroup LogicGroup
/// @brief 変数番号を表す型
/// 基本的にはただの符号なし整数だが，意味のある演算がほとんどないので
/// あえてクラスの形にしている．
/// 例えば変数どうしの四則演算に直接的な意味はない．
//////////////////////////////////////////////////////////////////////
class VarId
{
public:

  /// @brief 空のコンストラクタ
  /// @note 不正な値(kVarIdIllegal)になる．
  VarId();

  /// @brief 値を指定したコンストラクタ
  explicit
  VarId(ymuint val);

  /// @brief デストラクタ
  ~VarId();


public:
  //////////////////////////////////////////////////////////////////////
  // 外から使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  ymuint
  val() const;

  /// @brief 比較関数
  /// @param[in] left, right 比較対象の変数
  /// @retval -1 left < right
  /// @retval  0 left = right
  /// @retval  1 left > right
  friend
  int
  compare(const VarId& left,
	  const VarId& right);


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリファイルに出力する．
  /// @param[in] s 出力先のストリーム
  void
  dump(ODO& s) const;

  /// @brief バイナリファイルを読み込む．
  void
  restore(IDO& s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の値
  ymuint32 mVal;

};

/// @brief 等価比較
/// @param[in] left, right 比較対象の変数
bool
operator==(const VarId& left,
	   const VarId& right);

/// @brief 非等価比較
/// @param[in] left, right 比較対象の変数
bool
operator!=(const VarId& left,
	   const VarId& right);

/// @brief 小なり比較
/// @param[in] left, right 比較対象の変数
bool
operator<(const VarId& left,
	  const VarId& right);

/// @brief 大なり比較
/// @param[in] left, right 比較対象の変数
bool
operator>(const VarId& left,
	  const VarId& right);

/// @brief 小なりイコール比較
/// @param[in] left, right 比較対象の変数
bool
operator<=(const VarId& left,
	   const VarId& right);

/// @brief 大なりイコール比較
/// @param[in] left, right 比較対象の変数
bool
operator>=(const VarId& left,
	   const VarId& right);

/// @relates VarId
/// @brief ストリームに対する出力
/// @param[in] s 出力先のストリーム
/// @param[in] varid 対象の変数
ostream&
operator<<(ostream& s,
	   const VarId& varid);

/// @relates VarId
/// @brief バイナリファイルの出力
/// @param[in] s 出力先のストリーム
/// @param[in] varid 対象の変数
ODO&
operator<<(ODO& s,
	   const VarId& varid);

/// @relates VarId
/// @brief バイナリファイルの読み込み
/// @param[in] s 入力元のストリーム
/// @param[out] varid 対象の変数
IDO&
operator>>(IDO& s,
	   VarId& varid);


//////////////////////////////////////////////////////////////////////
// VarId を要素とするコンテナクラスの定義
//////////////////////////////////////////////////////////////////////

/// @ingroup LogicGroup
/// @brief 変数番号のベクタ
typedef vector<VarId> VarVector;

/// @ingroup LogicGroup
/// @brief 変数番号のリスト
typedef list<VarId> VarList;

/// @ingroup LogicGroup
/// @brief 変数番号から変数番号への写像 (連想配列)
typedef unordered_map<VarId, VarId> VarVarMap;

/// @ingroup LogicGroup
/// @brief 文字列から変数番号への写像 (連想配列)
typedef unordered_map<string, VarId> StrVarMap;

/// @ingroup LogicGroup
/// @brief 変数番号から文字列への写像 (連想配列)
typedef unordered_map<VarId, string> VarStrMap;


//////////////////////////////////////////////////////////////////////
// 定数
//////////////////////////////////////////////////////////////////////

/// @relates VarId
/// @brief 不正な値
extern
const VarId kVarIdIllegal;


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
VarId::VarId() :
  mVal(0xFFFFFFFFU)
{
}

// @brief 値を指定したコンストラクタ
inline
VarId::VarId(ymuint val) :
  mVal(val)
{
}

// @brief デストラクタ
inline
VarId::~VarId()
{
}

// @brief 値を取り出す．
inline
ymuint
VarId::val() const
{
  return mVal;
}

// @brief 比較関数
// @param[in] left, right 比較対象の変数
// @retval -1 left < right
// @retval  0 left = right
// @retval  1 left > right
inline
int
compare(const VarId& left,
	const VarId& right)
{
  if ( left.mVal < right.mVal ) {
    return -1;
  }
  if ( left.mVal > right.mVal ) {
    return 1;
  }
  return 0;
}

// @brief 等価比較
// @param[in] left, right 比較対象の変数
inline
bool
operator==(const VarId& left,
	   const VarId& right)
{
  return compare(left, right) == 0;
}

// @brief 非等価比較
// @param[in] left, right 比較対象の変数
inline
bool
operator!=(const VarId& left,
	   const VarId& right)
{
  return !operator==(left, right);
}

// @brief 小なり比較
// @param[in] left, right 比較対象の変数
inline
bool
operator<(const VarId& left,
	  const VarId& right)
{
  return compare(left, right) == -1;
}

// @brief 大なり比較
// @param[in] left, right 比較対象の変数
inline
bool
operator>(const VarId& left,
	  const VarId& right)
{
  return operator<(right, left);
}

// @brief 小なりイコール比較
// @param[in] left, right 比較対象の変数
inline
bool
operator<=(const VarId& left,
	   const VarId& right)
{
  return !operator<(right, left);
}

// @brief 大なりイコール比較
// @param[in] left, right 比較対象の変数
inline
bool
operator>=(const VarId& left,
	   const VarId& right)
{
  return !operator<(left, right);
}

// @brief バイナリファイルに出力する．
// @param[in] s 出力先のストリーム
inline
void
VarId::dump(ODO& s) const
{
  s << mVal;
}

// @brief バイナリファイルを読み込む．
inline
void
VarId::restore(IDO& s)
{
  s >> mVal;
}

// @relates VarId
// @brief ストリームに対する出力
// @param[in] s 出力先のストリーム
// @param[in] varid 対象の変数
inline
ostream&
operator<<(ostream& s,
	   const VarId& varid)
{
  return s << varid.val();
}

// @relates VarId
// @brief バイナリファイルの出力
// @param[in] s 出力先のストリーム
// @param[in] varid 対象の変数
inline
ODO&
operator<<(ODO& s,
	   const VarId& varid)
{
  varid.dump(s);
  return s;
}

// @relates Varid
// @brief バイナリファイルの読み込み
// @param[in] s 入力元のストリーム
// @param[out] varid 対象の変数
inline
IDO&
operator>>(IDO& s,
	   VarId& varid)
{
  varid.restore(s);
  return s;
}

END_NAMESPACE_YM

BEGIN_NAMESPACE_HASH
// VarId をキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::VarId>
{
  ymuint
  operator()(const nsYm::VarId& varid) const
  {
    return varid.val();
  }
};
END_NAMESPACE_HASH

#endif // LOGIC_VARID_H
