#ifndef PGPAT_H
#define PGPAT_H

/// @file PgPat.h
/// @brief PgPat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class PgPat PgPat.h "PgPat.h"
/// @brief RwtPat を生成するための元データ
//////////////////////////////////////////////////////////////////////
class PgPat
{
  friend class RwtPatGen;

private:

  /// @brief コンストラクタ
  PgPat();

  /// @brief デストラクタ
  ~PgPat();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根の極性＋入力数を得る．
  ymuint32
  input_num() const;

  /// @brief ノード数を得る．
  ymuint32
  node_num() const;

  /// @brief ノード番号を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
  ymuint32
  node(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // 入力数 ＋ 根のノードの極性
  ymuint32 mInputNum;

  // ノード番号の配列
  ymuint32* mNodeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
PgPat::PgPat()
{
  mNodeNum = 0U;
  mInputNum = 0U;
  mNodeList = NULL;
}

// @brief デストラクタ
inline
PgPat::~PgPat()
{
}

// @brief 入力数を得る．
inline
ymuint32
PgPat::input_num() const
{
  return mInputNum;
}

// @brief ノード数を得る．
inline
ymuint32
PgPat::node_num() const
{
  return mNodeNum;
}

// @brief ノード番号を得る．
// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
inline
ymuint32
PgPat::node(ymuint pos) const
{
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return mNodeList[pos];
}

END_NAMESPACE_YM_NETWORKS

#endif // PGPAT_H
