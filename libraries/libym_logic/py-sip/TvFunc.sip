
/// @file TvFunc.sip
/// @brief TvFunc の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class TvFunc
{
%TypeHeaderCode
#include "logic/TvFunc.h"
using nsYm::TvFunc;
%End

public:

  /// @brief 恒偽関数を作る．
  /// @param[in] ni 入力数
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  const_zero(unsigned int ni);

  /// @brief 恒真関数を作る．
  /// @param[in] ni 入力数
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  const_one(unsigned int ni);

  /// @brief 肯定のリテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] varid リテラルの変数番号
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  posi_literal(unsigned int ni,
	       VarId varid);

  /// @brief 否定のリテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] varid リテラルの変数番号
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  nega_literal(unsigned int ni,
	       VarId varid);

  /// @brief 入力数を得る．
  unsigned int
  input_num() const;

  /// @brief 入力値を2進数と見なしたときの pos 番目の値を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < 2^(input_num()) )
  /// 答は 0 か 1 だが int 型
  int
  value(unsigned int pos) const;

  /// @brief 0 の数を数える．
  unsigned int
  count_zero() const;

  /// @brief 1 の数を数える．
  unsigned int
  count_one() const;

  /// @brief pos 番目の変数がサポートの時 true を返す．
  /// @param[in] pos 変数番号
  bool
  check_sup(VarId pos) const;

  /// @brief pos1 番目と pos2 番目の変数が対称のとき true を返す．
  /// @param[in] pos1, pos2 変数番号
  /// @param[in] inv 極性
  bool
  check_sym(VarId pos1,
	    VarId pos2,
	    bool inv = false) const;

  /// @brief ハッシュ値を返す．
  unsigned int
  hash() const;

  /// @brief 否定を求める．
  /// @param[in] src オペランド
  TvFunc
  operator~() const;

  /// @brief 論理積を求める．
  /// @param[in] left, right オペランド
  TvFunc
  operator&(const TvFunc& right) const;

  /// @brief 論理和を求める．
  /// @param[in] left, right オペランド
  TvFunc
  operator|(const TvFunc& right);

  /// @brief 排他的論理和を求める．
  /// @param[in] left, right オペランド
  TvFunc
  operator^(const TvFunc& right);

};
