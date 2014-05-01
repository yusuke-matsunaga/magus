
/// @file TpgNode.cc
/// @brief TpgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"
#include "TpgPrimitive.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// ゲートタイプに応じた制御値を返す．
inline
Bool3
gate_c_val(tTgGateType type,
	   Bool3 val)
{
  switch ( type ) {
  case kTgGateBuff:
    // そのまま返す．
    return val;

  case kTgGateNot:
    // 反転して返す．
    return ~val;

  case kTgGateAnd:
    // 0 の時のみ 0
    return val == kB3False ? kB3False : kB3X;

  case kTgGateNand:
    // 0 の時のみ 1
    return val == kB3False ? kB3True : kB3X;

  case kTgGateOr:
    // 1 の時のみ 1
    return val == kB3True ? kB3True : kB3X;

  case kTgGateNor:
    // 1 の時のみ 0
    return val == kB3True ? kB3False : kB3X;

  case kTgGateXor:
  case kTgGateXnor:
    // 常に X
    return kB3X;

  case kTgGateCplx:
    // これは使わない．
    return kB3X;
  }
}

Bool3
prim_val(TpgPrimitive* prim,
	 const vector<Bool3>& val_array)
{
  assert_cond( prim->is_logic(), __FILE__, __LINE__);
  ymuint ni = prim->fanin_num();
  bool has_x = false;
  switch ( prim->gate_type() ) {
  case kTgGateBuff:
    return val_array[prim->fanin(0)->subid()];

  case kTgGateNot:
    return ~val_array[prim->fanin(0)->subid()];

  case kTgGateAnd:
    for (ymuint i = 0; i < ni; ++ i) {
      Bool3 ival = val_array[prim->fanin(i)->subid()];
      if ( ival == kB3False ) {
	return kB3False;
      }
      if ( ival == kB3X ) {
	has_x = true;
      }
    }
    return has_x ? kB3X : kB3True;

  case kTgGateNand:
    for (ymuint i = 0; i < ni; ++ i) {
      Bool3 ival = val_array[prim->fanin(i)->subid()];
      if ( ival == kB3False ) {
	return kB3True;
      }
      if ( ival == kB3X ) {
	has_x = true;
      }
    }
    return has_x ? kB3X : kB3False;

  case kTgGateOr:
    for (ymuint i = 0; i < ni; ++ i) {
      Bool3 ival = val_array[prim->fanin(i)->subid()];
      if ( ival == kB3True ) {
	return kB3True;
      }
      if ( ival == kB3X ) {
	has_x = true;
      }
    }
    return has_x ? kB3X : kB3False;

  case kTgGateNor:
    for (ymuint i = 0; i < ni; ++ i) {
      Bool3 ival = val_array[prim->fanin(i)->subid()];
      if ( ival == kB3True ) {
	return kB3False;
      }
      if ( ival == kB3X ) {
	has_x = true;
      }
    }
    return has_x ? kB3X : kB3True;

  case kTgGateXor:
    {
      Bool3 val = kB3False;
      for (ymuint i = 0; i < ni; ++ i) {
	Bool3 ival = val_array[prim->fanin(i)->subid()];
	val ^= ival;
      }
      return val;
    }

  case kTgGateXnor:
    {
      Bool3 val = kB3True;
      for (ymuint i = 0; i < ni; ++ i) {
	Bool3 ival = val_array[prim->fanin(i)->subid()];
	val ^= ival;
      }
      return val;
    }

  case kTgGateCplx:
    assert_not_reached(__FILE__, __LINE__);
    return kB3X;
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス TpgNode
//////////////////////////////////////////////////////////////////////

// @brief ファンインを固定した時の出力の値を得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
// @param[in] val ファンインの値
// @return 出力の値を返す．
Bool3
TpgNode::c_val(ymuint pos,
	       Bool3 val) const
{
  assert_cond( pos < fanin_num(), __FILE__, __LINE__);

  if ( is_output() ) {
    // そのまま返す．
    return val;
  }

  if ( !is_cplx_logic() ) {
    return gate_c_val(gate_type(), val);
  }

  // ここに来ているのは kTgGateCplx のみ．
  ymuint np = primitive_num();
  vector<Bool3> val_array(np, kB3X);
  for (ymuint i = 0; i < np; ++ i) {
    TpgPrimitive* prim = primitive(i);
    if ( prim->is_input() ) {
      if ( prim->input_id() == pos ) {
	val_array[i] = val;
      }
    }
    else if ( prim->is_not_input() ) {
      if ( prim->input_id() == pos ) {
	val_array[i] = ~val;
      }
    }
    else if ( prim->is_logic() ) {
      val_array[i] = prim_val(prim, val_array);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }
  return val_array[np - 1];
}

// @brief cplx_logic タイプのときにプリミティブを返す．
// @param[in] pos 位置番号 ( 0 <= pos < primitive_num() )
TpgPrimitive*
TpgNode::primitive(ymuint pos) const
{
  assert_cond( pos < primitive_num(), __FILE__, __LINE__);
  return &mPrimitiveList[pos];
}

// @brief 後方含意で出力に値を割り当てる．
// @param[in] from_node 含意元のノード
// @param[in] val 値
// @param[in] node_list 割り当ての行われたノードを格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
TpgNode::bwd_prop(TpgNode* from_node,
		   Bool3 val,
		   vector<TpgNode*>& node_list)
{
  // ノードのファンイン方向への後方含意と
  // from_node 以外のファンアウト方向への前方含意を行う．
  return bwd_imp(val, node_list) && fanout_prop(from_node, val, node_list);
}

// @brief ファンアウト先に0を伝播する．
// @param[in] from_node 含意元のノード
// @param[in] node_list 割り当ての行われたノードを格納するリスト
// @param[in] val 値
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
TpgNode::fanout_prop(TpgNode* from_node,
		      Bool3 val,
		      vector<TpgNode*>& node_list)
{
  // from_node 以外の(アクティブな)ファンアウトに対して
  // 前方含意を行う．
  for (ymuint i = 0; i < mActFanoutNum; ++ i) {
    TpgNode* fo_node = mActFanouts[i];
    if ( fo_node == from_node ) {
      continue;
    }

    if ( !fo_node->fwd_imp(this, val, node_list) ) {
      return false;
    }
  }
  return true;
}

// @brief 後方含意を行う．
// @param[in] val 値
// @param[in] node_list 割り当ての行われたノードを格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
TpgNode::bwd_imp(Bool3 val,
		  vector<TpgNode*>& node_list)
{
  if ( mMaVal != kB3X ) {
    // 既に値が決まっている場合は
    // 新しい値が等しいかを試す．
    if ( mMaVal == val ) {
      return true;
    }
    else {
      return false;
    }
  }

  mMaVal = val;
  node_list.push_back(this);

  if ( is_input() ) {
    return true;
  }

  if ( is_output() ) {
    TpgNode* inode = fanin(0);
    return inode->bwd_prop(this, val, node_list);
  }

  if ( is_cplx_logic() ) {
    // 未完
    assert_not_reached(__FILE__, __LINE__);
    return false;
  }

  switch ( gate_type() ) {
  case kTgGateBuff:
    {
      TpgNode* inode = fanin(0);
      return inode->bwd_prop(this, val, node_list);
    }

  case kTgGateNot:
    {
      TpgNode* inode = fanin(0);
      return inode->bwd_prop(this, ~val, node_list);
    }

  case kTgGateAnd:
    if ( val == kB3True ) {
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( !inode->bwd_prop(this, kB3True, node_list) ) {
	  return false;
	}
      }
      return true;
    }
    break;

  case kTgGateNand:
    if ( val == kB3False ) {
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( !inode->bwd_prop(this, kB3True, node_list) ) {
	  return false;
	}
      }
      return true;
    }
    break;

  case kTgGateOr:
    if ( val == kB3False ) {
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( !inode->bwd_prop(this, kB3False, node_list) ) {
	  return false;
	}
      }
      return true;
    }
    break;

  case kTgGateNor:
    if ( val == kB3True ) {
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( !inode->bwd_prop(this, kB3False, node_list) ) {
	  return false;
	}
      }
      return true;
    }
    break;

  case kTgGateXor:
  case kTgGateXnor:
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  return true;
}

