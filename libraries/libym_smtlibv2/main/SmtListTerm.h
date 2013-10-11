#ifndef SMTLISTTERM_H
#define SMTLISTTERM_H

/// @file SmtListTerm.h
/// @brief SmtListTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtListTerm SmtListTerm.h "SmtListTerm.h"
/// @brief SmtTerm のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class SmtListTerm :
  public SmtTermImpl
{
  friend class SmtMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] elem_list 入力のリスト
  SmtListTerm(const vector<const SmtTerm*>& elem_list);

  /// @brief デストラクタ
  virtual
  ~SmtListTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kList 型の場合に要素数を返す．
  virtual
  ymuint
  elem_num() const;

  /// @brief kList 型の場合に要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  virtual
  const SmtTerm*
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mElemNum;

  // 要素の配列
  // 実際には必要なサイズを確保する．
  const SmtTerm* mElemList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTLISTTERM_H
