#ifndef BDNMGRIMPL_H
#define BDNMGRIMPL_H

/// @file BdnMgrImpl.h
/// @brief BdnMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "networks/BdnMgr.h"
#include "networks/BdnNodeHandle.h"
#include "utils/SimpleAlloc.h"
#include "utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnMgrImpl BdnMgrImpl.h BdnMgrImp.h
/// @brief BdnMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class BdnMgrImpl
{
public:

  /// @brief コンストラクタ
  /// @note 空のネットワークに初期化される．
  BdnMgrImpl();

  /// @brief デストラクタ
  ~BdnMgrImpl();


public:

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

  /// @brief D-FFのIDの最大値 + 1 の取得
  ymuint
  max_dff_id() const;

  /// @brief ID番号から D-FF を得る．
  /// @param[in] id ID番号 ( 0 <= id < max_dff_id() )
  /// @note 該当するD-FFが無い場合には NULL を返す．
  const BdnDff*
  dff(ymuint id) const;

  /// @brief D-FF 数の取得
  ymuint
  dff_num() const;

  /// @brief D-FF のリストの取得
  const BdnDffList&
  dff_list() const;

  /// @brief ラッチのIDの最大値 + 1 の取得
  ymuint
  max_latch_id() const;

  /// @brief ID番号からラッチを得る．
  /// @param[in] id ID番号 ( 0 <= id < max_latch_id() )
  /// @note 該当するラッチが無い場合には NULL を返す．
  const BdnLatch*
  latch(ymuint id) const;

  /// @brief ラッチ数の取得
  ymuint
  latch_num() const;

  /// @brief ラッチのリストの取得
  const BdnLatchList&
  latch_list() const;


public:

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  ymuint
  max_node_id() const;

  /// @brief ID 番号をキーにノードを取り出す．
  const BdnNode*
  node(ymuint id) const;

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

  /// @brief 入出力ノードのリストを得る．
  const BdnNodeList&
  inout_list() const;

  /// @brief 論理ノード数を得る．
  ymuint
  lnode_num() const;

  /// @brief 論理ノードのリストを得る．
  const BdnNodeList&
  lnode_list() const;

  /// @brief 論理ノードを探す．
  /// @param[in] is_xor XOR の時 true にするフラグ(false なら AND)
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 該当のノード+極性を表すハンドルを返す．
  /// @note 見つからなかったらエラーハンドルを返す．
  BdnNodeHandle
  find_logic(bool is_xor,
	     BdnNodeHandle inode1_handle,
	     BdnNodeHandle inode2_handle) const;

  /// @brief ソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  void
  sort(vector<const BdnNode*>& node_list) const;

  /// @brief ソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  void
  _sort(vector<BdnNode*>& node_list);

  /// @brief 逆順でソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  void
  rsort(vector<const BdnNode*>& node_list) const;

  /// @brief 逆順でソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  void
  _rsort(vector<BdnNode*>& node_list);

  /// @brief 最大段数を求める．
  /// @note 段数とは入力ノードから出力ノードへ至る経路中の論理ノードの数
  ymuint
  level() const;


public:

  /// @brief 空にする．
  void
  clear();

  /// @brief 名前を設定する．
  /// @param[in] name 新しい名前
  void
  set_name(const char* name);

  /// @brief どこにもファンアウトしていないノードを削除する．
  void
  clean_up();

  /// @brief ポートを作る．
  /// @param[in] name 名前
  /// @param[in] iovect ビットごとの方向を指定する配列
  /// @note iovect の要素の値の意味は以下の通り
  /// - 0 : なし
  /// - 1 : 入力のみ
  /// - 2 : 出力のみ
  /// - 3 : 入力と出力
  BdnPort*
  new_port(const char* name,
	   const vector<ymuint>& iovect);

  /// @brief D-FF を作る．
  /// @param[in] name 名前
  /// @return 生成されたD-FFを返す．
  BdnDff*
  new_dff(const char* name);

  /// @brief ラッチを作る．
  /// @param[in] name 名前
  /// @return 生成されたラッチを返す．
  BdnLatch*
  new_latch(const char* name);


public:

  /// @brief ANDノードの内容を設定する．
  /// @param[in] node 設定するノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return ノード＋極性を返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  /// @note なければ node に設定する．
  /// @note node が NULL の場合，新しいノードを確保する．
  BdnNodeHandle
  set_and(BdnNode* node,
	  BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

  /// @brief XORノードの内容を設定する．
  /// @param[in] node 設定するノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return ノード＋極性を返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  /// @note なければ node に設定する．
  /// @note node が NULL の場合，新しいノードを確保する．
  BdnNodeHandle
  set_xor(BdnNode* node,
	  BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

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

  /// @brief 論理ノードの内容を変更する．
  /// @param[in] node 変更対象の論理ノード
  /// @param[in] new_handle 設定する新しいハンドル
  /// @note node のファンアウト先の情報を書き換える．
  void
  change_logic(BdnNode* node ,
	       BdnNodeHandle new_handle);

  /// @brief 出力ノードのファンインを変更する
  /// @param[in] node 変更対象の出力ノード
  /// @param[in] inode_handle ファンインのノード+極性
  void
  change_output_fanin(BdnNode* node,
		      BdnNodeHandle inode_handle);

  /// @brief ノード間の接続を変更する．
  /// @param[in] from 接続の入力側のノード
  /// @param[in] to 接続の出力側のノード
  /// @param[in] pos to のファンイン番号
  /// @note to の pos 番目にすでに接続があった場合には自動的に削除される．
  void
  connect(BdnNode* from,
	  BdnNode* to,
	  ymuint pos);

  /// @brief コピーコンストラクタと代入演算子用のサブルーティン
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const BdnMgr& src);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バランス木を作る．
  /// @param[in] node 根のノード
  /// @param[in] is_xor XOR の時 true にするフラグ(false なら AND)
  /// @param[in] iinv 入力の反転属性
  /// @param[in] start 開始位置
  /// @param[in] num 要素数
  /// @param[in] node_list 入力のノードのリスト
  /// @note node が NULL の場合，新しいノードを確保する．
  BdnNodeHandle
  make_tree(BdnNode* node,
	    bool is_xor,
	    bool iinv,
	    ymuint start,
	    ymuint num,
	    const vector<BdnNodeHandle>& node_list);

  /// @brief 論理ノードの内容を設定する．
  /// @param[in] node 設定するノード
  /// @param[in] is_xor XOR の時 true にするフラグ(false なら AND)
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return ノード＋極性を返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  /// @note なければ node に設定する．
  /// @note node が NULL の場合，新しいノードを確保する．
  BdnNodeHandle
  set_logic(BdnNode* node,
	    bool is_xor,
	    BdnNodeHandle inode1_handle,
	    BdnNodeHandle inode2_handle);

  /// @brief 論理ノードの自明な簡単化を行う．
  /// @param[in] is_xor XOR の時 true にするフラグ(false なら AND)
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 該当のノード+極性を表すハンドルを返す．
  /// @note 見つからなかったらエラーハンドルを返す．
  BdnNodeHandle
  is_trivial(bool is_xor,
	     BdnNodeHandle inode1_handle,
	     BdnNodeHandle inode2_handle) const;

  /// @brief 論理ノードに設定する情報の正規化を行う．
  /// @param[in] is_xor XOR の時 true にするフラグ(false なら AND)
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @param[out] inode1 正規化された1番めの入力ノード
  /// @param[out] inode2 正規化された2番目の入力ノード
  /// @param[out] oinv 出力極性
  /// @return 機能コードを返す．
  /// @note 機能コードの各ビットの意味は以下のとおり，
  ///  - 0bit: ファンイン0の反転属性
  ///  - 1bit: ファンイン1の反転属性
  ///  - 2bit: XOR/AND フラグ( 0: AND, 1: XOR)
  ymuint
  cannonicalize(bool is_xor,
		BdnNodeHandle inode1_handle,
		BdnNodeHandle inode2_handle,
		BdnNode*& inode1,
		BdnNode*& inode2,
		bool& oinv) const;

  /// @brief ノードを探索する．
  /// @param[in] fcode 機能コード
  /// @param[in] node1 1番めのノード
  /// @param[in] node2 2番めのノード
  /// @return 該当のノードを返す．
  /// @note 見つからなければ NULL を返す．
  BdnNode*
  find_node(ymuint fcode,
	    const BdnNode* node1,
	    const BdnNode* node2) const;

  /// @brief D-FF を削除する．
  /// @param[in] dff 削除対象の D-FF
  void
  delete_dff(BdnDff* dff);

  /// @brief ラッチを削除する．
  /// @param[in] latch 削除対象のラッチ
  void
  delete_latch(BdnLatch* latch);

  /// @brief ノードを作成する．
  /// @return 作成されたノードを返す．
  BdnNode*
  alloc_node();

  /// @brief node を削除する．
  /// @param[in] node 削除対象のノード
  void
  delete_node(BdnNode* node);

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 確保するサイズ
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // ネットワーク名
  string mName;

  // ID 番号をキーにしたポートの配列
  vector<BdnPort*> mPortArray;

  // D-FF の ID 番号を管理するためのオブジェクト
  ItvlMgr mDffItvlMgr;

  // ID 番号をキーにした DFF の配列
  vector<BdnDff*> mDffArray;

  // D-FFノードのリスト
  BdnDffList mDffList;

  // ラッチの ID番号を管理するためのオブジェクト
  ItvlMgr mLatchItvlMgr;

  // ID番号をキーにしたラッチの配列
  vector<BdnLatch*> mLatchArray;

  // ラッチのリスト
  BdnLatchList mLatchList;

  // ノードの ID 番号を管理するためのオブジェクト
  ItvlMgr mNodeItvlMgr;

  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<BdnNode*> mNodeArray;

  // 入力ノードのリスト
  BdnNodeList mInputList;

  // 出力ノードのリスト
  BdnNodeList mOutputList;

  // 論理ノードのリスト
  BdnNodeList mLnodeList;

  // 論理ノードのハッシュ表
  BdnNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

  // 最大レベル (最下位ビットは valid フラグ)
  mutable
  ymuint32 mLevel;

private:
  //////////////////////////////////////////////////////////////////////
  // 機能コード用の定数
  //////////////////////////////////////////////////////////////////////

  static
  const ymuint I0_BIT  = 1U;
  static
  const ymuint I1_BIT  = 2U;
  static
  const ymuint XOR_BIT = 4U;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス BdnMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief 名前を得る．
inline
string
BdnMgrImpl::name() const
{
  return mName;
}

// @brief ポート数の取得
inline
ymuint
BdnMgrImpl::port_num() const
{
  return mPortArray.size();
}

// @brief ポートの取得
// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
inline
const BdnPort*
BdnMgrImpl::port(ymuint pos) const
{
  return mPortArray[pos];
}

// @brief D-FFのIDの最大値 + 1 の取得
inline
ymuint
BdnMgrImpl::max_dff_id() const
{
  return mDffArray.size();
}

// @brief ID番号から D-FF を得る．
// @param[in] id ID番号 ( 0 <= id < max_dff_id() )
// @note 該当するD-FFが無い場合には NULL を返す．
inline
const BdnDff*
BdnMgrImpl::dff(ymuint id) const
{
  return mDffArray[id];
}

// @brief D-FF 数の取得
inline
ymuint
BdnMgrImpl::dff_num() const
{
  return mDffList.size();
}

// @brief D-FF のリストの取得
inline
const BdnDffList&
BdnMgrImpl::dff_list() const
{
  return mDffList;
}

// @brief ラッチのIDの最大値 + 1 の取得
inline
ymuint
BdnMgrImpl::max_latch_id() const
{
  return mLatchArray.size();
}

// @brief ID番号からラッチを得る．
// @param[in] id ID番号 ( 0 <= id < max_latch_id() )
// @note 該当するラッチが無い場合には NULL を返す．
inline
const BdnLatch*
BdnMgrImpl::latch(ymuint id) const
{
  return mLatchArray[id];
}

// @brief ラッチ数の取得
inline
ymuint
BdnMgrImpl::latch_num() const
{
  return mLatchList.size();
}

// @brief ラッチのリストの取得
inline
const BdnLatchList&
BdnMgrImpl::latch_list() const
{
  return mLatchList;
}

// ノード番号の最大値 + 1 を返す．
inline
ymuint
BdnMgrImpl::max_node_id() const
{
  return mNodeArray.size();
}

// @brief ID 番号をキーにノードを取り出す．
inline
const BdnNode*
BdnMgrImpl::node(ymuint id) const
{
  return mNodeArray[id];
}

// @brief 入力ノード数の取得
// @return 入力ノード数を返す．
inline
ymuint
BdnMgrImpl::input_num() const
{
  return mInputList.size();
}

// @brief 入力ノードのリストを得る．
inline
const BdnNodeList&
BdnMgrImpl::input_list() const
{
  return mInputList;
}

// @brief 出力ノード数を得る．
inline
ymuint
BdnMgrImpl::output_num() const
{
  return mOutputList.size();
}

// @brief 出力ノードのリストを得る．
inline
const BdnNodeList&
BdnMgrImpl::output_list() const
{
  return mOutputList;
}

// 論理ノード数を得る．
inline
ymuint
BdnMgrImpl::lnode_num() const
{
  return mLnodeList.size();
}

// 論理ノードのリストを得る．
inline
const BdnNodeList&
BdnMgrImpl::lnode_list() const
{
  return mLnodeList;
}

// @brief ANDノードの内容を設定する．
// @param[in] node 設定するノード
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return ノード＋極性を返す．
// @note すでに構造的に同じノードがあればそれを返す．
// @note なければ node に設定する．
// @note node が NULL の場合，新しいノードを確保する．
inline
BdnNodeHandle
BdnMgrImpl::set_and(BdnNode* node,
		    BdnNodeHandle inode1_handle,
		    BdnNodeHandle inode2_handle)
{
  return set_logic(node, false, inode1_handle, inode2_handle);
}

// @brief XORノードの内容を設定する．
// @param[in] node 設定するノード
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return ノード＋極性を返す．
// @note すでに構造的に同じノードがあればそれを返す．
// @note なければ node に設定する．
// @note node が NULL の場合，新しいノードを確保する．
inline
BdnNodeHandle
BdnMgrImpl::set_xor(BdnNode* node,
		    BdnNodeHandle inode1_handle,
		    BdnNodeHandle inode2_handle)
{
  return set_logic(node, true, inode1_handle, inode2_handle);
}

// @brief AND のバランス木を作る．
// @param[in] node 根のノード
// @param[in] node_list 入力のノードのリスト
// @note node が NULL の場合，新しいノードを確保する．
inline
BdnNodeHandle
BdnMgrImpl::make_and_tree(BdnNode* node,
			  const vector<BdnNodeHandle>& node_list)
{
  return make_tree(node, false, false, 0, node_list.size(), node_list);
}

// @brief OR のバランス木を作る．
// @param[in] node 根のノード
// @param[in] node_list 入力のノードのリスト
// @note node が NULL の場合，新しいノードを確保する．
inline
BdnNodeHandle
BdnMgrImpl::make_or_tree(BdnNode* node,
			 const vector<BdnNodeHandle>& node_list)
{
  return ~make_tree(node, false, true, 0, node_list.size(), node_list);
}

// @brief XOR のバランス木を作る．
// @param[in] node 根のノード
// @param[in] node_list 入力のノードのリスト
// @note node が NULL の場合，新しいノードを確保する．
inline
BdnNodeHandle
BdnMgrImpl::make_xor_tree(BdnNode* node,
			  const vector<BdnNodeHandle>& node_list)
{
  return make_tree(node, true, false, 0, node_list.size(), node_list);
}

END_NAMESPACE_YM_NETWORKS_BDN

#endif // BDNMGRIMPL_H
