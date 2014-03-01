#ifndef LOGIC_LITERAL_H
#define LOGIC_LITERAL_H

/// @file logic/Literal.h
/// @brief Literal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "logic/VarId.h"
#include "utils/IDO.h"
#include "utils/ODO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @ingroup LogicGroup
/// @class Literal Literal.h "logic/Literal.h"
/// @brief リテラル(変数番号＋極性)を表すクラス
/// @sa VarId
//////////////////////////////////////////////////////////////////////
class Literal
{
public:

  /// @brief デフォルトコンストラクタ
  /// 内容は不定なのであまり使わない方が良い．
  Literal();

  /// @brief 変数番号と極性を指定したコンストラクタ
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  Literal(VarId varid,
	  bool inv);

  /// @brief index からの変換関数
  /// @param[in] index 変数番号を極性をエンコードしたもの
  static
  Literal
  index2literal(ymuint index);

  // コピーコンストラクタ,代入演算子,デストラクタはデフォルト
  // のものがそのまま使える．


public:
  //////////////////////////////////////////////////////////////////////
  // 内容をセットする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  void
  set(VarId varid,
      bool inv);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数番号を得る．
  /// @return 変数番号
  VarId
  varid() const;

  /// @brief 正極性のリテラルの時 true を返す．
  bool
  is_positive() const;

  /// @brief 負極性のリテラルの時 true を返す．
  bool
  is_negative() const;

  /// @brief 極性の反転
  /// @return 極性を反転させたリテラルを返す．
  Literal
  operator~() const;

  /// @brief 同じ変数の正極性リテラルを返す．
  Literal
  make_positive() const;

  /// @brief 同じ変数の負極性リテラルを返す．
  Literal
  make_negative() const;

  /// @brief ハッシュ用の関数
  ymuint
  hash() const;

  /// @brief 配列のインデックスとして使用可能な数を返す．
  ymuint
  index() const;


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリファイルに出力する．
  /// @param[in] s 出力先のストリーム
  void
  store(ODO& s) const;

