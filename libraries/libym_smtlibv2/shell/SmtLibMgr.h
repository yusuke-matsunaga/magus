#ifndef SMTLIBMGR_H
#define SMTLIBMGR_H

/// @file SmtLibMgr.h
/// @brief SmtLibMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"
#include "SmtIdMgr.h"
#include "SmtSortMgr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtLibNode;
class SmtId;
class SmtSort;

//////////////////////////////////////////////////////////////////////
/// @class SmtLibMgr SmtLibMgr.h "SmtLibMgr.h"
/// @brief SmtLib の S式の evaluation を行うクラス
//////////////////////////////////////////////////////////////////////
class SmtLibMgr
{
public:

  /// @brief コンストラクタ
  SmtLibMgr();

  /// @brief デストラクタ
  ~SmtLibMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief S式を数値に変換する．
  /// @param[in] node S式を表すノード
  ymint32
  eval_to_int(const SmtLibNode* node);

  /// @brief S式をシンボルに変換する．
  /// @param[in] node S式を表すノード
  ShString
  eval_to_symbol(const SmtLibNode* node);

  /// @brief S式をキーワードに変換する．
  /// @param[in] node S式を表すノード
  ShString
  eval_to_keyword(const SmtLibNode* node);

  /// @brief S式を識別子に変換する．
  /// @param[in] node S式を表すノード
  const SmtId*
  eval_to_id(const SmtLibNode* node);

  /// @brief S式を sort に変換する．
  /// @param[in] node S式を表すノード
  const SmtSort*
  eval_to_sort(const SmtLibNode* node);



private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  SmtIdMgr mIdMgr;

  SmtSortMgr mSortMgr;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTLIBMGR_H
