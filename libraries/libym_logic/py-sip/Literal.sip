
/// @file Literal.sip
/// @brief Literal の sip ダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class Literal
{
%TypeHeaderCode
#include "YmLogic/Literal.h"
using nsYm::Literal;
%End

public:

  /// @brief デフォルトコンストラクタ
  /// 内容は不定なのであまり使わない方が良い．
  Literal();

  /// @brief 変数番号と極性を指定したコンストラクタ
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  Literal(VarId varid,
	  bool inv);

  /// @brief 内容を設定する．
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  void
  set(VarId varid,
      bool inv);

  /// @brief 変数番号を得る．
  /// @return 変数番号
  VarId
  varid() const;

  /// @brief 正極性のリテラルの時 true を返す．
  bool
  is_positive() const;

  /// @brief 負極性のリテラルの時 true を返す．
  bool
  is_negative() const;

  /// @brief 同じ変数の正極性リテラルを返す．
  Literal
  make_positive() const;

  /// @brief 同じ変数の負極性リテラルを返す．
  Literal
  make_negative() const;

  /// @brief ハッシュ用の関数
  unsigned int
  hash() const;

  /// @brief 配列のインデックスとして使用可能な数を返す．
  unsigned int
  index() const;

  /// @brief バイナリファイルに出力する．
  /// @param[in] s 出力先のストリーム
  void
  store(ODO& s) const;

  /// @brief バイナリファイルから読み込む．
  /// @param[in] s 入力元のストリーム
  void
  restore(IDO& s);

  const char*
  __repr__() const;
  %MethodCode
  using std::ostringstream;
  ostringstream buf;
  buf << *sipCpp;
  sipRes = buf.str().c_str();
  %End

};

bool
operator==(Literal left,
	   Literal right);

bool
operator<(Literal left,
	  Literal right);
