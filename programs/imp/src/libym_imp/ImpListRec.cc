
/// @file ImpListRec.cc
/// @brief ImpListRec の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpListRec.h"
#include "ImpNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpListRec
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] imp_list 含意結果を格納するリスト
ImpListRec::ImpListRec(vector<ImpVal>& imp_list) :
  mImpList(imp_list)
{
}

// @brief デストラクタ
ImpListRec::~ImpListRec()
{
}

// @brief 含意結果を記録する仮想関数
// @param[in] src_node 含意元のノード
// @param[in] src_val 含意元の値
// @param[in] dst_node 含意先のノード
// @param[in[ dst_val 含意先の値
void
ImpListRec::record(ImpNode* src_node,
		   ymuint src_val,
		   ImpNode* dst_node,
		   ymuint dst_val)
{
  ymuint src_id = src_node->id();
  ymuint dst_id = dst_node->id();
  if ( dst_id != src_id ) {
    mImpList.push_back(ImpVal(dst_id, dst_val));
  }
}

END_NAMESPACE_YM_NETWORKS

