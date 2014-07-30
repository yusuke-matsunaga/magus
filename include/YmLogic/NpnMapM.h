#ifndef YMYMLOGIC_NPNMAPM_H
#define YMYMLOGIC_NPNMAPM_H

/// @file YmLogic/NpnMapM.h
/// @brief NpnMapM のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/npn_nsdef.h"
#include "YmLogic/VarId.h"
#include "YmLogic/NpnVmap.h"
#include "YmUtils/IDO.h"
#include "YmUtils/ODO.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// @class NpnMapM NpnMapM.h "YmLogic/NpnMapM.h"
/// @ingroup NpnGroup
/// @brief NPN変換の情報を入れるクラス
///
/// @sa tNpnImap
//////////////////////////////////////////////////////////////////////
class NpnMapM
{
public:

  /// @brief 空のコンストラクタ．
  /// @note 内容は不定
  NpnMapM();

  /// @brief 入力数と出力数を指定したコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] no 出力数
  /// @note 各変数の変換内容は不正な値になっている．
  NpnMapM(ymuint ni,
	  ymuint no);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  NpnMapM(const NpnMapM& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への定数参照を返す．
  const NpnMapM&
  operator=(const NpnMapM& src);

  /// @brief NpnMap からの変換コンストラクタ
  /// @note 出力数が1となる．
  explicit
  NpnMapM(const NpnMap& src);

  /// @brief デストラクタ
  ~NpnMapM();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  /// @note 各変数の変換内容は不正な値になる．
  void
  clear();

  /// @brief 入力数と出力数を再設定する．
  /// @param[in] ni 入力数
  /// @param[in] no 出力数
  /// @note 以前の内容はクリアされる．
  void
  resize(ymuint ni,
	 ymuint no);

  /// @brief 恒等変換を表すように設定する．
  /// @param[in] ni 入力数
  /// @param[in] no 出力数
  void
  set_identity(ymuint ni,
	       ymuint no);

  /// @brief 入力の変換内容の設定
  /// @param[in] src_var 入力番号
  /// @param[in] dst_var 変換先の入力番号
  /// @param[in] int 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  void
  set_imap(VarId src_var,
	   VarId dst_var,
	   bool inv);

  /// @brief 入力の変換内容の設定
  /// @param[in] var 入力番号
  /// @param[in] imap 変換情報(変換先の入力番号と極性)
  /// @sa NpnVmap
  void
  set_imap(VarId var,
	   NpnVmap imap);

  /// @brief 出力の変換内容の設定
  /// @param[in] src_var 出力番号
  /// @param[in] dst_var 変換先の出力番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  void
  set_omap(VarId src_var,
	   VarId dst_var,
	   bool inv);

  /// @brief 出力の変換内容の設定
  /// @param[in] var 出力番号
  /// @param[in] omap 変換情報(変換先の出力番号と極性)
  /// @sa NpnVmap
  void
  set_omap(VarId var,
	   NpnVmap omap);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  /// @return 入力数
  ymuint
  input_num() const;

  /// @brief 出力数を得る．
  ymuint
  output_num() const;

  /// @brief 入力の変換情報の取得
  /// @param[in] var 入力番号
  /// @return var の変換情報
  /// @note var に対応するマッピング情報がないときには不正な値を返す．
  /// @sa NpnVmap
  NpnVmap
  imap(VarId var) const;

  /// @brief 出力の変換情報の取得
  /// @param[in] var 出力番号
  /// @return var の変換情報
  /// @note var に対応するマッピング情報がないときには不正な値を返す．
  /// @sa NpnVmap
  NpnVmap
  omap(VarId var) const;

  /// @brief 内容が等しいか調べる．
  /// @param[in] src 比較対象のマップ
  /// @return 自分自身と src が等しいときに true を返す．
  bool
  operator==(const NpnMapM& src) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  void
  copy(const NpnMapM& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // 出力数
  ymuint32 mOutputNum;

  // 入力と出力のマッピング情報
  NpnVmap* mMapArray;

};


//////////////////////////////////////////////////////////////////////
// NpnMapM に関連した関数
//////////////////////////////////////////////////////////////////////

/// @relates NpnMapM
/// @brief 逆写像を求める．
/// @param[in] src 入力となるマップ
/// @return src の逆写像
/// @note 1対1写像でなければ答えは不定．
NpnMapM
inverse(const NpnMapM& src);

/// @relates NpnMapM
/// @brief 合成を求める．
/// @param[in] src1,src2 入力となるマップ
/// @return src1 と src2 を合成したもの
/// src1の値域とsrc2の定義域は一致していなければならない．
/// そうでなければ答えは不定．
NpnMapM
operator*(const NpnMapM& src1,
	  const NpnMapM& src2);

/// @relates NpnMapM
/// @brief 内容を表示する(主にデバッグ用)．
/// @param[in] s 出力ストリーム
/// @param[in] map 出力対象のマップ
/// @note 改行はしない．
ostream&
operator<<(ostream& s,
	   const NpnMapM& map);

/// @relates NpnMapM
/// @brief バイナリ出力
/// @param[in] s 出力ストリーム
/// @param[in] map 変換マップ
/// @return s
ODO&
operator<<(ODO& s,
	   const NpnMapM& map);

/// @relates NpnMapM
/// @brief バイナリ入力
/// @param[in] s 入力ストリーム
/// @param[out] map 結果を格納する変数
/// @return s
IDO&
operator>>(IDO& s,
	   NpnMapM& map);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 入力数を得る．
inline
ymuint
NpnMapM::input_num() const
{
  return mInputNum;
}

// @brief 出力数を得る．
inline
ymuint
NpnMapM::output_num() const
{
  return mOutputNum;
}

// var に対応するマッピング情報を得る．
inline
NpnVmap
NpnMapM::imap(VarId var) const
{
  ymuint idx = var.val();
  if ( idx < input_num() ) {
    return mMapArray[idx];
  }
  return NpnVmap::invalid();
}

// @brief 出力の変換情報の取得
// @param[in] var 出力番号
// @return var の出力の変換情報
// @note var に対応するマッピング情報がないときには
// kImapBad を返す．
// @sa NpnVmap
inline
NpnVmap
NpnMapM::omap(VarId var) const
{
  ymuint idx = var.val();
  if ( idx < output_num() ) {
    return mMapArray[idx + input_num()];
  }
  return NpnVmap::invalid();
}

// @brief 入力の変換内容の設定
// @param[in] src_var 入力番号
// @param[in] dst_var 変換先の入力番号
// @param[in] inv 極性
//                - false: 反転なし (正極性)
//                - true:  反転あり (負極性)
inline
void
NpnMapM::set_imap(VarId src_var,
		  VarId dst_var,
		  bool inv)
{
  set_imap(src_var, NpnVmap(dst_var, inv));
}

// @brief 出力の変換内容の設定
// @param[in] src_var 出力番号
// @param[in] dst_var 変換先の出力番号
// @param[in] inv 極性
//                - false: 反転なし (正極性)
//                - true:  反転あり (負極性)
inline
void
NpnMapM::set_omap(VarId src_var,
		  VarId dst_var,
		  bool inv)
{
  set_omap(src_var, NpnVmap(dst_var, inv));
}

END_NAMESPACE_YM_NPN

#endif // YMYMLOGIC_NPNMAPM_H
