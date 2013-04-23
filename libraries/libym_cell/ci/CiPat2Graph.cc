
/// @file CiPat2Graph.cc
/// @brief CiPat2Graph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CiPat2Graph.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiPat2Graph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiPat2Graph::CiPat2Graph() :
  mInputNum(0U),
  mEdgeNum(0U),
  mEdgeList(NULL)
{
}

// @brief デストラクタ
CiPat2Graph::~CiPat2Graph()
{
  // mEdgeList はメモリアロケータが管理している．
}

// @brief 代表関数番号を返す．
ymuint
CiPat2Graph::rep_id() const
{
  return mRepId;
}

// @brief 根のノード番号を返す．
ymuint
CiPat2Graph::root_id() const
{
  // 枝の番号を2で割ればファンアウト先のノード番号
  return mEdgeList[0] / 2;
}

// @brief 根の反転属性を返す．
bool
CiPat2Graph::root_inv() const
{
  return static_cast<bool>(mInputNum & 1U);
}

// @brief 入力数を返す．
ymuint
CiPat2Graph::input_num() const
{
  return (mInputNum >> 1);
}

// @brief 枝数を返す．
ymuint
CiPat2Graph::edge_num() const
{
  return mEdgeNum;
}

// @brief 枝(の番号)を返す．
// @param[in] pos 位置 ( 0 <= pos < edge_num() )
ymuint
CiPat2Graph::edge(ymuint pos) const
{
  assert_cond( pos < edge_num(), __FILE__, __LINE__);
  return mEdgeList[pos];
}

// @brief バイナリダンプを行う．
// @param[in] bos 出力先のストリーム
void
CiPat2Graph::dump(BinO& bos) const
{
  bos << mRepId
      << mInputNum
      << mEdgeNum;
  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    bos << mEdgeList[i];
  }
}

// @brief バイナリファイルを読み込む．
// @param[in] bis 入力元のストリーム
void
CiPat2Graph::restore(BinI& bis,
		     Alloc& alloc)
{
  bis >> mRepId
      >> mInputNum
      >> mEdgeNum;
  alloc_array(alloc);
  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    bis >> mEdgeList[i];
  }
}

// @brief 初期化する．
// @param[in] rep_id 代表番号
// @param[in] input_num 入力数
// @param[in] edge_num 枝数
// @param[in] alloc メモリアロケータ
void
CiPat2Graph::init(ymuint rep_id,
		  ymuint input_num,
		  ymuint edge_num,
		  Alloc& alloc)
{
  mRepId = rep_id;
  mInputNum = input_num;
  mEdgeNum = edge_num;
  alloc_array(alloc);
}

// @brief 枝のデータを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
// @param[in] edge 枝
// @note この関数を呼ぶ前に init() が呼ばれている必要がある．
void
CiPat2Graph::set_edge(ymuint pos,
		      ymuint edge)
{
  assert_cond( pos < edge_num(), __FILE__, __LINE__);
  mEdgeList[pos] = edge;
}

// @brief mEdgeList を確保する．
// @param[in] alloc メモリアロケータ
// @note mEdgeNum に値が設定されているものとする．
void
CiPat2Graph::alloc_array(Alloc& alloc)
{
  if ( mEdgeNum > 0 ) {
    void* p = alloc.get_memory(sizeof(ymuint32) * mEdgeNum);
    mEdgeList = new (p) ymuint32[mEdgeNum];
  }
  else {
    mEdgeList = NULL;
  }
}

END_NAMESPACE_YM_CELL

