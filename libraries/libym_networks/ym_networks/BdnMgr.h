#ifndef YM_BDN_BDNMGR_H
#define YM_BDN_BDNMGR_H

/// @file ym_networks/BdnMgr.h
/// @brief BdnMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdnMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "ym_networks/bdn_nsdef.h"
#include "ym_networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnMgr BdnMgr.h "ym_networks/BdnMgr.h"
/// @brief 2入力ノードのネットワークを表すクラス
//////////////////////////////////////////////////////////////////////
class BdnMgr
{
public:

  /// @brief コンストラクタ
  /// @note 空のネットワークに初期化される．
  BdnMgr();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  BdnMgr(const BdnMgr& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への参照を返す．
  const BdnMgr&
  operator=(const BdnMgr& src);

  /// @brief デストラクタ
  ~BdnMgr();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ネットワーク全体の情報の取得
  /// @{

  /// @brief 名前の取得
  string
  name() const;

  /// @brief ポート数の取得
  ymuint
  port_num() const;

  /// @brief ポートの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
  const BdnPort*
  port(ymuint pos) const;

  /// @brief D-FF 数の取得
  ymuint
  dff_num() const;

  /// @brief D-FF のリストの取得
  const BdnDffList&
  dff_list() const;

  /// @brief ラッチ数の取得
  ymuint
  latch_num() const;

  /// @brief ラッチのリストの取得
  const BdnLatchList&
  latch_list() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノード関連の情報の取得
  /// @{

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  ymuint
  max_node_id() const;

  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  ymuint
  input_num() const;

  /// @brief 入力ノードのリストを得る．
  const BdnNodeList&
  input_list() const;

  /// @brief 出力ノード数を得る．
  ymuint
  output_num() const;

  /// @brief 出力ノードのリストを得る．
  const BdnNodeList&
  output_list() const;

  /// @brief 論理ノード数を得る．
  ymuint
  lnode_num() const;

  /// @brief 論理ノードのリストを得る．
  const BdnNodeList&
  lnode_list() const;

  /// @brief ソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  void
  sort(vector<BdnNode*>& node_list) const;

  /// @brief 逆順でソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  void
  rsort(vector<BdnNode*>& node_list) const;

  /// @brief 最大段数を求める．
  /// @note 段数とは入力ノードから出力ノードへ至る経路中の論理ノードの数
  ymuint
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ネットワーク全体の情報の設定を行う関数
  /// @{

  /// @brief 名前を設定する．
  /// @param[in] name 新しい名前
  void
  set_name(const string& name);

  /// @brief 空にする．
  void
  clear();

  /// @brief どこにもファンアウトしていないノードを削除する．
  void
  clean_up();

  /// @brief ポートを作る．
  /// @param[in] name 名前
  /// @param[in] bit_width ビット幅
  BdnPort*
  new_port(const string& name,
	   ymuint bit_width);

  /// @brief D-FF を作る．
  /// @param[in] name 名前
  /// @return 生成されたD-FFを返す．
  BdnDff*
  new_dff(const string& name = string());

  /// @brief ラッチを作る．
  /// @param[in] name 名前
  /// @return 生成されたラッチを返す．
  BdnLatch*
  new_latch(const string& name = string());

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成および変更を行う関数
  /// @{

  /// @brief 外部入力を作る．
  /// @param[in] port ポート
  /// @param[in] bitpos ビット位置
  /// @return 作成したノードを返す．
  /// @note エラー条件は以下の通り
  ///  - bitpos が port のビット幅を越えている．
  ///  - port の bitpos にすでにノードがある．
  BdnNode*
  new_port_input(BdnPort* port,
		 ymuint bitpos);

  /// @brief 外部入力とポートを同時に作る．
  /// @param[in] port_name ポート名
  /// @return 作成したノードを返す．
  /// @note ポートのビット幅は1ビットとなる．
  BdnNode*
  new_port_input(const string& port_name);

  /// @brief 外部出力ノードを作る．
  /// @param[in] port ポート
  /// @param[in] bitpos ビット位置
  /// @return 作成したノードを返す．
  /// @note エラー条件は以下の通り
  ///  - bitpos が port のビット幅を越えている．
  ///  - port の bitpos にすでにノードがある．
  BdnNode*
  new_port_output(BdnPort* port,
		  ymuint bitpos);

  /// @brief 外部出力ノードとポートを同時にを作る．
  /// @param[in] port_name ポート名
  /// @return 作成したノードを返す．
  /// @note ポートのビット幅は1ビットとなる．
  BdnNode*
  new_port_output(const string& port_name);

  /// @brief 出力ノードのファンインを変更する
  /// @param[in] node 変更対象の出力ノード
  /// @param[in] inode_handle ファンインのノード+極性
  void
  change_output_fanin(BdnNode* node,
		      BdnNodeHandle inode_handle);

  /// @brief AND ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_and(BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

  /// @brief AND ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_and(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief NAND ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nand(BdnNodeHandle inode1_handle,
	   BdnNodeHandle inode2_handle);

