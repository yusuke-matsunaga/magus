#ifndef SMTLET_H
#define SMTLET_H

/// @file SmtLet.h
/// @brief SmtLet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtLet SmtLet.h "SmtLet.h"
/// @brief let 文を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtLet :
  public SmtTermImpl
{
  friend class SmtLibMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] var_binding 変数割り当てのリスト
  /// @param[in] body 本体
  SmtLet(const vector<SmtVarBinding>& var_binding,
	 const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtLet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kLet 型の場合に変数バインディングリストの要素数を返す．
  virtual
  ymuint
  let_binding_num() const;

  /// @brief kLet 型の場合に変数バインディングリストの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < let_binding_num() )
  virtual
  const SmtVarBinding*
  let_binding(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  const SmtTerm* mBody;

  // 変数割り当ての数
  ymuint32 mBindingNum;

  // 変数割り当ての配列
  // 実際には必要な領域が確保される．
  SmtVarBinding mBindingList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTLET_H
