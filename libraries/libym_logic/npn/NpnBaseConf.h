#ifndef LIBYM_NPN_NPNBASECONF_H
#define LIBYM_NPN_NPNBASECONF_H

/// @file libym_npn/NpnBaseConf.h
/// @brief NpnBaseConf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/NpnMap.h"
#include "ym_logic/TvFunc.h"


#define USE_MALLOC 0

BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// @class NpnBaseConf NpnBaseConf.h "NpnBaseConf.h"
/// @brief NPN同値類を区別するためのシグネチャを表すクラス
//////////////////////////////////////////////////////////////////////
class NpnBaseConf
{
public:

  /// @brief コンストラクタ
  NpnBaseConf();

  /// @brief デストラクタ
  ~NpnBaseConf();


public:

  /// @brief W0/W1 を用いて正規化する．
  /// @param[in] func 対象の関数
  void
  normalize(const TvFunc& func);


public:

  /// @brief 対象の関数を得る．
  const TvFunc&
  func() const;

  /// @brief 入力数を得る．
  ymuint
  ni() const;

#if 0
  /// @brief 重み別 Walsh の 0次係数を得る．
  int
  walsh_w0(ymuint w,
	   tPol opol,
	   tPol ipol[]) const;
#endif

  /// @brief Walsh の 1次係数を得る．
  /// @param[in] pos 入力番号 ( 0 <= pos < ni() )
  int
  walsh_1(ymuint pos) const;

  /// @brief Walsh の 2次係数を得る．
  /// @param[in] pos1, pos2 入力番号 ( 0 <= pos1, pos2 < ni() )
  /// @return pos1 番めと pos2 番めの入力に対応する Walsh の 2次係数を返す．
  int
  walsh_2(ymuint pos1,
	  ymuint pos2) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 極性情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力極性を得る．
  /// @retval  0 未定
  /// @retval  1 正極性
  /// @retval  2 負極性
  int
  opol() const;

