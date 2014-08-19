
/// @file magus/src/seal/DupCmd.cc
/// @brief DupCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DupCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DupCmd.h"
#include "YmNetworks/BNetManip.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// 回路を部分的に多重化するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DupCmd::DupCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
  set_usage_string("node_num[=INT]");
}

// @brief デストラクタ
DupCmd::~DupCmd()
{
}


BEGIN_NONAMESPACE

void
add_fanin(BNetManip& manip,
	  BNode* onode,
	  ymuint ipos,
	  BNode* new_node)
{
  ymuint ni = onode->fanin_num();
  vector<BNode*> fanins(ni + 1);
  for (ymuint i = 0; i < ni; ++ i) {
    fanins[i] = onode->fanin(i);
  }
  fanins[ni] = new_node;
  Expr expr = onode->func();
  Expr new_expr;
  if ( expr.is_posiliteral() ) {
    assert_cond(ipos == 0 && ni == 1, __FILE__, __LINE__);
    new_expr = Expr::make_posiliteral(VarId(0)) &
      Expr::make_posiliteral(VarId(1));
  }
  else if ( expr.is_negaliteral() ) {
    assert_cond(ipos == 0 && ni == 1, __FILE__, __LINE__);
    new_expr = Expr::make_negaliteral(VarId(0)) &
      Expr::make_negaliteral(VarId(1));
  }
  else if ( expr.is_and() ) {
    int phase = 0;
    new_expr = Expr::make_one();
    for (ymuint i = 0; i < ni; ++ i) {
      Expr i_expr = expr.child(i);
      if ( i_expr.is_posiliteral() ) {
	if ( phase == -1 ) {
	  return;
	}
	phase = 1;
	new_expr &= Expr::make_posiliteral(VarId(i));
      }
      else if ( i_expr.is_negaliteral() ) {
	if ( phase == 1 ) {
	  return;
	}
	phase = -1;
	new_expr &= Expr::make_negaliteral(VarId(i));
      }
      else {
	return;
      }
    }
    assert_cond( phase != 0 , __FILE__, __LINE__);
    if ( phase > 0 ) {
      new_expr &= Expr::make_posiliteral(VarId(ni));
    }
    else {
      new_expr &= Expr::make_negaliteral(VarId(ni));
    }
  }
  else if ( expr.is_or() ) {
    int phase = 0;
    new_expr = Expr::make_zero();
    for (ymuint i = 0; i < ni; ++ i) {
      Expr i_expr = expr.child(i);
      if ( i_expr.is_posiliteral() ) {
	if ( phase == -1 ) {
	  return;
	}
	phase = 1;
	new_expr |= Expr::make_posiliteral(VarId(i));
      }
      else if ( i_expr.is_negaliteral() ) {
	if ( phase == 1 ) {
	  return;
	}
	phase = -1;
	new_expr |= Expr::make_negaliteral(VarId(i));
      }
      else {
	return;
      }
    }
    assert_cond( phase != 0 , __FILE__, __LINE__);
    if ( phase > 0 ) {
      new_expr |= Expr::make_posiliteral(VarId(ni));
    }
    else {
      new_expr |= Expr::make_negaliteral(VarId(ni));
    }
  }
  else {
    return;
  }
  bool stat = manip.change_logic(onode, new_expr, fanins);
  assert_cond(stat, __FILE__, __LINE__);
}

END_NONAMESPACE


// コマンド処理関数
int
DupCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }
  ymuint node_num;
  int stat = uint_conv(objv[1], node_num);
  if ( stat != TCL_OK ) {
    // たぶんエラー
    return stat;
  }

  BNetwork& network = *cur_network();
  BNetManip manip(&network);

  // 多重化するノードのリストを作る．
  BNodeVector node_list;
  network.tsort(node_list);
  { // node_list の順番をランダムに入れ替える．
#if 0
    ymuint nv = network.logic_node_num();
    RandGen rgen;
    vector<bool> mark(nv, false);
    for (ymuint i = 0; i < node_num; ++ i) {
      ymuint pos;
      do {
	pos = rgen.int32() % nv;
      } while ( mark[pos] );
      BNode* node = network.logic_node(pos);
      node_list[i] = node;
      mark[pos] = true;
    }
#endif
  }

  for (vector<BNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    BNode* node = *p;
    ymuint ni = node->fanin_num();
    vector<BNode*> fanins(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      fanins[i] = node->fanin(i);
    }
    Expr lexp = node->func();

    // node の複製を作る．
    BNode* node1 = manip.new_logic();
    bool stat = manip.change_logic(node1, lexp, fanins);
    assert_cond(stat, __FILE__, __LINE__);

    // ファンアウトを修正する．
    for (BNodeFoList::const_iterator p = node->fanouts_begin();
	 p != node->fanouts_end(); ++ p) {
      BNodeEdge* e = *p;
      BNode* onode = e->to();
      add_fanin(manip, onode, e->pos(), node1);
    }
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS
