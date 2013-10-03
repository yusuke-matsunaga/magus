#ifndef SMTIDTERM_H
#define SMTIDTERM_H

/// @file SmtIdTerm.h
/// @brief SmtIdTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtIdTerm SmtIdTerm.h "SmtIdTerm.h"
/// @brief 識別子を表す SmtTerm の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtIdTerm :
  public SmtTerm
{
public:

  /// @brief コンストラクタ
  /// @param[in] id 識別子
  /// @param[in] term_list 項のリスト
  explicit
  SmtIdTerm(const SmtIdentifier* id,
	    const vector<const SmtTerm*>& term_list = vector<const SmtTerm*>(0));

  /// @brief コンストラクタ
  /// @param[in] id 識別子
  /// @param[in] sort 型
  /// @param[in] term_list 項のリスト
  SmtIdTerm(const SmtIdentifier* id,
	    const SmtSort* sort,
	    const vector<const SmtTerm*>& term_list = vector<const SmtTerm*>(0));

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

  /// @brief kQualIdentifier 型の場合に識別子を返す．
  virtual
  const SmtIdentifier*
  identifier() const;

  /// @brief kQualIdentifier 型の場合に型を返す．
  /// @note 型がついていない場合には NULL を返す．
  virtual
  const SmtSort*
  identifier_sort() const;

  /// @brief kQualIdentifier 型の場合にパラメータの項数を返す．
  virtual
  ymuint
  identifier_term_num() const;

  /// @brief kQualIdentifier 型の場合にパラメータを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < identifier_term_num() )
  virtual
  const SmtTerm*
  identifier_term(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 識別子
  const SmtIdentifier* mId;

  // 型
  const SmtSort* mSort;

  // パラメータのリスト
  vector<const SmtTerm*> mTermList;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTIDTERM_H
