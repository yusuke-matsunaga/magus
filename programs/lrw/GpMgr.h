/// @file GpMgr.h
/// @brief GpMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

class GpNode;
class GpHandle;

//////////////////////////////////////////////////////////////////////
/// @class GpMgr GpMgr.h "GpMgr.h"
/// @brief GpNode を管理するためのクラス
//////////////////////////////////////////////////////////////////////
class GpMgr
{
public:

  /// @brief コンストラクタ
  GpMgr();

  /// @brief デストラクタ
  ~GpMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0ノードを生成する．
  GpHandle
  make_const0();

  /// @brief 定数1ノードを生成する．
  GpHandle
  make_const1();

  /// @brief 入力ノードを生成する．
  /// @param[in] input_id 入力番号
  /// @note すでに同じ入力番号のノードが存在していたらそれを返す．
  GpHandle
  make_input(ymuint32 input_id);

  /// @brief AND ノードを生成する．
  /// @param[in] fanin0 ファンイン0
  /// @param[in] fanin1 ファンイン1
  /// @note もしも既に同じ構造のノードがあればそれを返す．
  /// @note 場合によってはファンインその物や定数ノードを返すこともある．
  GpHandle
  make_and(GpHandle fanin0,
	   GpHandle fanin1);

  /// @brief XOR ノードを生成する．
  /// @param[in] fanin0 ファンイン0
  /// @param[in] fanin1 ファンイン1
  /// @note もしも既に同じ構造のノードがあればそれを返す．
  /// @note 場合によってはファンインその物や定数ノードを返すこともある．
  GpHandle
  make_xor(GpHandle fanin0,
	   GpHandle fanin1);

  /// @brief ハンドルとその子供の内容を出力する．
  void
  dump_handle(ostream& s,
	      GpHandle handle) const;

  /// @brief 複数のハンドルとその子供の内容を出力する．
  void
  dump_handle(ostream& s,
	      const vector<GpHandle>& handle_list) const;

  /// @brief 全ノード数を返す．
  ymuint
  node_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを生成する下請け関数
  GpNode*
  new_node();

  /// @brief node を根とするパタンのノード数を数える．
  ymuint32
  count_size(GpNode* node);

  /// @brief ハッシュ関数
  ymuint32
  hash_func(GpHandle fanin0,
	    GpHandle fanin1,
	    bool xor_flag);

  /// @brief ハッシュテーブルを拡大する．
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 全ノードのリスト
  vector<GpNode*> mNodeList;

  // 入力ノードのリスト
  vector<GpNode*> mInputList;

  // AND/XORノードのリスト
  vector<GpNode*> mAndList;

  // 構造ハッシュ
  GpNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // mHashTable を拡大する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM
