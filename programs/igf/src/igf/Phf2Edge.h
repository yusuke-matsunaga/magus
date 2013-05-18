#ifndef PHF2EDGE_H
#define PHF2EDGE_H

/// @file Phf2Edge.h
/// @brief Phf2Edge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class Phf2Node;

//////////////////////////////////////////////////////////////////////
/// @class Phf2Edge Phf2Edge.h "Phf2Edge.h"
/// @brief PHF 用の枝
//////////////////////////////////////////////////////////////////////
class Phf2Edge
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] node1 ノード1
  /// @param[in] node2 ノード2
  /// @param[in] val 値
  Phf2Edge(ymuint id,
	   Phf2Node* node1,
	   Phf2Node* node2,
	   ymuint val);

  /// @brief デストラクタ
  ~Phf2Edge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief ノード1を返す．
  Phf2Node*
  node1() const;

  /// @brief ノード2を返す．
  Phf2Node*
  node2() const;

  /// @brief 値を返す．
  ymuint
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // ノード1
  Phf2Node* mNode1;

  // ノード2
  Phf2Node* mNode2;

  // 値
  ymuint32 mVal;

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
Phf2Edge::Phf2Edge(ymuint id,
		   Phf2Node* node1,
		   Phf2Node* node2,
		   ymuint val)
{
  mId = id;
  mNode1 = node1;
  mNode2 = node2;
  mVal = val;
}

// @brief デストラクタ
inline
Phf2Edge::~Phf2Edge()
{
}

// @brief ID番号を返す．
inline
ymuint
Phf2Edge::id() const
{
  return mId;
}

// @brief ノード1を返す．
inline
Phf2Node*
Phf2Edge::node1() const
{
  return mNode1;
}

// @brief ノード2を返す．
inline
Phf2Node*
Phf2Edge::node2() const
{
  return mNode2;
}

// @brief 値を返す．
inline
ymuint
Phf2Edge::val() const
{
  return mVal;
}

END_NAMESPACE_YM_IGF

#endif // PHF2EDGE_H
