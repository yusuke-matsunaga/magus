#ifndef PHF3EDGE_H
#define PHF3EDGE_H

/// @file Phf3Edge.h
/// @brief Phf3Edge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class Phf3Node;

//////////////////////////////////////////////////////////////////////
/// @class Phf3Edge Phf3Edge.h "Phf3Edge.h"
/// @brief PHF 用の枝
//////////////////////////////////////////////////////////////////////
class Phf3Edge
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] node1 ノード1
  /// @param[in] node2 ノード2
  /// @param[in] node3 ノード3
  /// @param[in] val 値
  Phf3Edge(ymuint id,
	   Phf3Node* node1,
	   Phf3Node* node2,
	   Phf3Node* node3,
	   ymuint val);

  /// @brief デストラクタ
  ~Phf3Edge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief ノード1を返す．
  Phf3Node*
  node1() const;

  /// @brief ノード2を返す．
  Phf3Node*
  node2() const;

  /// @brief ノード2を返す．
  Phf3Node*
  node3() const;

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
  Phf3Node* mNode1;

  // ノード2
  Phf3Node* mNode2;

  // ノード3
  Phf3Node* mNode3;

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
// @param[in] node3 ノード3
// @param[in] val 値
inline
Phf3Edge::Phf3Edge(ymuint id,
		   Phf3Node* node1,
		   Phf3Node* node2,
		   Phf3Node* node3,
		   ymuint val)
{
  mId = id;
  mNode1 = node1;
  mNode2 = node2;
  mNode3 = node3;
  mVal = val;
}

// @brief デストラクタ
inline
Phf3Edge::~Phf3Edge()
{
}

// @brief ID番号を返す．
inline
ymuint
Phf3Edge::id() const
{
  return mId;
}

// @brief ノード1を返す．
inline
Phf3Node*
Phf3Edge::node1() const
{
  return mNode1;
}

// @brief ノード2を返す．
inline
Phf3Node*
Phf3Edge::node2() const
{
  return mNode2;
}

// @brief ノード3を返す．
inline
Phf3Node*
Phf3Edge::node3() const
{
  return mNode3;
}

// @brief 値を返す．
inline
ymuint
Phf3Edge::val() const
{
  return mVal;
}

END_NAMESPACE_YM_IGF

#endif // PHF3EDGE_H
