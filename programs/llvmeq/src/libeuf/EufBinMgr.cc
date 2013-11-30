
/// @file EufBinMgr.cc
/// @brief EufBinMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufBinMgr.h"
#include "EufEq.h"
#include "EufCon.h"
#include "EufDis.h"
#include "EufNeg.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufBinMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EufBinMgr::EufBinMgr() :
  mAlloc(sizeof(EufEq), 1024)
{
  mNum = 0;
  mTableSize = 0;
  mTable = NULL;
  expand(1024);
}

// @brief デストラクタ
EufBinMgr::~EufBinMgr()
{
  delete[] mTable;
}

// @brief 等価式を探す．
// @param[in] left, right 左辺式，右辺式
// @note 見つからなければ NULL を返す．
EufNode*
EufBinMgr::find_equality(EufNode* left,
			 EufNode* right) const
{
  EufNode* node = find(static_cast<ymuint>(EufNode::kEq), left, right);
  return node;
}

// @brief 連言(conjunction)を探す．
// @param[in] left, right 左辺式，右辺式
// @note 見つからなければ NULL を返す．
EufNode*
EufBinMgr::find_conjunction(EufNode* left,
			    EufNode* right) const
{
  EufNode* node = find(static_cast<ymuint>(EufNode::kCon), left, right);
  return node;
}

// @brief 選言(disjunction)を探す．
// @param[in] left, right 左辺式，右辺式
// @note 見つからなければ NULL を返す．
EufNode*
EufBinMgr::find_disjunction(EufNode* left,
			    EufNode* right) const
{
  EufNode* node = find(static_cast<ymuint>(EufNode::kDis), left, right);
  return node;
}

// @brief 否定(negation)を探す．
// @param[in] left オペランド
// @note 見つからなければ NULL を返す．
EufNode*
EufBinMgr::find_negation(EufNode* left) const
{
  EufNode* node = find(static_cast<ymuint>(EufNode::kNeg), left, left);
  return node;
}

// @brief 等価式を生成する．
// @param[in] id ID番号
// @param[in] vid SatSolver 用の変数番号
// @param[in] left, right 左辺式，右辺式
EufNode*
EufBinMgr::new_equality(ymuint id,
			VarId vid,
			EufNode* left,
			EufNode* right)
{
  void* p = mAlloc.get_memory(sizeof(EufEq));
  EufBin* node = new (p) EufEq(id, vid, left, right);

  put(node);

  return node;
}

// @brief 連言(conjunction)を生成する．
// @param[in] id ID番号
// @param[in] vid SatSolver 用の変数番号
// @param[in] left, right 左辺式，右辺式
EufNode*
EufBinMgr::new_conjunction(ymuint id,
			   VarId vid,
			   EufNode* left,
			   EufNode* right)
{
  void* p = mAlloc.get_memory(sizeof(EufCon));
  EufBin* node = new (p) EufCon(id, vid, left, right);

  put(node);

  return node;
}

// @brief 選言(disjunction)を生成する．
// @param[in] id ID番号
// @param[in] vid SatSolver 用の変数番号
// @param[in] left, right 左辺式，右辺式
EufNode*
EufBinMgr::new_disjunction(ymuint id,
			   VarId vid,
			   EufNode* left,
			   EufNode* right)
{
  void* p = mAlloc.get_memory(sizeof(EufDis));
  EufBin* node = new (p) EufDis(id, vid, left, right);

  put(node);

  return node;
}

// @brief 否定(negation)を生成する．
// @param[in] id ID番号
// @param[in] vid SatSolver 用の変数番号
// @param[in] left オペランド
EufNode*
EufBinMgr::new_negation(ymuint id,
			VarId vid,
			EufNode* left)
{
  void* p = mAlloc.get_memory(sizeof(EufNeg));
  EufBin* node = new (p) EufNeg(id, vid, left);

  put(node);

  return node;
}

// @brief ノードを探す．
// @param[in] type 型
// @param[in] left, right 左辺式，右辺式
// @note 見つからなければ NULL を返す．
EufNode*
EufBinMgr::find(ymuint type,
		EufNode* left,
		EufNode* right) const
{
  ymuint h = hash(type, left, right);
  for (EufBin* node = mTable[h]; node != NULL; node = node->mLink) {
    if ( static_cast<ymuint>(node->type()) == type &&
	 node->left() == left && node->right() == right ) {
      return node;
    }
  }
  return NULL;
}

// @brief ノードを追加する．
void
EufBinMgr::put(EufBin* node)
{
  if ( mNum >= mNextLimit ) {
    expand(mTableSize * 2);
  }

  ymuint h = hash(static_cast<ymuint>(node->type()), node->left(), node->right());
  node->mLink = mTable[h];
  mTable[h] = node;

  ++ mNum;
}

// @brief ハッシュテーブルを拡大する．
void
EufBinMgr::expand(ymuint req_size)
{
  ymuint old_size = mTableSize;
  EufBin** old_table = mTable;

  mTableSize = req_size;
  mTable = new EufBin*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (EufBin* node = old_table[i]; node != NULL; ) {
	EufBin* tmp_node = node;
	node = tmp_node->mLink;

	ymuint h = hash(static_cast<ymuint>(tmp_node->type()),
			tmp_node->left(), tmp_node->right());
	tmp_node->mLink = mTable[h];
	mTable[h] = tmp_node;
      }
    }
    delete [] old_table;
  }
}

// @brief ハッシュ値を計算する．
// @param[in] left, right 左辺式，右辺式
ymuint
EufBinMgr::hash(ymuint type,
		EufNode* left,
		EufNode* right) const
{
  return (type * 2047 + left->id() * 1023 + right->id()) % mTableSize;
}

END_NAMESPACE_YM_LLVMEQ
