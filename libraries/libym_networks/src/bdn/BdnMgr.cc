
/// @file BdnMgr.cc
/// @brief BdnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/BdnMgr.h"
#include "BdnMgrImpl.h"
#include "networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

///////////////////////////////////////////////////////////////////////
// クラス BdnMgr
///////////////////////////////////////////////////////////////////////

// コンストラクタ
BdnMgr::BdnMgr() :
  mImpl(new BdnMgrImpl)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
BdnMgr::BdnMgr(const BdnMgr& src) :
  mImpl(new BdnMgrImpl)
{
  mImpl->copy(src);
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身への参照を返す．
const BdnMgr&
BdnMgr::operator=(const BdnMgr& src)
{
  if ( &src != this ) {
    mImpl->clear();
    mImpl->copy(src);
  }
  return *this;
}

// デストラクタ
BdnMgr::~BdnMgr()
{
  delete mImpl;
}

// @brief 名前の取得
string
BdnMgr::name() const
{
  return mImpl->name();
}

// @brief ポート数の取得
ymuint
BdnMgr::port_num() const
{
  return mImpl->port_num();
}

// @brief ポートの取得
// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
const BdnPort*
BdnMgr::port(ymuint pos) const
{
  return mImpl->port(pos);
}

// @brief D-FFのIDの最大値 + 1 の取得
ymuint
BdnMgr::max_dff_id() const
{
  return mImpl->max_dff_id();
}

// @brief ID番号から D-FF を得る．
// @param[in] id ID番号 ( 0 <= id < max_dff_id() )
// @note 該当するD-FFが無い場合には NULL を返す．
const BdnDff*
BdnMgr::dff(ymuint id) const
{
  return mImpl->dff(id);
}

// @brief D-FF 数の取得
ymuint
BdnMgr::dff_num() const
{
  return mImpl->dff_num();
}

// @brief D-FF のリストの取得
const BdnDffList&
BdnMgr::dff_list() const
{
  return mImpl->dff_list();
}

// @brief ラッチのIDの最大値 + 1 の取得
ymuint
BdnMgr::max_latch_id() const
{
  return mImpl->max_latch_id();
}

// @brief ID番号からラッチを得る．
// @param[in] id ID番号 ( 0 <= id < max_latch_id() )
// @note 該当するラッチが無い場合には NULL を返す．
const BdnLatch*
BdnMgr::latch(ymuint id) const
{
  return mImpl->latch(id);
}

// @brief ラッチ数の取得
ymuint
BdnMgr::latch_num() const
{
  return mImpl->latch_num();
}

// @brief ラッチのリストの取得
const BdnLatchList&
BdnMgr::latch_list() const
{
  return mImpl->latch_list();
}

// @brief ノードIDの最大値 + 1 の取得
// @return ノードIDの最大値 + 1 を返す．
ymuint
BdnMgr::max_node_id() const
{
  return mImpl->max_node_id();
}

// @brief ID 番号をキーにノードを取り出す．
const BdnNode*
BdnMgr::node(ymuint id) const
{
  return mImpl->node(id);
}

// @brief 入力ノード数の取得
// @return 入力ノード数を返す．
ymuint
BdnMgr::input_num() const
{
  return mImpl->input_num();
}

// @brief 入力ノードのリストを得る．
const BdnNodeList&
BdnMgr::input_list() const
{
  return mImpl->input_list();
}

// @brief 出力ノード数を得る．
ymuint
BdnMgr::output_num() const
{
  return mImpl->output_num();
}

// @brief 出力ノードのリストを得る．
const BdnNodeList&
BdnMgr::output_list() const
{
  return mImpl->output_list();
}

// @brief 論理ノード数を得る．
ymuint
BdnMgr::lnode_num() const
{
  return mImpl->lnode_num();
}

// @brief 論理ノードのリストを得る．
const BdnNodeList&
BdnMgr::lnode_list() const
{
  return mImpl->lnode_list();
}

// @brief 指定されたANDタイプの論理ノードが存在するか調べる．
// @param[in] inode1_handle 1番目の入力ノード+極性
// @param[in] inode2_handle 2番目の入力ノード+極性
// @param[out] onode_handle 該当のノード+極性
// @return 見つかったら true を返す．
bool
BdnMgr::find_and(BdnNodeHandle inode1_handle,
		 BdnNodeHandle inode2_handle,
		 BdnNodeHandle& onode_handle)
{
  return mImpl->find_logic(false, inode1_handle, inode2_handle,
			   onode_handle);
}

// @brief 指定されたXORタイプの論理ノードが存在するか調べる．
// @param[in] inode1_handle 1番目の入力ノード+極性
// @param[in] inode2_handle 2番目の入力ノード+極性
// @param[out] onode_handle 該当のノード+極性
// @return 見つかったら true を返す．
bool
BdnMgr::find_xor(BdnNodeHandle inode1_handle,
		 BdnNodeHandle inode2_handle,
		 BdnNodeHandle& onode_handle)
{
  return mImpl->find_logic(true, inode1_handle, inode2_handle,
			   onode_handle);
}

// @brief ソートされたノードのリストを得る．
void
BdnMgr::sort(vector<const BdnNode*>& node_list) const
{
  mImpl->sort(node_list);
}

// @brief ソートされた論理ノードのリストを得る．
// @param[out] node_list ノードのリストの格納先
// @note 入力ノードと出力ノード，ラッチノードは含まない．
void
BdnMgr::_sort(vector<BdnNode*>& node_list) const
{
  mImpl->_sort(node_list);
}

// @brief 逆順でソートされたノードのリストを得る．
void
BdnMgr::rsort(vector<const BdnNode*>& node_list) const
{
  mImpl->rsort(node_list);
}

// @brief 段数を求める．
ymuint
BdnMgr::level() const
{
  return mImpl->level();
}

// 空にする．
void
BdnMgr::clear()
{
  mImpl->clear();
}

// @brief 名前を設定する．
// @param[in] name 新しい名前
void
BdnMgr::set_name(const string& name)
{
  mImpl->set_name(name);
}

// @brief どこにもファンアウトしていないノードを削除する．
void
BdnMgr::clean_up()
{
  mImpl->clean_up();
}

// @brief 入力ポートを作る．
// @param[in] name 名前
// @param[in] bit_width ビット幅
BdnPort*
BdnMgr::new_input_port(const string& name,
		       ymuint bit_width)
{
  return mImpl->new_port(name, vector<ymuint>(bit_width, 1U));
}

// @brief 出力ポートを作る．
// @param[in] name 名前
// @param[in] bit_width ビット幅
BdnPort*
BdnMgr::new_output_port(const string& name,
			ymuint bit_width)
{
  return mImpl->new_port(name, vector<ymuint>(bit_width, 2U));
}

// @brief ポートを作る．
// @param[in] name 名前
// @param[in] iovect ビットごとの方向を指定する配列
// @note iovect の要素の値の意味は以下の通り
// - 0 : なし
// - 1 : 入力のみ
// - 2 : 出力のみ
// - 3 : 入力と出力
BdnPort*
BdnMgr::new_port(const string& name,
		 const vector<ymuint>& iovect)
{
  return mImpl->new_port(name, iovect);
}

// @brief D-FF を作る．
// @param[in] name 名前
// @return 生成されたD-FFを返す．
BdnDff*
BdnMgr::new_dff(const string& name)
{
  return mImpl->new_dff(name);
}

// @brief ラッチを作る．
// @param[in] name 名前
// @return 生成されたラッチを返す．
BdnLatch*
BdnMgr::new_latch(const string& name)
{
  return mImpl->new_latch(name);
}

// @brief 出力ノードの内容を変更する
// @param[in] 変更対象の出力ノード
// @param[in] inode 入力のノード
void
BdnMgr::change_output_fanin(BdnNode* node,
			    BdnNodeHandle inode_handle)
{
  mImpl->change_output_fanin(node, inode_handle);
}

// @brief AND ノードを作る．
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_and(BdnNodeHandle inode1_handle,
		BdnNodeHandle inode2_handle)
{
  return mImpl->set_and(NULL, inode1_handle, inode2_handle);
}

// @brief AND ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_and(const vector<BdnNodeHandle>& inode_handle_list)
{
  return mImpl->make_and_tree(NULL, inode_handle_list);
}

// @brief NAND ノードを作る．
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note fcode の出力極性を正規化する．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_nand(BdnNodeHandle inode1_handle,
		 BdnNodeHandle inode2_handle)
{
  return ~mImpl->set_and(NULL, inode1_handle, inode2_handle);
}

// @brief NAND ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_nand(const vector<BdnNodeHandle>& inode_handle_list)
{
  return ~mImpl->make_and_tree(NULL, inode_handle_list);
}

// @brief OR ノードを作る．
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_or(BdnNodeHandle inode1_handle,
	       BdnNodeHandle inode2_handle)
{
  return ~mImpl->set_and(NULL, ~inode1_handle, ~inode2_handle);
}

// @brief OR ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_or(const vector<BdnNodeHandle>& inode_handle_list)
{
  return mImpl->make_or_tree(NULL, inode_handle_list);
}

// @brief NOR ノードを作る．
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_nor(BdnNodeHandle inode1_handle,
		BdnNodeHandle inode2_handle)
{
  return mImpl->set_and(NULL, ~inode1_handle, ~inode2_handle);
}

// @brief NOR ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_nor(const vector<BdnNodeHandle>& inode_handle_list)
{
  return ~mImpl->make_or_tree(NULL, inode_handle_list);
}

// @brief XOR ノードを作る．
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_xor(BdnNodeHandle inode1_handle,
		BdnNodeHandle inode2_handle)
{
  return mImpl->set_xor(NULL, inode1_handle, inode2_handle);
}

// @brief XOR ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_xor(const vector<BdnNodeHandle>& inode_handle_list)
{
  return mImpl->make_xor_tree(NULL, inode_handle_list);
}

// @brief XNOR ノードを作る．
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_xnor(BdnNodeHandle inode1_handle,
		 BdnNodeHandle inode2_handle)
{
  return ~mImpl->set_xor(NULL, inode1_handle, inode2_handle);
}

// @brief XNOR ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_xnor(const vector<BdnNodeHandle>& inode_handle_list)
{
  return ~mImpl->make_xor_tree(NULL, inode_handle_list);
}

// @brief AND タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
void
BdnMgr::change_and(BdnNode* node,
		   BdnNodeHandle inode1_handle,
		   BdnNodeHandle inode2_handle)
{
  BdnNodeHandle new_handle = mImpl->set_and(node, inode1_handle, inode2_handle);
  mImpl->change_logic(node, new_handle);
}

// @brief AND タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_and(BdnNode* node,
		   const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = mImpl->make_and_tree(node, inode_handle_list);
  mImpl->change_logic(node, new_handle);
}

