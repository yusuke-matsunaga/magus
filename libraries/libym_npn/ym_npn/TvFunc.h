#ifndef YM_NPN_TVFUNC_H
#define YM_NPN_TVFUNC_H

/// @file ym_npn/TvFunc.h
/// @brief TvFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvFunc.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_npn/npn_nsdef.h>
#include <ym_npn/NpnMap.h>


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// @class TvFunc TvFunc.h <ym_npn/TvFunc.h>
/// @ingroup NpnGroup
/// @brief 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////
class TvFunc
{
public:
  
  /// @brief 入力数のみ指定したコンストラクタ
  /// 中身は恒偽関数
  explicit
  TvFunc(size_t ni = 0);

  /// @brief 入力数と真理値を指定したコンストラクタ
  TvFunc(size_t ni,
	 const vector<int>& values);

  /// @brief コピーコンストラクタ
  TvFunc(const TvFunc& src);

  /// @brief 代入演算子
  const TvFunc&
  operator=(const TvFunc& src);
  
  /// @brief デストラクタ
  ~TvFunc();


public:

  /// @brief 恒偽関数を作る．
  static
  TvFunc
  const_zero(size_t ni);

  /// @brief 恒真関数を作る．
  static
  TvFunc
  const_one(size_t ni);

  /// @brief 肯定のリテラル関数を作る．
  static
  TvFunc
  posi_literal(size_t ni,
	       size_t i);

  /// @brief 否定のリテラル関数を作る．
  static
  TvFunc
  nega_literal(size_t ni,
	       size_t i);


public:

  /// @brief 自分自身を否定する．
  const TvFunc&
  negate();

  /// @brief src1 との論理積を計算し自分に代入する．
  const TvFunc&
  operator&=(const TvFunc& src1);

  /// @brief src1 との論理和を計算し自分に代入する．
  const TvFunc&
  operator|=(const TvFunc& src1);

  /// @brief src1 との排他的論理和を計算し自分に代入する．
  const TvFunc&
  operator^=(const TvFunc& src1);

  
public:

  /// @brief 入力数を得る．
  size_t
  ni() const;

  /// @brief 入力値を2進数と見なしたときの pos 番目の値を得る．
  /// 答は 0 か 1 だが int 型
  int
  value(size_t pos) const;

  /// @brief 0 の数を数える．
  size_t
  count_zero() const;

  /// @brief 1 の数を数える．
  size_t
  count_one() const;

  /// @brief 0次の Walsh 係数を求める．
  int
  walsh_0() const;

  /// @brief 1次の Walsh 係数を求める．
  int
  walsh_1(size_t i) const;

  /// @brief 2次の Walsh 係数を求める．
  int
  walsh_2(size_t i,
	  size_t j) const;

  /// @brief 0次と 1次の Walsh 係数を求める．
  int
  walsh_01(int vec[]) const;

  /// @brief 0次と 1次と 2次の Walsh 係数を求める．
  int
  walsh_012(int vec1[],
	    int vec2[]) const;

  /// @brief 重み別の 0 次の Walsh 係数を求める．
  int
  walsh_w0(size_t w,
	   tPol opol,
	   size_t ibits) const;

  /// @brief 重み別の 1 次の Walsh 係数を求める．
  int
  walsh_w1(size_t i,
	   size_t w,
	   tPol opol,
	   size_t ibits) const;
  
  /// @brief i 番目の変数がサポートの時 true を返す．
  bool
  check_sup(tVarId i) const;
  
  /// @brief i 番目と j 番目の変数が対称のとき true を返す．
  bool
  check_sym(tVarId i,
	    tVarId j,
	    tPol pol = kPolPosi) const;

  /// @brief npnmap に従った変換を行う．
  TvFunc
  xform(const NpnMap& npnmap) const;

  /// @brief ハッシュ値を返す．
  size_t
  hash() const;

  /// @brief 内容の出力
  /// mode は 2 か 16
  void
  dump(ostream& s,
       int mode = 2) const;


public:

  /// @brief ブロック数を得る．
  size_t
  nblk() const;

  /// @brief 生のデータを得る．
  size_t
  raw_data(size_t blk) const;


public:
  
  friend
  bool
  operator==(const TvFunc& func1,
	     const TvFunc& func2);

  friend
  bool
  operator<(const TvFunc& func1,
	    const TvFunc& func2);

  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////
  
  // 恒真関数を作るコンストラクタ
  // 2番目の引数はダミー
  TvFunc(size_t ni,
	 int dummy);

  // リテラル関数を作るコンストラクタ
  TvFunc(size_t ni,
	 size_t i,
	 tPol pol);
  
  // 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
  static
  size_t
  nblock(size_t ni);

  // pos 番目の要素のブロック位置を計算する．
  static
  size_t
  block(size_t pos);

