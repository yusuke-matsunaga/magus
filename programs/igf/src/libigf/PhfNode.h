#ifndef PHFNODE_H
#define PFHNODE_H

/// @file PhfNode.h
/// @brief PhfNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class PhfEdge;

//////////////////////////////////////////////////////////////////////
/// @class PhfNode PhfNode.h "PhfNode.h"
/// @brief PHF 用のノード
//////////////////////////////////////////////////////////////////////
class PhfNode
{
  friend class PhfGraph;
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] pat パタン
  PhfNode(ymuint id,
	  ymuint32 pat);

  /// @brief デストラクタ
  ~PhfNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief パタンを返す．
  ymuint32
  pat() const;

  /// @brief 接続している枝の数を返す．
  ymuint
  edge_num() const;

  /// @brief 接続している枝を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
  PhfEdge*
  edge(ymuint pos) const;

  /// @brief 値を得る．
  ymuint
  val() const;

  /// @brief 値が設定されていたら true を返す．
  bool
  is_assigned() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝を追加する．
  void
  add_edge(PhfEdge* edge);

  /// @brief 値を設定する．
  void
  set_val(ymuint32 val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード ID
  ymuint32 mId;

  // パタン
  ymuint32 mPat;

  // 接続している枝のリスト
  vector<PhfEdge*> mEdgeList;

  // このノードの値
  ymuint32 mVal;

  // active フラグ
  bool mActive;

  bool mAssigned;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] pat パタン
inline
PhfNode::PhfNode(ymuint id,
		 ymuint32 pat)
{
  mId = id;
  mPat = pat;
  mVal = 0;
  mAssigned = false;
}

// @brief デストラクタ
inline
PhfNode::~PhfNode()
{
}

// @brief ID番号を返す．
inline
ymuint
PhfNode::id() const
{
  return mId;
}

// @brief パタンを返す．
inline
ymuint32
PhfNode::pat() const
{
  return mPat;
}

// @brief 接続している枝の数を返す．
inline
ymuint
PhfNode::edge_num() const
{
  return mEdgeList.size();
}

// @brief 接続している枝を返す．
// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
inline
PhfEdge*
PhfNode::edge(ymuint pos) const
{
  ASSERT_COND( pos < mEdgeList.size() );
  return mEdgeList[pos];
}

// @brief 値を得る．
inline
ymuint
PhfNode::val() const
{
  return mVal;
}

// @brief 値が設定されていたら true を返す．
inline
bool
PhfNode::is_assigned() const
{
  return mAssigned;
}

// @brief 枝を追加する．
inline
void
PhfNode::add_edge(PhfEdge* edge)
{
  mEdgeList.push_back(edge);
}

// @brief 値を設定する．
inline
void
PhfNode::set_val(ymuint32 val)
{
  ASSERT_COND( mAssigned == false );
  mVal = val;
  mAssigned = true;
}

END_NAMESPACE_YM_IGF

#endif // PHFNODE_H
