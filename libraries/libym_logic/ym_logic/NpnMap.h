#ifndef YM_LOGIC_NPNMAP_H
#define YM_LOGIC_NPNMAP_H

/// @file ym_logic/NpnMap.h
/// @brief NpnMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/npn_nsdef.h"
#include "ym_logic/VarId.h"
#include "ym_logic/Pol.h"
#include "ym_logic/NpnVmap.h"
#include "ym_logic/TvFunc.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// @class NpnMap NpnMap.h "ym_logic/NpnMap.h"
/// @ingroup NpnGroup
/// @brief NPN変換の情報を入れるクラス
///
/// @sa tPol, NpnVmap
//////////////////////////////////////////////////////////////////////
class NpnMap
{
public:

  /// @brief 空のコンストラクタ．
  /// @note 内容は不定
  NpnMap();

  /// @brief 入力数(と出力極性)を指定したコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] pol 出力極性
  /// @note 各入力の変換内容は不正な値になっている．
  explicit
  NpnMap(ymuint ni,
	 tPol pol = kPolPosi);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  NpnMap(const NpnMap& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への定数参照を返す．
  const NpnMap&
  operator=(const NpnMap& src);

  /// @brief デストラクタ
  ~NpnMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  /// @note 入力の変換内容は不正な値になる．
  /// 出力の極性は kPolPosi
  void
  clear();

  /// @brief 入力数を再設定する．
  /// @param[in] ni 入力数
  /// @note 以前の内容はクリアされる．
  void
  resize(ymuint ni);

  /// @brief 恒等変換を表すように設定する．
  /// @param[in] ni 入力数
  void
  set_identity(ymuint ni);

  /// @brief 入力の変換内容の設定
  /// @param[in] src_var 入力変数
  /// @param[in] dst_var 変換先の入力変数
  /// @param[in] pol 極性
  void
  set(VarId src_var,
      VarId dst_var,
      tPol pol);

  /// @brief 入力の変換内容の設定
  /// @param[in] src_var 入力変数
  /// @param[in] imap 変換情報(変換先の入力番号と極性)
  /// @sa NpnVmap
  void
  set(VarId src_var,
      NpnVmap imap);

  /// @brief 出力極性を設定する．
  /// @param[in] pol 出力極性
  void
  set_opol(tPol pol);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  /// @return 入力数
  ymuint
  ni() const;

  /// @brief 入力の変換情報の取得
  /// @param[in] var 入力変数
  /// @return pos 番目の入力の変換情報
  /// @note var に対応するマッピング情報がないときには不正な値を返す．
  /// @sa NpnVmap
  NpnVmap
  imap(VarId var) const;

  /// @brief 出力極性を返す．
  /// @return 出力極性
  tPol
  opol() const;

  /// @brief 内容が等しいか調べる．
  /// @param[in] src 比較対象のマップ
  /// @return 自分自身と src が等しいときに true を返す．
  bool
  operator==(const NpnMap& src) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数 + 出力の極性
  ymuint32 mNiPol;

  // 入力のマッピング情報
  NpnVmap mImap[TvFunc::kMaxNi];

};


//////////////////////////////////////////////////////////////////////
// NpnMap に関連する関数
//////////////////////////////////////////////////////////////////////

/// @relates NpnMap
/// @brief 逆写像を求める．
/// @param[in] src 入力となるマップ
/// @return src の逆写像
/// @note 1対1写像でなければ答えは不定．
NpnMap
inverse(const NpnMap& src);

/// @relates NpnMap
/// @brief 合成を求める．
/// @param[in] src1,src2 入力となるマップ
/// @return src1 と src2 を合成したもの
/// @note src1の値域とsrc2の定義域は一致していな
/// ければならない．そうでなければ答えは不定．
NpnMap
operator*(const NpnMap& src1,
	  const NpnMap& src2);

/// @relates NpnMap
/// @brief 内容を表示する(主にデバッグ用)．
/// @param[in] s 出力ストリーム
/// @param[in] map 出力対象のマップ
/// @note 改行はしない．
ostream&
operator<<(ostream& s,
	   const NpnMap& map);

/// @relates NpnMap
/// @brief バイナリ出力
/// @param[in] s 出力ストリーム
/// @param[in] map 変換マップ
/// @return s
BinO&
operator<<(BinO& s,
	   const NpnMap& map);

/// @relates NpnMap
/// @brief バイナリ入力
/// @param[in] s 入力ストリーム
/// @param[out] map 結果を格納する変数
/// @return s
BinI&
operator>>(BinI& s,
	   NpnMap& map);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 入力数を得る．
inline
ymuint
NpnMap::ni() const
{
  return mNiPol >> 1;
}

// var に対応するマッピング情報を得る．
inline
NpnVmap
NpnMap::imap(VarId var) const
{
  ymuint idx = var.val();
  if ( idx < ni() ) {
    return mImap[idx];
  }
  else {
    return NpnVmap::invalid();
  }
}

// 出力極性を返す．
inline
tPol
NpnMap::opol() const
{
  return (mNiPol & 1) ? kPolNega : kPolPosi;
}

// @brief 入力の変換内容の設定
// @param[in] src_var 入力変数
// @param[in] dst_var 変換先の入力変数
// @param[in] pol 極性
inline
void
NpnMap::set(VarId src_var,
	    VarId dst_var,
	    tPol pol)
{
  set(src_var, NpnVmap(dst_var, pol));
}

END_NAMESPACE_YM_NPN

#endif // YM_LOGIC_NPNMAP_H
