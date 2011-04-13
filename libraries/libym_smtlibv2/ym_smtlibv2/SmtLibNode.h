#ifndef YM_SMTLIBV2_SMTLIBNODE_H
#define YM_SMTLIBV2_SMTLIBNODE_H

/// @file ym_smtlibv2/SmtLibNode.h
/// @brief SmtLibNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtLibNode SmtLibNode.h "ym_smtlibv2/SmtLibNode.h"
/// @brief SmtLib のS式を表すノードの基底クラス
//////////////////////////////////////////////////////////////////////
class SmtLibNode
{
public:

  /// @brief ノードの種類
  enum tType {
    /// @brief NUMERAL
    kNum,
    /// @brief DECIMAL
    kDec,
    /// @brief HEXADECIMAL
    kHex,
    /// @brief BINARY
    kBin,
    /// @brief STRING
    kString,
    /// @brief SYMBOL
    kSymbol,
    /// @brief KEYWORD
    kKeyword,
    /// @brief LIST
    kList
  };


public:

  /// @brief デストラクタ
  virtual
  ~SmtLibNode() { }


public:

  /// @brief 型を返す．
  virtual
  tType
  type() = 0;

  /// @brief 終端型の場合の値を返す．
  virtual
  string
  value() = 0;

  /// @brief LIST型の場合の car を返す．
  virtual
  const SmtLibNode*
  car() = 0;

  /// @brief LIST型の場合の cdr を返す．
  virtual
  const SmtLibNode*
  cdr() = 0;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTLIBNODE_H
