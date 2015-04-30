#ifndef BTJUST2_H
#define BTJUST2_H

/// @file BtJust2.h
/// @brief BtJust2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtJustBase.h"
#include "YmUtils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BtJust2 BtJust2.h "BtJust2.h"
/// @brief 必要なノードのみ正当化する BackTracer
//////////////////////////////////////////////////////////////////////
class BtJust2 :
  public BtJustBase
{
public:

  /// @brief コンストラクタ
  BtJust2();

  /// @brief デストラクタ
  virtual
  ~BtJust2();


public:
  //////////////////////////////////////////////////////////////////////
  // BackTracer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードID番号の最大値を設定する．
  /// @param[in] max_id ID番号の最大値
  virtual
  void
  set_max_id(ymuint max_id);

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] node_set 故障に関係するノード集合
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  virtual
  void
  run(const TpgNode* fnode,
      const NodeSet& node_set,
      const ValMap& val_map,
      NodeValList& assign_list);


protected:
  //////////////////////////////////////////////////////////////////////
  // BtJustBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief clear_justified() 中で呼ばれるフック関数
  virtual
  void
  clear_justified_hook(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct NodeList
  {
    const TpgNode* mNode;

    NodeList* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief solve 中で変数割り当ての正当化を行なう．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  NodeList*
  justify(const TpgNode* node,
	  const ValMap& val_map);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  NodeList*
  just_sub1(const TpgNode* node,
	    const ValMap& val_map);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[in] val 値
  NodeList*
  just_sub2(const TpgNode* node,
	    const ValMap& val_map,
	    Val3 val);

  /// @brief 新しいリストのセルを返す．
  NodeList*
  new_list_cell(const TpgNode* node);

  /// @brief リストをマージする．
  void
  list_merge(NodeList*& dst_list,
	     NodeList* src_list);

  /// @brief リストのサイズを返す．
  static
  ymuint
  list_size(NodeList* node_list);

  /// @brief リストを削除する．
  void
  list_free(NodeList* node_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のオブジェクト
  UnitAlloc mAlloc;

  // node->id() をキーにして入力番号のリストを納める配列
  vector<NodeList*> mJustArray;

};

END_NAMESPACE_YM_SATPG

#endif // BTJUST2_H
