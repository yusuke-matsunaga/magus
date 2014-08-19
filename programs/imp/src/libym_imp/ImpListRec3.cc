
/// @file ImpListRec3.cc
/// @brief ImpListRec3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpListRec3.h"
#include "ImpNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpListRec3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] imp_list_array 含意結果を格納するリストの配列
ImpListRec3::ImpListRec3(vector<vector<ImpVal> >& imp_list_array) :
  mImpListArray(imp_list_array)
{
}

// @brief デストラクタ
ImpListRec3::~ImpListRec3()
{
}

// @brief 含意結果を記録する仮想関数
// @param[in] src_node 含意元のノード
// @param[in] src_val 含意元の値
// @param[in] dst_node 含意先のノード
// @param[in[ dst_val 含意先の値
void
ImpListRec3::record(ImpNode* src_node,
		    ymuint src_val,
		    ImpNode* dst_node,
		    ymuint dst_val)
{
  ymuint src_id = src_node->id();
  ymuint dst_id = dst_node->id();
  if ( src_id != dst_id ) {
    mImpListArray[dst_id * 2 + dst_val].push_back(ImpVal(src_id, src_val));
    ymuint src_val1 = src_val ^ 1;
    ymuint dst_val1 = dst_val ^ 1;
    mImpListArray[src_id * 2 + src_val1].push_back(ImpVal(dst_id, dst_val1));
  }
}

END_NAMESPACE_YM_NETWORKS

