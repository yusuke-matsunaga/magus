#ifndef NODESET_H
#define NODESET_H

/// @file NodeSet.h
/// @brief NodeSet のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class NodeSet NodeSet.h "NodeSet.h"
/// @brief 故障箇所の TFO の TFI に印をつけるためのクラス
//////////////////////////////////////////////////////////////////////
class NodeSet
{
public:

  /// @brief コンストラクタ
  NodeSet();

  /// @brief デストラクタ
  ~NodeSet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障位置を与えてその TFO の TFI リストを作る．
  /// @param[in] max_node_id ノード番号の最大値
  /// @param[in] fnode 故障位置のノード
  ///
  /// 結果は mTfoList に格納される．
  /// 故障位置の TFO が mTfoList の [0: mTfoEnd - 1] に格納される．
  void
  mark_region(ymuint max_node_id,
	      const TpgNode* fnode);

  /// @brief 故障位置を与えてその TFO の TFI リストを作る．
  /// @param[in] max_node_id ノード番号の最大値
  /// @param[in] fnode_list 故障位置のノードのリスト
  ///
  /// 結果は mTfoList に格納される．
  /// 故障位置の TFO が mTfoList の [0: mTfoEnd - 1] に格納される．
  void
  mark_region(ymuint max_node_id,
	      const vector<const TpgNode*>& fnode_list);

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_id() const;

  /// @brief TFO ノードの数を得る．
  ymuint
  tfo_size() const;

  /// @brief TFI ノードの数を得る．
  ymuint
  tfi_size() const;

  /// @brief TFO ノードと TFI ノードの総数を得る．
  ymuint
  tfo_tfi_size() const;

  /// @brief TFO/TFI ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < tfo_tfi_size() )
  ///
  /// pos が tfo_size() 未満のときは TFO ノード
  /// それ以上は TFI ノードとなっている．
  const TpgNode*
  tfo_tfi_node(ymuint pos) const;

  /// @brief tfo マークを読む．
  /// @param[in] node 対象のノード
  bool
  tfo_mark(const TpgNode* node) const;

  /// @brief tfi マークを読む．
  /// @param[in] node 対象のノード
  bool
  tfi_mark(const TpgNode* node) const;

  /// @brief tfo マークと tfi マークの OR を返す．
  bool
  tfo_tfi_mark(const TpgNode* node);

  /// @brief 入力のノードのリストを返す．
  const vector<const TpgNode*>&
  input_list() const;

  /// @brief 出力のノードのリストを返す．
  const vector<const TpgNode*>&
  output_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief tfo マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tfo_mark(const TpgNode* node);

  /// @brief tfi マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tfi_mark(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのIDの最大値
  ymuint32 mMaxNodeId;

  // ノードごとのいくつかのフラグをまとめた配列
  vector<ymuint8> mMarkArray;

  // 故障の TFO のノードリスト
  vector<const TpgNode*> mTfoList;

  // TFO ノードの最後の位置
  ymuint32 mTfoEnd;

  // 現在の故障に関係のありそうな外部入力のリスト
  vector<const TpgNode*> mInputList;

  // 現在の故障に関係ありそうな外部出力のリスト
  vector<const TpgNode*> mOutputList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 故障位置を与えてその TFO の TFI リストを作る．
// @param[in] max_node_id ノード番号の最大値
// @param[in] fnode 故障位置のノード
//
// 結果は mTfoList に格納される．
// 故障位置の TFO が mTfoList の [0: mTfoEnd - 1] に格納される．
inline
void
NodeSet::mark_region(ymuint max_node_id,
		     const TpgNode* fnode)
{
  mark_region(max_node_id, vector<const TpgNode*>(1, fnode));
}

// @brief ノード番号の最大値を返す．
inline
ymuint
NodeSet::max_id() const
{
  return mMaxNodeId;
}

// @brief TFO ノードの数を得る．
inline
ymuint
NodeSet::tfo_size() const
{
  return mTfoEnd;
}

// @brief TFI ノードの数を得る．
inline
ymuint
NodeSet::tfi_size() const
{
  return mTfoList.size() - mTfoEnd;
}

// @brief TFO ノードと TFI ノードの総数を得る．
inline
ymuint
NodeSet::tfo_tfi_size() const
{
  return mTfoList.size();
}

// @brief TFO/TFI ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < tfo_tfi_size() )
//
// pos が tfo_size() 未満のときは TFO ノード
// それ以上は TFI ノードとなっている．
inline
const TpgNode*
NodeSet::tfo_tfi_node(ymuint pos) const
{
  return mTfoList[pos];
}

// @brief 入力のノードのリストを返す．
inline
const vector<const TpgNode*>&
NodeSet::input_list() const
{
  return mInputList;
}

// @brief 出力のノードのリストを返す．
inline
const vector<const TpgNode*>&
NodeSet::output_list() const
{
  return mOutputList;
}

// tfo マークをつける．
inline
void
NodeSet::set_tfo_mark(const TpgNode* node)
{
  mMarkArray[node->id()] |= 1U;
  mTfoList.push_back(node);
  if ( node->is_output() ) {
    mOutputList.push_back(node);
  }
}

// @brief tfo マークを読む．
inline
bool
NodeSet::tfo_mark(const TpgNode* node) const
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// tfi マークをつける．
inline
void
NodeSet::set_tfi_mark(const TpgNode* node)
{
  mMarkArray[node->id()] |= 2U;
  mTfoList.push_back(node);
  if ( node->is_input() ) {
    mInputList.push_back(node);
  }
}

// @brief tfi マークを読む．
inline
bool
NodeSet::tfi_mark(const TpgNode* node) const
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief tfo マークと tfi マークの OR を返す．
inline
bool
NodeSet::tfo_tfi_mark(const TpgNode* node)
{
  if ( (mMarkArray[node->id()] & 3U) != 0U ) {
    return true;
  }
  else {
    return false;
  }
}

END_NAMESPACE_YM_SATPG

#endif // NODESET_H