  /// @brief バイナリファイルから読み込む．
  /// @param[in] s 入力元のストリーム
  void
  restore(IDO& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部でのみ用いるコンストラクタ
  explicit
  Literal(ymuint32 body);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号と極性をパックしたもの
  ymuint32 mBody;

};

/// @relates Literal
/// @brief 未定義リテラル
extern
const Literal kLiteralX;

/// @relates Literal
/// @brief 比較関数
/// @param[in] lit1, lit2 比較対象のリテラル
/// @retval -1 lit1 < lit2
/// @retval  0 lit1 = lit2
/// @retval  1 lit1 > lit2
int
compare(Literal lit1,
	Literal lit2);

/// @relates Literal
/// @brief 等価比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 と lit2 が等しいリテラルの時 true を返す．
bool
operator==(Literal lit1,
	   Literal lit2);

/// @relates Literal
/// @brief 非等価比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 と lit2 が等しいリテラルでないとき true を返す．
bool
operator!=(Literal lit1,
	   Literal lit2);

/// @relates Literal
/// @brief 小なり比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より小さいとき true を返す．
bool
operator<(Literal lit1,
	  Literal lit2);

/// @relates Literal
/// @brief 大なり比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より大きいとき true を返す．
bool
operator>(Literal lit1,
	  Literal lit2);

/// @relates Literal
/// @brief 小なりイコール比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より小さいか等しいとき true を返す．
bool
operator<=(Literal lit1,
	   Literal lit2);

/// @relates Literal
/// @brief 大なりイコール比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より大きいか等しいとき true を返す．
bool
operator>=(Literal lit1,
	   Literal lit2);

/// @relates Literal
/// @brief Literal の内容を ostream に出力する関数
/// @param[in] s 出力ストリーム
/// @param[in] lit 出力対象のリテラル
/// @return s
ostream&
operator<<(ostream& s,
	   const Literal& lit);

/// @relates Literal
/// @brief Literal の内容をバイナリファイルに出力する関数
/// @param[in] s 出力先のストリーム
/// @param[in] lit 対象のリテラル
/// @return s
ODO&
operator<<(ODO& s,
	   const Literal& lit);

/// @relates Literal
/// @brief Literal の内容をバイナリファイルから読み込む関数
/// @param[in] s 入力元のストリーム
/// @param[out] lit 対象のリテラル
/// @return s
IDO&
operator>>(IDO& s,
	   Literal& lit);


//////////////////////////////////////////////////////////////////////
// 上記のクラスを要素とするコンテナクラス
//////////////////////////////////////////////////////////////////////

/// @brief リテラルのベクタ
typedef vector<Literal> LiteralVector;

/// @brief リテラルのリスト
typedef list<Literal> LiteralList;


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// 内容を設定する．
inline
void
Literal::set(VarId varid,
	     bool inv)
{
  mBody = (varid.val() << 1) + static_cast<ymuint32>(inv);
}

// デフォルトコンストラクタ
inline
Literal::Literal() :
  mBody(0xfffffffe)
{
}

// 変数番号と極性を指定したコンストラクタ
inline
Literal::Literal(VarId varid,
		 bool inv)
{
  set(varid, inv);
}

// 内部でのみ用いるコンストラクタ
inline
Literal::Literal(ymuint32 body) :
  mBody(body)
{
}

// @brief index からの変換関数
inline
Literal
Literal::index2literal(ymuint32 index)
{
  return Literal(index);
}

// 変数番号を得る．
inline
VarId
Literal::varid() const
{
  return VarId(mBody >> 1);
}

// @brief 正極性のリテラルの時 true を返す．
inline
bool
Literal::is_positive() const
{
  return !is_negative();
}

// @brief 負極性のリテラルの時 true を返す．
inline
bool
Literal::is_negative() const
{
  return static_cast<bool>(mBody & 1U);
}

// 極性を反転させたリテラルを返す．
inline
Literal
Literal::operator~() const
{
  return Literal(mBody ^ 1U);
}

// @brief 同じ変数の正極性リテラルを返す．
inline
Literal
Literal::make_positive() const
{
  return Literal(mBody & ~1U);
}

// @brief 同じ変数の負極性リテラルを返す．
inline
Literal
Literal::make_negative() const
{
  return Literal(mBody | 1U);
}

// @brief バイナリファイルに出力する．
// @param[in] s 出力先のストリーム
inline
void
Literal::store(ODO& s) const
{
  s << mBody;
}

// @brief バイナリファイルから読み込む．
// @param[in] s 入力元のストリーム
inline
void
Literal::restore(IDO& s)
{
  s >> mBody;
}

// @brief 比較関数
// @param[in] lit1, lit2 比較対象のリテラル
// @retval -1 lit1 < lit2
// @retval  0 lit1 = lit2
// @retval  1 lit1 > lit2
inline
int
compare(Literal lit1,
	Literal lit2)
{
  if ( lit1.index() < lit2.index() ) {
    return -1;
  }
  if ( lit1.index() > lit2.index() ) {
    return 1;
  }
  return 0;
}

// 等価比較
inline
bool
operator==(Literal lit1,
	   Literal lit2)
{
  return compare(lit1, lit2) == 0;
}
inline
bool
operator!=(Literal lit1,
	   Literal lit2)
{
  return !operator==(lit1, lit2);
}

// @brief 小なり比較
inline
bool
operator<(Literal lit1,
	  Literal lit2)
{
  return compare(lit1, lit2) == -1;
}

// @brief 大なり比較
inline
bool
operator>(Literal lit1,
	  Literal lit2)
{
  return operator<(lit2, lit1);
}

// @brief 小なりイコール比較
inline
bool
operator<=(Literal lit1,
	   Literal lit2)
{
  return !operator<(lit2, lit1);
}

// @brief 大なりイコール比較
inline
bool
operator>=(Literal lit1,
	   Literal lit2)
{
  return !operator<(lit1, lit2);
}

// @brief Literal の内容をバイナリファイルに出力する関数
// @param[in] s 出力先のストリーム
// @param[in] lit 対象のリテラル
// @return s
inline
ODO&
operator<<(ODO& s,
	   const Literal& lit)
{
  lit.store(s);
  return s;
}

// @brief Literal の内容をバイナリファイルから読み込む関数
// @param[in] s 入力元のストリーム
// @param[out] lit 対象のリテラル
// @return s
inline
IDO&
operator>>(IDO& s,
	   Literal& lit)
{
  lit.restore(s);
  return s;
}

// ハッシュ用の関数
inline
ymuint
Literal::hash() const
{
  return mBody;
}

// @brief 配列のインデックスとして使用可能な数を返す．
inline
ymuint
Literal::index() const
{
  return mBody;
}

END_NAMESPACE_YM


BEGIN_NAMESPACE_STD

template <>
struct equal_to<nsYm::Literal>
{
  bool
  operator()(nsYm::Literal n1,
	     nsYm::Literal n2) const
  {
    return n1 == n2;
  }
};

END_NAMESPACE_STD


BEGIN_NAMESPACE_HASH

// Literal をキーにしたハッシュ関数クラス
template <>
struct hash<nsYm::Literal>
{
  ymuint
  operator()(nsYm::Literal lit) const
  {
    return lit.hash();
  }
};

END_NAMESPACE_HASH

#endif // LOGIC_LITERAL_H
