
/// @file CompTbl.cc
/// @brief CompTbl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "CompTbl.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス CompTbl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] name デバッグ用
CompTbl::CompTbl(BddMgrImpl* mgr,
		 const char* name) :
  mMgr(mgr),
  mName(name),
  mTableSize(0),
  mMaxSize(0)
{
}

// @brief デストラクタ
CompTbl::~CompTbl()
{
}

// @brief 最大のテーブルサイズを設定する．
void
CompTbl::max_size(ymuint64 max_size)
{
  mMaxSize = max_size;
}

// @brief テーブルサイズを設定する．
// @param[in] new_size 設定する新しいサイズ
void
CompTbl::set_table_size(ymuint64 new_size)
{
  mTableSize = new_size;
  double load_limit = mMgr->rt_load_limit();
  mNextLimit = static_cast<ymuint64>(double(mTableSize) * load_limit);
}

// BddMgr からメモリを確保する．
void*
CompTbl::allocate(ymuint64 size)
{
  return mMgr->allocate(size);
}

// BddMgr にメモリを返す．
void
CompTbl::deallocate(void* ptr,
		    ymuint64 size)
{
  mMgr->deallocate(ptr, size);
}

// ログ出力用のストリームを得る．
ostream&
CompTbl::logstream() const
{
  return mMgr->logstream();
}

END_NAMESPACE_YM_BDD
