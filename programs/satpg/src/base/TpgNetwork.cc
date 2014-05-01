
/// @file TpgNetwork.cc
/// @brief TpgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgPrimitive.h"
#include "TpgFault.h"
#include "networks/TgNetwork.h"
#include "networks/TgNode.h"
#include "networks/TgBlifReader.h"
#include "networks/TgIscas89Reader.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// 全ての論理ノードを kTgGateCplx タイプとして登録する時に true にする．
// TpgPrimitive 関係のコードのデバッグ用
const bool FORCE_TO_CPLX_LOGIC = false;

// 指定された型の配列を確保するテンプレート関数
template <typename T>
T*
alloc_array(Alloc& alloc,
	    ymuint n)
{
  void* p = alloc.get_memory(sizeof(T) * n);
  return new (p) T[n];
}


// immediate dominator リストをマージする．
TpgNode*
merge(TpgNode* node1,
      TpgNode* node2)
{
  for ( ; ; ) {
    if ( node1 == node2 ) {
      return node1;
    }
    if ( node1 == NULL || node2 == NULL ) {
      return NULL;
    }
    ymuint id1 = node1->id();
    ymuint id2 = node2->id();
    if ( id1 < id2 ) {
      node1 = node1->imm_dom();
    }
    else if ( id1 > id2 ) {
      node2 = node2->imm_dom();
    }
  }
}

// pair の最初の要素で比較を行なうファンクター
// こういうのは lambda 表記があると簡潔にすむ．
struct Lt
{
  bool
  operator()(const pair<ymuint, ymuint>& left,
	     const pair<ymuint, ymuint>& right)
  {
    return left.first < right.first;
  }

};