// @brief NAND タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
void
BdnMgr::change_nand(BdnNode* node,
		    BdnNodeHandle inode1_handle,
		    BdnNodeHandle inode2_handle)
{
  BdnNodeHandle new_handle = mImpl->set_and(node, inode1_handle, inode2_handle);
  mImpl->change_logic(node, ~new_handle);
}

// @brief NAND タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_nand(BdnNode* node,
		    const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = mImpl->make_and_tree(node, inode_handle_list);
  mImpl->change_logic(node, ~new_handle);
}

// @brief OR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
void
BdnMgr::change_or(BdnNode* node,
		  BdnNodeHandle inode1_handle,
		  BdnNodeHandle inode2_handle)
{
  BdnNodeHandle new_handle = mImpl->set_and(node, ~inode1_handle, ~inode2_handle);
  mImpl->change_logic(node, ~new_handle);
}

// @brief OR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_or(BdnNode* node,
		  const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = mImpl->make_or_tree(node, inode_handle_list);
  mImpl->change_logic(node, new_handle);
}

// @brief NOR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
void
BdnMgr::change_nor(BdnNode* node,
		   BdnNodeHandle inode1_handle,
		   BdnNodeHandle inode2_handle)
{
  BdnNodeHandle new_handle = mImpl->set_and(node, ~inode1_handle, ~inode2_handle);
  mImpl->change_logic(node, new_handle);
}

