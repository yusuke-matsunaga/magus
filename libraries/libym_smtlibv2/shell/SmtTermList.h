#ifndef SMTTERMLIST_H
#define SMTTERMLIST_H

/// @file SmtTermList.h
/// @brief SmtTermList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtTermList SmtTermList.h "SmtTermList.h"
/// @brief SmtTerm のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class SmtTermList :
  public SmtTermImpl
{
  friend class SmtLibMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] term_list SmtTerm のリスト
  SmtTermList(const vector<const SmtTerm*>& term_list);

  /// @brief デストラクタ
  virtual
  ~SmtTermList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kTermList 型の場合に項数を返す．
  virtual
  ymuint
  term_num() const;

  /// @brief kTermList 型の場合に項を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < identifier_term_num() )
  virtual
  const SmtTerm*
  term(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 項数
  ymuint32 mTermNum;

  // 項の配列
  // 実際には必要なサイズを確保する．
  const SmtTerm* mTermList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTTERMLIST_H