// 論理式から必要なプリミティブ数を数える．
ymuint
primitive_count(const Expr& expr)
{
  if ( expr.is_posiliteral() ) {
    // 肯定のリテラルは入力プリミティブ1つ
    // 同じ入力番号のリテラルがあったとしても共有しない．
    return 1;
  }
  if ( expr.is_negaliteral() ) {
    // 否定のリテラルはNOT入力プリミティブ1つ
    // 同じ入力番号のリテラルがあったとしても共有しない．
    return 1;
  }

  // 演算子のプリミティブが1つ
  ymuint n = 1;

  // あとは子供の論理式に対するプリミティブ数を足し合わせる．
  ymuint nc = expr.child_num();
  for (ymuint i = 0; i < nc; ++ i) {
    Expr expr1 = expr.child(i);
    n += primitive_count(expr1);
  }

  return n;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TpgNetwork
//////////////////////////////////////////////////////////////////////

// @brief blif ファイルを読み込んでインスタンスを作る．
// @param[in] filename ファイル名
// @param[in] cell_library セルライブラリ
// @note エラーが起こったら NULL を返す．
TpgNetwork*
TpgNetwork::read_blif(const string& filename,
		      const CellLibrary* cell_library)
{
  TgNetwork tgnetwork;
  TgBlifReader read;

  bool stat = read(filename, tgnetwork, cell_library);
  if ( !stat ) {
    return NULL;
  }

  TpgNetwork* network = new TpgNetwork(tgnetwork);

  return network;
}

// @brief iscas89 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @note エラーが起こったら NULL を返す．
TpgNetwork*
TpgNetwork::read_iscas89(const string& filename)
{
  TgNetwork tgnetwork;
  TgIscas89Reader read;

  bool stat = read(filename, tgnetwork);
  if ( !stat ) {
    return NULL;
  }

  TpgNetwork* network = new TpgNetwork(tgnetwork);

  return network;
}

// @brief コンストラクタ
// @param[in] tgnetwork もとのネットワーク
TpgNetwork::TpgNetwork(const TgNetwork& tgnetwork) :
  mAlloc(4096)
{
  //////////////////////////////////////////////////////////////////////
  // 要素数を数え，必要なメモリ領域を確保する．
  //////////////////////////////////////////////////////////////////////

  mNodeNum = tgnetwork.node_num();
  mInputNum = tgnetwork.input_num1();
  mOutputNum = tgnetwork.output_num1();
  mFFNum = tgnetwork.ff_num();

  mNodeArray = alloc_array<TpgNode>(mAlloc, mNodeNum);

  mNodeMap = alloc_array<TpgNode*>(mAlloc, mNodeNum);
  mInputArray = alloc_array<TpgNode*>(mAlloc, input_num2());
  mOutputArray = alloc_array<TpgNode*>(mAlloc, output_num2());
  mOutputArray2 = alloc_array<TpgNode*>(mAlloc, output_num2());

  mActNodeNum = 0;
  mActNodeArray = alloc_array<TpgNode*>(mAlloc, mNodeNum);

  mTmpNodeNum = 0;
  mTmpNodeList = alloc_array<TpgNode*>(mAlloc, mNodeNum);

  mTmpMark = alloc_array<bool>(mAlloc, mNodeNum);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    mTmpMark[i] = false;
  }

  ymuint id = 0;

  //////////////////////////////////////////////////////////////////////
  // 外部入力を作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npi = input_num2();
  for (ymuint i = 0; i < npi; ++ i, ++ id) {
    const TgNode* tgnode = tgnetwork.input(i);
    TpgNode* node = make_node(id, tgnode);
    mInputArray[i] = node;
  }


  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  // ただし mNodeArray は入力からのトポロジカル順になる．
  //////////////////////////////////////////////////////////////////////
  ymuint nl = tgnetwork.logic_num();
  for (ymuint i = 0; i < nl; ++ i, ++ id) {
    const TgNode* tgnode = tgnetwork.sorted_logic(i);
    TpgNode* node = make_node(id, tgnode);
    if ( tgnode->is_cplx_logic() ) {
      // 論理式をとってくる．
      ymuint fid = tgnode->func_id();
      Expr expr = tgnetwork.get_lexp(fid);

      // プリミティブ数を数え，必要なメモリ領域を確保する．
      ymuint np = primitive_count(expr);
      node->mPrimitiveNum = np;
      node->mPrimitiveList = alloc_array<TpgPrimitive>(mAlloc, np);
      for (ymuint j = 0; j < np; ++ j) {
	node->mPrimitiveList[j].mSubId = j;
      }

      // expr の内容を表すプリミティブの木を作る．
      // 結果は node->mPrimitiveList に直接書き込まれる．
      ymuint subid = 0;
      make_primitive(expr, tgnode, node->mPrimitiveList, subid);

      // 念のため計算どおりのプリミティブ数か確かめる．
      assert_cond( subid == np, __FILE__, __LINE__);
    }
    else if ( FORCE_TO_CPLX_LOGIC ) {
      // デバッグ用のコード
      // 組み込み型のゲートも TpgPrimitive を用いて表す．
      ymuint ni = tgnode->fanin_num();
      tTgGateType gate_type = tgnode->gate_type();

      // プリミティブ数はファンイン数＋1
      ymuint np = ni + 1;
      node->mPrimitiveNum = np;
      node->mPrimitiveList = alloc_array<TpgPrimitive>(mAlloc, np);
      for (ymuint j = 0; j < np; ++ j) {
	node->mPrimitiveList[j].mSubId = j;
      }

      // 入力プリミティブの設定
      for (ymuint i = 0; i < ni; ++ i) {
	TpgPrimitive* prim = &node->mPrimitiveList[i];
	set_input_primitive(prim, i);
      }

      // 演算子(AND/OR/XOR)プリミティブの設定
      TpgPrimitive* prim = &node->mPrimitiveList[ni];
      set_logic_primitive(prim, gate_type, ni);
      for (ymuint i = 0; i < ni; ++ i) {
	prim->mFanins[i] = &node->mPrimitiveList[i];
      }

      // cplx_logic マークにつけ直す．
      node->mTypeId = 3U | (kTgGateCplx << 2);
    }
  }


  //////////////////////////////////////////////////////////////////////
  // 外部出力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_num2();
  for (ymuint i = 0; i < npo; ++ i, ++ id) {
    const TgNode* tgnode = tgnetwork.output(i);
    TpgNode* node = make_node(id, tgnode);
    mOutputArray[i] = node;
  }

  assert_cond( id == mNodeNum, __FILE__, __LINE__);


  //////////////////////////////////////////////////////////////////////
  // ファンアウトの情報をコピーする．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    const TgNode* tgnode = tgnetwork.node(i);
    ymuint nfo = tgnode->fanout_num();
    TpgNode* node = mNodeMap[tgnode->gid()];
    for (ymuint i = 0; i < nfo; ++ i) {
      const TgNode* otgnode = tgnode->fanout(i);
      TpgNode* onode = mNodeMap[otgnode->gid()];
      node->mFanouts[i] = onode;
    }
  }

  // 全部アクティブにしておく．
  activate_all();

  // TFI のサイズの昇順に並べ方出力順を
  // mOutputArray2 に記録する．
  vector<pair<ymuint, ymuint> > tmp_list(output_num2());
  for (ymuint i = 0; i < output_num2(); ++ i) {
    TpgNode* onode = output(i);
    tfimark(onode);
    ymuint n = mTmpNodeNum;
    clear_tfimark();
    tmp_list[i] = make_pair(n, i);
  }

  sort(tmp_list.begin(), tmp_list.end(), Lt());

  for (ymuint i = 0; i < output_num2(); ++ i) {
    mOutputArray2[i] = mOutputArray[tmp_list[i].second];
  }
}

