#ifndef YM_LOGIC_TVFUNCM_H
#define YM_LOGIC_TVFUNCM_H

/// @file ym_logic/TvFuncM.h
/// @brief TvFuncM のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/TvFunc.h"
#include "ym_logic/npn_nsdef.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TvFuncM TvFunc.h "ym_logic/TvFunc.h"
/// @brief 多出力論理関数を表すクラス
//////////////////////////////////////////////////////////////////////
class TvFuncM
{
public:

  /// @brief 入力数と出力数を指定したコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] no 出力数 ( > 0 )
  /// @note 中身は恒偽関数
  explicit
  TvFuncM(ymuint ni = 0,
	  ymuint no = 1);

  /// @brief TvFunc を用いたコンストラクタ
  /// @param[in] src_list 各出力の論理関数
  /// @note src_list の関数の入力数は等しくなければならない．
  TvFuncM(const vector<TvFunc>& src_list);

  /// @brief コピーコンストラクタ
  TvFuncM(const TvFuncM& src);

  /// @brief TvFunc からの変換用コンストラクタ
  explicit
  TvFuncM(const TvFunc& src);

  /// @brief 代入演算子
  const TvFuncM&
  operator=(const TvFuncM& src);

  /// @brief デストラクタ
  ~TvFuncM();


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算を伴った代入
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自身への参照を返す．
  const TvFuncM&
  negate();

  /// @brief src1 との論理積を計算し自分に代入する．
  /// @param[in] src1 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFuncM&
  operator&=(const TvFuncM& src1);

  /// @brief src1 との論理和を計算し自分に代入する．
  /// @param[in] src1 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFuncM&
  operator|=(const TvFuncM& src1);

  /// @brief src1 との排他的論理和を計算し自分に代入する．
  /// @param[in] src1 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFuncM&
  operator^=(const TvFuncM& src1);

  /// @brief コファクターを計算し自分に代入する．
  /// @param[in] varid 変数番号
  /// @param[in] pol 極性
  /// @return 自身への参照を返す．
  const TvFuncM&
  set_cofactor(ymuint varid,
	       tPol pol);


public:
  //////////////////////////////////////////////////////////////////////
  // 一つに出力に対して論理演算を伴った代入
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @param[in] opos 出力番号
  /// @return 自身への参照を返す．
  const TvFuncM&
  negate(ymuint opos);

  /// @brief src1 との論理積を計算し自分に代入する．
  /// @param[in] opos 出力番号
  /// @param[in] src1 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFuncM&
  and_assign(ymuint opos,
	     const TvFunc& src1);

  /// @brief src1 との論理和を計算し自分に代入する．
  /// @param[in] opos 出力番号
  /// @param[in] src1 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFuncM&
  or_assign(ymuint opos,
	    const TvFunc& src1);

  /// @brief src1 との排他的論理和を計算し自分に代入する．
  /// @param[in] opos 出力番号
  /// @param[in] src1 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFuncM&
  xor_assign(ymuint opos,
	     const TvFunc& src1);

  /// @brief コファクターを計算し自分に代入する．
  /// @param[in] opos 出力番号
  /// @param[in] varid 変数番号
  /// @param[in] pol 極性
  /// @return 自身への参照を返す．
  const TvFuncM&
  set_cofactor(ymuint opos,
	       ymuint varid,
	       tPol pol);


public:
  //////////////////////////////////////////////////////////////////////
  //　情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  ymuint
  ni() const;

  /// @brief 出力数を得る．
  ymuint
  no() const;

  /// @brief 1出力の論理関数を切り出す．
  /// @param[in] opos 出力番号
  TvFunc
  output(ymuint opos) const;

  /// @brief 入力値を2進数と見なしたときの pos 番目の値を得る．
  /// @param[in] opos 出力番号
  /// @param[in] pos 変数番号
  /// 答は 0 か 1 だが int 型
  int
  value(ymuint opos,
	ymuint pos) const;

  /// @brief pos 番目の変数がサポートの時 true を返す．
  /// @param[in] pos 変数番号
  bool
  check_sup(tVarId pos) const;

  /// @brief pos1 番目と pos2 番目の変数が対称のとき true を返す．
  /// @param[in] pos1, pos2 変数番号
  /// @param[in] pol 極性
  bool
  check_sym(tVarId pos1,
	    tVarId pos2,
	    tPol pol = kPolPosi) const;

  /// @brief ハッシュ値を返す．
  ymuint
  hash() const;


public:
  //////////////////////////////////////////////////////////////////////
  // デバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容の出力
  /// @param[in] s 出力先のストリーム
  /// @param[in] mode 出力モード
  /// @note mode は 2 か 16
  void
  print(ostream& s,
	int mode = 2) const;


public:
  //////////////////////////////////////////////////////////////////////
  // BinIO 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリファイルの書き出し
  /// @param[in] s 出力先のストリーム
  void
  dump(BinO& s) const;

