
/// @file ZddMgrImpl.cc
/// @brief ZddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddMgrImpl.h"

#include "ym_logic/Zdd.h"


BEGIN_NAMESPACE_YM_ZDD


//////////////////////////////////////////////////////////////////////
// クラス ZddMgrImpl
//////////////////////////////////////////////////////////////////////

ZddMgrImpl* ZddMgrImpl::mDefaultMgr = NULL;

// デフォルトマネージャを返すクラスメソッド
ZddMgrImpl*
ZddMgrImpl::default_mgr()
{
  if ( !mDefaultMgr ) {
    mDefaultMgr = new ZddMgrImpl("default manager");
    assert_cond(mDefaultMgr, __FILE__, __LINE__);
  }
  return mDefaultMgr;
}

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] option オプション
ZddMgrImpl::ZddMgrImpl(const string& name,
		       const string& option) :
  mName(name)
{
  mRefCount = 0;

  // ログ出力用ストリームの初期化
  mNullStream = new ofstream("/dev/null", ios::out);
  if ( !mNullStream->is_open() ) {
    cerr << "Panic! : could not open \"/dev/null\"" << endl;
    abort();
  }
  mLogFp = mNullStream;

  // ZDD リストの初期化
  mTopZdd = NULL;
}

// デストラクタ
ZddMgrImpl::~ZddMgrImpl()
{
  assert_cond(this != mDefaultMgr, __FILE__, __LINE__);

  // ZDD の解放
  // と言っても Zdd のオブジェクトを削除するわけには行かないので
  // デフォルトマネージャのエラーZDDにすり替える．
  if ( mTopZdd ) {
    Zdd* last = NULL;
    for (Zdd* bdd = mTopZdd; bdd; bdd = bdd->mNext) {
      bdd->mRoot = ZddEdge::make_error();
      bdd->mMgr = mDefaultMgr;
      last = bdd;
    }
    Zdd* first = mDefaultMgr->mTopZdd;
    mDefaultMgr->mTopZdd = mTopZdd;
    mTopZdd->mPrev = NULL;
    last->mNext = first;
    if ( first ) {
      first->mPrev = last;
    }
  }

  // /dev/null ストリームの破壊
  delete mNullStream;
}

// log用ストリームを設定する．
void
ZddMgrImpl::set_logstream(ostream& s)
{
  mLogFp = &s;
}

// log用ストリームを解除する．
void
ZddMgrImpl::unset_logstream()
{
  mLogFp = mNullStream;
}

// log用ファイルポインタを読み出す．
ostream&
ZddMgrImpl::logstream() const
{
  return *mLogFp;
}

END_NAMESPACE_YM_ZDD
