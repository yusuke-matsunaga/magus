
/// @file EqSet.cc
/// @brief EqSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "EqSet.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス EqSet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EqSet::EqSet()
{
}

// @brief デストラクタ
EqSet::~EqSet()
{
}

// @brief 初期化を行う．
// @param[in] elem_list 要素番号のリスト
void
EqSet::init(const vector<ymuint>& elem_list)
{
  mElemArray.clear();
  mMarkArray.clear();

  mMaxId = 0;
  for (ymuint i = 0; i < elem_list.size(); ++ i) {
    ymuint id = elem_list[i];
    if ( mMaxId < id ) {
      mMaxId = id;
    }
  }
  ++ mMaxId;

  mElemArray.resize(mMaxId);
  mMarkArray.resize(mMaxId, false);

  mTop = NULL;
  Elem* prev = NULL;
  for (ymuint i = 0; i < elem_list.size(); ++ i) {
    ymuint id = elem_list[i];
    Elem* elem = &mElemArray[id];
    if ( mTop == NULL ) {
      mTop = elem;
    }
    else {
      prev->mLink = elem;
    }
    elem->mId = id;
    elem->mLink = NULL;
    elem->mNextTop = NULL;
    prev = elem;
  }

  mNeedFinalize = true;
}

// @brief 細分化を行う．
// @param[in] elem_list 一方の部分集合の要素のリスト
// @return 変化があったら true を返す．
bool
EqSet::refinement(const vector<ymuint>& elem_list)
{
  // elem_list に含まれる要素に印をつける．
  for (ymuint i = 0; i < elem_list.size(); ++ i) {
    ymuint id = elem_list[i];
    mMarkArray[id] = true;
  }

  bool chg = false;

  // 個々の同値類候補リストを mMarkArray の内容にしたがって分類する．
  Elem* next = NULL;
  for (Elem* top = mTop; top != NULL; top = next) {
    next = top->mNextTop;
    Elem* top0 = NULL;
    Elem* prev0 = NULL;
    Elem* top1 = NULL;
    Elem* prev1 = NULL;
    for (Elem* tmp = top; tmp != NULL; tmp = tmp->mLink) {
      if ( mMarkArray[tmp->mId] ) {
	if ( top1 != NULL ) {
	  prev1->mLink = tmp;
	}
	else {
	  top1 = tmp;
	}
	prev1 = tmp;
      }
      else {
	if ( top0 != NULL ) {
	  prev0->mLink = tmp;
	}
	else {
	  top0 = tmp;
	}
	prev0 = tmp;
      }
    }
    if ( prev0 != NULL ) {
      prev0->mLink = NULL;
    }
    if ( prev1 != NULL ) {
      prev1->mLink = NULL;
    }
    if ( top == top0 ) {
      if ( top1 != NULL ) {
	top0->mNextTop = top1;
	top1->mNextTop = next;
	mNeedFinalize = true;
	chg = true;
      }
    }
    else {
      if ( top0 != NULL ) {
	top1->mNextTop = top0;
	top0->mNextTop = next;
	mNeedFinalize = true;
	chg = true;
      }
    }
  }

  // mMarkArray の印を消す．
  for (ymuint i = 0; i < elem_list.size(); ++ i) {
    ymuint id = elem_list[i];
    mMarkArray[id] = false;
  }

  return chg;
}

// @brief 同値類候補数を返す．
ymuint
EqSet::class_num() const
{
  finalize();

  return mTopArray.size();
}

// @brief 同値類候補を変える．
// @param[in] pos 位置番号 (0 <= pos < class_num())
// @param[out] elem_list 同値類の要素番号を収めるリスト
void
EqSet::class_list(ymuint pos,
		  vector<ymuint>& elem_list) const
{
  finalize();

  ASSERT_COND( pos < class_num() );
  Elem* top = mTopArray[pos];

  ymuint n = 0;
  for (Elem* tmp = top; tmp != NULL; tmp = tmp->mLink, ++ n) { }
  elem_list.clear();
  elem_list.reserve(n);

  for (Elem* tmp = top; tmp != NULL; tmp = tmp->mLink) {
    elem_list.push_back(tmp->mId);
  }
}

// @brief 最終的な処理を行う．
void
EqSet::finalize() const
{
  if ( !mNeedFinalize ) {
    return;
  }

  ymuint n = 0;
  for (Elem* top = mTop; top != NULL; top = top->mNextTop, ++ n) {
    ;
  }

  mTopArray.clear();
  mTopArray.reserve(n);
  for (Elem* top = mTop; top != NULL; top = top->mNextTop, ++ n) {
    mTopArray.push_back(top);
  }

  mNeedFinalize = false;
}

// @brief 内容を出力する．
void
EqSet::dump(ostream& s) const
{
  ymuint i = 0;
  for (Elem* top = mTop; top != NULL; top = top->mNextTop, ++ i) {
    s << "[" << setw(4) << i << "]: ";
    for (Elem* tmp = top; tmp != NULL; tmp = tmp->mLink) {
      s << " " << tmp->mId;
    }
    s << endl;
  }
  s << endl;
}

END_NAMESPACE_YM_SATPG