// @brief 前方含意を行う．
// @param[in] from_node 含意元のノード
// @param[in] val 値
// @param[in] node_list 割り当ての行われたノードを格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
TpgNode::fwd_imp(TpgNode* from_node,
		  Bool3 val,
		  vector<TpgNode*>& node_list)
{
  if ( is_input() ) {
    assert_not_reached(__FILE__, __LINE__);
    return false;
  }

  if ( is_output() ) {
    if ( mMaVal == kB3X ) {
      mMaVal = val;
      node_list.push_back(this);
    }
    else if ( mMaVal != val ) {
      return false;
    }
    return true;
  }

  if ( is_cplx_logic() ) {
    // 未完
    assert_not_reached(__FILE__, __LINE__);
    return false;
  }

  switch ( gate_type() ) {
  case kTgGateBuff:
    if ( mMaVal == kB3X ) {
      mMaVal = val;
      node_list.push_back(this);
    }
    else if ( mMaVal != val ) {
      return false;
    }
    return fanout_prop(NULL, val, node_list);

  case kTgGateNot:
    if ( mMaVal == kB3X ) {
      mMaVal = ~val;
      node_list.push_back(this);
    }
    else if ( mMaVal == val ) {
      return false;
    }
    return fanout_prop(NULL, ~val, node_list);

  case kTgGateAnd:
    if ( mMaVal == kB3X ) {
      bool has_0 = false;
      bool has_x = false;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	}
	else if ( inode->mMaVal == kB3False ) {
	  has_0 = true;
	  break;
	}
      }
      if ( has_0 ) {
	mMaVal = kB3False;
	node_list.push_back(this);
	return fanout_prop(NULL, mMaVal, node_list);
      }
      if ( !has_x ) {
	mMaVal = kB3True;
	node_list.push_back(this);
	return fanout_prop(NULL, mMaVal, node_list);
      }
    }
    else if ( mMaVal == kB3False ) {
      // justify されているか調べる．
      // - justify できない(all-1)の場合 false を返す．
      // - justify されていなくて x のファンインが1個の場合，そのファンインに後方含意する．
      bool has_0 = false;
      bool has_x = false;
      TpgNode* x_node = NULL;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	  if ( x_node == NULL ) {
	    x_node = inode;
	  }
	  else {
	    x_node = NULL;
	    break;
	  }
	}
	else if ( inode->mMaVal == kB3False ) {
	  has_0 = true;
	  break;
	}
      }
      if ( has_0 ) {
	return true;
      }
      if ( has_x ) {
	if ( x_node == NULL ) {
	  return true;
	}
	return x_node->bwd_prop(this, kB3False, node_list);
      }
      return false;
    }
    else { // mMaVal == kB3True
      if ( val == kB3False ) {
	return false;
      }
    }
    return true;

  case kTgGateNand:
    if ( mMaVal == kB3X ) {
      bool has_0 = false;
      bool has_x = false;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	}
	else if ( inode->mMaVal == kB3False ) {
	  has_0 = true;
	  break;
	}
      }
      if ( has_0 ) {
	mMaVal = kB3True;
	node_list.push_back(this);
	return fanout_prop(NULL, mMaVal, node_list);
      }
      if ( !has_x ) {
	mMaVal = kB3False;
	node_list.push_back(this);
	return fanout_prop(NULL, mMaVal, node_list);
      }
    }
    else if ( mMaVal == kB3True ) {
      // justify されているか調べる．
      // - justify できない(all-1)の場合 false を返す．
      // - justify されていなくて x のファンインが1個の場合，そのファンインに後方含意する．
      bool has_0 = false;
      bool has_x = false;
      TpgNode* x_node = NULL;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	  if ( x_node == NULL ) {
	    x_node = inode;
	  }
	  else {
	    x_node = NULL;
	    break;
	  }
	}
	else if ( inode->mMaVal == kB3False ) {
	  has_0 = true;
	  break;
	}
      }
      if ( has_0 ) {
	return true;
      }
      if ( has_x ) {
	if ( x_node == NULL ) {
	  return true;
	}
	return x_node->bwd_prop(this, kB3False, node_list);
      }
      return false;
    }
    else { // mMaVal == kB3False
      if ( val == kB3False ) {
	return false;
      }
    }
    return true;

  case kTgGateOr:
    if ( mMaVal == kB3X ) {
      bool has_1 = false;
      bool has_x = false;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	}
	else if ( inode->mMaVal == kB3True ) {
	  has_1 = true;
	  break;
	}
      }
      if ( has_1 ) {
	mMaVal = kB3True;
	node_list.push_back(this);
	return fanout_prop(NULL, mMaVal, node_list);
      }
      if ( !has_x ) {
	mMaVal = kB3False;
	node_list.push_back(this);
	return fanout_prop(NULL, mMaVal, node_list);
      }
    }
    else if ( mMaVal == kB3True ) {
      // justify されているか調べる．
      // - justify できない(all-0)の場合 false を返す．
      // - justify されていなくて x のファンインが1個の場合，そのファンインに後方含意する．
      bool has_1 = false;
      bool has_x = false;
      TpgNode* x_node = NULL;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	  if ( x_node == NULL ) {
	    x_node = inode;
	  }
	  else {
	    x_node = NULL;
	    break;
	  }
	}
	else if ( inode->mMaVal == kB3True ) {
	  has_1 = true;
	  break;
	}
      }
      if ( has_1 ) {
	return true;
      }
      if ( has_x ) {
	if ( x_node == NULL ) {
	  return true;
	}
	return x_node->bwd_prop(this, kB3True, node_list);
      }
      return false;
    }
    else { // mMaVal == kB3False
      if ( val == kB3True ) {
	return false;
      }
    }
    return true;

  case kTgGateNor:
    if ( mMaVal == kB3X ) {
      bool has_1 = false;
      bool has_x = false;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	}
	else if ( inode->mMaVal == kB3True ) {
	  has_1 = true;
	  break;
	}
      }
      if ( has_1 ) {
	mMaVal = kB3False;
	node_list.push_back(this);
	return fanout_prop(NULL, mMaVal, node_list);
      }
      if ( !has_x ) {
	mMaVal = kB3True;
	node_list.push_back(this);
	return fanout_prop(NULL, mMaVal, node_list);
      }
    }
    else if ( mMaVal == kB3False ) {
      // justify されているか調べる．
      // - justify できない(all-0)の場合 false を返す．
      // - justify されていなくて x のファンインが1個の場合，そのファンインに後方含意する．
      bool has_1 = false;
      bool has_x = false;
      TpgNode* x_node = NULL;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	  if ( x_node == NULL ) {
	    x_node = inode;
	  }
	  else {
	    x_node = NULL;
	    break;
	  }
	}
	else if ( inode->mMaVal == kB3True ) {
	  has_1 = true;
	  break;
	}
      }
      if ( has_1 ) {
	return true;
      }
      if ( has_x ) {
	if ( x_node == NULL ) {
	  return true;
	}
	return x_node->bwd_prop(this, kB3True, node_list);
      }
      return false;
    }
    else { // mMaVal == kB3True
      if ( val == kB3True ) {
	return false;
      }
    }
    return true;

  case kTgGateXor:
    if ( mMaVal == kB3X ) {
      bool has_x = false;
      Bool3 val = kB3False;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	  break;
	}
	val ^= inode->mMaVal;
      }
      if ( has_x ) {
	return true;
      }
      mMaVal = val;
      node_list.push_back(this);
      return fanout_prop(NULL, mMaVal, node_list);
    }
    else {
      bool has_x = false;
      TpgNode* x_node = NULL;
      Bool3 val = kB3False;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	  if ( x_node == NULL ) {
	    x_node = inode;
	  }
	  else {
	    x_node = NULL;
	    break;
	  }
	}
	else {
	  val ^= inode->mMaVal;
	}
      }
      if ( has_x ) {
	if ( x_node != NULL ) {
	  if ( val == mMaVal ) {
	    return x_node->fanout_prop(this, kB3False, node_list);
	  }
	  else {
	    return x_node->fanout_prop(this, kB3True, node_list);
	  }
	}
	return true;
      }
      if ( val != mMaVal ) {
	return false;
      }
    }
    return true;

  case kTgGateXnor:
    if ( mMaVal == kB3X ) {
      bool has_x = false;
      Bool3 val = kB3True;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	  break;
	}
	val ^= inode->mMaVal;
      }
      if ( has_x ) {
	return true;
      }
      mMaVal = val;
      node_list.push_back(this);
      return fanout_prop(NULL, mMaVal, node_list);
    }
    else {
      bool has_x = false;
      TpgNode* x_node = NULL;
      Bool3 val = kB3False;
      for (ymuint i = 0; i < fanin_num(); ++ i) {
	TpgNode* inode = fanin(i);
	if ( inode->mMaVal == kB3X ) {
	  has_x = true;
	  if ( x_node == NULL ) {
	    x_node = inode;
	  }
	  else {
	    x_node = NULL;
	    break;
	  }
	}
	else {
	  val ^= inode->mMaVal;
	}
      }
      if ( has_x ) {
	if ( x_node != NULL ) {
	  if ( val == mMaVal ) {
	    return x_node->fanout_prop(this, kB3False, node_list);
	  }
	  else {
	    return x_node->fanout_prop(this, kB3True, node_list);
	  }
	}
	return true;
      }
      if ( val != mMaVal ) {
	return false;
      }
    }
    return true;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return false;
}

END_NAMESPACE_YM_SATPG
