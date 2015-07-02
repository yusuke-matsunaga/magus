
/// @file MaxClique.cc
/// @brief MaxClique の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MaxClique.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス MaxClique
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] size ノード数
MaxClique::MaxClique(ymuint size) :
  mCostArray(size),
  mNlistArray(size)
{
}

// @brief デストラクタ
MaxClique::~MaxClique()
{
}

// @brief コストを設定する．
void
MaxClique::set_cost(ymuint32 id,
		    double cost)
{
  mCostArray[id] = cost;
}

// @brief 2つのノードを隣接させる．
// @param[in] id1, id2 ノード番号
void
MaxClique::connect(ymuint32 id1,
		   ymuint32 id2)
{
  mNlistArray[id1].push_back(id2);
  mNlistArray[id2].push_back(id1);
}


BEGIN_NONAMESPACE

// left の要素のうち right にも含まれるもののみを残す．
void
merge_nlist(vector<ymuint32>& left,
	    const vector<ymuint32>& right)
{
#if 0
  cout << "mrege_nlist" << endl;
  cout << " left = ";
  for (vector<ymuint32>::iterator p = left.begin();
       p != left.end(); ++ p) {
    cout << " " << *p;
  }
  cout << endl;
  cout << " right = ";
  for (vector<ymuint32>::const_iterator p = right.begin();
       p != right.end(); ++ p) {
    cout << " " << *p;
  }
  cout << endl;
#endif
  vector<ymuint32>::iterator l_rpos = left.begin();
  vector<ymuint32>::iterator l_wpos = left.begin();
  vector<ymuint32>::iterator l_end = left.end();
  vector<ymuint32>::const_iterator r_rpos = right.begin();
  vector<ymuint32>::const_iterator r_end = right.end();
  for ( ; ; ) {
    if ( l_rpos == l_end ) {
      break;
    }
    ymuint32 l_id = *l_rpos;
    if ( r_rpos == r_end ) {
      left.erase(l_wpos, l_end);
      break;
    }
    ymuint32 r_id = *r_rpos;
    if ( l_id < r_id ) {
      ++ l_rpos;
    }
    else if ( l_id == r_id ) {
      if ( l_rpos != l_wpos ) {
	*l_wpos = l_id;
      }
      ++ l_rpos;
      ++ l_wpos;
      ++ r_rpos;
    }
    else { // l_id > r_id
      ++ r_rpos;
    }
  }
#if 0
  cout << " result = ";
  for (vector<ymuint32>::iterator p = left.begin();
       p != left.end(); ++ p) {
    cout << " " << *p;
  }
  cout << endl;
#endif
}

END_NONAMESPACE


// @brief 最大クリークを求める．
// @param[out] ans 解のノード番号を入れる配列
double
MaxClique::solve(vector<ymuint32>& ans)
{
  ans.clear();

  double cost = 0.0;
  ymuint32 n = mCostArray.size();
  vector<bool> mark(n, false);
  for ( ; ; ) {
    ymuint min_num = UINT_MAX;
    ymuint min_row = 0;
    bool found = false;
    for (ymuint i = 0; i < n; ++ i) {
      if ( mark[i] ) {
	continue;
      }
      ymuint num = mNlistArray[i].size();
      if ( min_num > num ) {
	min_num = num;
	min_row = i;
	found = true;
      }
    }
    if ( !found ) {
      break;
    }

    ans.push_back(min_row);
    mark[min_row] = true;

    for (vector<ymuint32>::iterator p = mNlistArray[min_row].begin();
	 p != mNlistArray[min_row].end(); ++ p) {
      ymuint row = *p;
      mark[row] = true;
    }
    cost += mCostArray[min_row];
  }
  return cost;
}

END_NAMESPACE_YM_MINCOV
