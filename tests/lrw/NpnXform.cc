
/// @file NpnXform.cc
/// @brief NpnXform の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "NpnXform.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// 順列を表す配列
ymuint8 perm_table[24][4] = {
#include "perm_table"
};

// 順列のみの合成テーブル
ymuint8 comp_table[24 * 24] = {
#include "comp_table"
};

// 順列のみの逆変換テーブル
ymuint8 inv_table[24] = {
#include "inv_table"
};

// 順列のみの正規化テーブル
ymuint8 perm_normal_table[24 * 4] = {
#include "perm_normal_table"
};

// 入出力の反転ビットの変換テーブル
ymuint8 nperm_table[32 * 24] = {
#include "nperm_table"
};

// 入出力の反転ビットの逆変換テーブル
ymuint8 inv_nperm_table[32 * 24] = {
#include "inv_nperm_table"
};

// サポートごとの代表変換テーブル
ymuint8 rep_perm_table[24][16] = {
#include "rep_perm_table"
};

// 0 番めと 1 番めの順番を入れ替えるテーブル
ymuint8 xchg2_table[24] = {
#include "xchg2_table"
};

END_NONAMESPACE


// @brief 入力の置換を得る．
// @param[in] pos 元の入力位置 ( 0 <= pos < 4 )
// @return 置換先の位置
ymuint
NpnXform::input_perm(ymuint pos) const
{
  ymuint pid = get_perm();
  return perm_table[pid][pos];
}

// @brief 与えられた関数のサポートに関する同値類の代表変換を求める．
NpnXform
NpnXform::rep(ymuint8 sup) const
{
  ymuint perm = get_perm();
  ymuint pols = get_pols();
  ymuint rep_perm = rep_perm_table[perm][sup];
  ymuint rep_pols = pols & ((sup << 1) | 1U);

  // 特別な場合の処理
  // 入力ノードの場合，入力の反転属性を出力に付け替える．
  if ( sup == 1U && rep_pols & 2U ) {
    rep_pols ^= 3U;
  }

  return NpnXform(rep_perm, rep_pols);
}

// @brief 0 番めと 1番めを取り替える．
void
NpnXform::xchg2()
{
  ymuint perm = get_perm();
  ymuint pols = get_pols();
  ymuint xperm = xchg2_table[perm];
  ymuint pols0 = pols & 2U;
  ymuint pols1 = pols & 4U;
  ymuint xpols = pols & ~6U;
  xpols |= pols0 << 1;
  xpols |= pols1 >> 1;
  mData = (xperm << 5) | xpols;
}

// @brief 合成する．
const NpnXform&
NpnXform::operator*=(NpnXform right)
{
  ymuint l_perm = get_perm();
  ymuint r_perm = right.get_perm();
  ymuint c_perm = comp_table[l_perm * 24 + r_perm];
  ymuint l_pols = get_pols();
  ymuint r_pols = right.get_pols();
  ymuint c_pols = l_pols ^ inv_nperm_table[r_pols * 24 + l_perm];
  mData = (c_perm << 5) | c_pols;
  return *this;
}

// @brief 正規化する．
// @param[in] sup サポート数
// @return 正規化後の自分自身を返す．
// @note サポートに含まれていない変数の変換を消去する．
const NpnXform&
NpnXform::normalize(ymuint sup)
{
  ymuint supidx = sup;
  if ( supidx == 4 ) {
    supidx = 3;
  }
  ymuint perm = perm_normal_table[get_perm() * 4 + supidx];

  ymuint polsmask = 0U;
  switch ( sup ) {
  case 0: polsmask = 0x01U; break;
  case 1: polsmask = 0x03U; break;
  case 2: polsmask = 0x07U; break;
  case 3: polsmask = 0x0FU; break;
  case 4: polsmask = 0x1FU; break;
  }
  ymuint pols = get_pols() & polsmask;
  if ( sup == 1 ) {
    if ( pols == 0x3U ) {
      pols = 0x00U;
    }
  }
  mData = (perm << 5) | pols;
  return *this;
}

// @brief 逆変換を求める．
NpnXform
inverse(NpnXform left)
{
  ymuint l_perm = left.get_perm();
  ymuint l_pols = left.get_pols();
  ymuint i_perm = inv_table[l_perm];
  ymuint i_pols = nperm_table[l_pols * 24 + l_perm];
  return NpnXform(i_perm, i_pols);
}

// @brief 入力の順列から順列番号を得る．
ymuint
NpnXform::perm_id(ymuint perm[])
{
  for (ymuint pid = 0; pid < 24; ++ pid) {
    if ( perm_table[pid][0] == perm[0] &&
	 perm_table[pid][1] == perm[1] &&
	 perm_table[pid][2] == perm[2] &&
	 perm_table[pid][3] == perm[3] ) {
      return pid;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 関数ベクタを変換する．
ymuint16
xform(ymuint16 func,
      NpnXform xf)
{
  ymuint16 ans = 0U;
  for (ymuint p = 0; p < 16; ++ p) {
    if ( func & (1U << p) ) {
      ymuint v0 = (p >> 0) & 1U;
      ymuint v1 = (p >> 1) & 1U;
      ymuint v2 = (p >> 2) & 1U;
      ymuint v3 = (p >> 3) & 1U;
      if ( xf.input_inv(0) ) {
	v0 ^= 1U;
      }
      if ( xf.input_inv(1) ) {
	v1 ^= 1U;
      }
      if ( xf.input_inv(2) ) {
	v2 ^= 1U;
      }
      if ( xf.input_inv(3) ) {
	v3 ^= 1U;
      }
      ymuint q = 0U;
      if ( v0 ) {
	q |= (1U << xf.input_perm(0));
      }
      if ( v1 ) {
	q |= (1U << xf.input_perm(1));
      }
      if ( v2 ) {
	q |= (1U << xf.input_perm(2));
      }
      if ( v3 ) {
	q |= (1U << xf.input_perm(3));
      }
      ans |= (1U << q);
    }
  }
  if ( xf.output_inv() ) {
    ans ^= 0xFFFF;
  }
  return ans;
}

BEGIN_NONAMESPACE

void
print_input(ostream& s,
	    ymuint pos,
	    bool inv)
{
  s << pos;
  if ( inv ) {
    s << "N";
  }
  else {
    s << "P";
  }
}

END_NONAMESPACE

// @brief 内容を表示する．
ostream&
operator<<(ostream& s,
	   NpnXform xf)
{
  s << "(";
  print_input(s, xf.input_perm(0), xf.input_inv(0));
  s << ", ";
  print_input(s, xf.input_perm(1), xf.input_inv(1));
  s << ", ";
  print_input(s, xf.input_perm(2), xf.input_inv(2));
  s << ", ";
  print_input(s, xf.input_perm(3), xf.input_inv(3));
  s << ")|";
  if ( xf.output_inv() ) {
    s << "N";
  }
  else {
    s << "P";
  }

  return s;
}

END_NAMESPACE_YM
