
/// @file ItvlList.cc
/// @brief ItvlList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ItvlList.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス ItvlList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ItvlList::ItvlList() :
  mAlloc(4096),
  mRoot(NULL),
  mSize(0)
{
}

// @brief デストラクタ
ItvlList::~ItvlList()
{
}

// @brief 該当する区間を探す．
// @param[in] val 値．この値が lb 以上，ub 未満であるような区間を探す．
// @param[out] lb 下限
// @param[out] ub 上限
// @param[out] f 対応する関数
// @retval true 区間が見つかった．
// @retval false 区間が見つからなかった．
bool
ItvlList::find(double val,
	       double& lb,
	       double& ub,
	       Bdd& f)
{
  Node* node = find_node(val, mRoot);
  if ( node ) {
    lb = node->mLb;
    ub = node->mUb;
    f = node->mF;
    return true;
  }
  return false;
}

// @brief 区間を追加する．
// @param[in] lb 下限
// @param[in] ub 上限
// @param[in] f 対応する関数
void
ItvlList::add(double lb,
	      double ub,
	      const Bdd& f)
{
  void* p = mAlloc.get_memory(sizeof(Node));
  Node* node = new (p) Node;
  node->mLb = lb;
  node->mUb = ub;
  node->mF = f;
  node->mLchd = NULL;
  node->mRchd = NULL;
  node->mBalance = 0;
  ++ mSize;
  add_node(node, mRoot);
}

// @brief 要素数を返す．
ymuint
ItvlList::size() const
{
  return mSize;
}

// @brief val を含む区間を探す．
ItvlList::Node*
ItvlList::find_node(double val,
		    Node* node)
{
  while ( node ) {
    if ( node->mLb > val ) {
      node = node->mLchd;
    }
    else if ( node->mUb <= val ) {
      node = node->mRchd;
    }
    else {
      // node->mLb <= val < node->mRb
      break;
    }
  }
  return node;
}

// @brief ノードを追加する．
bool
ItvlList::add_node(Node* node,
		   Node*& ptr)
{
  if ( ptr == NULL ) {
    ptr = node;
    ptr->mBalance = 0;
    return true;
  }
  cout << "add_node([" << node->mLb << ", " << node->mUb << ") )" << endl
       << "  ptr = [" << ptr->mLb << ", " << ptr->mUb << ")" << endl;
  if ( node->mUb <= ptr->mLb ) {
    // 左の部分木に対して挿入を試みる．
    bool chg = add_node(node, ptr->mLchd);
    if ( chg ) {
      // 左の部分木の高さが変わった．
      switch ( ptr->mBalance ) {
      case 1: // 今まで左が低かったのでこれでバランスした．
	ptr->mBalance = 0;
	return false;

      case 0: // 左が高くなった．
	ptr->mBalance = -1;
	return true;

      case -1: // 左が2つ高くなった．
	{
	  Node* left = ptr->mLchd;
	  if ( left->mBalance == -1 ) {
	    // 単LL回転
	    ptr->mLchd = left->mRchd;
	    left->mRchd = ptr;
	    ptr->mBalance = 0;
	    ptr = left;
	  }
	  else {
	    // 二重LR回転
	    Node* right = left->mRchd;
	    left->mRchd = right->mLchd;
	    right->mLchd = left;
	    ptr->mLchd = right->mRchd;
	    right->mRchd = ptr;
	    if ( right->mBalance == -1 ) {
	      ptr->mBalance = 1;
	    }
	    else {
	      ptr->mBalance = 0;
	    }
	    if ( right->mBalance == 1 ) {
	      left->mBalance = -1;
	    }
	    else {
	      left->mBalance = 0;
	    }
	    ptr = right;
	  }
	  ptr->mBalance = 0;
	  return false;
	}
      }
    }
  }
  else if ( ptr->mUb <= node->mLb ) {
    // 右の部分木に対して挿入を試みる．
    bool chg = add_node(node, ptr->mRchd);
    if ( chg ) {
      // 右の部分木の高さが変わった．
      switch ( ptr->mBalance ) {
      case -1: // 今まで右が低かったのでバランスした．
	ptr->mBalance = 0;
	return false;

      case 0: // 右が高くなった．
	ptr->mBalance = 1;
	return true;

      case 1: // 右が2つ高くなった．
	{
	  Node* right = ptr->mRchd;
	  if ( right->mBalance == 1 ) {
	    // 単RR回転
	    ptr->mRchd = right->mLchd;
	    right->mLchd = ptr;
	    ptr->mBalance = 0;
	    ptr = right;
	  }
	  else {
	    // 二重RL回転
	    Node* left = right->mLchd;
	    right->mLchd = left->mRchd;
	    left->mRchd = right;
	    ptr->mRchd = left->mLchd;
	    left->mLchd = ptr;
	    if ( left->mBalance == 1 ) {
	      ptr->mBalance = -1;
	    }
	    else {
	      ptr->mBalance = 0;
	    }
	    if ( left->mBalance == -1 ) {
	      right->mBalance = 1;
	    }
	    else {
	      right->mBalance = 0;
	    }
	    ptr = left;
	  }
	  ptr->mBalance = 0;
	  return false;
	}
      }
    }
  }
  else {
    // 重複した追加はエラー
    assert_not_reached(__FILE__, __LINE__);
  }
  return false;
}

END_NAMESPACE_YM
