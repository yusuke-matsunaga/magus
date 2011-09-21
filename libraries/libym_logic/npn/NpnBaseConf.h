#ifndef LIBYM_NPN_NPNBASECONF_H
#define LIBYM_NPN_NPNBASECONF_H

/// @file libym_npn/NpnBaseConf.h
/// @brief NpnBaseConf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/NpnMap.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM_NPN

class NpnConf;

//////////////////////////////////////////////////////////////////////
/// @class NpnBaseConf NpnBaseConf.h "NpnBaseConf.h"
/// @brief NPN同値類を区別するためのシグネチャを表すクラス
//////////////////////////////////////////////////////////////////////
class NpnBaseConf
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 対象の関数
  NpnBaseConf(const TvFunc& func);

  /// @brief デストラクタ
  ~NpnBaseConf();


public:

  /// @brief W0/W1 を用いて正規化する．
  /// @param[out] conf 結果を格納するオブジェクト
  void
  normalize(NpnConf& conf);


public:

  /// @brief 対象の関数を得る．
  const TvFunc&
  func() const;

  /// @brief 入力数を得る．
  ymuint
  ni() const;

  /// @brief 重み別 Walsh の 0次係数を得る．
  int
  walsh_w0(ymuint w,
	   tPol opol,
	   tPol ipol[]) const;

  /// @brief Walsh の 2次係数を得る．
  /// @param[in] pos1, pos2 入力番号
  /// @return pos1 番めと pos2 番めの入力に対応する Walsh の 2次係数を返す．
  /// @note \f$0 \leq pos1, pos2 < ni()\f$
  int
  walsh_2(ymuint pos1,
	  ymuint pos2) const;


public:

  /// @brief 出力極性を得る．
  /// @retval  1 正極性
  /// @retval  0 未定
  /// @retval -1 負極性
  int
  opol() const;

  /// @brief 入力極性を得る．
  /// @retval  1 正極性
  /// @retval  0 未定
  /// @retval -1 負極性
  int
  ipol(ymuint pos) const;


public:

  /// @brief 等価入力クラス数を返す．
  ymuint
  nc() const;

  /// @brief 等価入力クラスの要素数を返す．
  /// @param[in] rep 入力クラス番号 ( 0 <= rep < nc() )
  ymuint
  ic_num(ymuint rep) const;

  /// @brief 等価入力クラスの pos の次の要素を返す．
  ymuint
  ic_link(ymuint pos) const;

  /// @brief 独立(無関係)な入力クラスの先頭番号を返す．
  ymuint
  indep_rep() const;

  /// @brief 独立な入力クラスの要素数を返す．
  ymuint
  indep_num() const;

  /// @brief 等価入力クラスの bisym マークを返す．
  /// @param[in] rep 入力クラス番号 ( 0 <= rep < nc() )
  bool
  bisym(ymuint rep) const;

  /// @brief rep1 が rep2 より大きければ true を返す．
  bool
  w1gt(ymuint rep1,
       ymuint rep2) const;

  /// @brief rep1 と rep2 が等しければ true を返す．
  bool
  w1eq(ymuint rep1,
       ymuint rep2) const;


private:

  /// @brief すべてのシグネチャを反転させる．
  void
  invert_all();

  /// @brief 一つの入力に関するシグネチャを反転する．
  void
  invert_input(ymuint pos);

  /// @brief 2次の係数のみを反転する．
  void
  invert_all_w2();

  /// @brief w2 の要素を反転する．
  void
  invert_w2(ymuint pos1,
	    ymuint pos2);

  /// @brief 等価入力クラスに要素を足す．
  void
  add_elem(ymuint rep,
	   ymuint pos);

  /// @brief 等価入力クラスに bisym マークをつける．
  void
  set_bisym(ymuint rep);

  /// @brief 独立な入力クラスに要素を足す．
  void
  add_indep(ymuint rep);


