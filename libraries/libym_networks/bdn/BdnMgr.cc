
/// @file libym_networks/BdnMgr.cc
/// @brief BdnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnMgr.h"
#include "BdnMgrImpl.h"
#include "ym_networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

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

// @brief ソートされたノードのリストを得る．
void
BdnMgr::sort(vector<BdnNode*>& node_list) const
{
  mImpl->sort(node_list);
}

// @brief 逆順でソートされたノードのリストを得る．
void
BdnMgr::rsort(vector<BdnNode*>& node_list) const
{
  mImpl->rsort(node_list);
}

// @brief 段数を求める．
ymuint
BdnMgr::level() const
{
  return mImpl->level();
}

// @brief 名前を設定する．
// @param[in] name 新しい名前
void
BdnMgr::set_name(const string& name)
{
  mImpl->set_name(name);
}

// 空にする．
void
BdnMgr::clear()
{
  mImpl->clear();
}

// @brief ポートを作る．
BdnPort*
BdnMgr::new_port(const string& name,
		 ymuint bit_width)
{
  return mImpl->new_port(name, bit_width);
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

// @brief 外部入力を作る．
// @param[in] port ポート
// @param[in] bitpos ビット位置
// @return 作成したノードを返す．
// @note エラー条件は以下の通り
//  - bitpos が port のビット幅を越えている．
//  - port の bitpos にすでにノードがある．
BdnNode*
BdnMgr::new_port_input(BdnPort* port,
		       ymuint bitpos)
{
  return mImpl->new_port_input(port, bitpos);
}

// @brief 外部出力ノードを作る．
// @param[in] port ポート
// @param[in] bitpos ビット位置
// @return 作成したノードを返す．
// @note エラー条件は以下の通り
//  - bitpos が port のビット幅を越えている．
//  - port の bitpos にすでにノードがある．
BdnNode*
BdnMgr::new_port_output(BdnPort* port,
			ymuint bitpos)
{
  return mImpl->new_port_output(port, bitpos);
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


BEGIN_NONAMESPACE

// new_logic 用の定数
const ymuint I0_BIT  = 1U;
const ymuint I1_BIT  = 2U;
const ymuint AND_BIT = 0U;
const ymuint XOR_BIT = 4U;
const ymuint O_BIT   = 8U;

const ymuint AND_00  = AND_BIT;
const ymuint AND_01  = AND_BIT | I0_BIT;
const ymuint AND_10  = AND_BIT | I1_BIT;
const ymuint AND_11  = AND_BIT | I0_BIT | I1_BIT;

const ymuint NAND_00 = AND_00 | O_BIT;
const ymuint NAND_01 = AND_01 | O_BIT;
const ymuint NAND_10 = AND_10 | O_BIT;
const ymuint NAND_11 = AND_11 | O_BIT;

const ymuint OR_00   = NAND_11;
const ymuint OR_01   = NAND_10;
const ymuint OR_10   = NAND_01;
const ymuint OR_11   = NAND_00;

const ymuint NOR_00  = OR_00 | O_BIT;
const ymuint NOR_01  = OR_01 | O_BIT;
const ymuint NOR_10  = OR_10 | O_BIT;
const ymuint NOR_11  = OR_11 | O_BIT;

const ymuint XOR     = XOR_BIT;

const ymuint XNOR    = XOR_BIT | O_BIT;

END_NONAMESPACE

// @brief AND ノードを作る．
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
BdnNodeHandle
BdnMgr::new_and(BdnNodeHandle inode1_handle,
		BdnNodeHandle inode2_handle)
{
  return mImpl->set_logic(NULL, AND_00, inode1_handle, inode2_handle);
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
  return mImpl->set_logic(NULL, NAND_00, inode1_handle, inode2_handle);
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
  return mImpl->set_logic(NULL, OR_00, inode1_handle, inode2_handle);
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
  return mImpl->set_logic(NULL, NOR_00, inode1_handle, inode2_handle);
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
  return mImpl->set_logic(NULL, XOR, inode1_handle, inode2_handle);
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
  return mImpl->set_logic(NULL, XNOR, inode1_handle, inode2_handle);
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
  BdnNodeHandle new_handle = mImpl->set_logic(node, AND_00,
					      inode1_handle, inode2_handle);
  mImpl->change_logic(node, new_handle);
}

// @brief AND タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_and(BdnNode* node,
		   const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = make_and_tree(node, inode_handle_list);
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
  BdnNodeHandle new_handle = mImpl->set_logic(node, NAND_00,
					      inode1_handle, inode2_handle);
  mImpl->change_logic(node, new_handle);
}

// @brief NAND タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_nand(BdnNode* node,
		    const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = make_and_tree(node, inode_handle_list);
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
  BdnNodeHandle new_handle = mImpl->set_logic(node, OR_00,
					      inode1_handle, inode2_handle);
  mImpl->change_logic(node, new_handle);
}