  /// @brief NAND ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nand(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief OR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_or(BdnNodeHandle inode1_handle,
	 BdnNodeHandle inode2_handle);

  /// @brief OR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_or(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief NOR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nor(BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

  /// @brief NOR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nor(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief XOR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xor(BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

  /// @brief XOR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xor(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief XNOR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xnor(BdnNodeHandle inode1_handle,
	   BdnNodeHandle inode2_handle);

  /// @brief XNOR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xnor(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief AND タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_and(BdnNode* node,
	     BdnNodeHandle inode1_handle,
	     BdnNodeHandle inode2_handle);

  /// @brief AND タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_and(BdnNode* node,
	     const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief NAND タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_nand(BdnNode* node,
	     BdnNodeHandle inode1_handle,
	     BdnNodeHandle inode2_handle);

  /// @brief NAND タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_nand(BdnNode* node,
	      const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief OR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_or(BdnNode* node,
	    BdnNodeHandle inode1_handle,
	    BdnNodeHandle inode2_handle);

  /// @brief OR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_or(BdnNode* node,
	    const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief NOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_nor(BdnNode* node,
	     BdnNodeHandle inode1_handle,
	     BdnNodeHandle inode2_handle);

  /// @brief NOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_nor(BdnNode* node,
	     const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief XOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_xor(BdnNode* node,
	     BdnNodeHandle inode1_handle,
	     BdnNodeHandle inode2_handle);

  /// @brief XOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_xor(BdnNode* node,
	     const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief XNOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_xnor(BdnNode* node,
	      BdnNodeHandle inode1_handle,
	      BdnNodeHandle inode2_handle);

  /// @brief XNOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_xnor(BdnNode* node,
	      const vector<BdnNodeHandle>& inode_handle_list);

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND のバランス木を作る．
  /// @param[in] node 根のノード
  /// @param[in] node_list 入力のノードのリスト
  /// @note node が NULL の場合，新しいノードを確保する．
  BdnNodeHandle
  make_and_tree(BdnNode* node,
		const vector<BdnNodeHandle>& node_list);

  /// @brief OR のバランス木を作る．
  /// @param[in] node 根のノード
  /// @param[in] node_list 入力のノードのリスト
  /// @note node が NULL の場合，新しいノードを確保する．
  BdnNodeHandle
  make_or_tree(BdnNode* node,
	       const vector<BdnNodeHandle>& node_list);

  /// @brief XOR のバランス木を作る．
  /// @param[in] node 根のノード
  /// @param[in] node_list 入力のノードのリスト
  /// @note node が NULL の場合，新しいノードを確保する．
  BdnNodeHandle
  make_xor_tree(BdnNode* node,
		const vector<BdnNodeHandle>& node_list);

  /// @brief バランス木を作る．
  /// @param[in] node 根のノード
  /// @param[in] fcode 機能コード
  /// @param[in] start 開始位置
  /// @param[in] num 要素数
  /// @param[in] node_list 入力のノードのリスト
  /// @note node が NULL の場合，新しいノードを確保する．
  /// @note fcode の各ビットの意味は以下のとおり，
  ///  - 0bit: ファンイン0の反転属性
  ///  - 1bit: ファンイン1の反転属性
  ///  - 2bit: XOR/AND フラグ( 0: AND, 1: XOR)
  ///  - 3bit: 出力の反転属性
  BdnNodeHandle
  make_tree(BdnNode* node,
	    ymuint fcode,
	    ymuint start,
	    ymuint num,
	    const vector<BdnNodeHandle>& node_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装クラス
  BdnMgrImpl* mImpl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 外部入力とポートを同時に作る．
// @param[in] port_name ポート名
// @return 作成したノードを返す．
// @note ポートのビット幅は1ビットとなる．
inline
BdnNode*
BdnMgr::new_port_input(const string& port_name)
{
  BdnPort* port = new_port(port_name, 1);
  return new_port_input(port, 0);
}

// @brief 外部出力ノードとポートを同時にを作る．
// @param[in] port_name ポート名
// @return 作成したノードを返す．
// @note ポートのビット幅は1ビットとなる．
inline
BdnNode*
BdnMgr::new_port_output(const string& port_name)
{
  BdnPort* port = new_port(port_name, 1);
  return new_port_output(port, 0);
}

// @brief AND ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdnMgr::new_and(const vector<BdnNodeHandle>& inode_handle_list)
{
  return make_and_tree(NULL, inode_handle_list);
}

// @brief NAND ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdnMgr::new_nand(const vector<BdnNodeHandle>& inode_handle_list)
{
  return ~make_and_tree(NULL, inode_handle_list);
}

// @brief OR ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdnMgr::new_or(const vector<BdnNodeHandle>& inode_handle_list)
{
  return make_or_tree(NULL, inode_handle_list);
}

// @brief NOR ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdnMgr::new_nor(const vector<BdnNodeHandle>& inode_handle_list)
{
  return ~make_or_tree(NULL, inode_handle_list);
}

// @brief XOR ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdnMgr::new_xor(const vector<BdnNodeHandle>& inode_handle_list)
{
  return make_xor_tree(NULL, inode_handle_list);
}

// @brief XNOR ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdnMgr::new_xnor(const vector<BdnNodeHandle>& inode_handle_list)
{
  return ~make_xor_tree(NULL, inode_handle_list);
}

END_NAMESPACE_YM_BDN

#endif // YM_BDN_BDNMGR_H
