#ifndef GCNODE_H
#define GCNODE_H

/// @file GcNode.h
/// @brief GcNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GcNode GcNode.h "GcNode.h"
/// @brief 彩色問題を表すグラフのノード
//////////////////////////////////////////////////////////////////////
class GcNode
{
public:

  /// @brief コンストラクタ
  GcNode();

  /// @brief デストラクタ
  ~GcNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init(ymuint id,
       ymuint vectlen);

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 隣接するノードのリストを得る．
  const vector<GcNode*>&
  link_list() const;

  /// @brief 隣接するノードの度数を返す．
  ymuint
  adj_degree() const;

  /// @brief saturation degree を返す．
  ymuint
  sat_degree() const;

  /// @brief 色番号を設定する．
  void
  set_color(ymuint color);

  /// @brief 色番号を得る．
  /// @note 0 は未彩色を表す．
  ymuint
  color() const;

  /// @brief color のノードが隣接しているときに true を返す．
  bool
  check_adj_color(ymuint color) const;

  /// @brief color を隣接色に加える．
  void
  add_adj_color(ymuint color);

  /// @brief 2つのノードを接続する．
  friend
  void
  connect(GcNode* node1,
	  GcNode* node2);

  /// @brief ヒープ上の位置(+1)を返す．
  ///
  /// ヒープになければ 0 を返す．
  ymuint
  heap_location() const;

  /// @brief ヒープ上の位置を設定する．
  void
  set_heap_location(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // ヒープ中の位置(+1)
  ymuint mHeapIdx;

  // 隣接するノードのリスト
  vector<GcNode*> mLinkList;

  // 隣接するノードの色の集合を表すビットベクタ
  ymuint64* mColorSet;

  // SAT degree
  ymuint mSatDegree;

  // 色番号
  // 0 が未彩色
  ymuint mColor;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
ymuint
GcNode::id() const
{
  return mId;
}

// @brief 隣接するノードのリストを得る．
inline
const vector<GcNode*>&
GcNode::link_list() const
{
  return mLinkList;
}

// @brief 隣接するノードの度数を返す．
inline
ymuint
GcNode::adj_degree() const
{
  return mLinkList.size();
}

// @brief saturation degree を返す．
inline
ymuint
GcNode::sat_degree() const
{
  return mSatDegree;
}

// @brief 色番号を設定する．
inline
void
GcNode::set_color(ymuint color)
{
  mColor = color;
}

// @brief 色番号を得る．
// @note 0 は未彩色を表す．
inline
ymuint
GcNode::color() const
{
  return mColor;
}

// @brief color のノードが隣接しているときに true を返す．
inline
bool
GcNode::check_adj_color(ymuint color) const
{
  ymuint blk = color / 64;
  ymuint sft = color % 64;
  return ((mColorSet[blk] >> sft) & 1UL) == 1UL;
}

// @brief color を隣接色に加える．
inline
void
GcNode::add_adj_color(ymuint color)
{
  ymuint blk = color / 64;
  ymuint sft = color % 64;
  mColorSet[blk] |= (1UL << sft);
  ++ mSatDegree;
}

// @brief ヒープ上の位置(+1)を返す．
//
// ヒープになければ 0 を返す．
inline
ymuint
GcNode::heap_location() const
{
  return mHeapIdx;
}

// @brief ヒープ上の位置を設定する．
inline
void
GcNode::set_heap_location(ymuint pos)
{
  mHeapIdx = pos;
}

// @brief 2つのノードを接続する．
inline
void
connect(GcNode* node1,
	GcNode* node2)
{
  node1->mLinkList.push_back(node2);
  node2->mLinkList.push_back(node1);
}

END_NAMESPACE_YM

#endif // GCNODE_H