// @brief デストラクタ
TpgNetwork::~TpgNetwork()
{
  // このオブジェクトが確保したメモリは mAlloc のデストラクタが
  // 開放してくれる．
}

// @brief 一つの外部出力に関係するノードのみをアクティブにする．
// @param[in] po_pos 出力番号
void
TpgNetwork::activate_po(ymuint po_pos)
{
  TpgNode* onode = output2(po_pos);

  // pos 番めの出力から到達可能なノードにマークをつける．
  tfimark(onode);

  activate_sub();
}

// @brief 全てのノードをアクティブにする．
void
TpgNetwork::activate_all()
{
  // といっても PO に到達できないノードは除外する．

  // すべての PO から到達可能なノードにマークをつける．
  for (ymuint i = 0; i < mOutputNum + mFFNum; ++ i) {
    tfimark(output(i));
  }

  activate_sub();
}

// @brief ノードの TFI にマークをつける．
// @note 結果は mTmpMark[node->id()] に格納される．
void
TpgNetwork::tfimark(TpgNode* node)
{
  if ( mTmpMark[node->id()] ) {
    return;
  }

  mTmpMark[node->id()] = true;
  mTmpNodeList[mTmpNodeNum] = node;
  ++ mTmpNodeNum;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    tfimark(node->fanin(i));
  }
}

// @brief activate_po(), activate_all() の下請け関数
void
TpgNetwork::activate_sub()
{
  // マークにしたがってファンアウトなどの情報をセットする．
  mActNodeNum = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = &mNodeArray[i];
    if ( !mTmpMark[i] ) {
      node->clear_active();
      continue;
    }
    node->set_active();
    mActNodeArray[mActNodeNum] = node;
    ++ mActNodeNum;

    ymuint nfo = node->fanout_num();
    ymuint act_nfo = 0;
    for (ymuint i = 0; i < nfo; ++ i) {
      TpgNode* onode = node->fanout(i);
      if ( mTmpMark[onode->id()] ) {
	node->mActFanouts[act_nfo] = onode;
	++ act_nfo;
      }
    }
    node->mActFanoutNum = act_nfo;
  }

  // immediate dominator を求める．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    TpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint nfo = node->active_fanout_num();
    if ( nfo == 0 ) {
      assert_cond( node->is_output(), __FILE__, __LINE__);
      node->mImmDom = NULL;
    }
    else {
      TpgNode* node0 = node->active_fanout(0);
      for (ymuint i = 1; i < nfo; ++ i) {
	node0 = merge(node0, node->active_fanout(i));
	if ( node0 == NULL ) {
	  break;
	}
      }
      node->mImmDom = node0;
    }
  }

  // マークを消す．
  clear_tfimark();
}

// @brief TFI マークを消す．
// @note この関数が終了すると mTmpNodeNum は 0 になる．
void
TpgNetwork::clear_tfimark()
{
  for (ymuint i = 0; i < mTmpNodeNum; ++ i) {
    mTmpMark[mTmpNodeList[i]->id()] = false;
  }
  mTmpNodeNum = 0;
}

// @brief ノードを得る．
TpgNode*
TpgNetwork::node(ymuint pos)
{
  // アドレス計算のために TpgNode の定義が必要なのでここに置く．
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return &mNodeArray[pos];
}

// @brief ノードを得る．
const TpgNode*
TpgNetwork::node(ymuint pos) const
{
  // アドレス計算のために TpgNode の定義が必要なのでここに置く．
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return &mNodeArray[pos];
}

