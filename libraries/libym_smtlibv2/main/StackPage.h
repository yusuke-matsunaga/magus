#ifndef STACK_PAGE_H
#define STACK_PAGE_H

/// @file StackPage.h
/// @brief StackPage のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/SimpleAlloc.h"
#include "SmtSortMgr.h"
#include "SmtFunMgr.h"


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

  // SmtSort を管理するクラス
  SmtSortMgr mSortMgr;

  // SmtFun を管理するクラス
  SmtFunMgr mFunMgr;

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
  mSortMgr(mAlloc, 0, NULL),
  mFunMgr(mAlloc, 0, NULL)
{
}

// @brief コンストラクタ
// @param[in] level レベル
// @param[in] prev 前のページ
inline
StackPage::StackPage(ymuint level,
		     StackPage* prev) :
  mAlloc(4096),
  mSortMgr(mAlloc, level, &prev->mSortMgr),
  mFunMgr(mAlloc, level, &prev->mFunMgr)
{
}

END_NAMESPACE_YM_SMTLIBV2

#endif // STACK_PAGE_H
