
/// @file NpnMap.cc
/// @brief NpnMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/NpnMap.h"


//#define DEBUG_NPN_MAP


BEGIN_NAMESPACE_YM_NPN

// デバッグ用のフラグ変数
#if defined(DEBUG) || defined(DEBUG_NPN_MAP)
const int debug_npn_map = 1;
#else
const int debug_npn_map = 0;
#endif

// 空のコンストラクタ
NpnMap::NpnMap() :
  mNiPol(0UL)
{
}

// 入力数(と出力極性)を指定したコンストラクタ
// 各入力の変換内容は kImapBad になっている．
NpnMap::NpnMap(ymuint ni,
	       tPol pol) :
  mNiPol((ni << 1) | static_cast<ymuint32>(pol))
{
  for (ymuint i = 0; i < ni; ++ i) {
    mImap[i] = NpnVmap::invalid();
  }
}

// コピーコンストラクタ
NpnMap::NpnMap(const NpnMap& src) :
  mNiPol(src.mNiPol)
{
  for (ymuint i = 0; i < input_num(); ++ i) {
    mImap[i] = src.mImap[i];
  }
}

// 代入演算子
const NpnMap&
NpnMap::operator=(const NpnMap& src)
{
  mNiPol = src.mNiPol;
  for (ymuint i = 0; i < input_num(); ++ i) {
    mImap[i] = src.mImap[i];
  }
  return *this;
}

// デストラクタ
NpnMap::~NpnMap()
{
}

// 内容をクリアする．
// 入力の変換内容は kImapBad になる．
// 出力の極性は kPolPosi
void
NpnMap::clear()
{
  mNiPol &= ~(1UL);
  for (ymuint i = 0; i < input_num(); ++ i) {
    mImap[i] = NpnVmap::invalid();
  }
}

// 入力数を再設定して内容をクリアする．
void
NpnMap::resize(ymuint new_ni)
{
  mNiPol = new_ni << 1;
  for (ymuint i = 0; i < new_ni; ++ i) {
    mImap[i] = NpnVmap::invalid();
  }
}

// 恒等変換を表すように設定する．
void
NpnMap::set_identity(ymuint new_ni)
{
  mNiPol = new_ni << 1;
  for (ymuint i = 0; i < new_ni; ++ i) {
    mImap[i] = NpnVmap(VarId(i), kPolPosi);
  }
}

// @brief 入力の変換内容の設定
// @param[in] src_var 入力変数
// @param[in] dst_var 変換先の入力変数
// @param[in] pol 極性
void
NpnMap::set(VarId var,
	    NpnVmap imap)
{
  ymuint src_pos = var.val();
  if ( src_pos < input_num() ) {
    mImap[src_pos] = imap;
  }
}

// 出力極性を設定する．
void
NpnMap::set_opol(tPol pol)
{
  if ( pol == kPolPosi ) {
    mNiPol &= ~(1UL);
  }
  else {
    mNiPol |= 1UL;
  }
}

// 内容が等しいか調べる．
bool
NpnMap::operator==(const NpnMap& src) const
{
  if ( mNiPol != src.mNiPol ) {
    return false;
  }
  for (ymuint i = 0; i < input_num(); ++ i) {
    if ( mImap[i] != src.mImap[i] ) {
      return false;
    }
  }
  return true;
}

// 逆写像を求める．1対1写像でなければ答えは不定
NpnMap
inverse(const NpnMap& src)
{
  if ( debug_npn_map ) {
    cerr << "inverse :" << endl
	 << src
	 << endl;
  }

  ymuint src_ni = src.input_num();
  NpnMap dst_map(src_ni, src.opol());
  for (ymuint i = 0; i < src_ni; ++ i) {
    VarId src_var(i);
    NpnVmap imap = src.imap(src_var);
    if ( !imap.is_invalid() ) {
      VarId dst_var = imap.var();
      if ( dst_var.val() >= src_ni ) {
	if ( debug_npn_map ) {
	  cerr << "inverse(src): srcの値域と定義域が一致しません．";
	}
	return NpnMap(src_ni);
      }
      tPol pol = imap.pol();
      dst_map.set(dst_var, src_var, pol);
    }
  }

  if ( debug_npn_map ) {
    cerr << "--->" << endl
	 << dst_map
	 << endl << endl;
  }

  return dst_map;
}

