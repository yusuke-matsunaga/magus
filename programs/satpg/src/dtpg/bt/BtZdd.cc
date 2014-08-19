
/// @file BtZdd.cc
/// @brief BtZdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtZdd.h"
#include "YmLogic/Zdd.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'Zdd' タイプの生成を行なう．
// @param[in] tvmgr TvMgr
BackTracer*
new_BtZdd(TvMgr& tvmgr)
{
  return new BtZdd(tvmgr);
}


//////////////////////////////////////////////////////////////////////
// クラス BtZdd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tvmgr TvMgr
BtZdd::BtZdd(TvMgr& tvmgr) :
  BtJustBase(tvmgr),
  mMgr("")
{
}

// @brief デストラクタ
BtZdd::~BtZdd()
{
}

// @brief ノードID番号の最大値を設定する．
// @param[in] max_id ID番号の最大値
void
BtZdd::set_max_id(ymuint max_id)
{
  mJustArray.resize(max_id);
  BtJustBase::set_max_id(max_id);
}

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[in] input_list テストパタンに関係のある入力のリスト
// @param[in] output_list 故障伝搬の可能性のある出力のリスト
TestVector*
BtZdd::operator()(TpgNode* fnode,
		  const vector<Bool3>& model,
		  const vector<TpgNode*>& input_list,
		  const vector<TpgNode*>& output_list)
{
  Zdd ans = mMgr.make_empty();

  typedef unordered_map<ymuint, TpgNode*> IdNodeMap;

  // 入力に対応する変数を作る．
  IdNodeMap input_map;
  for (vector<TpgNode*>::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    TpgNode* node = *p;
    assert_cond( node->is_input(), __FILE__, __LINE__);
    ymuint iid = node->input_id();
    mMgr.new_var(VarId(iid));
    input_map.insert(make_pair(iid, node));
  }

  // 故障差の伝搬している外部出力を選ぶ．
  for (vector<TpgNode*>::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    TpgNode* node = *p;
    if ( node_dval(node, model) == kB3True ) {
      ans |= justify(node, model);
    }
  }
  clear_justified();

  TestVector* tv = new_vector();

  ans = mMgr.minimum_set(ans);

  while ( !ans.is_one() ) {
    assert_cond( ans.edge0().is_zero(), __FILE__, __LINE__);
    VarId vid = ans.root_var();
    IdNodeMap::iterator p = input_map.find(vid.val());
    assert_cond( p != input_map.end(), __FILE__, __LINE__);
    TpgNode* node = p->second;
    record_value(node, model);
    ans = ans.edge1();
  }

  return tv;
}

// @brief clear_justified() 中で呼ばれるフック関数
void
BtZdd::clear_justified_hook(TpgNode* node)
{
  mJustArray[node->id()] = mMgr.make_empty();
}

// @brief ノードの値を正当化する入力の組み合わせを求める．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
Zdd
BtZdd::justify(TpgNode* node,
	       const vector<Bool3>& model)
{
  if ( node->is_input() ) {
    ymuint iid = node->input_id();
    Zdd ans = mMgr.make_base().swap(VarId(iid));
    return ans;
  }

  if ( justified_mark(node) ) {
    return mJustArray[node->id()];
  }
  set_justified(node);

  Bool3 gval = node_gval(node, model);
  Bool3 fval = node_fval(node, model);

  Zdd ans;

  if ( gval != fval ) {
    // 正常値と故障値が異なっていたら
    // すべてのファンインをたどる．
    ans = just_sub1(node, model);
  }
  else {
    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      // 無条件で唯一のファンインをたどる．
      ans = justify(node->fanin(0), model);
      break;

    case kTgGateAnd:
      if ( gval == kB3True ) {
	// すべてのファンインノードをたどる．
	ans = just_sub1(node, model);
      }
      else if ( gval == kB3False ) {
	// 0の値を持つノードをたどる．
	ans = just_sub2(node, model, kB3False);
      }
      break;

    case kTgGateNand:
      if ( gval == kB3True ) {
	// 0の値を持つノードをたどる．
	ans = just_sub2(node, model, kB3False);
      }
      else if ( gval == kB3False ) {
	// すべてのファンインノードをたどる．
	ans = just_sub1(node, model);
      }
      break;

    case kTgGateOr:
      if ( gval == kB3True ) {
	// 1の値を持つノードをたどる．
	ans = just_sub2(node, model, kB3True);
      }
      else if ( gval == kB3False ) {
	// すべてのファンインノードをたどる．
	ans = just_sub1(node, model);
      }
      break;

    case kTgGateNor:
      if ( gval == kB3True ) {
	// すべてのファンインノードをたどる．
	ans = just_sub1(node, model);
      }
      else if ( gval == kB3False ) {
	// 1の値を持つノードをたどる．
	ans = just_sub2(node, model, kB3True);
      }
      break;

    case kTgGateXor:
    case kTgGateXnor:
      // すべてのファンインノードをたどる．
      ans = just_sub1(node, model);
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
  mJustArray[node->id()] = ans;
  return ans;
}

// @brief すべてのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
Zdd
BtZdd::just_sub1(TpgNode* node,
		 const vector<Bool3>& model)
{
  Zdd ans = mMgr.make_base();
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    Zdd ans1 = justify(inode, model);
    ans = mMgr.merge(ans, ans1);
  }
  return ans;
}

// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[in] val 値
Zdd
BtZdd::just_sub2(TpgNode* node,
		 const vector<Bool3>& model,
		 Bool3 val)
{
  Zdd gans = mMgr.make_empty();
  Zdd fans = mMgr.make_empty();
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    Bool3 igval = node_gval(inode, model);
    Bool3 ifval = node_fval(inode, model);
    if ( igval != val && ifval != val ) {
      continue;
    }
    Zdd tmp = justify(inode, model);
    if ( igval == val ) {
      gans |= tmp;
    }
    if ( ifval == val ) {
      fans |= tmp;
    }
  }
  Zdd ans = mMgr.merge(gans, fans);
  return ans;
}

END_NAMESPACE_YM_SATPG
