
/// @file MinPatSimple.cc
/// @brief MinPatSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatSimple.h"

#include "FgMgr.h"
#include "TpgCnf1.h"



BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
MinPat*
new_MinPat2()
{
  return new MinPatSimple();
}

//////////////////////////////////////////////////////////////////////
// クラス MinPatSimple
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatSimple::MinPatSimple()
{
}

// @brief デストラクタ
MinPatSimple::~MinPatSimple()
{
}

// @brief 故障を追加するグループを選ぶ．
// @param[in] fgmgr 故障グループを管理するオブジェクト
// @param[in] fault 故障
//
// グループが見つからなければ fgmgr.group_num() を返す．
ymuint
MinPatSimple::find_group(FgMgr& fgmgr,
			 TpgFault* fault)
{

  ymuint ng = fgmgr.group_num();
  bool first = true;
  ymuint min_gid = ng;
  ymuint min_suf = 0;
  for (ymuint gid = 0; gid < ng; ++ gid) {
    TpgCnf1 tpg_cnf(string(), string(), NULL);
    tpg_cnf.make_fval_cnf(fault, max_node_id());
    const NodeValList& suf_list0 = fgmgr.suf_list(gid);
    NodeValList tmp_suf_list;
    if ( tpg_cnf.get_suf_list(suf_list0, tmp_suf_list) ) {
      ymuint suf_size = tmp_suf_list.size();
      if ( first || min_suf > suf_size ) {
	first = false;
	min_gid = gid;
	min_suf = suf_size;
      }
    }
  }
  return min_gid;
}

END_NAMESPACE_YM_SATPG
