
/// @file NpnMapM.sip
/// @brief NpnMapM の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class NpnMapM
{
%TypeHeaderCode
#include "logic/NpnMapM.h"
using nsYm::NpnMapM;
%End

public:

  /// @brief 空のコンストラクタ．
  /// @note 内容は不定
  NpnMapM();

  /// @brief 入力数と出力数を指定したコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] no 出力数
  /// @note 各変数の変換内容は不正な値になっている．
  NpnMapM(unsigned int ni,
	  unsigned int no);

  /// @brief NpnMap からの変換コンストラクタ
  /// @note 出力数が1となる．
  NpnMapM(const NpnMap& src);

  /// @brief 内容をクリアする．
  /// @note 各変数の変換内容は不正な値になる．
  void
  clear();

  /// @brief 入力数と出力数を再設定する．
  /// @param[in] ni 入力数
  /// @param[in] no 出力数
  /// @note 以前の内容はクリアされる．
  void
  resize(unsigned int ni,
	 unsigned int no);

  /// @brief 恒等変換を表すように設定する．
  /// @param[in] ni 入力数
  /// @param[in] no 出力数
  void
  set_identity(unsigned int ni,
	       unsigned int no);

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

  /// @brief 入力数を得る．
  /// @return 入力数
  unsigned int
  input_num() const;

  /// @brief 出力数を得る．
  unsigned int
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

  /// @relates NpnMapM
  /// @brief 逆写像を求める．
  /// @param[in] src 入力となるマップ
  /// @return src の逆写像
  /// @note 1対1写像でなければ答えは不定．
  NpnMapM
  inverse() const;
  %MethodCode
  sipRes = new NpnMapM(inverse(*sipCpp));
  %End

};

/// @relates NpnMapM
/// @brief 合成を求める．
/// @param[in] src1,src2 入力となるマップ
/// @return src1 と src2 を合成したもの
/// src1の値域とsrc2の定義域は一致していなければならない．
/// そうでなければ答えは不定．
NpnMapM
operator*(const NpnMapM& src1,
	  const NpnMapM& src2);
