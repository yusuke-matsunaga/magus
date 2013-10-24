#ifndef STACKPAGE_H
#define STACKPAGE_H

/// @file StackPage.h
/// @brief StackPage のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/SimpleAlloc.h"
#include "SortTemplMgr.h"
#include "NameMgr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class StackPage StackPage.h "StackPage.h"
/// @brief スタックの1ページ
//////////////////////////////////////////////////////////////////////
struct StackPage
{

  /// @brief コンストラクタ(レベル0用)
  StackPage();

  /// @brief コンストラクタ
  /// @param[in] level レベル
  /// @param[in] prev 前のページ
  StackPage(ymuint level,
	    StackPage* prev);

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // SortTempl を管理するクラス
  SortTemplMgr mSortTemplMgr;

  // SmtVarFun を管理するクラス
  NameMgr mNameMgr;

  // assert されている term のリスト
  vector<const SmtTerm*> mTermList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ(レベル0用)
inline
StackPage::StackPage() :
  mAlloc(4096),
  mSortTemplMgr(mAlloc, 0, NULL),
  mNameMgr(mAlloc, 0, NULL)
{
}

// @brief コンストラクタ
// @param[in] level レベル
// @param[in] prev 前のページ
inline
StackPage::StackPage(ymuint level,
		     StackPage* prev) :
  mAlloc(4096),
  mSortTemplMgr(mAlloc, level, &prev->mSortMgr),
  mNameMgr(mAlloc, level, &prev->mNameMgr)
{
}

END_NAMESPACE_YM_SMTLIBV2

#endif // STACKPAGE_H
