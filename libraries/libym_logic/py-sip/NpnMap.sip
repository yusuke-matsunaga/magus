
/// @file NpnMap.sip
/// @brief NpnMap の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class NpnMap
{
%TypeHeaderCode
#include "logic/NpnMap.h"
using nsYm::NpnMap;
%End

public:

  /// @brief 空のコンストラクタ．
  /// @note 内容は不定
  NpnMap();

  /// @brief 入力数(と出力極性)を指定したコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] oinv 出力極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @note 各入力の変換内容は不正な値になっている．
  NpnMap(unsigned int ni,
	 bool oinv = false);

  /// @brief 内容をクリアする．
  /// @note 入力の変換内容は不正な値になる．
  /// 出力の極性は kPolPosi
  void
  clear();

  /// @brief 入力数を再設定する．
  /// @param[in] ni 入力数
  /// @note 以前の内容はクリアされる．
  void
  resize(unsigned int ni);

  /// @brief 恒等変換を表すように設定する．
  /// @param[in] ni 入力数
  void
  set_identity(unsigned int ni);

  /// @brief 入力の変換内容の設定
  /// @param[in] src_var 入力変数
  /// @param[in] dst_var 変換先の入力変数
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  void
  set(VarId src_var,
      VarId dst_var,
      bool inv);

  /// @brief 入力の変換内容の設定
  /// @param[in] src_var 入力変数
  /// @param[in] imap 変換情報(変換先の入力番号と極性)
  /// @sa NpnVmap
  void
  set(VarId src_var,
      NpnVmap imap);

  /// @brief 出力極性を設定する．
  /// @param[in] inv 出力極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  void
  set_oinv(bool inv);

  /// @brief 入力数を得る．
  /// @return 入力数
  unsigned int
  input_num() const;

  /// @brief 入力の変換情報の取得
  /// @param[in] var 入力変数
  /// @return pos 番目の入力の変換情報
  /// @note var に対応するマッピング情報がないときには不正な値を返す．
  /// @sa NpnVmap
  NpnVmap
  imap(VarId var) const;

  /// @brief 出力極性を返す．
  /// @retval false 反転なし
  /// @retval true 反転あり
  bool
  oinv() const;

  /// @brief 内容が等しいか調べる．
  /// @param[in] src 比較対象のマップ
  /// @return 自分自身と src が等しいときに true を返す．
  bool
  operator==(const NpnMap& src) const;

  /// @relates NpnMap
  /// @brief 逆写像を求める．
  /// @param[in] src 入力となるマップ
  /// @return src の逆写像
  /// @note 1対1写像でなければ答えは不定．
  NpnMap
  inverse() const;
  %MethodCode
  sipRes = new NpnMap(inverse(*sipCpp));
  %End

};

/// @relates NpnMap
/// @brief 合成を求める．
/// @param[in] src1,src2 入力となるマップ
/// @return src1 と src2 を合成したもの
/// @note src1の値域とsrc2の定義域は一致していな
/// ければならない．そうでなければ答えは不定．
NpnMap
operator*(const NpnMap& src1,
	  const NpnMap& src2);