// @brief TpgNode の内容を設定する．
// @param[in] id ID番号
// @param[in] tgnode もととなる TgNode
// @param[in] type ノードタイプ
// @param[in] sub_id サブID
TpgNode*
TpgNetwork::make_node(ymuint id,
		      const TgNode* tgnode)
{
  TpgNode* node = &mNodeArray[id];
  node->mId = id;

  const char* src_name = tgnode->name();
  if ( tgnode->is_output() ) {
    src_name = tgnode->fanin(0)->name();
  }
  if ( src_name != NULL ) {
    ymuint len = strlen(src_name) + 1;
    char* dst_name = alloc_array<char>(mAlloc, len);
    for (ymuint i = 0; i < len; ++ i) {
      dst_name[i] = src_name[i];
    }
    node->mName = dst_name;
  }
  else {
    node->mName = NULL;
  }

  mNodeMap[tgnode->gid()] = node;

  node->mCval = kB3X;
  node->mNval = kB3X;
  node->mMaVal = kB3X;

  if ( tgnode->is_input() ) {
    node->mTypeId = 1U | (tgnode->lid() << 2);
  }
  else if ( tgnode->is_output() ) {
    node->mTypeId = 2U | (tgnode->lid() << 2);
  }
  else if ( tgnode->is_logic() ) {
    node->mTypeId = 3U | (static_cast<ymuint>(tgnode->gate_type()) << 2);

    switch ( tgnode->gate_type() ) {
    case kTgGateAnd:
    case kTgGateNand:
      node->mCval = kB3False;
      node->mNval = kB3True;
      break;

    case kTgGateOr:
    case kTgGateNor:
      node->mCval = kB3True;
      node->mNval = kB3False;
      break;

    default:
      break;
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }

  node->mPrimitiveNum = 0;
  node->mPrimitiveList = NULL;

  ymuint ni = tgnode->fanin_num();
  node->mFaninNum = ni;
  if ( ni > 0 ) {
    node->mFanins = alloc_array<TpgNode*>(mAlloc, ni);
    node->mInputFault = alloc_array<TpgFault*>(mAlloc, ni * 2);
  }
  else {
    node->mFanins = NULL;
    node->mInputFault = NULL;
  }

  for (ymuint i = 0; i < ni; ++ i) {
    const TgNode* itgnode = tgnode->fanin(i);
    TpgNode* inode = mNodeMap[itgnode->gid()];
    node->mFanins[i] = inode;
  }

  ymuint no = tgnode->fanout_num();
  node->mFanoutNum = no;
  node->mFanouts = alloc_array<TpgNode*>(mAlloc, no);
  node->mActFanoutNum = 0;
  node->mActFanouts = alloc_array<TpgNode*>(mAlloc, no);

  for (ymuint i = 0; i < ni; ++ i) {
    node->mInputFault[i * 2 + 0] = NULL;
    node->mInputFault[i * 2 + 1] = NULL;
  }
  node->mOutputFault[0] = NULL;
  node->mOutputFault[1] = NULL;

  node->mMarks = 0U;

  return node;
}

// @brief 複雑な論理式に対応するプリミティブを作る．
// @param[in] expr 論理式
// @param[in] tgnode もととなる TgNode
// @param[in] primitive_list プリミティブを設定する領域
// @param[inout] id プリミティブID
// @note id は内部でインクリメントされる．
TpgPrimitive*
TpgNetwork::make_primitive(const Expr& expr,
			   const TgNode* tgnode,
			   TpgPrimitive* primitive_list,
			   ymuint& id)
{
  if ( expr.is_posiliteral() ) {
    // 肯定のリテラル
    ymuint ipos = expr.varid().val();
    TpgPrimitive* input = &primitive_list[id];
    ++ id;
    set_input_primitive(input, ipos);
    return input;
  }
  if ( expr.is_negaliteral() ) {
    // 否定のリテラル
    ymuint ipos = expr.varid().val();
    TpgPrimitive* input = &primitive_list[id];
    ++ id;
    set_not_input_primitive(input, ipos);
    return input;
  }

  ymuint nc = expr.child_num();
  vector<TpgPrimitive*> fanin(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    // 子供の論理式に対するプリミティブを作る．
    Expr expr1 = expr.child(i);
    TpgPrimitive* inode = make_primitive(expr1, tgnode, primitive_list, id);
    fanin[i] = inode;
  }

  TpgPrimitive* prim = &primitive_list[id];
  ++ id;

  tTgGateType gate_type;
  if ( expr.is_and() ) {
    gate_type = kTgGateAnd;
  }
  else if ( expr.is_or() ) {
    gate_type = kTgGateOr;
  }
  else if ( expr.is_xor() ) {
    gate_type = kTgGateXor;
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  set_logic_primitive(prim, gate_type, nc);
  for (ymuint i = 0; i < nc; ++ i) {
    prim->mFanins[i] = fanin[i];
  }
  return prim;
}

// @brief 入力プリミティブの設定を行なう．
// @param[in] prim プリミティブ
// @param[in] id 入力番号
void
TpgNetwork::set_input_primitive(TpgPrimitive* prim,
				ymuint id)
{
  prim->mTypeId = 0U | (id << 2);
  prim->mFaninNum = 0;
  prim->mFanins = NULL;
}

// @brief 否定付き入力プリミティブの設定を行なう．
// @param[in] prim プリミティブ
// @param[in] id 入力番号
void
TpgNetwork::set_not_input_primitive(TpgPrimitive* prim,
				    ymuint id)
{
  prim->mTypeId = 1U | (id << 2);
  prim->mFaninNum = 0;
  prim->mFanins = NULL;
}

// @brief 論理プリミティブの設定を行なう．
// @param[in] prim プリミティブ
// @param[in] gate_type ゲートタイプ
// @param[in] ni 入力数
void
TpgNetwork::set_logic_primitive(TpgPrimitive* prim,
				tTgGateType gate_type,
				ymuint ni)
{
  prim->mTypeId = 2U | (static_cast<ymuint32>(gate_type) << 2);
  prim->mFaninNum = ni;
  prim->mFanins = alloc_array<TpgPrimitive*>(mAlloc, ni);
}


//////////////////////////////////////////////////////////////////////
// 必要割り当て関係のコード
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// node のファンアウトに mark1 をつける．
void
set_tfo_mark(TpgNode* node)
{
  if ( node->mark1() ) {
    return;
  }
  node->set_mark1();

  ymuint n = node->active_fanout_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* onode = node->active_fanout(i);
    set_tfo_mark(onode);
  }
}

// node のファンアウトに mark1 をつける．
void
clear_tfo_mark(TpgNode* node)
{
  if ( !node->mark1() ) {
    return;
  }
  node->clear_mark1();

  ymuint n = node->active_fanout_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* onode = node->active_fanout(i);
    clear_tfo_mark(onode);
  }
}