// @brief OR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_or(BdnNode* node,
		  const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = make_or_tree(node, inode_handle_list);
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
  BdnNodeHandle new_handle = mImpl->set_logic(node, NOR_00,
					      inode1_handle, inode2_handle);
  mImpl->change_logic(node, new_handle);
}

// @brief NOR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_nor(BdnNode* node,
		   const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = make_or_tree(node, inode_handle_list);
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
  BdnNodeHandle new_handle = mImpl->set_logic(node, XOR,
					      inode1_handle, inode2_handle);
  mImpl->change_logic(node, new_handle);
}

// @brief XOR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_xor(BdnNode* node,
		   const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = make_xor_tree(node, inode_handle_list);
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
  BdnNodeHandle new_handle = mImpl->set_logic(node, XNOR,
					      inode1_handle, inode2_handle);
  mImpl->change_logic(node, new_handle);
}

// @brief XNOR タイプに変更する．
// @param[in] node 対象のノード
// @param[in] inode_handle_list 入力ノード+極性のリスト
void
BdnMgr::change_xnor(BdnNode* node,
		    const vector<BdnNodeHandle>& inode_handle_list)
{
  BdnNodeHandle new_handle = make_xor_tree(node, inode_handle_list);
  mImpl->change_logic(node, ~new_handle);
}

// @brief AND のバランス木を作る．
// @param[in] node 根のノード
// @param[in] node_list 入力のノードのリスト
// @note node が NULL の場合，新しいノードを確保する．
BdnNodeHandle
BdnMgr::make_and_tree(BdnNode* node,
		      const vector<BdnNodeHandle>& node_list)
{
  return make_tree(node, AND_00, 0, node_list.size(), node_list);
}

// @brief OR のバランス木を作る．
// @param[in] node 根のノード
// @param[in] node_list 入力のノードのリスト
// @note node が NULL の場合，新しいノードを確保する．
BdnNodeHandle
BdnMgr::make_or_tree(BdnNode* node,
		     const vector<BdnNodeHandle>& node_list)
{
  return make_tree(node, OR_00, 0, node_list.size(), node_list);
}

// @brief XOR のバランス木を作る．
// @param[in] node 根のノード
// @param[in] node_list 入力のノードのリスト
// @note node が NULL の場合，新しいノードを確保する．
BdnNodeHandle
BdnMgr::make_xor_tree(BdnNode* node,
		      const vector<BdnNodeHandle>& node_list)
{
  return make_tree(node, XOR, 0, node_list.size(), node_list);
}

// @brief バランス木を作る．
// @param[in] node 根のノード
// @param[in] fcode 機能コード
// @param[in] start 開始位置
// @param[in] num 要素数
// @param[in] node_list 入力のノードのリスト
// @note node が NULL の場合，新しいノードを確保する．
BdnNodeHandle
BdnMgr::make_tree(BdnNode* node,
		  ymuint fcode,
		  ymuint start,
		  ymuint num,
		  const vector<BdnNodeHandle>& node_list)
{
  switch ( num ) {
  case 0:
    assert_not_reached(__FILE__, __LINE__);

  case 1:
    return node_list[start];

  case 2:
    return mImpl->set_logic(node, fcode, node_list[start], node_list[start + 1]);

  default:
    break;
  }

  ymuint nh = num / 2;
  BdnNodeHandle l = make_tree(NULL, fcode, start, nh, node_list);
  BdnNodeHandle r = make_tree(NULL, fcode, start + nh, num - nh, node_list);
  return mImpl->set_logic(node, fcode, l, r);
}

END_NAMESPACE_YM_BDN