  /// @brief バイナリファイルの読み込み
  /// @param[in] s 入力元のストリーム
  void
  restore(BinI& s);


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算
  // 多くは演算子のオーバーロードになっているのでここは少ない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを返す．
  /// @param[in] varid 変数番号
  /// @param[in] pol 極性
  TvFuncM
  cofactor(ymuint varid,
	   tPol pol) const;

  /// @brief npnmap に従った変換を行う．
  /// @param[in] npnmap 変換マップ
  /// @return 変換した関数を返す．
  TvFuncM
  xform(const NpnMapM& npnmap) const;


public:
  //////////////////////////////////////////////////////////////////////
  // その他の実装依存の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ブロック数を得る．
  ymuint
  nblk() const;

  /// @brief 生のデータを得る．
  ymulong
  raw_data(ymuint blk) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大の入力数
  // 特に根拠はないが，これなら Walsh 係数が 32 ビット整数で収まる．
  // あと真理値表ベースの手法ではこれくらいが限度
  static
  const ymuint kMaxNi = 20;


public:
  //////////////////////////////////////////////////////////////////////
  // フレンド関数の定義
  // ここの public に意味はない
  //////////////////////////////////////////////////////////////////////

  friend
  bool
  operator==(const TvFuncM& func1,
	     const TvFuncM& func2);

  friend
  bool
  operator<(const TvFuncM& func1,
	    const TvFuncM& func2);

  friend
  bool
  operator>(const TvFuncM& func1,
	    const TvFuncM& func2);

  friend
  bool
  operator<=(const TvFuncM& func1,
	    const TvFuncM& func2);

  friend
  bool
  operator>=(const TvFuncM& func1,
	    const TvFuncM& func2);