  /// @brief 入力極性を得る．
  /// @param[in] pos 入力番号 ( 0 <= pos < ni() )
  /// @retval  0 未定
  /// @retval  1 正極性
  /// @retval  2 負極性
  int
  ipol(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力順情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価入力クラス数を返す．
  ymuint
  nc() const;

  /// @brief 等価入力クラスの先頭番頭を返す．
  /// @param[in] cid クラス番号 ( 0 <= cid < nc() )
  ymuint
  ic_rep(ymuint cid) const;

  /// @brief 等価入力クラスの要素数を返す．
  /// @param[in] pos 先頭の入力番号 ( 0 <= pos < ni() )
  ymuint
  ic_num(ymuint pos) const;

  /// @brief 等価入力クラスの bisym マークを返す．
  /// @param[in] pos 先頭の入力番号 ( 0 <= pos < ni() )
  bool
  bisym(ymuint pos) const;

  /// @brief 独立な入力クラスの要素数を返す．
  ymuint
  indep_num() const;

  /// @brief 独立(無関係)な入力クラスの先頭番号を返す．
  /// @note 2番め以降の要素は ic_link() でたどる．
  ymuint
  indep_rep() const;

  /// @brief 等価入力クラスの pos の次の要素を返す．
  /// @param[in] pos 入力番号 ( 0 <= pos < ni() )
  ymuint
  ic_link(ymuint pos) const;

  /// @brief pos1 が pos2 より大きければ true を返す．
  bool
  w1gt(ymuint pos1,
       ymuint pos2) const;

  /// @brief pos1 と pos2 が等しければ true を返す．
  bool
  w1eq(ymuint pos1,
       ymuint pos2) const;


public:
  //////////////////////////////////////////////////////////////////////
  // デバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Walsh 係数を出力する．
  void
  dump_walsh(ostream& s) const;

  /// @brief 極性情報を出力する．
  void
  dump_pols(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 新しい等価入力クラスを作る．
  /// @param[in] pos 入力番号
  void
  new_ic(ymuint pos);

  /// @brief 等価入力クラスに要素を足す．
  /// @param[in] rep 入力クラスの先頭の入力番号 ( 0 <= rep < ni() )
  /// @param[in] pos 入力番号 ( 0 <= pos < ni() )
  void
  add_elem(ymuint rep,
	   ymuint pos);

  /// @brief 等価入力クラスに bisym マークをつける．
  /// @param[in] rep 入力クラスの先頭の入力番号 ( 0 <= rep < ni() )
  void
  set_bisym(ymuint rep);

  /// @brief 独立な入力クラスに要素を足す．
  /// @param[in] pos 入力番号
  void
  add_indep(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の関数
  TvFunc mFunc;

  // 関数の入力数
  ymuint32 mNi;

  // Walsh の 0次係数
  ymint32 mW0;

  // Walsh の 1次係数
  // 大きさ mNi の配列へのポインタ
#if USE_MALLOC
  ymint32* mW1;
#else
  ymint32 mW1[TvFunc::kMaxNi];
#endif

  // Walsh の 2次係数
  // 大きさ mNi * mNi の配列へのポインタ
  mutable
#if USE_MALLOC
  ymint32* mW2;
#else
  ymint32 mW2[TvFunc::kMaxNi * TvFunc::kMaxNi];
#endif

  // mW2 が計算済みかどうかを記録するフラグ
  // 大きさ mNi * mNi の配列へのポインタ
  // 最下位ビットが計算済みかどうかのフラグ
  // 1ビット目が極性反転を表すフラグ
  mutable
#if USE_MALLOC
  ymuint8* mW2flag;
#else
  ymuint8 mW2flag[TvFunc::kMaxNi * TvFunc::kMaxNi];
#endif

  // 出力極性
  ymuint8 mOpol;

  // 入力極性を表す配列
  // 大きさ mNi の配列へのポインタ
  // インデックスは入力番号
#if USE_MALLOC
  ymuint8* mIpols;
#else
  ymuint8 mIpols[TvFunc::kMaxNi];
#endif

  // 等価入力クラスの数
  ymuint32 mNc;

  // 等価入力クラスの先頭番号のリスト
  // インデックスはクラス番号
#if USE_MALLOC
  ymuint32* mIcRep;
#else
  ymuint32 mIcRep[TvFunc::kMaxNi];
#endif

  // 等価入力クラスの要素数の配列
  // インデックスは入力番号
#if USE_MALLOC
  ymuint32* mIcNum;
#else
  ymuint32 mIcNum[TvFunc::kMaxNi];
#endif

  // 独立な入力クラスの先頭番号
  ymuint32 mIndepRep;

  // 独立な入力クラスの要素数
  ymuint32 mIndepNum;

  // 等価入力クラスの次の要素を指す配列
  // インデックスは入力番号
#if USE_MALLOC
  ymuint32* mIcLink;
#else
  ymuint32 mIcLink[TvFunc::kMaxNi];
#endif

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 対象の関数を得る．
inline
const TvFunc&
NpnBaseConf::func() const
{
  return mFunc;
}

// 入力数を得る．
inline
ymuint
NpnBaseConf::ni() const
{
  return mNi;
}

// @brief Walsh の 1次係数を得る．
// @param[in] pos 入力番号
inline
int
NpnBaseConf::walsh_1(ymuint pos) const
{
  return mW1[pos];
}

// Walsh の 2次係数を得る．
inline
int
NpnBaseConf::walsh_2(ymuint pos1,
		     ymuint pos2) const
{
  // 正規化する．
  if ( pos2 > pos1 ) {
    int tmp = pos1;
    pos1 = pos2;
    pos2 = tmp;
  }
  ymuint index = pos1 * ni() + pos2;
  if ( mW2flag[index] == 0 ) {
    mW2[index] = mFunc.walsh_2(VarId(pos1), VarId(pos2));
    mW2flag[index] = 1;
  }
  return mW2[index];
}

// 出力極性を得る．
inline
int
NpnBaseConf::opol() const
{
  return mOpol;
}

// 入力極性を得る．
inline
int
NpnBaseConf::ipol(ymuint pos) const
{
  return mIpols[pos];
}

// @brief 等価入力クラス数を返す．
inline
ymuint
NpnBaseConf::nc() const
{
  return mNc;
}

// @brief 等価入力クラスの先頭番頭を返す．
// @param[in] cid クラス番号 ( 0 <= cid < nc() )
inline
ymuint
NpnBaseConf::ic_rep(ymuint cid) const
{
  return mIcRep[cid];
}

// @brief 等価入力クラスの要素数を返す．
// @param[in] pos 先頭の入力番号 ( 0 <= pos < ni() )
inline
ymuint
NpnBaseConf::ic_num(ymuint pos) const
{
  return (mIcNum[pos] >> 1);
}

// @brief 等価入力クラスの bisym マークを返す．
// @param[in] pos 先頭の入力番号 ( 0 <= pos < ni() )
inline
bool
NpnBaseConf::bisym(ymuint pos) const
{
  return static_cast<bool>(mIcNum[pos] & 1);
}

// @brief 独立(無関係)な入力クラスの先頭番号を返す．
inline
ymuint
NpnBaseConf::indep_rep() const
{
  return mIndepRep;
}

// @brief 独立な入力クラスの要素数を返す．
inline
ymuint
NpnBaseConf::indep_num() const
{
  return mIndepNum;
}

// 等価入力クラスの pos の次の要素を返す．
inline
ymuint
NpnBaseConf::ic_link(ymuint pos) const
{
  return mIcLink[pos];
}

// pos1 が pos2 より大きければ true を返す．
inline
bool
NpnBaseConf::w1gt(ymuint pos1,
		  ymuint pos2) const
{
  int diff = mW1[pos1] - mW1[pos2];
  if ( diff > 0 ) {
    return true;
  }
  if ( diff == 0 ) {
    return mIcNum[pos1] >= mIcNum[pos2];
  }
  return false;
}

// pos1 と pos2 が等しければ true を返す．
inline
bool
NpnBaseConf::w1eq(ymuint pos1,
		  ymuint pos2) const
{
  return mW1[pos1] == mW1[pos2] && mIcNum[pos1] == mIcNum[pos2];
}

// @brief 新しい等価入力クラスを作る．
// @param[in] pos 入力番号
inline
void
NpnBaseConf::new_ic(ymuint pos)
{
  mIcRep[mNc] = pos;
  mIcNum[pos] = 2; // 要素数:1, bisym: false
  ++ mNc;
}

// 等価入力クラスに要素を足す．
inline
void
NpnBaseConf::add_elem(ymuint rep,
		      ymuint pos)
{
  // 末尾の要素を求める．
  // O(n) のバカな実装
  ymuint n = (mIcNum[rep] >> 1);
  ymuint pos0 = rep;
  for (ymuint i = 1; i < n; ++ i) {
    pos0 = mIcLink[pos0];
  }
  // pos0 の次を pos にする．
  mIcLink[pos0] = pos;
  // 最下位ビットは bi-sym フラグなので2を足す．
  mIcNum[rep] += 2;
}

// 等価入力クラスに bisym マークをつける．
inline
void
NpnBaseConf::set_bisym(ymuint rep)
{
  mIcNum[rep] |= 1;
}

// @brief 独立な入力クラスに要素を足す．
inline
void
NpnBaseConf::add_indep(ymuint pos)
{
  if ( mIndepNum == 0 ) {
    mIndepRep = pos;
  }
  else {
    // 末尾の要素を求める．
    ymuint pos0 = mIndepRep;
    for (ymuint i = 1; i < mIndepNum; ++ i) {
      pos0 = mIcLink[pos0];
    }
    mIcLink[pos0] = pos;
  }
  ++ mIndepNum;
}

END_NAMESPACE_YM_NPN

#endif // LIBYM_NPN_NPNBASECONF_H
