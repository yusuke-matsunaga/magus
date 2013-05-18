#ifndef PHFEDGE_H
#define PHFEDGE_H

/// @file PhfEdge.h
/// @brief PhfEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
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
  /// @param[in] node_list ノードのリスト
  /// @param[in] val 値
  PhfEdge(ymuint id,
	  const vector<PhfNode*>& node_list,
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

  /// @brief ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
  PhfNode*
  node(ymuint pos) const;

  /// @brief 値を返す．
  ymuint
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // ノードのリスト
  vector<PhfNode*> mNodeList;

  // 値
  ymuint32 mVal;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] node_list ノードのリスト
// @param[in] val 値
inline
PhfEdge::PhfEdge(ymuint id,
		 const vector<PhfNode*>& node_list,
		 ymuint val) :
  mId(id),
  mNodeList(node_list),
  mVal(val)
{
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

// @brief ノード数を返す．
inline
ymuint
PhfEdge::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
inline
PhfNode*
PhfEdge::node(ymuint pos) const
{
  assert_cond( pos < mNodeList.size(), __FILE__, __LINE__);
  return mNodeList[pos];
}

// @brief 値を返す．
inline
ymuint
PhfEdge::val() const
{
  return mVal;
}

END_NAMESPACE_YM_IGF

#endif // PHFEDGE_H