// 合成を求める．src1の値域とsrc2の定義域は一致していなければならない．
NpnMap
operator*(const NpnMap& src1,
	  const NpnMap& src2)
{
  if ( debug_npn_map ) {
    cerr << "compose :"
	 << endl
	 << src1
	 << endl
	 << "with"
	 << endl
	 << src2
	 << endl;
  }

  ymuint ni1 = src1.input_num();
  NpnMap dst_map(ni1, src1.opol() * src2.opol());
  for (ymuint i1 = 0; i1 < ni1; ++ i1) {
    VarId var1(i1);
    NpnVmap imap1 = src1.imap(var1);
    if ( imap1.is_invalid() ) {
      dst_map.set(var1, NpnVmap::invalid());
    }
    else {
      VarId var2 = imap1.var();
      tPol pol2 = imap1.pol();
      NpnVmap imap2 = src2.imap(var2);
      if ( imap2.is_invalid() ) {
	if ( debug_npn_map ) {
	  cerr << "src1 * src2: src1の値域とsrc2の定義域が一致しません．";
	}
      }
      else {
	VarId var3 = imap2.var();
	tPol pol3 = imap2.pol();
	dst_map.set(var1, var3, pol2 * pol3);
      }
    }
  }

  if ( debug_npn_map ) {
    cerr << "--->" << endl
	 << dst_map
	 << endl;
  }

  return dst_map;
}

#if 0
// mImapの内容に従ってBDDを変換する．
// 例えばmap[3] = { 5, kPolNega }だったら，
// fの3番目の変数を反転させて5番めの変数にする．
Bdd
NpnMap::xform_bdd(const Bdd& f) const
{
  if ( debug_npn_map ) {
    cerr << "NpnMap::xform_bdd()" << endl
	 << *this << endl;
    f.display(cerr);
  }

  // 変換内容を subst_array にセットする．
  // 恒等変換ならセットしない．
  VarBddMap subst_array;
  BddMgrRef mgr = f.mgr();
  ymuint ni = mImap.size();
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint old_id = i;
    NpnVmap imap = mImap[i];
    ymuint new_id = imap.pos();
    tPol pol = imap.pol();
    if ( pol == kPolNega || old_id != new_id ) {
      subst_array[old_id] = mgr.make_literal(new_id, pol);
    }
  }
  Bdd ans = f.compose(subst_array);

  if ( opol() == kPolNega ) {
    ans.negate();
  }

  if ( debug_npn_map ) {
    cerr << "---->" << endl;
    ans.display(cerr);
  }

  return ans;
}
#endif

// ストリーム出力演算子
ostream&
operator<<(ostream& s,
	   const NpnMap& map)
{
  const char* comma = "";
  s << "INPUT(";
  for (ymuint i = 0; i < map.input_num(); ++ i) {
    s << comma;
    comma = ", ";
    s << i << " ==> ";
    VarId var(i);
    NpnVmap imap = map.imap(var);
    if ( imap.is_invalid() ) {
      s << "---";
    }
    else {
      VarId dst_var = imap.var();
      tPol pol = imap.pol();
      if ( pol == kPolNega ) {
	s << '~';
      }
      s << dst_var;
    }
  }
  s << ")";
  s << " OUTPUT(0 ==> ";
  if ( map.opol() == kPolNega ) {
    s << '~';
  }
  s << "0)";

  return s;
}

// バイナリ出力
ODO&
operator<<(ODO& bos,
	   const NpnMap& map)
{
  ymuint32 ni = map.input_num();
  bos << ni;
  for (ymuint i = 0; i < ni; ++ i) {
    NpnVmap vmap = map.imap(VarId(i));
    bos << vmap;
  }
  bos << (map.opol() == kPolNega);

  return bos;
}

// バイナリ入力
IDO&
operator>>(IDO& bis,
	   NpnMap& map)
{
  ymuint32 ni;
  bis >> ni;
  map.resize(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    NpnVmap vmap;
    bis >> vmap;
    map.set(VarId(i), vmap);
  }
  bool inv;
  bis >> inv;
  tPol opol = inv ? kPolNega : kPolPosi;
  map.set_opol(opol);

  return bis;
}

END_NAMESPACE_YM_NPN
