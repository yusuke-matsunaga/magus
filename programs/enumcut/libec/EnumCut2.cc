
/// @file EnumCut2.cc
/// @brief EnumCut2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCut2.h"
#include "YmNetworks/BdnNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス EnumCut2
//////////////////////////////////////////////////////////////////////
#if 0
// @brief コンストラクタ
EnumCut2::EnumCut2()
{
}

// @brief デストラクタ
EnumCut2::~EnumCut2()
{
}

// @brief 処理の最初に呼ばれる関数
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
// @param[in] mode カット列挙モード
void
EnumCut2::all_init(const BdnMgr& sbjgraph,
		   ymuint limit)
{
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
// @param[in] node 根のノード
void
EnumCut2::node_init(const BdnNode* node)
{
}

// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
// @param[in] root 根のノード
void
EnumCut2::found_cut(const BdnNode* root)
{
}

// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
// @param[in] root 根のノード
// @param[in] ni 入力数
// @param[in] inputs 入力ノードの配列
void
EnumCut2::found_cut(const BdnNode* root,
		    ymuint32 ni,
		    ymuint32 inputs[])
{
  ++ mNcCur;

#if 0
  mCutList.push_back(vector<ymuint32>(ni));
  vector<ymuint32>& cut = mCutList.back();
  for (ymuint i = 0; i < ni; ++ i) {
    cut[i] = inputs[i];
  }
#endif

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
EnumCut2::node_end(const BdnNode* node)
{
  ASSERT_COND( node == mCurNode );
  ++ mCurPos;
  mNcAll += mNcCur;

#if 0
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
#endif

#if 0
  cout << "    " << mNcCur << " cuts" << endl
       << endl;
#endif
}

// @brief 処理の最後に呼ばれる関数
void
EnumCut2::all_end(const BdnMgr& sbjgraph,
		  ymuint limit)
{
#if 0
  cout << "Total " << mNcAll << " cuts" << endl;
#endif
}
#endif

END_NAMESPACE_YM