  // pos 番目の要素のシフト量を計算する．
  static
  size_t
  shift(size_t pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  size_t mNi;

  // ブロック数
  size_t mNblk;
  
  // パックされた真理値ベクトル
  size_t* mVector;

};

/// @relates TvFunc
/// @brief 否定を求める．
TvFunc
operator~(const TvFunc& src);

/// @relates TvFunc
/// @brief 論理積を求める．
TvFunc
operator&(const TvFunc& src1,
	  const TvFunc& src2);

/// @relates TvFunc
/// @brief 論理和を求める．
TvFunc
operator|(const TvFunc& src1,
	  const TvFunc& src2);

/// @relates TvFunc
/// @brief 排他的論理和を求める．
TvFunc
operator^(const TvFunc& src1,
	  const TvFunc& src2);

/// @relates TvFunc
/// @brief 等価比較
bool
operator==(const TvFunc& func1,
	   const TvFunc& func2);

/// @relates TvFunc
/// @brief 非等価比較
bool
operator!=(const TvFunc& src1,
	   const TvFunc& src2);

/// @relates TvFunc
/// @brief 大小比較(小なり)
bool
operator<(const TvFunc& func1,
	  const TvFunc& func2);

/// @relates TvFunc
/// @brief 大小比較(大なり)
bool
operator>(const TvFunc& src1,
	  const TvFunc& src2);

/// @relates TvFunc
/// @brief 大小比較(小なりイコール)
bool
operator<=(const TvFunc& src1,
	   const TvFunc& src2);

/// @relates TvFunc
/// @brief 大小比較(大なりイコール)
bool
operator>=(const TvFunc& src1,
	   const TvFunc& src2);

/// @relates TvFunc
/// @brief ストリームに対する出力
ostream&
operator<<(ostream& s,
	   const TvFunc& func);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 入力数を得る．
inline
size_t
TvFunc::ni() const
{
  return mNi;
}

// 入力値を2進数と見なしたときの pos 番目の値を得る．
// 答は 0 か 1 だが int 型
inline
int
TvFunc::value(size_t pos) const
{
  return (mVector[block(pos)] >> shift(pos)) & 1;
}

// ブロック数を得る．
inline
size_t
TvFunc::nblk() const
{
  return mNblk;
}

// 生のデータを得る．
inline
size_t
TvFunc::raw_data(size_t blk) const
{
  return mVector[blk];
}

// 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
inline
size_t
TvFunc::nblock(size_t ni)
{
  const size_t wsize = sizeof(size_t) * 8;
  return ((1 << ni) + wsize - 1) / wsize;
}

// pos 番目の要素のブロック位置を計算する．
inline
size_t
TvFunc::block(size_t pos)
{
  const size_t wsize = sizeof(size_t) * 8;
  return pos / wsize;
}

// pos 番目の要素のシフト量を計算する．
inline
size_t
TvFunc::shift(size_t pos)
{
  const size_t wsize = sizeof(size_t) * 8;
  return pos % wsize;
}

// 否定を求める．
inline
TvFunc
operator~(const TvFunc& src)
{
  return TvFunc(src).negate();
}

// 論理積を求める．
inline
TvFunc
operator&(const TvFunc& src1,
	  const TvFunc& src2)
{
  return TvFunc(src1).operator&=(src2);
}

// 論理和を求める．
inline
TvFunc
operator|(const TvFunc& src1,
	  const TvFunc& src2)
{
  return TvFunc(src1).operator|=(src2);
}

// 排他的論理和を求める．
inline
TvFunc
operator^(const TvFunc& src1,
	  const TvFunc& src2)
{
  return TvFunc(src1).operator^=(src2);
}

// 等価比較
inline
bool
operator!=(const TvFunc& src1,
	   const TvFunc& src2)
{
  return !operator==(src1, src2);
}

// 大小比較のバリエーション
inline
bool
operator>(const TvFunc& src1,
	  const TvFunc& src2)
{
  return operator<(src2, src1);
}

inline
bool
operator<=(const TvFunc& src1,
	   const TvFunc& src2)
{
  return !operator<(src2, src1);
}

inline
bool
operator>=(const TvFunc& src1,
	   const TvFunc& src2)
{
  return !operator<(src1, src2);
}

// ストリームに対する出力
inline
ostream&
operator<<(ostream& s,
	   const TvFunc& func)
{
  func.dump(s, 2);
  return s;
}

END_NAMESPACE_YM_NPN

BEGIN_NAMESPACE_HASH
// TvFunc をキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsNpn::TvFunc>
{
  size_t
  operator()(const nsYm::nsNpn::TvFunc& f) const
  {
    return f.hash();
  }
};
END_NAMESPACE_HASH

#endif // YM_NPN_TVFUNC_H
