#ifndef YM_SMTLIBV2_SMTCONST_H
#define YM_SMTLIBV2_SMTCONST_H

/// @file ym_smtlibv2/SmtConst.h
/// @brief SmtConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtConst SmtConst.h "ym_smtlibv2/SmtConst.h"
/// @brief s-expr を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtConst
{
public:
  //////////////////////////////////////////////////////////////////////
  // 型の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を表す列挙型
  enum tType {
    /// @brief <numeral>
    kNumConst,
    /// @brief <decimal>
    kDecConst,
    /// @brief <hexadecimal>
    kHexConst,
    /// @brief <binary>
    kBinConst,
    /// @brief <string>
    kStrConst
  };


public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] value 値を表す文字列
  SmtConst(tType type,
	   const ShString& value);

  /// @brief デストラクタ
  ~SmtConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  tType
  type() const;

  /// @brief 値を表す文字列を返す．
  ShString
  value() const;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
// @param[in] value 値を表す文字列
inline
SmtConst::SmtConst(tType type,
		   const ShString& value) :
  mType(type),
  mValue(value)
{
}

// @brief デストラクタ
inline
SmtConst::~SmtConst()
{
}

// @brief 型を返す．
inline
SmtConst::tType
SmtConst::type() const
{
  return mType;
}

// @brief 値を表す文字列を返す．
inline
ShString
SmtConst::value() const
{
  return mValue;
}

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTCONST_H
