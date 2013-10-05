
/// @file SmtConstTerm.cc
/// @brief SmtConstTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtConstTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtNumTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtNumTerm::SmtNumTerm(ymint32 val) :
  mVal(val)
{
}

// @brief デストラクタ
SmtNumTerm::~SmtNumTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtNumTerm::type() const
{
  return kNumConst;
}

// @brief kNumConst 型の場合に整数値を返す．
ymint32
SmtNumTerm::int_value()
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtStrTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtStrTerm::SmtStrTerm(const ShString& val) :
  mVal(val)
{
}

// @brief デストラクタ
SmtStrTerm::~SmtStrTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtStrTerm::type() const
{
  return kStrConst;
}

// @brief kDecConst, kHexConst, kBinConst, kStrConst 型の場合に文字列を返す．
ShString
SmtStrTerm::str_value()
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDecTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtDecTerm::SmtDecTerm(const ShString& val) :
  SmtStrTerm(val)
{
}

// @brief デストラクタ
SmtDecTerm::~SmtDecTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtDecTerm::type() const
{
  return kDecConst;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtHexTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtHexTerm::SmtHexTerm(const ShString& val) :
  SmtStrTerm(val)
{
}

// @brief デストラクタ
SmtHexTerm::~SmtHexTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtHexTerm::type() const
{
  return kHexConst;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtBinTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtBinTerm::SmtBinTerm(const ShString& val) :
  SmtStrTerm(val)
{
}

// @brief デストラクタ
SmtBinTerm::~SmtBinTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtBinTerm::type() const
{
  return kBinConst;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtKeywordTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtKeywordTerm::SmtKeywordTerm(const ShString& val) :
  SmtStrTerm(val)
{
}

// @brief デストラクタ
SmtKeywordTerm::~SmtKeywordTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtKeywordTerm::type() const
{
  return kKeyword;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtSymbolTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtSymbolTerm::SmtSymbolTerm(const ShString& val) :
  SmtStrTerm(val)
{
}

// @brief デストラクタ
SmtSymbolTerm::~SmtSymbolTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtSymbolTerm::type() const
{
  return kSymbol;
}

END_NAMESPACE_YM_SMTLIBV2