// @brief NOR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_nor(BdnNode* node,
		   const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = mImpl->make_or_tree(node, inode_handle_list);
  mImpl->change_logic(node, ~new_handle);
}

// @brief XOR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
void
BdnMgr::change_xor(BdnNode* node,
		   BdnNodeHandle inode1_handle,
		   BdnNodeHandle inode2_handle)
{
  BdnNodeHandle new_handle = mImpl->set_xor(node, inode1_handle, inode2_handle);
  mImpl->change_logic(node, new_handle);
}

// @brief XOR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_xor(BdnNode* node,
		   const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = mImpl->make_xor_tree(node, inode_handle_list);
  mImpl->change_logic(node, new_handle);
}

// @brief XNOR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
void
BdnMgr::change_xnor(BdnNode* node,
		    BdnNodeHandle inode1_handle,
		    BdnNodeHandle inode2_handle)
{
  BdnNodeHandle new_handle = mImpl->set_xor(node, inode1_handle, inode2_handle);
  mImpl->change_logic(node, ~new_handle);
}

// @brief XNOR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_xnor(BdnNode* node,
		    const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = mImpl->make_xor_tree(node, inode_handle_list);
  mImpl->change_logic(node, ~new_handle);
}

END_NAMESPACE_YM_NETWORKS_BDN
