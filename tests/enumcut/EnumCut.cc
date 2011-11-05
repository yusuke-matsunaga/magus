
/// @file EnumCut.cc
/// @brief EnumCut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCut.h"
#include "ym_networks/BdnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス EnumCut
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EnumCut::EnumCut()
{
}

// @brief デストラクタ
EnumCut::~EnumCut()
{
}

// @brief 処理の最初に呼ばれる関数
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
// @param[in] mode カット列挙モード
void
EnumCut::all_init(const BdnMgr& sbjgraph,
		  ymuint limit)
{
  mCurPos = 0;
  mNcAll = 0;
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
// @param[in] node 根のノード
void
EnumCut::node_init(const BdnNode* node)
{
  mNcCur = 0;
  mCurNode = node;
  cout << "#" << mCurPos << ": Node#" << node->id() << endl;
}

// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
// @param[in] root 根のノード
void
EnumCut::found_cut(const BdnNode* root)
{
  ++ mNcCur;

  mCutList.push_back(vector<ymuint32>(1, root->id()));

#if 0
  cout << "found_cut()" << endl;
#endif
}

// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
// @param[in] root 根のノード
// @param[in] ni 入力数
// @param[in] inputs 入力ノードの配列
void
EnumCut::found_cut(const BdnNode* root,
		   ymuint32 ni,
		   ymuint32 inputs[])
{
  ++ mNcCur;

  mCutList.push_back(vector<ymuint32>(ni));
  vector<ymuint32>& cut = mCutList.back();
  for (ymuint i = 0; i < ni; ++ i) {
    cut[i] = inputs[i];
  }

#if 0
  cout << "found_cut(";
  for (ymuint i = 0; i < ni; ++ i) {
    cout << " " << inputs[i];
  }
  cout << ")" << endl;
#endif
}

// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
// @param[in] node 根のノード
void
EnumCut::node_end(const BdnNode* node)
{
  assert_cond( node == mCurNode, __FILE__, __LINE__);
  ++ mCurPos;
  mNcAll += mNcCur;

  for (list<vector<ymuint32> >::iterator p = mCutList.begin();
       p != mCutList.end(); ++ p) {
    const vector<ymuint32>& cut = *p;
    ymuint n = cut.size();
    cout << "{";
    for (ymuint i = 0; i < n; ++ i) {
      cout << " " << cut[i];
    }
    cout << "}" << endl;
  }
  mCutList.clear();

  cout << "    " << mNcCur << " cuts" << endl
       << endl;
}

// @brief 処理の最後に呼ばれる関数
void
EnumCut::all_end(const BdnMgr& sbjgraph,
		 ymuint limit)
{
  cout << "Total " << mNcAll << " cuts" << endl;
}

END_NAMESPACE_YM_NETWORKS
