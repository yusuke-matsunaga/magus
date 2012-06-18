#ifndef NPNNODEMGR_H
#define NPNNODEMGR_H

/// @file NpnNodeMgr.h
/// @brief NpnNodeMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

class NpnNode;
class NpnHandle;

//////////////////////////////////////////////////////////////////////
/// @class NpnNodeMgr NpnNodeMgr.h "NpnNodeMgr.h"
/// @brief NpnNode を管理するクラス
//////////////////////////////////////////////////////////////////////
class NpnNodeMgr
{
public:

  /// @brief コンストラクタ
  NpnNodeMgr();

  /// @brief デストラクタ
  ~NpnNodeMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0ノードを生成する．
  NpnHandle
  make_const0();

  /// @brief 定数1ノードを生成する．
  NpnHandle
  make_const1();

  /// @brief 入力ノードを生成する．
  /// @param[in] input_id ( 0 <= input_id < 4 )
  /// @note すでに同じ入力番号のノードが存在していたらそれを返す．
  NpnHandle
  make_input(ymuint input_id);

  /// @brief ANDノードを生成する．
  /// @param[in] fanin0 ファンイン0
  /// @param[in] fanin1 ファンイン1
  /// @note もしも既に同じ構造のノードがあればそれを返す．
  /// @note 場合によってはファンインその物や定数ノードを返すこともある．
  NpnHandle
  make_and(NpnHandle fanin0,
	   NpnHandle fanin1);

  /// @brief ORノードを生成する．
  /// @param[in] fanin0 ファンイン0
  /// @param[in] fanin1 ファンイン1
  /// @note もしも既に同じ構造のノードがあればそれを返す．
  /// @note 場合によってはファンインその物や定数ノードを返すこともある．
  /// @note 実際には AND ノード＋反転属性
  NpnHandle
  make_or(NpnHandle fanin0,
	  NpnHandle fanin1);

  /// @brief XORノードを生成する．
  /// @param[in] fanin0 ファンイン0
  /// @param[in] fanin1 ファンイン1
  /// @note もしも既に同じ構造のノードがあればそれを返す．
  /// @note 場合によってはファンインその物や定数ノードを返すこともある．
  NpnHandle
  make_xor(NpnHandle fanin0,
	   NpnHandle fanin1);

  /// @brief 指定された番号のノードを返す．
  NpnNode*
  node(ymuint id) const;

  /// @brief ハンドルの表す関数を返す．
  ymuint16
  func(NpnHandle handle) const;

  /// @brief ハンドルとその子供の内容を出力する．
  void
  dump_handle(ostream& s,
	      NpnHandle handle) const;

  /// @brief 複数のハンドルとその子供の内容を出力する．
  void
  dump_handle(ostream& s,
	      const vector<NpnHandle>& handle_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝を正規化する．
  NpnHandle
  cannonical(NpnHandle src);

  /// @brief 新しいノードを登録する関数
  /// @param[in] is_xor XOR ノードの時 true にするフラグ
  /// @param[in] func 関数ベクタ
  /// @param[in] fanin0, fanin1 ファンインのハンドル
  /// @note おなじノードが既に存在していたらそのノードを返す．
  NpnNode*
  new_node(bool is_xor,
	   ymuint16 func,
	   NpnHandle fanin0,
	   NpnHandle fanin1);

  /// @brief ノードを生成する関数
  NpnNode*
  alloc_node();

  /// @brief ハッシュ関数
  ymuint32
  hash_func(NpnHandle fanin0,
	    NpnHandle fanin1,
	    bool xor_flag);

  /// @brief ハッシュテーブルを拡大する．
  void
  alloc_table(ymuint req_size);

  /// @brief dump_handle() の下請け関数
  void
  dh_sub(ostream& s,
	 ymuint id,
	 hash_set<ymuint32>& node_hash) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // NpnNode 用のメモリアロケータ
  SimpleAlloc mAlloc;

  // 全ノードのリスト
  vector<NpnNode*> mNodeList;

  // 唯一の定数0ノード
  NpnNode* mConstNode;

  // 唯一の入力ノード
  NpnNode* mInputNode;

  // AND/XORノードのリスト
  vector<NpnNode*> mAndList;

  // 構造ハッシュ
  NpnNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // mHashTable を拡大する目安
  ymuint32 mNextLimit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 指定された番号のノードを返す．
inline
NpnNode*
NpnNodeMgr::node(ymuint id) const
{
  return mNodeList[id];
}

END_NAMESPACE_YM

#endif // NPNNODEMGR_H