END_NONAMESPACE

// @brief f の検出に必要な割り当てを求める．
// @param[in] f 対象の故障
// @param[in] ma_list 割り当て結果を格納するリスト
// @return 矛盾が生じたら(fが冗長故障の場合) false を返す．
// @note TpgNetwork のメンバにはアクセスしないので static メンバになっている．
// @note ma_list の内容は TpgNode::id() * 2 + val (0 / 1)
bool
TpgNetwork::get_mandatory_assignment(TpgFault* f,
				     vector<ymuint32>& ma_list)
{
  TpgNode* fnode = f->node();
  TpgNode* fsrc = fnode;
  if ( f->is_input_fault() ) {
    fsrc = fnode->fanin(f->pos());
  }

  vector<TpgNode*> node_list;
  Bool3 gval;
  if ( f->val() == 1 ) {
    gval = kB3False;
  }
  else {
    gval = kB3True;
  }
  bool stat = fsrc->bwd_prop(NULL, gval, node_list);
  if ( !stat ) {
    goto untestable;
  }

  if ( f->is_input_fault() ) {
    Bool3 nval = fnode->nval();
    if ( nval != kB3X ) {
      ymuint ni = fnode->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	if ( i == f->pos() ) continue;
	TpgNode* inode = fnode->fanin(i);
	if ( !inode->bwd_prop(fnode, nval, node_list) ) {
	  goto untestable;
	}
      }
    }
  }
  if ( fnode->imm_dom() != NULL ) {
    set_tfo_mark(fnode);
    for (TpgNode* dom = fnode->imm_dom(); dom != NULL; dom = dom->imm_dom()) {
      Bool3 nval = dom->nval();
      if ( nval != kB3X ) {
	ymuint ni = dom->fanin_num();
	for (ymuint i = 0; i < ni; ++ i) {
	  TpgNode* inode = dom->fanin(i);
	  if ( inode->mark1() ) continue;
	  if ( !inode->bwd_prop(dom, nval, node_list) ) {
	    clear_tfo_mark(fnode);
	    goto untestable;
	  }
	}
      }
    }
    clear_tfo_mark(fnode);
  }

  for (vector<TpgNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    TpgNode* node = *p;
    assert_cond( node->ma_value() != kB3X, __FILE__, __LINE__);
    int val = node->ma_value() == kB3True ? 1 : 0;
    ma_list.push_back(node->id() * 2 + val);
    node->clear_ma_value();
  }
  sort(ma_list.begin(), ma_list.end());
  return true;

 untestable:
  for (vector<TpgNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    TpgNode* node = *p;
    node->clear_ma_value();
  }
  return false;
}

END_NAMESPACE_YM_SATPG
