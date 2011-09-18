
/// @file NpnMapM.cc
/// @brief NpnMapM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/NpnMapM.h"
#include "ym_logic/NpnMap.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
// クラス NpnMapM
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ．
// @note 内容は不定
NpnMapM::NpnMapM() :
  mNi(0),
  mNo(0),
  mMapArray(NULL)
{
}

// @brief 入力数と出力数を指定したコンストラクタ
// @param[in] ni 入力数
// @param[in] no 出力数
// @note 各変数の変換内容は不正な値になっている．
NpnMapM::NpnMapM(ymuint ni,
		 ymuint no) :
  mNi(0),
  mNo(0),
  mMapArray(NULL)
{
  resize(ni, no);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
NpnMapM::NpnMapM(const NpnMapM& src) :
  mNi(0),
  mNo(0),
  mMapArray(NULL)
{
  copy(src);
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身への定数参照を返す．
const NpnMapM&
NpnMapM::operator=(const NpnMapM& src)
{
  if ( this != &src ) {
    copy(src);
  }
  return *this;
}

// @brief コピーする．
void
NpnMapM::copy(const NpnMapM& src)
{
  ymuint ni = src.mNi;
  ymuint no = src.mNo;
  resize(ni, no);
  ymuint n = ni + no;
  for (ymuint i = 0; i < n; ++ i) {
    mMapArray[i] = src.mMapArray[i];
  }
}

// @brief NpnMap からの変換コンストラクタ
// @note 出力数が1となる．
NpnMapM::NpnMapM(const NpnMap& src) :
  mNi(0),
  mNo(0),
  mMapArray(NULL)
{
  resize(src.ni(), 1);
  for (ymuint i = 0; i < mNi; ++ i) {
    mMapArray[i] = src.imap(i);
  }
  set_omap(0, 0, src.opol());
}

// @brief デストラクタ
NpnMapM::~NpnMapM()
{
  delete [] mMapArray;
}

// @brief 内容をクリアする．
// @note 各変数の変換内容は不正な値になる．
void
NpnMapM::clear()
{
  ymuint n = mNi + mNo;
  for (ymuint i = 0; i < n; ++ i) {
    mMapArray[i] = NpnVmap::invalid();
  }
}

// @brief 入力数と出力数を再設定する．
// @param[in] ni 入力数
// @param[in] no 出力数
// @note 以前の内容はクリアされる．
void
NpnMapM::resize(ymuint ni,
		ymuint no)
{
  ymuint n = ni + no;
  if ( mNi + mNo != n ) {
    delete [] mMapArray;
    mMapArray = new NpnVmap[n];
  }
  mNi = ni;
  mNo = no;
  clear();
}

// @brief 恒等変換を表すように設定する．
// @param[in] ni 入力数
// @param[in] no 出力数
void
NpnMapM::set_identity(ymuint ni,
		      ymuint no)
{
  resize(ni, no);
  for (ymuint i = 0; i < ni; ++ i) {
    set_imap(i, i, kPolPosi);
  }
  for (ymuint i = 0; i < no; ++ i) {
    set_omap(i, i, kPolPosi);
  }
}

// @brief 入力の変換内容の設定
// @param[in] pos 入力番号
// @param[in] dst_pos 変換先の入力番号
// @param[in] pol 極性
void
NpnMapM::set_imap(ymuint pos,
		  ymuint dst_pos,
		  tPol pol)
{
  set_imap(pos, NpnVmap(dst_pos, pol));
}

// @brief 入力の変換内容の設定
// @param[in] pos 入力番号
// @param[in] imap 変換情報(変換先の入力番号と極性)
// @sa NpnVmap
void
NpnMapM::set_imap(ymuint pos,
		  NpnVmap imap)
{
  mMapArray[pos] = imap;
}

// @brief 出力の変換内容の設定
// @param[in] pos 出力番号
// @param[in] dst_pos 変換先の出力番号
// @param[in] pol 極性
void
NpnMapM::set_omap(ymuint pos,
		  ymuint dst_pos,
		  tPol pol)
{
  set_omap(pos, NpnVmap(dst_pos, pol));
}

// @brief 出力の変換内容の設定
// @param[in] pos 出力番号
// @param[in] omap 変換情報(変換先の出力番号と極性)
// @sa NpnVmap
void
NpnMapM::set_omap(ymuint pos,
		  NpnVmap omap)
{
  mMapArray[pos + mNi] = omap;
}

// @brief 内容が等しいか調べる．
// @param[in] src 比較対象のマップ
// @return 自分自身と src が等しいときに true を返す．
bool
NpnMapM::operator==(const NpnMapM& src) const
{
  if ( src.mNi != mNi || src.mNo != mNo ) {
    return false;
  }
  ymuint n = mNi + mNo;
  for (ymuint i = 0; i < n; ++ i) {
    if ( mMapArray[i] != src.mMapArray[i] ) {
      return false;
    }
  }
  return true;
}

// @brief 逆写像を求める．
// @param[in] src 入力となるマップ
// @return src の逆写像
// @note 1対1写像でなければ答えは不定．
NpnMapM
inverse(const NpnMapM& src)
{
  ymuint ni = src.ni();
  ymuint no = src.no();

  NpnMapM ans(ni, no);

  for (ymuint i = 0; i < ni; ++ i) {
    NpnVmap imap = src.imap(i);
    if ( imap.is_invalid() ) {
      // 不正な値を返す．
      return NpnMapM(ni, no);
    }
    ymuint dst_pos = imap.pos();
    tPol pol = imap.pol();
    ans.set_imap(dst_pos, i, pol);
  }
  for (ymuint i = 0; i < no; ++ i) {
    NpnVmap omap = src.omap(i);
    if ( omap.is_invalid() ) {
      // 不正な値を返す．
      return NpnMapM(ni, no);
    }
    ymuint dst_pos = omap.pos();
    tPol pol = omap.pol();
    ans.set_omap(dst_pos, i, pol);
  }

  return ans;
}

// @brief 合成を求める．
// @param[in] src1,src2 入力となるマップ
// @return src1 と src2 を合成したもの
// @note src1の値域とsrc2の定義域は一致していな
// ければならない．そうでなければ答えは不定．
NpnMapM
operator*(const NpnMapM& src1,
	  const NpnMapM& src2)
{
  ymuint ni = src1.ni();
  ymuint no = src1.no();
  if ( ni != src2.ni() || no != src2.no() ) {
    // 不正な値を返す．
    return NpnMapM(ni, no);
  }

  NpnMapM ans(ni, no);

  for (ymuint i = 0; i < ni; ++ i) {
    NpnVmap imap1 = src1.imap(i);
    if ( imap1.is_invalid() ) {
      return NpnMapM(ni, no);
    }
    ymuint dst_pos1 = imap1.pos();
    tPol pol1 = imap1.pol();
    NpnVmap imap2 = src2.imap(dst_pos1);
    if ( imap2.is_invalid() ) {
      return NpnMapM(ni, no);
    }
    ymuint dst_pos2 = imap2.pos();
    tPol pol2 = imap2.pol();
    ans.set_imap(i, dst_pos2, pol1 * pol2);
  }

  for (ymuint i = 0; i < no; ++ i) {
    NpnVmap omap1 = src1.omap(i);
    if ( omap1.is_invalid() ) {
      return NpnMapM(ni, no);
    }
    ymuint dst_pos1 = omap1.pos();
    tPol pol1 = omap1.pol();
    NpnVmap omap2 = src2.omap(dst_pos1);
    if ( omap2.is_invalid() ) {
      return NpnMapM(ni, no);
    }
    ymuint dst_pos2 = omap2.pos();
    tPol pol2 = omap2.pol();
    ans.set_omap(i, dst_pos2, pol1 * pol2);
  }

  return ans;
}

// @brief 内容を表示する(主にデバッグ用)．
// @param[in] s 出力ストリーム
// @param[in] map 出力対象のマップ
// @note 改行はしない．
ostream&
operator<<(ostream& s,
	   const NpnMapM& map)
{
  const char* comma = "";

  s << "INPUT(";
  for (ymuint i = 0; i < map.ni(); ++ i) {
    s << comma;
    comma = ", ";
    s << i << " ==> ";
    NpnVmap imap = map.imap(i);
    if ( imap.is_invalid() ) {
      s << "--";
    }
    else {
      ymuint dst_pos = imap.pos();
      tPol pol = imap.pol();
      if ( pol == kPolNega ) {
	s << "~";
      }
      s << dst_pos;
    }
  }
  s << ")";

  comma = "";
  s << " OUTPUT(";
  for (ymuint i = 0; i < map.no(); ++ i) {
    s << comma;
    comma = ", ";
    s << i << " ==> ";
    NpnVmap omap = map.omap(i);
    if ( omap.is_invalid() ) {
      s << "--";
    }
    else {
      ymuint dst_pos = omap.pos();
      tPol pol = omap.pol();
      if ( pol == kPolNega ) {
	s << "~";
      }
      s << dst_pos;
    }
  }
  s << ")";

  return s;
}

END_NAMESPACE_YM_NPN

