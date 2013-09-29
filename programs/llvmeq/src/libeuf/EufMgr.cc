
/// @file EufMgr.cc
/// @brief EufMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufMgr.h"
#include "EufBinMgr.h"
#include "EufFuncMgr.h"
#include "EufVarMgr.h"
#include "EufNode.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EufMgr::EufMgr()
{
  mVarMgr = new EufVarMgr;
  mFuncMgr = new EufFuncMgr;
  mBinMgr = new EufBinMgr;
  mLastId = 0;
}

// @brief デストラクタ
EufMgr::~EufMgr()
{
  delete mVarMgr;
  delete mFuncMgr;
  delete mBinMgr;
}

// @brief 変数を生成する．
// @param[in] name 変数名
EufNode*
EufMgr::new_variable(const string& name)
{
  EufNode* var = mVarMgr->find(name);
  if ( var == NULL ) {
    var = mVarMgr->new_variable(mLastId, name);
    ++ mLastId;
  }
  return var;
}

BEGIN_NONAMESPACE

// EufNode を整列させるためのファンクタ
struct Lt
{
  bool
  operator()(EufNode* left,
	     EufNode* right)
  {
    return left->id() < right->id();
  }
};

END_NONAMESPACE

// @brief 関数を生成する．
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
// @param[in] sym_flag 対称関数の時 true にするフラグ
// @note sym_flag が true の時は arg_list の順番を正規化する．
EufNode*
EufMgr::new_function(const string& name,
		     const vector<EufNode*>& arg_list,
		     bool sym_flag)
{
  // ソートするかもしれないのでローカルな変数にコピーする．
  vector<EufNode*> tmp_list(arg_list.begin(), arg_list.end());

  if ( sym_flag ) {
    // ID番号でソートする．
    sort(tmp_list.begin(), tmp_list.end(), Lt());
  }

  EufNode* node = mFuncMgr->find(name, tmp_list);
  if ( node == NULL ) {
    node = mFuncMgr->new_function(mLastId, name, tmp_list);
    ++ mLastId;
  }

  return node;
}

// @brief 等価演算子を生成する．
// @param[in] left 左辺式
// @param[in] right 右辺式
// @note 場合によっては左辺式と右辺式は交換される．
EufNode*
EufMgr::new_equality(EufNode* left,
		     EufNode* right)
{
  // left と right は対称なので正規化する．
  if ( left->id() > right->id() ) {
    EufNode* tmp_node = left;
    left = right;
    right = tmp_node;
  }

  EufNode* node = mBinMgr->find_equality(left, right);
  if ( node == NULL ) {
    node = mBinMgr->new_equality(mLastId, left, right);
    ++ mLastId;
  }
  return node;
}

// @brief 連言(conjunction)を生成する．
// @param[in] left 左辺式
// @param[in] right 右辺式
EufNode*
EufMgr::new_conjunction(EufNode* left,
			EufNode* right)
{
  // left と right は対称なので正規化する．
  if ( left->id() > right->id() ) {
    EufNode* tmp_node = left;
    left = right;
    right = tmp_node;
  }

  EufNode* node = mBinMgr->find_conjunction(left, right);
  if ( node == NULL ) {
    node = mBinMgr->new_conjunction(mLastId, left, right);
    ++ mLastId;
  }
  return node;
}

// @brief 選言(disjunction)を生成する．
// @param[in] left 左辺式
// @param[in] right 右辺式
EufNode*
EufMgr::new_disjunction(EufNode* left,
			EufNode* right)
{
  // left と right は対称なので正規化する．
  if ( left->id() > right->id() ) {
    EufNode* tmp_node = left;
    left = right;
    right = tmp_node;
  }

  EufNode* node = mBinMgr->find_disjunction(left, right);
  if ( node == NULL ) {
    node = mBinMgr->new_disjunction(mLastId, left, right);
    ++ mLastId;
  }
  return node;
}

// @brief 否定(negation)を生成する．
// @param[in] operand 式
EufNode*
EufMgr::new_negation(EufNode* operand)
{
  EufNode* node = mBinMgr->find_negation(operand);
  if ( node == NULL ) {
    node = mBinMgr->new_negation(mLastId, operand);
    ++ mLastId;
  }
  return node;
}

END_NAMESPACE_YM_LLVMEQ
