#ifndef SATCLAUSE_H
#define SATCLAUSE_H

/// @file SatClause.h
/// @brief SatClause のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/sat_nsdef.h"
#include "logic/Literal.h"


// リテラルの並び替え時に挿入ではなく置き換えを行う．
#define SWAP_LITERALS 0


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatClause SatClause.h "SatClause.h"
/// @brief 節を表すクラス
///
/// 中身は本当に Literal の配列．しかも，クラスのインスタンスと別に配列
/// 領域を確保するのはポインタ参照が一回増えて嫌なので，見かけはサイズ1
/// の配列 mLits[1] を定義しておいて，実際には要素数分の領域を確保した
/// メモリブロックを SatClause* として扱う．
/// このために「ポインタ付き new」演算子を用いている．
/// 詳しくは YmSat::new_clause() を参照
/// SatClause はそれ以外の情報として，制約節か学習節かを区別する1ビット
/// (サイズと合わせて1ワード)のフラグ，activity を表す double 変数を
/// 持つ．
//////////////////////////////////////////////////////////////////////
class SatClause
{
public:

  /// @brief コンストラクタ
  /// @param[in] lit_num リテラル数
  /// @param[in] lits リテラルの配列
  /// @param[in] learnt 学習節の場合 true
  /// @note 上に書いたように普通にこのコンストラクタを呼んではいけない．
  SatClause(ymuint lit_num,
	    Literal* lits,
	    bool learnt);

  /// @brief デストラクタ
  ~SatClause();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．(2リテラル節用)
  /// @param[in] lit0, lit1 リテラル
  void
  set(Literal lit0,
      Literal lit1);

  /// @brief watch literal を入れ替える．
  void
  xchange_wl();

  /// @brief src_pos 番めのリテラルを dst_pos に移動する．
  /// @param[in] src_pos もとの位置
  /// @param[in] dst_pos 移動先
  /// @note src_pos < dst_pos でなければならない．
  /// @note 間のリテラルは一つずつ右にずれる．
  void
  insert(ymuint src_pos,
	 ymuint dst_pos);

  /// @brief literal block distance を設定する．
  void
  set_lbd(ymuint lbd);

  /// @brief アクティビティを増加させる．
  void
  increase_activity(double delta);

  /// @brief アクティビティを定数倍する．
  void
  factor_activity(double factor);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラル数の取得
  ymuint
  lit_num() const;

  /// @brief リテラルのアクセス
  /// @param[in] pos リテラルの位置 ( 0 <= pos < lit_num() )
  Literal
  lit(ymuint pos) const;

  /// @brief 0番めの watch literal を得る．
  Literal
  wl0() const;

  /// @brief 1番めの watch literal を得る．
  Literal
  wl1() const;

  /// @brief 学習節の場合 true を返す．
  bool
  is_learnt() const;

  /// @brief Literal Block Distance を返す．
  ymuint
  lbd() const;

  /// @brief 学習節の場合にアクティビティを返す．
  double
  activity() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サイズと learnt フラグをパックしたもの
  ymuint32 mSizeLearnt;

  // リテラルブロック距離
  ymuint32 mLBD;

  // activity
  double mActivity;

  // リテラルの配列
  // 実際にはこの後にリテラル数分の領域を確保する．
  Literal mLits[1];

};

/// @relates SatClause
/// @brief SatClause の内容を出力する
ostream&
operator<<(ostream& s,
	   const SatClause& c);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lit_num リテラル数
// @param[in] lits リテラルの配列
// @param[in] learnt 学習節の場合 true
inline
SatClause::SatClause(ymuint lit_num,
		     Literal* lits,
		     bool learnt)
{
  mSizeLearnt = (lit_num << 1) | static_cast<ymuint>(learnt);
  mActivity = 0.0;
  for (ymuint i = 0; i < lit_num; ++ i) {
    mLits[i] = lits[i];
  }
}

// @brief デストラクタ
inline
SatClause::~SatClause()
{
}

// @brief 内容を設定する．(2リテラル節用)
// @param[in] lit0, lit1 リテラル
inline
void
SatClause::set(Literal lit0,
	       Literal lit1)
{
  mLits[0] = lit0;
  mLits[1] = lit1;
}

// @brief watch literal を入れ替える．
inline
void
SatClause::xchange_wl()
{
  Literal tmp = mLits[0];
  mLits[0] = mLits[1];
  mLits[1] = tmp;
}

// @brief src_pos 番めのリテラルを dst_pos に移動する．
// @param[in] src_pos もとの位置
// @param[in] dst_pos 移動先
// @note src_pos < dst_pos でなければならない．
// @note 間のリテラルは一つずつ右にずれる．
inline
void
SatClause::insert(ymuint src_pos,
		  ymuint dst_pos)
{
  Literal tmp = mLits[src_pos];
  for (ymuint i = src_pos; i > dst_pos; -- i) {
    mLits[i] = mLits[i - 1];
  }
  mLits[dst_pos] = tmp;
}

// @brief literal block distance を設定する．
inline
void
SatClause::set_lbd(ymuint lbd)
{
  mLBD = lbd;
}

// @brief リテラル数の取得
inline
ymuint
SatClause::lit_num() const
{
  return (mSizeLearnt >> 1);
}

// @brief リテラルのアクセス
inline
Literal
SatClause::lit(ymuint pos) const
{
  return mLits[pos];
}

// @brief 0番めの watch literal を得る．
inline
Literal
SatClause::wl0() const
{
  return mLits[0];
}

// @brief 1番めの watch literal を得る．
inline
Literal
SatClause::wl1() const
{
  return mLits[1];
}

// @brief 学習節の場合 true を返す．
inline
bool
SatClause::is_learnt() const
{
  return static_cast<bool>(mSizeLearnt & 1UL);
}

// @brief Literal Block Distance を返す．
inline
ymuint
SatClause::lbd() const
{
  return mLBD;
}

// @brief 学習節の場合にアクティビティを返す．
inline
double
SatClause::activity() const
{
  return mActivity;
}

// @brief アクティビティを増加させる．
inline
void
SatClause::increase_activity(double delta)
{
  mActivity += delta;
}

// @brief アクティビティを定数倍する．
inline
void
SatClause::factor_activity(double factor)
{
  mActivity *= factor;
}

END_NAMESPACE_YM_SAT

#endif // SATCLAUSE_H
