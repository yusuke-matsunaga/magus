
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
// @param[in] src_id 含意元のノード番号
// @param[in] imp_list 含意結果を格納するリスト
ImpListRec::ImpListRec(ymuint src_id,
		       vector<ImpVal>& imp_list) :
  mSrcId(src_id),
  mImpList(imp_list)
{
}

// @brief デストラクタ
ImpListRec::~ImpListRec()
{
}

// @brief 含意結果を記録する仮想関数
void
ImpListRec::record(ImpNode* node,
		   ymuint val)
{
  ymuint id = node->id();
  if ( id != mSrcId ) {
    mImpList.push_back(ImpVal(id, val));
  }
}

END_NAMESPACE_YM_NETWORKS

