#ifndef LEXPNODEPTR_H
#define LEXPNODEPTR_H

/// @file LexpNodePtr.h
/// @brief LexpNodePtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/LogExpr.h"


BEGIN_NAMESPACE_YM_LEXP

//////////////////////////////////////////////////////////////////////
/// @class LexpNodePtr LexpNode.h "LexpNode.h*
/// @brief LexpNode 用のスマートポインタ
//////////////////////////////////////////////////////////////////////
class LexpNodePtr
{
  friend class LexpNodePtrList;

public:

  /// @brief コンストラクタ
  /// @param[in] ptr Cポインタ
  LexpNodePtr(const LexpNode* ptr = NULL);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  LexpNodePtr(const LexpNodePtr& src);

  /// @brief 代入演算子
  /// @param[in] src 代入元のオブジェクト
  const LexpNodePtr&
  operator=(const LexpNodePtr& src);

  /// @brief デストラクタ
  ~LexpNodePtr();


public:

  /// @brief 中身を取り出す演算子
  operator const LexpNode*() const;

  /// @brief 中身を取り出す演算子
  const LexpNode*
  operator->() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のポインタ
  const LexpNode* mPtr;

};

typedef vector<LexpNodePtr> LexpNodeList;
typedef vector<LexpNodePtr>::iterator LexpNode_itr;
typedef vector<LexpNodePtr>::const_iterator LexpNode_citr;
#if __GNUC__ == 4 && __GNUC_MINOR__ >= 6
typedef unordered_map<VarId, LexpNodePtr> VarLexpNodeMap;
#else
typedef hash_map<VarId, LexpNodePtr> VarLexpNodeMap;
#endif



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ptr Cポインタ
inline
LexpNodePtr::LexpNodePtr(const LexpNode* ptr) :
  mPtr(ptr)
{
  if ( mPtr ) {
    mPtr->inc_ref();
  }
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
LexpNodePtr::LexpNodePtr(const LexpNodePtr& src) :
  mPtr(src.mPtr)
{
  if ( mPtr ) {
    mPtr->inc_ref();
  }
}

// @brief 代入演算子
// @param[in] src 代入元のオブジェクト
inline
const LexpNodePtr&
LexpNodePtr::operator=(const LexpNodePtr& src)
{
  if ( src.mPtr ) {
    src.mPtr->inc_ref();
  }
  if ( mPtr ) {
    mPtr->dec_ref();
  }
  mPtr = src.mPtr;
  return *this;
}

// @brief デストラクタ
inline
LexpNodePtr::~LexpNodePtr()
{
  if ( mPtr ) {
    mPtr->dec_ref();
  }
}

// @brief 中身を取り出す演算子
inline
LexpNodePtr::operator const LexpNode*() const
{
  return mPtr;
}

// @brief 中身を取り出す演算子
inline
const LexpNode*
LexpNodePtr::operator->() const
{
  return mPtr;
}

END_NAMESPACE_YM_LEXP

#endif // LEXPNODEPTR_H
