#ifndef RWTPAT_H
#define RWTPAT_H

/// @file RwtPat.h
/// @brief RwtPat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ym_logic/NpnMap.h"


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

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief ノードを得る．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  const RwtNode*
  node(ymuint pos) const;

  /// @brief 代表関数に対する変換を得る．
  const NpnMap&
  cmap() const;

  /// @brief 同じ代表関数を表す次のノードを得る．
  const RwtPat*
  link() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // 入力数
  ymuint32 mInputNum;

  // ノードの配列
  // 0 〜 mInputNum - 1 個は入力ノード
  // 最後は根のノード
  const RwtNode** mNodeList;

  // 変換マップ
  NpnMap mCmap;

  // 次のパタンを指すリンク
  RwtPat* mLink;

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
  mLink = NULL;
}

// @brief デストラクタ
inline
RwtPat::~RwtPat()
{
  delete [] mNodeList;
}

// @brief ノード数を得る．
inline
ymuint
RwtPat::node_num() const
{
  return mNodeNum;
}

// @brief 入力数を得る．
inline
ymuint
RwtPat::input_num() const
{
  return mInputNum;
}

// @brief ノードを得る．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
inline
const RwtNode*
RwtPat::node(ymuint pos) const
{
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return mNodeList[pos];
}

// @brief 代表関数に対する変換を得る．
inline
const NpnMap&
RwtPat::cmap() const
{
  return mCmap;
}

// @brief 同じ代表関数を表す次のノードを得る．
inline
const RwtPat*
RwtPat::link() const
{
  return mLink;
}

END_NAMESPACE_YM_NETWORKS

#endif // RWTPAT_H
