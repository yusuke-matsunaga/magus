
/// @file libym_mvn/MvMgr.cc
/// @brief MvMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvMgr.h"

#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPort.h"

#include "MvInput.h"
#include "MvOutput.h"
#include "MvInout.h"

#include "ym_mvn/MvNet.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvMgr
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
MvMgr::MvMgr()
{
}

// @brief デストラクタ
MvMgr::~MvMgr()
{
}

// @brief トップレベルモジュールのリストを得る．
// @param[out] module_list モジュールを格納するリスト
// @return 要素数を返す．
// @note この関数はトップモジュール数ではなく全モジュール数に比例した
// 実行時間を要する．
ymuint
MvMgr::topmodule_list(list<const MvModule*>& module_list) const
{
  module_list.clear();
  for (vector<MvModule*>::const_iterator p = mModuleArray.begin();
       p != mModuleArray.end(); ++ p) {
    MvModule* module = *p;
    if ( module && module->parent() == NULL ) {
      module_list.push_back(module);
    }
  }
  return module_list.size();
}

// @brief モジュール番号の最大値+1を得る．
ymuint
MvMgr::max_module_id() const
{
  return mModuleArray.size();
}

// @brief モジュールIDをキーにしてモジュールにアクセスする．
// @param[in] id モジュールID ( 0 <= id < max_module_id() )
// @return 該当するモジュールを返す．
// @note 該当するモジュールがない場合は NULL を返す．
const MvModule*
MvMgr::module(ymuint id) const
{
  return mModuleArray[id];
}

// @brief モジュールIDをキーにしてモジュールにアクセスする．
// @param[in] id モジュールID ( 0 <= id < max_module_id() )
// @return 該当するモジュールを返す．
// @note 該当するモジュールがない場合は NULL を返す．
MvModule*
MvMgr::_module(ymuint id)
{
  return mModuleArray[id];
}

// @brief ノードの ID番号の最大値 + 1 を返す．
ymuint
MvMgr::max_node_id() const
{
  return mNodeArray.size();
}

// @brief ノードを得る．
// @param[in] id ID番号 ( 0 <= id < max_node_id() )
// @note NULL が還されることもある．
const MvNode*
MvMgr::node(ymuint id) const
{
  return mNodeArray[id];
}

// @brief ノードを得る．
// @param[in] id ID番号 ( 0 <= id < max_node_id() )
// @note NULL が還されることもある．
MvNode*
MvMgr::_node(ymuint id)
{
  return mNodeArray[id];
}

// @brief ネットの ID番号の最大値 + 1 を返す．
ymuint
MvMgr::max_net_id() const
{
  return mNetArray.size();
}

// @brief ネットを得る．
// @param[in] id ID番号 ( 0 <= id < max_net_id() )
const MvNet*
MvMgr::net(ymuint id) const
{
  return mNetArray[id];
}

// @brief ネットを得る．
// @param[in] id ID番号 ( 0 <= id < max_net_id() )
MvNet*
MvMgr::_net(ymuint id)
{
  return mNetArray[id];
}

