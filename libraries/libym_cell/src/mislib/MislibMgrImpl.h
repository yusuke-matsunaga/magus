#ifndef MISLIBMGRIMPL_H
#define MISLIBMGRIMPL_H

/// @file MislibMgrImpl.h
/// @brief MislibMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"
#include "YmUtils/FileRegion.h"
#include "YmUtils/SimpleAlloc.h"
#include "YmUtils/ShString.h"

#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibMgrImpl MislibMgrImpl.h "MislibMgrImpl.h"
/// @brief MislibNode を管理するクラス
//////////////////////////////////////////////////////////////////////
class MislibMgrImpl
{
public:

  /// @brief コンストラクタ
  MislibMgrImpl();

  /// @brief デストラクタ
  ~MislibMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // MislibMgr のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保したオブジェクトをすべて削除する．
  void
  clear();

  /// @brief ゲートのリストを返す．
  const MislibNode*
  gate_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // MislibNode を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief GATE ノードを生成する．(通常版)
  /// @note 結果はゲートのリストに追加される．
  void
  new_gate1(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* area,
	    const MislibNode* oname,
	    const MislibNode* expr,
	    const MislibNode* ipin_list);

  /// @brief GATE ノードを生成する．(ワイルドカードの入力ピン)
  /// @note 結果はゲートのリストに追加される．
  void
  new_gate2(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* area,
	    const MislibNode* oname,
	    const MislibNode* expr,
	    const MislibNode* ipin);

  /// @brief GATE ノードを生成する．(入力ピンなし:定数ノード)
  /// @note 結果はゲートのリストに追加される．
  void
  new_gate3(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* area,
	    const MislibNode* oname,
	    const MislibNode* expr);

  /// @brief PIN ノードを生成する．
  MislibNodeImpl*
  new_pin(const FileRegion& loc,
	  const MislibNode* name,
	  const MislibNode* phase,
	  const MislibNode* input_load,
	  const MislibNode* max_load,
	  const MislibNode* rise_block_delay,
	  const MislibNode* rise_fanout_delay,
	  const MislibNode* fall_block_delay,
	  const MislibNode* fall_fanout_delay);

  /// @brief リストノードを生成する．
  MislibNodeImpl*
  new_list();

  /// @brief NOT ノードを生成する．
  MislibNodeImpl*
  new_not(const FileRegion& loc,
	  const MislibNode* child1);

  /// @brief AND ノードを生成する．
  MislibNodeImpl*
  new_and(const FileRegion& loc,
	  const MislibNode* child1,
	  const MislibNode* child2);

  /// @brief OR ノードを生成する．
  MislibNodeImpl*
  new_or(const FileRegion& loc,
	 const MislibNode* child1,
	 const MislibNode* child2);

  /// @brief XOR ノードを生成する．
  MislibNodeImpl*
  new_xor(const FileRegion& loc,
	  const MislibNode* child1,
	  const MislibNode* child2);

  /// @brief 定数0ノードを生成する．
  MislibNodeImpl*
  new_const0(const FileRegion& loc);

  /// @brief 定数1ノードを生成する．
  MislibNodeImpl*
  new_const1(const FileRegion& loc);

  /// @brief NONINV ノードを生成する．
  MislibNodeImpl*
  new_noninv(const FileRegion& loc);

  /// @brief INV ノードを生成する．
  MislibNodeImpl*
  new_inv(const FileRegion& loc);

  /// @brief UNKNOWN ノードを生成する．
  MislibNodeImpl*
  new_unknown(const FileRegion& loc);

  /// @brief 文字列ノードを生成する．
  MislibNodeImpl*
  new_str(const FileRegion& loc,
	  ShString str);

  /// @brief 数値ノードを生成する．
  MislibNodeImpl*
  new_num(const FileRegion& loc,
	  double num);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートを追加する．
  void
  add_gate(MislibNodeImpl* gate);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // MislibNode のメモリ確保用アロケータ
  SimpleAlloc mAlloc;

  // ゲートのリスト
  MislibNodeImpl* mGateList;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBMGRIMPL_H
