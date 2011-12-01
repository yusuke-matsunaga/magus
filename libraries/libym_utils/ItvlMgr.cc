
/// @file libym_utils/ItvlMgr.cc
/// @brief ItvlMgrの実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: ItvlMgr.cc 958 2007-08-28 05:38:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/ItvlMgr.h"
#include "ItvlMgrImpl.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス ItvlMgr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
ItvlMgr::ItvlMgr() :
  mImpl(new ItvlMgrImpl)
{
}

// デストラクタ
ItvlMgr::~ItvlMgr()
{
  delete mImpl;
}

// 内容をクリアして全区間を使用可能とする．
void
ItvlMgr::clear()
{
  mImpl->clear();
}

// 使用可能な数字を得る．
// 内容は変化しない．
// 使用可能な区間がない場合(!!!)，-1を返す．
int
ItvlMgr::avail_num() const
{
  return mImpl->avail_num();
}

// [d1, d2]という区間が使用可能などうか調べる．
bool
ItvlMgr::check(int d1,
	       int d2) const
{
  return mImpl->check(d1, d2);
}

// 使用されている区間の最小値を求める．
// 全区間が未使用の場合は -1 を返す．
int
ItvlMgr::min_id() const
{
  return mImpl->min_id();
}

// 使用されている区間の最大値を求める．
// 全区間が未使用の場合は -1 を返す．
int
ItvlMgr::max_id() const
{
  return mImpl->max_id();
}

// d を使用可能な区間から削除する．
void
ItvlMgr::erase(int d)
{
  mImpl->erase(d);
  sanity_check();
}

// [d1, d2] を使用可能な区間から削除する．
void
ItvlMgr::erase(int d1,
	       int d2)
{
  mImpl->erase(d1, d2);
  sanity_check();
}

// d を使用可能区間に追加する．
void
ItvlMgr::add(int d)
{
  mImpl->add(d);
  sanity_check();
}

// [d1, d2] を使用可能区間に追加する．
void
ItvlMgr::add(int d1,
	     int d2)
{
  mImpl->add(d1, d2);
  sanity_check();
}

// 正しい構造かチェックする．
void
ItvlMgr::sanity_check() const
{
  mImpl->sanity_check();
}

// 内容を表示する．
void
ItvlMgr::print(ostream& s) const
{
  mImpl->print(s);
}

// 木構造を表示する．
void
ItvlMgr::print_tree(ostream& s) const
{
  mImpl->print_tree(s);
}

// @brief バイナリファイルに書き出す．
void
ItvlMgr::dump(BinO& s) const
{
  mImpl->dump(s);
}

// @brief バイナリファイルを読み込む．
void
ItvlMgr::restore(BinI& s)
{
  mImpl->restore(s);
}

END_NAMESPACE_YM