  friend
  bool
  operator&&(const TvFuncM& func1,
	     const TvFuncM& func2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 大小比較を実際に行う関数
  bool
  lt(const TvFuncM& right) const;

  /// @brief 2つの関数の入出力が等しいかチェックする．
  static
  bool
  check_nio(const TvFuncM& f1,
	    const TvFuncM& f2);

  /// @brief 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
  static
  ymuint
  nblock(ymuint ni);

  /// @brief ipos 番目の要素のブロック位置を計算する．
  static
  ymuint
  block(ymuint pos);

  /// @brief ipos 番目の要素のシフト量を計算する．
  static
  ymuint
  shift(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mNi;

  // 出力数
  ymuint32 mNo;

  // 1出力分のブロック数
  ymuint32 mNblk1;

  // ブロック数
  ymuint32 mNblk;

  // パックされた真理値ベクトル
  ymulong* mVector;

};


//////////////////////////////////////////////////////////////////////
// TvFuncM の演算子
//////////////////////////////////////////////////////////////////////

/// @relates TvFuncM
/// @brief 否定を求める．
TvFuncM
operator~(const TvFuncM& src);

/// @relates TvFuncM
/// @brief 論理積を求める．
TvFuncM
operator&(const TvFuncM& src1,
	  const TvFuncM& src2);

/// @relates TvFuncM
/// @brief 論理和を求める．
TvFuncM
operator|(const TvFuncM& src1,
	  const TvFuncM& src2);

/// @relates TvFuncM
/// @brief 排他的論理和を求める．
TvFuncM
operator^(const TvFuncM& src1,
	  const TvFuncM& src2);

/// @relates TvFuncM
/// @brief 等価比較
bool
operator==(const TvFuncM& func1,
	   const TvFuncM& func2);

/// @relates TvFuncM
/// @brief 非等価比較
bool
operator!=(const TvFuncM& src1,
	   const TvFuncM& src2);

/// @relates TvFuncM
/// @brief 大小比較(小なり)
bool
operator<(const TvFuncM& func1,
	  const TvFuncM& func2);

/// @relates TvFuncM
/// @brief 大小比較(大なり)
bool
operator>(const TvFuncM& src1,
	  const TvFuncM& src2);

/// @relates TvFuncM
/// @brief 大小比較(小なりイコール)
bool
operator<=(const TvFuncM& src1,
	   const TvFuncM& src2);

/// @relates TvFuncM
/// @brief 大小比較(大なりイコール)
bool
operator>=(const TvFuncM& src1,
	   const TvFuncM& src2);

/// @relates TvFuncM
/// @brief 交差チェック
bool
operator&&(const TvFuncM& src1,
	   const TvFuncM& src2);

/// @relates TvFuncM
/// @brief ストリームに対する出力
ostream&
operator<<(ostream& s,
	   const TvFuncM& func);

/// @brief バイナリ出力
/// @param[in] s 出力ストリーム
/// @param[in] func 関数
/// @return s
BinO&
operator<<(BinO& s,
	   const TvFuncM& func);

/// @brief バイナリ入力
/// @param[in] s 入力ストリーム
/// @param[out] func 関数
/// @return s
BinI&
operator>>(BinI& s,
	   TvFuncM& func);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 入力数を得る．
inline
ymuint
TvFuncM::ni() const
{
  return mNi;
}

// @brief 出力数を得る．
inline
ymuint
TvFuncM::no() const
{
  return mNo;
}

// 入力値を2進数と見なしたときの (opos, pos) 番目の値を得る．
// 答は 0 か 1 だが int 型
inline
int
TvFuncM::value(ymuint opos,
	       ymuint pos) const
{
  return (mVector[block(pos) + opos * mNblk1] >> shift(pos)) & 1;
}

// ブロック数を得る．
inline
ymuint
TvFuncM::nblk() const
{
  return mNblk;
}

// 生のデータを得る．
inline
ymulong
TvFuncM::raw_data(ymuint blk) const
{
  return mVector[blk];
}

// @brief 2つの関数の入出力が等しいかチェックする．
inline
bool
TvFuncM::check_nio(const TvFuncM& f1,
		   const TvFuncM& f2)
{
  return  (f1.mNi == f2.mNi) && (f1.mNo == f2.mNo);
}

// 入力数 ni, 出力数 no のベクタを納めるのに必要なブロック数を計算する．
inline
ymuint
TvFuncM::nblock(ymuint ni)
{
  const ymuint wsize = sizeof(ymulong) * 8;
  return ((1 << ni) + wsize - 1) / wsize;
}

// pos 番目の要素のブロック位置を計算する．
inline
ymuint
TvFuncM::block(ymuint pos)
{
  const ymuint wsize = sizeof(size_t) * 8;
  return pos / wsize;
}

// pos 番目の要素のシフト量を計算する．
inline
ymuint
TvFuncM::shift(ymuint pos)
{
  const ymuint wsize = sizeof(size_t) * 8;
  return pos % wsize;
}

// 否定を求める．
inline
TvFuncM
operator~(const TvFuncM& src)
{
  return TvFuncM(src).negate();
}

// 論理積を求める．
inline
TvFuncM
operator&(const TvFuncM& src1,
	  const TvFuncM& src2)
{
  return TvFuncM(src1).operator&=(src2);
}

// 論理和を求める．
inline
TvFuncM
operator|(const TvFuncM& src1,
	  const TvFuncM& src2)
{
  return TvFuncM(src1).operator|=(src2);
}

// 排他的論理和を求める．
inline
TvFuncM
operator^(const TvFuncM& src1,
	  const TvFuncM& src2)
{
  return TvFuncM(src1).operator^=(src2);
}

// @brief コファクターを返す．
// @param[in] varid 変数番号
// @param[in] pol 極性
inline
TvFuncM
TvFuncM::cofactor(ymuint varid,
		 tPol pol) const
{
  return TvFuncM(*this).set_cofactor(varid, pol);
}

// 等価比較
inline
bool
operator!=(const TvFuncM& src1,
	   const TvFuncM& src2)
{
  return !operator==(src1, src2);
}

// 大小比較のバリエーション
inline
bool
operator<(const TvFuncM& src1,
	  const TvFuncM& src2)
{
  if ( !TvFuncM::check_nio(src1, src2) ) {
    return false;
  }
  return src1.lt(src2);
}

inline
bool
operator>(const TvFuncM& src1,
	  const TvFuncM& src2)
{
  if ( !TvFuncM::check_nio(src1, src2) ) {
    return false;
  }
  return src2.lt(src1);
}

inline
bool
operator<=(const TvFuncM& src1,
	   const TvFuncM& src2)
{
  if ( !TvFuncM::check_nio(src1, src2) ) {
    return false;
  }
  return !src2.lt(src1);
}

inline
bool
operator>=(const TvFuncM& src1,
	   const TvFuncM& src2)
{
  if ( !TvFuncM::check_nio(src1, src2) ) {
    return false;
  }
  return !src1.lt(src2);
}

// ストリームに対する出力
inline
ostream&
operator<<(ostream& s,
	   const TvFuncM& func)
{
  func.print(s, 2);
  return s;
}

// @brief バイナリ出力
// @param[in] s 出力ストリーム
// @param[in] func 関数
// @return s
inline
BinO&
operator<<(BinO& s,
	   const TvFuncM& func)
{
  func.dump(s);
  return s;
}

// @brief バイナリ入力
// @param[in] s 入力ストリーム
// @param[out] func 関数
// @return s
inline
BinI&
operator>>(BinI& s,
	   TvFuncM& func)
{
  func.restore(s);
  return s;
}

END_NAMESPACE_YM


BEGIN_NAMESPACE_HASH
// TvFuncM をキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::TvFuncM>
{
  ymuint
  operator()(const nsYm::TvFuncM& f) const
  {
    return f.hash();
  }
};
END_NAMESPACE_HASH

#endif // YM_LOGIC_TVFUNCM_H
