
/// @file libym_networks/MvnMgr.cc
/// @brief MvnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnMgr.h"

#include "ym_networks/MvnModule.h"
#include "ym_networks/MvnPort.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnMgr
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
MvnMgr::MvnMgr()
{
}

// @brief デストラクタ
MvnMgr::~MvnMgr()
{
}

// @brief トップレベルモジュールのリストを得る．
// @param[out] module_list モジュールを格納するリスト
// @return 要素数を返す．
// @note この関数はトップモジュール数ではなく全モジュール数に比例した
// 実行時間を要する．
ymuint
MvnMgr::topmodule_list(list<const MvnModule*>& module_list) const
{
  module_list.clear();
  for (vector<MvnModule*>::const_iterator p = mModuleArray.begin();
       p != mModuleArray.end(); ++ p) {
    MvnModule* module = *p;
    if ( module && module->parent() == NULL ) {
      module_list.push_back(module);
    }
  }
  return module_list.size();
}

// @brief モジュール番号の最大値+1を得る．
ymuint
MvnMgr::max_module_id() const
{
  return mModuleArray.size();
}

// @brief モジュールIDをキーにしてモジュールにアクセスする．
// @param[in] id モジュールID ( 0 <= id < max_module_id() )
// @return 該当するモジュールを返す．
// @note 該当するモジュールがない場合は NULL を返す．
const MvnModule*
MvnMgr::module(ymuint id) const
{
  return mModuleArray[id];
}

// @brief モジュールIDをキーにしてモジュールにアクセスする．
// @param[in] id モジュールID ( 0 <= id < max_module_id() )
// @return 該当するモジュールを返す．
// @note 該当するモジュールがない場合は NULL を返す．
MvnModule*
MvnMgr::_module(ymuint id)
{
  return mModuleArray[id];
}

// @brief ノードの ID番号の最大値 + 1 を返す．
ymuint
MvnMgr::max_node_id() const
{
  return mNodeArray.size();
}

// @brief ノードを得る．
// @param[in] id ID番号 ( 0 <= id < max_node_id() )
// @note NULL が還されることもある．
const MvnNode*
MvnMgr::node(ymuint id) const
{
  return mNodeArray[id];
}

// @brief ノードを得る．
// @param[in] id ID番号 ( 0 <= id < max_node_id() )
// @note NULL が還されることもある．
MvnNode*
MvnMgr::_node(ymuint id)
{
  return mNodeArray[id];
}

// @brief モジュールを生成する．
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ni 入力ノード数
// @param[in] no 出力ノード数
// @param[in] nio 入出力ノード数
// @return 生成したモジュールを返す．
// @note 入出力ノードのビット幅は1で初期化される．
MvnModule*
MvnMgr::new_module(const char* name,
		   ymuint np,
		   ymuint ni,
		   ymuint no,
		   ymuint nio)
{
  vector<ymuint> ibitwidth_array(ni, 1);
  vector<ymuint> obitwidth_array(no, 1);
  vector<ymuint> iobitwidth_array(nio, 1);
  return new_module(name, np,
		    ibitwidth_array, obitwidth_array, iobitwidth_array);
}

// @brief モジュールを生成する．
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ibitwidth_array 入力のビット幅の配列
// @param[in] obitwidth_array 出力のビット幅の配列
// @param[in] iobitwidth_array 入出力のビット幅の配列
// @return 生成したモジュールを返す．
MvnModule*
MvnMgr::new_module(const char* name,
		   ymuint np,
		   const vector<ymuint>& ibitwidth_array,
		   const vector<ymuint>& obitwidth_array,
		   const vector<ymuint>& iobitwidth_array)
{
  int tmp = mModuleItvlMgr.avail_num();
  if ( tmp == -1 ) {
    // IDが枯渇？
    return NULL;
  }
  mModuleItvlMgr.erase(tmp);
  ymuint id = tmp;

  ymuint ni = ibitwidth_array.size();
  ymuint no = obitwidth_array.size();
  ymuint nio = iobitwidth_array.size();
  MvnModule* module = new MvnModule(name, np, ni, no, nio);
  module->mId = id;
  while ( mModuleArray.size() <= id ) {
    mModuleArray.push_back(NULL);
  }
  mModuleArray[id] = module;

  for (ymuint i = 0; i < ni; ++ i) {
    MvnNode* node = new_input(module, ibitwidth_array[i]);
    module->mInputArray[i] = node;
  }
  for (ymuint i = 0; i < no; ++ i) {
    MvnNode* node = new_output(module, obitwidth_array[i]);
    module->mOutputArray[i] = node;
  }
  for (ymuint i = 0; i < nio; ++ i) {
    MvnNode* node = new_inout(module, iobitwidth_array[i]);
    module->mInoutArray[i] = node;
  }
  return module;
}