// @brief モジュールを生成する．
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ni 入力ノード数
// @param[in] no 出力ノード数
// @param[in] nio 入出力ノード数
// @return 生成したモジュールを返す．
// @note 入出力ノードのビット幅は1で初期化される．
MvModule*
MvMgr::new_module(const char* name,
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
MvModule*
MvMgr::new_module(const char* name,
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
  MvModule* module = new MvModule(name, np, ni, no, nio);
  module->mId = id;
  while ( mModuleArray.size() <= id ) {
    mModuleArray.push_back(NULL);
  }
  mModuleArray[id] = module;

  for (ymuint i = 0; i < ni; ++ i) {
    MvNode* node = new MvInput(module, ibitwidth_array[i]);
    reg_node(node);
    module->mInputArray[i] = node;
  }
  for (ymuint i = 0; i < no; ++ i) {
    MvNode* node = new MvOutput(module, obitwidth_array[i]);
    reg_node(node);
    module->mOutputArray[i] = node;
  }
  for (ymuint i = 0; i < nio; ++ i) {
    MvNode* node = new MvInout(module, iobitwidth_array[i]);
    reg_node(node);
    module->mInoutArray[i] = node;
  }
  return module;
}

// @brief モジュールを削除する．
// @param[in] module 対象のモジュール
// @note モジュールインスタンスとして使われているモジュールは削除できない．
void
MvMgr::delete_module(MvModule* module)
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
MvMgr::init_port(MvModule* module,
		 ymuint pos,
		 const char* name,
		 ymuint port_ref_num)
{
  module->mPortArray[pos] = new MvPort(name, port_ref_num);
}

// @brief ポート参照式の内容を設定する(単純な形式)．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] port_ref_pos ポート参照式の番号
// @param[in] node 対応する入出力ノード
void
MvMgr::set_port_ref(MvModule* module,
		    ymuint pos,
		    ymuint port_ref_pos,
		    MvNode* node)
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
MvMgr::set_port_ref(MvModule* module,
		    ymuint pos,
		    ymuint port_ref_pos,
		    MvNode* node,
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
MvMgr::set_port_ref(MvModule* module,
		    ymuint pos,
		    ymuint port_ref_pos,
		    MvNode* node,
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
MvMgr::delete_node(MvNode* node)
{
  if ( node->type() == MvNode::kInput || node->type() == MvNode::kOutput ) {
    return;
  }
  for (ymuint i = 0; i < node->input_num(); ++ i) {
    if ( node->input(i)->net() ) {
      return;
    }
  }
  for (ymuint i = 0; i < node->output_num(); ++ i) {
    if ( !node->output(i)->net_list().empty() ) {
      return;
    }
  }
  unreg_node(node);
  delete node;
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
MvNet*
MvMgr::connect(MvNode* src_node,
	       ymuint src_pin_pos,
	       MvNode* dst_node,
	       ymuint dst_pin_pos)
{
  if ( src_node->parent() != dst_node->parent() ) {
    cerr << "connecting between differnt module" << endl;
    abort();
    return NULL;
  }
  MvOutputPin* src_pin = src_node->_output(src_pin_pos);
  MvInputPin* dst_pin = dst_node->_input(dst_pin_pos);
  if ( src_pin->bit_width() != dst_pin->bit_width() ) {
    cout << "src_pin->bit_width() = " << src_pin->bit_width()
	 << ", dst_pin->bit_width() = " << dst_pin->bit_width() << endl;
    cerr << "bit_width mismatch" << endl;
    abort();
    return NULL;
  }
  int tmp = mNetItvlMgr.avail_num();
  if ( tmp == -1 ) {
    // IDが枯渇？
    cerr << "ID exhausted" << endl;
    abort();
    return NULL;
  }
  mNetItvlMgr.erase(tmp);
  ymuint id = tmp;
  
  MvNet* net = new MvNet(src_pin, dst_pin);
  src_pin->mNetList.push_back(net);
  dst_pin->mNet = net;
  net->mId = id;
  while ( mNetArray.size() <= id ) {
    mNetArray.push_back(NULL);
  }
  mNetArray[id] = net;

  return net;
}

// @brief 接続を取り除く
// @param[in] net 接続を表すネット
void
MvMgr::disconnect(MvNet* net)
{
  MvOutputPin* opin = net->mSrc;
  opin->mNetList.erase(net);
  MvInputPin* ipin = net->mDst;
  ipin->mNet = NULL;

  mNetItvlMgr.add(net->mId);
  mNetArray[net->mId] = NULL;
  delete net;
}

// @brief 冗長な through ノードを取り除く
void
MvMgr::sweep()
{
  vector<MvNode*> node_list;
  ymuint n = max_node_id();
  node_list.reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    MvNode* node = _node(i);
    if ( node == NULL ) continue;
    node_list.push_back(node);
  }
  for (vector<MvNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    MvNode* node = *p;
    if ( node->type() != MvNode::kThrough ) continue;

    const MvInputPin* ipin = node->input(0);
    MvNet* src_net = ipin->net();
    if ( src_net == NULL ) continue;
    
    const MvOutputPin* src_pin = src_net->src_pin();
    MvNode* src_node = src_pin->node();
    ymuint src_pos = src_pin->pos();
    disconnect(src_net);
    const MvOutputPin* opin = node->output(0);
    const MvNetList& fo_list = opin->net_list();
    vector<MvNet*> tmp_list;
    tmp_list.reserve(fo_list.size());
    for (MvNetList::const_iterator q = fo_list.begin();
	 q != fo_list.end(); ++ q) {
      MvNet* net = *q;
      tmp_list.push_back(net);
    }
    for (vector<MvNet*>::iterator q = tmp_list.begin();
	 q != tmp_list.end(); ++ q) {
      MvNet* net = *q;
      MvInputPin* dst_pin = net->mDst;
      MvNode* dst_node = dst_pin->node();
      ymuint dst_pos = dst_pin->pos();
      disconnect(net);
      connect(src_node, src_pos, dst_node, dst_pos);
    }
    delete_node(node);
  }
}

// @brief ノードを登録する．
// @param[in] node 対象のノード
void
MvMgr::reg_node(MvNode* node)
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

  if ( node->type() != MvNode::kInput && node->type() != MvNode::kOutput ) {
    MvModule* module = node->mParent;
    list<MvNode*>& nodelist = module->mNodeList;
    nodelist.push_back(node);
    node->mSelfRef = nodelist.end();
    -- node->mSelfRef;
    assert_cond( *node->mSelfRef == node , __FILE__, __LINE__);
  }
}

// @brief ノードの登録を削除する．
// @param[in] node 対象のノード
void
MvMgr::unreg_node(MvNode* node)
{
  mNodeItvlMgr.add(node->id());
  mNodeArray[node->id()] = NULL;
  if ( node->type() != MvNode::kInput && node->type() != MvNode::kOutput ) {
    MvModule* module = node->mParent;
    module->mNodeList.erase(node->mSelfRef);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MvNet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src 入力元のピン
// @param[in] dst 出力先のピン
MvNet::MvNet(MvOutputPin* src,
	     MvInputPin* dst) :
  mId(0),
  mSrc(src),
  mDst(dst)
{
}

// @brief デストラクタ
MvNet::~MvNet()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvPin::MvPin() :
  mNode(NULL),
  mBitWidth(0),
  mPos(0)
{
}

// @brief デストラクタ
MvPin::~MvPin()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvInputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvInputPin::MvInputPin() :
  mNet(NULL)
{
}

// @brief デストラクタ
MvInputPin::~MvInputPin()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvOutputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvOutputPin::MvOutputPin()
{
}

// @brief デストラクタ
MvOutputPin::~MvOutputPin()
{
}

END_NAMESPACE_YM_MVN
