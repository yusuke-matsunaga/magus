#ifndef PHFEDGE_H
#define PHFEDGE_H

/// @file PhfEdge.h
/// @brief PhfEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class PhfNode;

//////////////////////////////////////////////////////////////////////
/// @class PhfEdge PhfEdge.h "PhfEdge.h"
/// @brief PHF 用の枝
//////////////////////////////////////////////////////////////////////
class PhfEdge
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] node1 ノード1
  /// @param[in] node2 ノード2
  /// @param[in] val 値
  PhfEdge(ymuint id,
	  PhfNode* node1,
	  PhfNode* node2,
	  ymuint val);

  /// @brief デストラクタ
  ~PhfEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief ノード1を返す．
  PhfNode*
  node1() const;

  /// @brief ノード2を返す．
  PhfNode*
  node2() const;

  /// @brief 値を返す．
  ymuint
  val() const;

  /// @brief 向きを返す．
  ymuint
  dir() const;

  /// @brief 向きを設定する．
  void
  set_dir(ymuint dir);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // ノード1
  PhfNode* mNode1;

  // ノード2
  PhfNode* mNode2;

  // 値
  ymuint32 mVal;

  // 向き
  ymuint8 mDir;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] node1 ノード1
// @param[in] node2 ノード2
// @param[in] val 値
inline
PhfEdge::PhfEdge(ymuint id,
		 PhfNode* node1,
		 PhfNode* node2,
		 ymuint val)
{
  mId = id;
  mNode1 = node1;
  mNode2 = node2;
  mVal = val;
  mDir = 0;
}

// @brief デストラクタ
inline
PhfEdge::~PhfEdge()
{
}

// @brief ID番号を返す．
inline
ymuint
PhfEdge::id() const
{
  return mId;
}

// @brief ノード1を返す．
inline
PhfNode*
PhfEdge::node1() const
{
  return mNode1;
}

// @brief ノード2を返す．
inline
PhfNode*
PhfEdge::node2() const
{
  return mNode2;
}

// @brief 値を返す．
inline
ymuint
PhfEdge::val() const
{
  return mVal;
}

// @brief 向きを返す．
inline
ymuint
PhfEdge::dir() const
{
  return mDir;
}

// @brief 向きを設定する．
inline
void
PhfEdge::set_dir(ymuint dir)
{
  mDir = dir;
}

END_NAMESPACE_YM_IGF

#endif // PHFEDGE_H
