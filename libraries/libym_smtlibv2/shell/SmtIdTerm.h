#ifndef SMTIDTERM_H
#define SMTIDTERM_H

/// @file SmtIdTerm.h
/// @brief SmtIdTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtIdTerm SmtIdTerm.h "SmtIdTerm.h"
/// @brief 識別子を表す SmtTerm の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtIdTerm :
  public SmtTermImpl
{
  friend class SmtLibMgr;
protected:

  /// @brief コンストラクタ
  /// @param[in] id 識別子
  SmtIdTerm(const SmtId* id);

  /// @brief デストラクタ
  virtual
  ~SmtIdTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kQualId 型の場合に識別子を返す．
  virtual
  const SmtId*
  identifier() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 識別子
  const SmtId* mId;

};

//////////////////////////////////////////////////////////////////////
/// @class SmtQualIdTerm SmtIdTerm.h "SmtIdTerm.h"
/// @brief 型つき識別子を表す SmtTerm の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtQualIdTerm :
  public SmtIdTerm
{
  friend class SmtLibMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] id 識別子
  /// @param[in] sort 型
  SmtQualIdTerm(const SmtId* id,
		const SmtSort* sort);

  /// @brief デストラクタ
  virtual
  ~SmtQualIdTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kQualIdentifier 型の場合に型を返す．
  virtual
  const SmtSort*
  identifier_sort() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  const SmtSort* mSort;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTIDTERM_H
