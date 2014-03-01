
/// @file VarId.sip
/// @brief VarId の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class VarId
{
%TypeHeaderCode
#include "logic/VarId.h"
using nsYm::VarId;
%End

public:

  /// @brief 空のコンストラクタ
  /// @note 不正な値(kVarIdIllegal)になる．
  VarId();

  /// @brief 値を指定したコンストラクタ
  explicit
  VarId(unsigned int val);

  /// @brief 値を取り出す．
  unsigned int
  val() const;

  /// @brief バイナリファイルに出力する．
  /// @param[in] s 出力先のストリーム
  void
  store(ODO& s) const;

  /// @brief バイナリファイルを読み込む．
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
operator==(VarId left,
	   VarId right);

bool
operator<(VarId left,
	  VarId right);
