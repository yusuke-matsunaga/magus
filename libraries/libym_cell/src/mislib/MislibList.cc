
/// @file MislibList.cc
/// @brief MislibList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibList.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibList
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibList::MislibList() :
  MislibNodeImpl(FileRegion()),
  mTop(NULL),
  mEnd(NULL)
{
}

// デストラクタ
MislibList::~MislibList()
{
}

// 種類を取り出す．
MislibNode::tType
MislibList::type() const
{
  return kList;
}

// 末尾に要素を追加する．
void
MislibList::push_back(MislibNodeImpl* obj)
{
  if ( mEnd ) {
    mEnd->set_next(obj);
    mEnd = obj;
  }
  else {
    mTop = mEnd = obj;
  }
}

// 先頭の要素を取り出す．
const MislibNode*
MislibList::top() const
{
  return mTop;
}

// 内容を出力する．
// デバッグ用
void
MislibList::dump(ostream& s) const
{
  s << "<PT_LIST>" << endl;
  dump_loc(s);

  for (const MislibNode* tmp = top(); tmp; tmp = tmp->next()) {
    tmp->dump(s);
  }

  s << "</PT_LIST>" << endl;
}

END_NAMESPACE_YM_MISLIB
