
/// @file BtZdd.cc
/// @brief BtZdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "BtZdd.h"
#include "ym_logic/Zdd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス BtZdd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_id ノードの最大 ID + 1 ( = TpgNetwork::node_num() )
BtZdd::BtZdd(ymuint max_id) :
  BtJustBase(max_id),
  mMgr(""),
  mJustArray(max_id)
{
}

// @brief デストラクタ
BtZdd::~BtZdd()
{
}

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] input_list テストパタンに関係のある入力のリスト
// @param[in] output_list 故障伝搬の可能性のある出力のリスト
void
BtZdd::operator()(TpgNode* fnode,
		  const vector<TpgNode*>& input_list,
		  const vector<TpgNode*>& output_list)
{
  Zdd ans = mMgr.make_empty();

  // 入力に対応する変数を作る．
  hash_map<ymuint, TpgNode*> input_map;
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
    if ( node_dval(node) == kB3True ) {
      ans |= justify(node);
    }
  }
  clear_justified();
  clear_val_list();

  ans = mMgr.minimum_set(ans);

  while ( !ans.is_one() ) {
    assert_cond( ans.edge0().is_zero(), __FILE__, __LINE__);
    VarId vid = ans.root_var();
    hash_map<ymuint, TpgNode*>::iterator p = input_map.find(vid.val());
    assert_cond( p != input_map.end(), __FILE__, __LINE__);
    TpgNode* node = p->second;
    record_value(node);
    ans = ans.edge1();
  }
}

// @brief clear_justified() 中で呼ばれるフック関数
void
BtZdd::clear_justified_hook(TpgNode* node)
{
  mJustArray[node->id()] = mMgr.make_empty();
}

// @brief ノードの値を正当化する入力の組み合わせを求める．
// @param[in] node 対象のノード
Zdd
BtZdd::justify(TpgNode* node)
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

  Bool3 gval = node_gval(node);
  Bool3 fval = node_fval(node);

  Zdd ans;

  if ( gval != fval ) {
    // 正常値と故障値が異なっていたら
    // すべてのファンインをたどる．
    ans = just_sub1(node);
  }
  else if ( node->is_cplx_logic() ) {
    ymuint np = node->primitive_num();
    TpgPrimitive* prim = node->primitive(np - 1);
    ans = justify_primitive(prim, node);
  }
  else {
    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      // 無条件で唯一のファンインをたどる．
      ans = justify(node->fanin(0));
      break;

    case kTgGateAnd:
      if ( gval == kB3True ) {
	// すべてのファンインノードをたどる．
	ans = just_sub1(node);
      }
      else if ( gval == kB3False ) {
	// 0の値を持つノードをたどる．
	ans = just_sub2(node, kB3False);
      }
      break;

    case kTgGateNand:
      if ( gval == kB3True ) {
	// 0の値を持つノードをたどる．
	ans = just_sub2(node, kB3False);
      }
      else if ( gval == kB3False ) {
	// すべてのファンインノードをたどる．
	ans = just_sub1(node);
      }
      break;

    case kTgGateOr:
      if ( gval == kB3True ) {
	// 1の値を持つノードをたどる．
	ans = just_sub2(node, kB3True);
      }
      else if ( gval == kB3False ) {
	// すべてのファンインノードをたどる．
	ans = just_sub1(node);
      }
      break;

    case kTgGateNor:
      if ( gval == kB3True ) {
	// すべてのファンインノードをたどる．
	ans = just_sub1(node);
      }
      else if ( gval == kB3False ) {
	// 1の値を持つノードをたどる．
	ans = just_sub2(node, kB3True);
      }
      break;

    case kTgGateXor:
    case kTgGateXnor:
      // すべてのファンインノードをたどる．
      ans = just_sub1(node);
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
Zdd
BtZdd::just_sub1(TpgNode* node)
{
  Zdd ans = mMgr.make_base();
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    Zdd ans1 = justify(inode);
    ans = mMgr.merge(ans, ans1);
  }
  return ans;
}

// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
// @param[in] val 値
Zdd
BtZdd::just_sub2(TpgNode* node,
		 Bool3 val)
{
  Zdd gans = mMgr.make_empty();
  Zdd fans = mMgr.make_empty();
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    Bool3 igval = node_gval(inode);
    Bool3 ifval = node_fval(inode);
    if ( igval != val && ifval != val ) {
      continue;
    }
    Zdd tmp = justify(inode);
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

// @brief プリミティブの値を正当化する入力の組み合わせを求める．
// @param[in] prim 対象のプリミティブ
// @param[in] node 対象のノード
Zdd
BtZdd::justify_primitive(TpgPrimitive* prim,
			 TpgNode* node)
{
  if ( prim->is_input() ) {
    ymuint ipos = prim->input_id();
    TpgNode* inode = node->fanin(ipos);
    return justify(inode);
  }

  Bool3 gval = primitive_gval(prim);
  Bool3 fval = primitive_fval(prim);
  if ( gval != fval ) {
    // すべてのファンインノードをたどる．
    return jp_sub1(prim, node);
  }

  switch ( prim->gate_type() ) {
  case kTgGateBuff:
  case kTgGateNot:
    // 唯一のファンインをたどる．
    return justify_primitive(prim->fanin(0), node);

  case kTgGateAnd:
    if ( gval == kB3True ) {
      // すべてのファンインをたどる．
      return jp_sub1(prim, node);
    }
    else if ( gval == kB3False ) {
      // 0 の値を持つファンインをたどる．
      return jp_sub2(prim, node, kB3False);
    }
    break;

  case kTgGateNand:
    if ( gval == kB3True ) {
      // 0 の値を持つファンインをたどる．
      return jp_sub2(prim, node, kB3False);
    }
    else if ( gval == kB3False ) {
      // すべてのファンインをたどる．
      return jp_sub1(prim, node);
    }
    break;

  case kTgGateOr:
    if ( gval == kB3True ) {
      // 1の値をもつファンインをたどる．
      return jp_sub2(prim, node, kB3True);
    }
    else if ( gval == kB3False ) {
      // すべてのファンインをたどる．
      return jp_sub1(prim, node);
    }
    break;

  case kTgGateNor:
    if ( gval == kB3True ) {
      // すべてのファンインをたどる．
      return jp_sub1(prim, node);
    }
    else if ( gval == kB3False ) {
      // 1の値をもつすべてのファンインをたどる．
      return jp_sub2(prim, node, kB3True);
    }
    break;

  case kTgGateXor:
  case kTgGateXnor:
    // すべてのファンインをたどる．
    return jp_sub1(prim, node);

  default:
    break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return mMgr.make_empty();
}

// @brief すべてのファンインに対して justify_primitive() を呼ぶ．
// @param[in] prim 対象のプリミティブ
// @param[in] node 対象のノード
Zdd
BtZdd::jp_sub1(TpgPrimitive* prim,
	       TpgNode* node)
{
  Zdd ans = mMgr.make_base();
  ymuint ni = prim->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgPrimitive* iprim = prim->fanin(i);
    Zdd ans1 = justify_primitive(iprim, node);
    ans = mMgr.merge(ans, ans1);
  }
  return ans;
}

// @brief 指定した値を持つファンインに対して justify_primitive() を呼ぶ．
// @param[in] prim 対象のプリミティブ
// @param[in] node 対象のノード
// @param[in] val 値
Zdd
BtZdd::jp_sub2(TpgPrimitive* prim,
	       TpgNode* node,
	       Bool3 val)
{
  Zdd gans = mMgr.make_empty();
  Zdd fans = mMgr.make_empty();
  ymuint ni = prim->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgPrimitive* iprim = prim->fanin(i);
    Bool3 igval = primitive_gval(iprim);
    Bool3 ifval = primitive_fval(iprim);
    if ( igval != val && ifval != val ) {
      continue;
    }
    Zdd tmp = justify_primitive(iprim, node);
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
