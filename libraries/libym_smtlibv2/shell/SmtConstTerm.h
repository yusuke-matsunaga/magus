#ifndef SMTCONSTTERM_H
#define SMTCONSTTERM_H

/// @file SmtConstTerm.h
/// @brief SmtConsTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtNumTerm SmtConstTerm.h "SmtConstTerm.h"
/// @brief <numeric> を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtNumTerm :
  public SmtTermImpl
{
  friend class SmtLibMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtNumTerm(ymint32 val);

  /// @brief デストラクタ
  virtual
  ~SmtNumTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kNumConst 型の場合に整数値を返す．
  virtual
  ymint32
  int_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ymint32 mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtStrTerm SmtConstTerm.h "SmtConstTerm.h"
/// @brief 文字列を返す定数クラス
//////////////////////////////////////////////////////////////////////
class SmtStrTerm :
  public SmtTermImpl
{
  friend class SmtLibMgr;
protected:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtStrTerm(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtStrTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kDecConst, kHexConst, kBinConst, kStrConst 型の場合に文字列を返す．
  virtual
  ShString
  str_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ShString mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDecTerm SmtConstTerm.h "SmtConstTerm.h"
/// @brief <decimal> を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtDecTerm :
  public SmtStrTerm
{
  friend class SmtLibMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtDecTerm(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtDecTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtHexTerm SmtConstTerm.h "SmtConstTerm.h"
/// @brief <hexadecimal> を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtHexTerm :
  public SmtStrTerm
{
  friend class SmtLibMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtHexTerm(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtHexTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtBinTerm SmtConstTerm.h "SmtConstTerm.h"
/// @brief <binary> を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtBinTerm :
  public SmtStrTerm
{
  friend class SmtLibMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtBinTerm(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtBinTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtKeywordTerm SmtConstTerm.h "SmtConstTerm.h"
/// @brief <keyword> を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtKeywordTerm :
  public SmtStrTerm
{
  friend class SmtLibMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtKeywordTerm(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtKeywordTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtSymbolTerm SmtConstTerm.h "SmtConstTerm.h"
/// @brief <symbol> を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtSymbolTerm :
  public SmtStrTerm
{
  friend class SmtLibMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtSymbolTerm(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtSymbolTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

};


END_NAMESPACE_YM_SMTLIBV2

#endif // SMTCONSTTERM_H