// @brief モジュールを削除する．
// @param[in] module 対象のモジュール
// @note モジュールインスタンスとして使われているモジュールは削除できない．
void
MvnMgr::delete_module(MvnModule* module)
{
  if ( module->parent() ) {
    // モジュールインスタンスは削除できない．
    return;
  }
  // module の要素を削除

  // module の下位モジュールを再帰的に削除

  mModuleItvlMgr.add(module->mId);
  mModuleArray[module->mId] = NULL;
  delete module;
}

// @brief モジュールのポートを初期化する．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] name 名前
// @param[in] port_ref_num ポート参照式の数
void
MvnMgr::init_port(MvnModule* module,
		  ymuint pos,
		  const char* name,
		  ymuint port_ref_num)
{
  module->mPortArray[pos] = new MvnPort(name, port_ref_num);
}

// @brief ポート参照式の内容を設定する(単純な形式)．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] port_ref_pos ポート参照式の番号
// @param[in] node 対応する入出力ノード
void
MvnMgr::set_port_ref(MvnModule* module,
		     ymuint pos,
		     ymuint port_ref_pos,
		     MvnNode* node)
{
  module->mPortArray[pos]->mPortRefArray[port_ref_pos].set(node);
}

// @brief ポート参照式の内容を設定する(ビット指定形式)．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] port_ref_pos ポート参照式の番号
// @param[in] node 対応する入出力ノード
// @param[in] index ビット指定位置
void
MvnMgr::set_port_ref(MvnModule* module,
		     ymuint pos,
		     ymuint port_ref_pos,
		     MvnNode* node,
		     ymuint index)
{
  module->mPortArray[pos]->mPortRefArray[port_ref_pos].set(node, index);
}

// @brief ポート参照式の内容を設定する(範囲指定形式)．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] port_ref_pos ポート参照式の番号
// @param[in] node 対応する入出力ノード
// @param[in] msb 範囲指定の MSB
// @param[in] lsb 範囲指定の LSB
void
MvnMgr::set_port_ref(MvnModule* module,
		     ymuint pos,
		     ymuint port_ref_pos,
		     MvnNode* node,
		     ymuint msb,
		     ymuint lsb)
{
  module->mPortArray[pos]->mPortRefArray[port_ref_pos].set(node, msb, lsb);
}

// @brief ノードを削除する．
// @param[in] node 対象のノード
// @note 入力ノード, 出力ノードは削除できない
// @note 接続が残っているノードは削除できない．
void
MvnMgr::delete_node(MvnNode* node)
{
  if ( node->type() == MvnNode::kInput || node->type() == MvnNode::kOutput ) {
    return;
  }
  for (ymuint i = 0; i < node->input_num(); ++ i) {
    if ( node->input(i)->src_pin() ) {
      cerr << "node" << node->id() << " has fanin" << endl;
      return;
    }
  }
  for (ymuint i = 0; i < node->output_num(); ++ i) {
    if ( !node->output(i)->dst_pin_list().empty() ) {
      cerr << "node" << node->id() << " has fanout" << endl;
      return;
    }
  }
  unreg_node(node);
  delete node;
}

bool
no_fanouts(MvnNode* node)
{
  ymuint no = node->output_num();
  for (ymuint j = 0; j < no; ++ j) {
    if ( !node->output(j)->dst_pin_list().empty() ) {
      return false;
    }
  }
  return true;
}

// @brief 冗長な through ノードを取り除く
void
MvnMgr::sweep()
{
  ymuint n = max_node_id();
  // ビット/部分選択と接続している連結演算の削除を行う．
  vector<MvnNode*> node_list;
  node_list.reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    MvnNode* node = _node(i);
    if ( node == NULL ) continue;
    node_list.push_back(node);
  }
  for (vector<MvnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    MvnNode* node = *p;
    MvnNode* alt_node = NULL;
    if ( node->type() == MvnNode::kThrough ) {
      const MvnOutputPin* src_pin = node->input(0)->src_pin();
      if ( src_pin != NULL ) {
	alt_node = src_pin->node();
      }
    }
    else if ( node->type() == MvnNode::kConstBitSelect ) {
      MvnNode* src_node = node->input(0)->src_pin()->node();
      if ( src_node->type() == MvnNode::kConcat ) {
	alt_node = select_from_concat(src_node, node->bitpos());
      }
      else if ( src_node->type() == MvnNode::kConstPartSelect ) {
	alt_node = select_from_partselect(src_node, node->bitpos());
      }
    }
    if ( alt_node != NULL ) {
      // node を alt_node に置き換える．
      replace(node, alt_node);
    }
  }

  // どこにも出力していないノードを削除する．
  vector<bool> marks(n, false);
  for (ymuint i = 0; i < n; ++ i) {
    MvnNode* node = _node(i);
    if ( node->type() == MvnNode::kDff ) {
      ymuint nc = node->input_num() - 2;
      for (ymuint j = 0; j < nc; ++ j) {
	const MvnNode* node1 = node->control_val(j);
	marks[node1->id()] = true;
      }
    }
  }
  list<MvnNode*> node_queue;
  for (ymuint i = 0; i < n; ++ i) {
    MvnNode* node = _node(i);
    if ( node == NULL ) continue;
    if ( node->type() == MvnNode::kInput ||
	 node->type() == MvnNode::kOutput ||
	 node->type() == MvnNode::kInout ) continue;
    if ( marks[node->id()] ) continue;
    if ( no_fanouts(node) ) {
      node_queue.push_back(node);
    }
  }
  while ( !node_queue.empty() ) {
    MvnNode* node = node_queue.front();
    node_queue.pop_front();
    ymuint ni = node->input_num();
    for (ymuint i = 0; i < ni; ++ i) {
      MvnOutputPin* src_pin = node->input(i)->src_pin();
      if ( src_pin ) {
	MvnNode* src_node = src_pin->node();
	disconnect(src_node, src_pin->pos(), node, i);
	if ( no_fanouts(src_node) ) {
	  node_queue.push_back(src_node);
	}
      }
    }
    delete_node(node);
  }
}

