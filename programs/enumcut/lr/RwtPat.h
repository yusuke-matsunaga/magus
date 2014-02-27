#ifndef RWTPAT_H
#define RWTPAT_H

/// @file RwtPat.h
/// @brief RwtPat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "networks/bdn.h"
#include "logic/NpnMap.h"


BEGIN_NAMESPACE_YM_NETWORKS

class RwtNode;

//////////////////////////////////////////////////////////////////////
/// @class RwtPat RwtPat.h "RwtPat.h"
/// @brief rewriting 用のパタンを表すクラス
//////////////////////////////////////////////////////////////////////
class RwtPat
{
  friend class RwtMgr;

private:

  /// @brief コンストラクタ
  RwtPat();

  /// @brief デストラクタ
  ~RwtPat();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードを得る．
  const RwtNode*
  root_node() const;

  /// @brief 根の極性を得る．
  bool
  root_inv() const;

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief 入力のノードを得る．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  const RwtNode*
  input_node(ymuint pos) const;

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを得る．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  /// @note 0 〜 (input_num() - 1) は入力ノード
  /// @note 最後のノードは根のノード
  const RwtNode*
  node(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // 入力数 + 根のノードの極性
  ymuint32 mInputNum;

  // ノードの配列
  // 0 〜 mInputNum - 1 個は入力ノード
  // 最後は根のノード
  RwtNode** mNodeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
RwtPat::RwtPat()
{
  mNodeNum = 0;
  mInputNum = 0;
  mNodeList = NULL;
}

// @brief デストラクタ
inline
RwtPat::~RwtPat()
{
  // mNodeList は RwtMgr が管理する．
}

// @brief 根のノードを得る．
inline
const RwtNode*
RwtPat::root_node() const
{
  return mNodeList[mNodeNum - 1];
}

// @brief 根の極性を得る．
inline
bool
RwtPat::root_inv() const
{
  return static_cast<bool>(mInputNum & 1U);
}

// @brief 入力数を得る．
inline
ymuint
RwtPat::input_num() const
{
  return mInputNum >> 1;
}

// @brief 入力のノードを得る．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
inline
const RwtNode*
RwtPat::input_node(ymuint pos) const
{
  return node(pos);
}

// @brief ノード数を得る．
inline
ymuint
RwtPat::node_num() const
{
  return mNodeNum;
}

// @brief ノードを得る．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
// @note 0 〜 (input_num() - 1) は入力ノード
// @note 最後のノードは根のノード
inline
const RwtNode*
RwtPat::node(ymuint pos) const
{
  return mNodeList[pos];
}

END_NAMESPACE_YM_NETWORKS

#endif // RWTPAT_H
