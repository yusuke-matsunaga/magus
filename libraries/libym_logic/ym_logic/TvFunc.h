#ifndef YM_LOGIC_TVFUNC_H
#define YM_LOGIC_TVFUNC_H

/// @file ym_logic/TvFunc.h
/// @brief TvFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/VarId.h"
#include "ym_logic/Pol.h"
#include "ym_logic/npn_nsdef.h"
#include "ym_utils/IDO.h"
#include "ym_utils/ODO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TvFunc TvFunc.h "ym_logic/TvFunc.h"
/// @ingroup NpnGroup
/// @brief 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////
class TvFunc
{
public:

  /// @brief 入力数のみ指定したコンストラクタ
  /// @param[in] ni 入力数
  /// 中身は恒偽関数
  explicit
  TvFunc(ymuint ni = 0);

  /// @brief 入力数と真理値を指定したコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] values 真理値のベクタ
  TvFunc(ymuint ni,
	 const vector<int>& values);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のソースオブジェクト
  TvFunc(const TvFunc& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のソースオブジェクト
  /// @return 自分自身への参照を返す．
  const TvFunc&
  operator=(const TvFunc& src);

  /// @brief デストラクタ
  ~TvFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // オブジェクト生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 恒偽関数を作る．
  /// @param[in] ni 入力数
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  const_zero(ymuint ni);

  /// @brief 恒真関数を作る．
  /// @param[in] ni 入力数
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  const_one(ymuint ni);

  /// @brief 肯定のリテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] varid リテラルの変数番号
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  posi_literal(ymuint ni,
	       VarId varid);

  /// @brief 否定のリテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] varid リテラルの変数番号
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  nega_literal(ymuint ni,
	       VarId varid);


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算を伴った代入
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自身への参照を返す．
  const TvFunc&
  negate();

  /// @brief src1 との論理積を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFunc&
  operator&=(const TvFunc& src1);

  /// @brief src1 との論理和を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFunc&
  operator|=(const TvFunc& src1);

  /// @brief src1 との排他的論理和を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  const TvFunc&
  operator^=(const TvFunc& src1);

  /// @brief コファクターを計算し自分に代入する．
  /// @param[in] varid 変数番号
  /// @param[in] pol 極性
  /// @return 自身への参照を返す．
  const TvFunc&
  set_cofactor(VarId varid,
	       tPol pol);


public:
  //////////////////////////////////////////////////////////////////////
  //　情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief 入力値を2進数と見なしたときの pos 番目の値を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < 2^(input_num()) )
  /// 答は 0 か 1 だが int 型
  int
  value(ymuint pos) const;

  /// @brief 0 の数を数える．
  ymuint
  count_zero() const;

  /// @brief 1 の数を数える．
  ymuint
  count_one() const;

  /// @brief 0次の Walsh 係数を求める．
  ymint
  walsh_0() const;

  /// @brief 1次の Walsh 係数を求める．
  /// @param[in] varid 変数番号
  ymint
  walsh_1(VarId varid) const;

  /// @brief 2次の Walsh 係数を求める．
  /// @param[in] var1, var2 変数番号
  int
  walsh_2(VarId var1,
	  VarId var2) const;

  /// @brief 0次と 1次の Walsh 係数を求める．
  /// @param[out] vec 値を格納する配列
  int
  walsh_01(int vec[]) const;

  /// @brief 0次と 1次と 2次の Walsh 係数を求める．
  /// @param[out] vec1, vec2 値を格納する配列
  int
  walsh_012(int vec1[],
	    int vec2[]) const;

  /// @brief 重み別の 0 次の Walsh 係数を求める．
  /// @param[in] w
  /// @param[in] opol
  /// @param[in] ibits
  int
  walsh_w0(ymuint w,
	   tPol opol,
	   ymuint ibits) const;

  /// @brief 重み別の 1 次の Walsh 係数を求める．
  int
  walsh_w1(VarId i,
	   ymuint w,
	   tPol opol,
	   ymuint ibits) const;

  /// @brief pos 番目の変数がサポートの時 true を返す．
  /// @param[in] pos 変数番号
  bool
  check_sup(VarId pos) const;

  /// @brief pos1 番目と pos2 番目の変数が対称のとき true を返す．
  /// @param[in] pos1, pos2 変数番号
  /// @param[in] pol 極性
  bool
  check_sym(VarId pos1,
	    VarId pos2,
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
  // IDO/ODO 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリファイルの書き出し
  /// @param[in] s 出力先のストリーム
  void
  dump(ODO& s) const;

  /// @brief バイナリファイルの読み込み
  /// @param[in] s 入力元のストリーム
  void
  restore(IDO& s);


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算
  // 多くは演算子のオーバーロードになっているのでここは少ない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを返す．
  /// @param[in] varid 変数番号
  /// @param[in] pol 極性
  TvFunc
  cofactor(VarId varid,
	   tPol pol) const;

  /// @brief npnmap に従った変換を行う．
  /// @param[in] npnmap 変換マップ
  /// @return 変換した関数を返す．
  TvFunc
  xform(const NpnMap& npnmap) const;


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
  /// 特に根拠はないが，これなら Walsh 係数が 32 ビット整数で収まる．
  /// あと真理値表ベースの手法ではこれくらいが限度
  static
  const ymuint kMaxNi = 20;


public:
  //////////////////////////////////////////////////////////////////////
  // フレンド関数の定義
  // ここの public に意味はない
  //////////////////////////////////////////////////////////////////////

  friend
  bool
  operator==(const TvFunc& left,
	     const TvFunc& right);

  friend
  bool
  operator<(const TvFunc& left,
	    const TvFunc& right);

  friend
  bool
  operator&&(const TvFunc& left,
	     const TvFunc& right);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 恒真関数を作るコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] dummy ダミー
  /// 2番目の引数はダミー
  TvFunc(ymuint ni,
	 int dummy);

  /// @brief リテラル関数を作るコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] varid 変数番号
  /// @param[in] pol 極性
  TvFunc(ymuint ni,
	 VarId varid,
	 tPol pol);

  /// @brief 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
  /// @param[in] ni 入力数
  static
  ymuint
  nblock(ymuint ni);

  /// @brief pos 番目の要素のブロック位置を計算する．
  /// @param[in] pos 位置番号
  static
  ymuint
  block(ymuint pos);

  /// @brief pos 番目の要素のシフト量を計算する．
  /// @param[in] pos 位置番号
  static
  ymuint
  shift(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // ブロック数
  ymuint32 mBlockNum;

  // パックされた真理値ベクトル
  ymulong* mVector;

};


//////////////////////////////////////////////////////////////////////
// TvFunc の演算子
//////////////////////////////////////////////////////////////////////

/// @relates TvFunc
/// @brief 否定を求める．
/// @param[in] src オペランド
TvFunc
operator~(const TvFunc& src);

/// @relates TvFunc
/// @brief 論理積を求める．
/// @param[in] left, right オペランド
TvFunc
operator&(const TvFunc& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 論理和を求める．
/// @param[in] left, right オペランド
TvFunc
operator|(const TvFunc& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 排他的論理和を求める．
/// @param[in] left, right オペランド
TvFunc
operator^(const TvFunc& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 等価比較
/// @param[in] left, right オペランド
bool
operator==(const TvFunc& left,
	   const TvFunc& right);

/// @relates TvFunc
/// @brief 非等価比較
/// @param[in] left, right オペランド
bool
operator!=(const TvFunc& left,
	   const TvFunc& right);

/// @relates TvFunc
/// @brief 大小比較(小なり)
/// @param[in] left, right オペランド
bool
operator<(const TvFunc& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 大小比較(大なり)
/// @param[in] left, right オペランド
bool
operator>(const TvFunc& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 大小比較(小なりイコール)
/// @param[in] left, right オペランド
bool
operator<=(const TvFunc& left,
	   const TvFunc& right);

/// @relates TvFunc
/// @brief 大小比較(大なりイコール)
/// @param[in] left, right オペランド
bool
operator>=(const TvFunc& left,
	   const TvFunc& right);

/// @relates TvFunc
/// @brief 交差チェック
/// @param[in] left, right オペランド
bool
operator&&(const TvFunc& left,
	   const TvFunc& right);

/// @relates TvFunc
/// @brief ストリームに対する出力
/// @param[in] s 出力先のストリーム
/// @param[in] func 対象の関数
ostream&
operator<<(ostream& s,
	   const TvFunc& func);

/// @brief バイナリ出力
/// @param[in] s 出力ストリーム
/// @param[in] func 関数
/// @return s
ODO&
operator<<(ODO& s,
	   const TvFunc& func);

/// @brief バイナリ入力
/// @param[in] s 入力ストリーム
/// @param[out] func 関数
/// @return s
IDO&
operator>>(IDO& s,
	   TvFunc& func);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 入力数を得る．
inline
ymuint
TvFunc::input_num() const
{
  return mInputNum;
}

// 入力値を2進数と見なしたときの pos 番目の値を得る．
// 答は 0 か 1 だが int 型
inline
int
TvFunc::value(ymuint pos) const
{
  return (mVector[block(pos)] >> shift(pos)) & 1;
}

// ブロック数を得る．
inline
ymuint
TvFunc::nblk() const
{
  return mBlockNum;
}

// 生のデータを得る．
inline
ymulong
TvFunc::raw_data(ymuint blk) const
{
  return mVector[blk];
}

// 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
inline
ymuint
TvFunc::nblock(ymuint ni)
{
  const ymuint wsize = sizeof(ymulong) * 8;
  return ((1 << ni) + wsize - 1) / wsize;
}

// pos 番目の要素のブロック位置を計算する．
inline
ymuint
TvFunc::block(ymuint pos)
{
  const ymuint wsize = sizeof(size_t) * 8;
  return pos / wsize;
}

// pos 番目の要素のシフト量を計算する．
inline
ymuint
TvFunc::shift(ymuint pos)
{
  const ymuint wsize = sizeof(size_t) * 8;
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
operator&(const TvFunc& left,
	  const TvFunc& right)
{
  return TvFunc(left).operator&=(right);
}

// 論理和を求める．
inline
TvFunc
operator|(const TvFunc& left,
	  const TvFunc& right)
{
  return TvFunc(left).operator|=(right);
}

// 排他的論理和を求める．
inline
TvFunc
operator^(const TvFunc& left,
	  const TvFunc& right)
{
  return TvFunc(left).operator^=(right);
}

// @brief コファクターを返す．
// @param[in] varid 変数番号
// @param[in] pol 極性
inline
TvFunc
TvFunc::cofactor(VarId varid,
		 tPol pol) const
{
  return TvFunc(*this).set_cofactor(varid, pol);
}

// 等価比較
inline
bool
operator!=(const TvFunc& left,
	   const TvFunc& right)
{
  return !operator==(left, right);
}

// 大小比較のバリエーション
inline
bool
operator>(const TvFunc& left,
	  const TvFunc& right)
{
  return operator<(right, left);
}

inline
bool
operator<=(const TvFunc& left,
	   const TvFunc& right)
{
  return !operator<(right, left);
}

inline
bool
operator>=(const TvFunc& left,
	   const TvFunc& right)
{
  return !operator<(left, right);
}

// ストリームに対する出力
inline
ostream&
operator<<(ostream& s,
	   const TvFunc& func)
{
  func.print(s, 2);
  return s;
}

// @brief バイナリ出力
// @param[in] s 出力ストリーム
// @param[in] func 関数
// @return s
inline
ODO&
operator<<(ODO& s,
	   const TvFunc& func)
{
  func.dump(s);
  return s;
}

// @brief バイナリ入力
// @param[in] s 入力ストリーム
// @param[out] func 関数
// @return s
inline
IDO&
operator>>(IDO& s,
	   TvFunc& func)
{
  func.restore(s);
  return s;
}

END_NAMESPACE_YM

BEGIN_NAMESPACE_HASH
// TvFunc をキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::TvFunc>
{
  ymuint
  operator()(const nsYm::TvFunc& f) const
  {
    return f.hash();
  }
};
END_NAMESPACE_HASH

#endif // YM_LOGIC_TVFUNC_H