public:

  /// @brief Walsh 係数を出力する．
  void
  dump_walsh(ostream& s) const;

  /// @brief 極性情報を出力する．
  void
  dump_pols(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の関数
  const TvFunc& mFunc;

  // 関数の入力数
  ymuint32 mNi;

  // Walsh の 0次係数
  ymint32 mW0;

  // Walsh の 1次係数
  ymint32* mW1;

  // Walsh の 2次係数
  mutable
  ymint32* mW2;

  // mW2 が計算済みかどうかを記録するビットマップ
  mutable
  ymuint8* mW2flag;

  // 出力極性
  ymint8 mOpol;

  // 入力極性を表す配列
  ymint8* mIpols;

  // 等価入力クラスの数
  ymuint32 mNc;

  // 等価入力クラスの先頭番号のリスト
  ymuint32* mIcRep;

  // 独立な入力クラスの先頭番号
  ymuint32 mIndepRep;

  // 独立な入力クラスの要素数
  ymuint32 mIndepNum;

  // 等価入力クラスの要素数の配列
  // キーは先頭番号
  ymuint32* mIcNum;

  // 等価入力クラスの次の要素を指す配列
  ymuint32* mIcLink;

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

// Walsh の 2次係数を得る．
inline
int
NpnBaseConf::walsh_2(ymuint pos1,
		     ymuint pos2) const
{
  if ( pos2 > pos1 ) {
    int tmp = pos1;
    pos1 = pos2;
    pos2 = tmp;
  }
  ymuint base = pos1 * ni() + pos2;
  if ( (mW2flag[base] & 1) == 0 ) {
    int w2 = mFunc.walsh_2(pos1, pos2);
    if ( mW2flag[base] & 2 ) {
      w2 = -w2;
    }
    mW2[base] = w2;
    mW2flag[base] = 1;
  }
  return mW2[base];
}

#if 1
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
#endif

// @brief 等価入力クラス数を返す．
inline
ymuint
NpnBaseConf::nc() const
{
  return mNc;
}

// 等価入力クラスの要素数を返す．
inline
ymuint
NpnBaseConf::ic_num(ymuint rep) const
{
  return (mIcNum[rep] >> 1);
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

// 等価入力クラスの bisym マークを返す．
inline
bool
NpnBaseConf::bisym(ymuint rep) const
{
  return static_cast<bool>(mIcNum[rep] & 1);
}

// rep1 が rep2 より大きければ true を返す．
inline
bool
NpnBaseConf::w1gt(ymuint rep1,
		ymuint rep2) const
{
  int diff = mW1[rep1] - mW1[rep2];
  if ( diff > 0 ) {
    return true;
  }
  if ( diff == 0 ) {
    return mIcNum[rep1] >= mIcNum[rep2];
  }
  return false;
}

// rep1 と rep2 が等しければ true を返す．
inline
bool
NpnBaseConf::w1eq(ymuint rep1,
		ymuint rep2) const
{
  return mW1[rep1] == mW1[rep2] && mIcNum[rep1] == mIcNum[rep2];
}

// 等価入力クラスに要素を足す．
inline
void
NpnBaseConf::add_elem(ymuint rep,
		    ymuint pos)
{
  ymuint n = (mIcNum[rep] >> 1);
  ymuint pos0 = rep;
  for (ymuint i = 1; i < n; ++ i) {
    pos0 = mIcLink[pos0];
  }
  mIcLink[pos0] = pos;
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
NpnBaseConf::add_indep(ymuint rep)
{
  if ( mIndepNum == 0 ) {
    mIndepRep = rep;
  }
  else {
    ymuint pos = mIndepRep;
    for (ymuint i = 1; i < mIndepNum; ++ i) {
      pos = mIcLink[pos];
    }
    mIcLink[pos] = rep;
  }
  ++ mIndepNum;
}

END_NAMESPACE_YM_NPN

#endif // LIBYM_NPN_NPNBASECONF_H
