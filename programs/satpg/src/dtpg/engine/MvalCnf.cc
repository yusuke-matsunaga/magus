
/// @file MvalCnf.cc
/// @brief MvalCnf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MvalCnf.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス MvalCnf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値
MvalCnf::MvalCnf(ymuint max_node_id) :
  mMaxId(max_node_id),
  mGvalCnf(max_node_id),
  mFvarMap(max_node_id),
  mDvarMap(max_node_id),
  mFdMapArray(max_node_id, NULL)
{
}

// @brief デストラクタ
MvalCnf::~MvalCnf()
{
  for (ymuint i = 0; i < mMaxId; ++ i) {
    delete mFdMapArray[i];
  }
}

// @brief 初期化する．
// @param[in] max_node_id ノード番号の最大値
void
MvalCnf::init(ymuint max_node_id)
{
  for (ymuint i = 0; i < mMaxId; ++ i) {
    delete mFdMapArray[i];
  }

  mMaxId = max_node_id;
  mGvalCnf.init(max_node_id);
  mFvarMap.init(max_node_id);
  mDvarMap.init(max_node_id);
  mFdMapArray.clear();
  mFdMapArray.resize(max_node_id, NULL);
}

// @brief ノードに検出対象の故障があるか調べる．
bool
MvalCnf::has_faults(const TpgNode* node) const
{
  return mFdMapArray[node->id()] != NULL;
}

// @brief 出力の故障に対応した変数を得る．
// @param[in] node ノード
// @param[in] fval 故障値 (0/1)
VarId
MvalCnf::ofvar(const TpgNode* node,
	       int fval) const
{
  ASSERT_COND( has_faults(node) );
  ASSERT_COND( fval == 0 || fval == 1 );
  return mFdMapArray[node->id()]->mOvar[fval];
}

// @brief 入力の故障に対応した変数を得る．
// @param[in] node ノード
// @param[in] pos 入力番号
// @param[in] fval 故障値 (0/1)
VarId
MvalCnf::ifvar(const TpgNode* node,
	       ymuint pos,
	       int fval) const
{
  ASSERT_COND( has_faults(node) );
  ASSERT_COND( pos < node->fanin_num() );
  ASSERT_COND( fval == 0 || fval == 1 );
  return mFdMapArray[node->id()]->mIvar[pos * 2 + fval];
}

// @brief 出力の故障検出用の変数を割り当てる．
// @param[in] node ノード
// @param[in] fval 故障値 (0/1)
// @param[in] fdvar 変数番号
void
MvalCnf::set_ofvar(const TpgNode* node,
		   int fval,
		   VarId fdvar)
{
  ASSERT_COND( fval == 0 || fval == 1 );
  FdMap* fdmap = setup_fdmap(node);
  fdmap->mOvar[fval] = fdvar;
}

// @brief 入力の故障検出用の変数を割り当てる．
// @param[in] node ノード
// @param[in] pos 入力番号
// @param[in] fval 故障値 (0/1)
// @param[in] fdvar 変数番号
void
MvalCnf::set_ifvar(const TpgNode* node,
		   ymuint pos,
		   int fval,
		   VarId fdvar)
{
  ASSERT_COND( pos < node->fanin_num() );
  ASSERT_COND( fval == 0 || fval == 1 );
  FdMap* fdmap = setup_fdmap(node);
  fdmap->mIvar[pos * 2 + fval] = fdvar;
}

// @brief FdMap を初期化する．
// @param[in] node ノード
MvalCnf::FdMap*
MvalCnf::setup_fdmap(const TpgNode* node)
{
  ymuint id = node->id();
  FdMap* fdmap = mFdMapArray[id];
  if ( fdmap == NULL ) {
    ymuint ni = node->fanin_num();
    fdmap = new FdMap(ni);
    fdmap->mOvar[0] = kVarIdIllegal;
    fdmap->mOvar[1] = kVarIdIllegal;
    for (ymuint i = 0; i < ni; ++ i) {
      fdmap->mIvar[i * 2 + 0] = kVarIdIllegal;
      fdmap->mIvar[i * 2 + 1] = kVarIdIllegal;
    }
    mFdMapArray[id] = fdmap;
  }
  return fdmap;
}


END_NAMESPACE_YM_SATPG