// @brief 連結演算からビットを抜き出す．
// @param[in] src_node 連結演算ノード
// @param[in] bitpos 抜き出すビット位置
MvnNode*
MvnMgr::select_from_concat(MvnNode* src_node,
			   ymuint bitpos)
{
  assert_cond( src_node->type() == MvnNode::kConcat, __FILE__, __LINE__);
  ymuint ni = src_node->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint idx = ni - i - 1;
    const MvnInputPin* ipin = src_node->input(idx);
    ymuint bw = ipin->bit_width();
    if ( bitpos < bw ) {
      MvnNode* inode = ipin->src_pin()->node();
      if ( inode->type() == MvnNode::kConcat ) {
	return select_from_concat(inode, bitpos);
      }
      else if ( inode->type() == MvnNode::kConstPartSelect ) {
	return select_from_partselect(inode, bitpos);
      }
      else if ( bw == 1 ) {
	return inode;
      }
      else {
	MvnNode* bitsel = new_constbitselect(src_node->mParent, bitpos, bw);
	connect(inode, 0, bitsel, 0);
	return bitsel;
      }
    }
    bitpos -= bw;
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 部分指定子からビットを抜き出す．
// @param[in] src_node 部分指定ノード
// @param[in] bitpos 抜き出すビット位置
MvnNode*
MvnMgr::select_from_partselect(MvnNode* src_node,
			       ymuint bitpos)
{
  assert_cond( src_node->type() == MvnNode::kConstPartSelect,
	       __FILE__, __LINE__);
  const MvnInputPin* ipin = src_node->input(0);
  ymuint bw = ipin->bit_width();
  assert_cond( bitpos < bw, __FILE__, __LINE__);
  ymuint msb = src_node->msb();
  ymuint lsb = src_node->lsb();
  if ( msb > lsb ) {
    bitpos = bitpos + lsb;
  }
  else {
    bitpos = lsb - bitpos;
  }
  MvnNode* inode = ipin->src_pin()->node();
  if ( inode->type() == MvnNode::kConcat ) {
    return select_from_concat(inode, bitpos);
  }
  else if ( inode->type() == MvnNode::kConstPartSelect ) {
    return select_from_partselect(inode, bitpos);
  }
  else {
    MvnNode* bitsel = new_constbitselect(src_node->mParent, bitpos, bw);
    connect(inode, 0, bitsel, 0);
    return bitsel;
  }
}

// @brief node を alt_node に置き換える．
// @param[in] node 置換元のノード
// @param[in] alt_node 置換先のノード
// @note node は削除される．
void
MvnMgr::replace(MvnNode* node,
		MvnNode* alt_node)
{
  ymuint no = node->output_num();
  assert_cond( no == alt_node->output_num(), __FILE__, __LINE__);
  for (ymuint i = 0; i < no; ++ i) {
    reconnect(node, i, alt_node, i);
  }
}

// @brief ピンとピンを接続する．
// @param[in] src_node 入力元のノード
// @param[in] src_pin_pos 入力元のピン番号
// @param[in] dst_node 出力先のノード
// @param[in] dst_pin 出力先のピン番号
// @return 接続を表すネットを返す．
// @note 接続が失敗したら NULLを返す．
// @note 接続が失敗するのは，
//  - ピンが異なるモジュールに属していた．
//  - ピンのビット幅が異なっていた．
bool
MvnMgr::connect(MvnNode* src_node,
		ymuint src_pin_pos,
		MvnNode* dst_node,
		ymuint dst_pin_pos)
{
  if ( src_node->parent() != dst_node->parent() ) {
    cerr << "connecting between differnt module" << endl;
    abort();
    return false;
  }
  MvnOutputPin* src_pin = src_node->_output(src_pin_pos);
  MvnInputPin* dst_pin = dst_node->_input(dst_pin_pos);
  if ( src_pin->bit_width() != dst_pin->bit_width() ) {
    cerr << "src_pin->bit_width() = " << src_pin->bit_width()
	 << ", dst_pin->bit_width() = " << dst_pin->bit_width() << endl;
    cerr << "bit_width mismatch" << endl;
    abort();
    return false;;
  }
  src_pin->mDstPinList.push_back(dst_pin);
  dst_pin->mSrcPin = src_pin;
  return true;
}

// @brief 接続を取り除く
// @param[in] src_node 入力元のノード
// @param[in] src_pin_pos 入力元のピン番号
// @param[in] dst_node 出力先のノード
// @param[in] dst_pin 出力先のピン番号
void
MvnMgr::disconnect(MvnNode* src_node,
		   ymuint src_pin_pos,
		   MvnNode* dst_node,
		   ymuint dst_pin_pos)
{
  MvnOutputPin* src_pin = src_node->_output(src_pin_pos);
  MvnInputPin* dst_pin = dst_node->_input(dst_pin_pos);
  assert_cond( dst_pin->mSrcPin == src_pin, __FILE__, __LINE__);
  src_pin->mDstPinList.erase(dst_pin);
  dst_pin->mSrcPin = NULL;
}

// @brief 接続を切り替える．
// @param[in] old_node 元のノード
// @param[in] old_pin_pos 元のピン番号
// @param[in] new_node 新しいノード
// @param[in] new_pin 新しいピン番号
void
MvnMgr::reconnect(MvnNode* old_node,
		  ymuint old_pin_pos,
		  MvnNode* new_node,
		  ymuint new_pin_pos)
{
  MvnOutputPin* old_pin = old_node->_output(old_pin_pos);
  MvnOutputPin* new_pin = new_node->_output(new_pin_pos);
  const MvnInputPinList& fo_list = old_pin->dst_pin_list();
  // リンクトリストをたどっている途中でリンクの変更はできないので
  // 配列にコピーする．
  vector<MvnInputPin*> tmp_list;
  tmp_list.reserve(fo_list.size());
  for (MvnInputPinList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    MvnInputPin* ipin = *p;
    tmp_list.push_back(ipin);
  }
  for (vector<MvnInputPin*>::iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    MvnInputPin* ipin = *p;
    old_pin->mDstPinList.erase(ipin);
    ipin->mSrcPin = new_pin;
    new_pin->mDstPinList.push_back(ipin);
  }
}

// @brief ノードを登録する．
// @param[in] node 対象のノード
void
MvnMgr::reg_node(MvnNode* node)
{
  int tmp = mNodeItvlMgr.avail_num();
  if ( tmp == -1 ) {
    // ID番号が枯渇？
    return;
  }
  mNodeItvlMgr.erase(tmp);
  ymuint id = tmp;
  node->mId = id;
  while ( mNodeArray.size() <= id ) {
    mNodeArray.push_back(NULL);
  }
  mNodeArray[id] = node;

  if ( node->type() != MvnNode::kInput &&
       node->type() != MvnNode::kOutput &&
       node->type() != MvnNode::kInout ) {
    MvnModule* module = node->mParent;
    list<MvnNode*>& nodelist = module->mNodeList;
    nodelist.push_back(node);
    node->mSelfRef = nodelist.end();
    -- node->mSelfRef;
    assert_cond( *node->mSelfRef == node , __FILE__, __LINE__);
  }
}

// @brief ノードの登録を削除する．
// @param[in] node 対象のノード
void
MvnMgr::unreg_node(MvnNode* node)
{
  mNodeItvlMgr.add(node->id());
  mNodeArray[node->id()] = NULL;

  if ( node->type() != MvnNode::kInput &&
       node->type() != MvnNode::kOutput &&
       node->type() != MvnNode::kInout ) {
    MvnModule* module = node->mParent;
    module->mNodeList.erase(node->mSelfRef);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MvnInputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnInputPin::MvnInputPin() :
  mNode(NULL),
  mPos(0),
  mBitWidth(0),
  mSrcPin(NULL)
{
}

// @brief デストラクタ
MvnInputPin::~MvnInputPin()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvnOutputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnOutputPin::MvnOutputPin() :
  mNode(NULL),
  mPos(0),
  mBitWidth(0)
{
}

// @brief デストラクタ
MvnOutputPin::~MvnOutputPin()
{
}

END_NAMESPACE_YM_NETWORKS_MVN
