#ifndef EXPRNODEPTR_H
#define EXPRNODEPTR_H

/// @file ExprNodePtr.h
/// @brief ExprNodePtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/Expr.h"


BEGIN_NAMESPACE_YM_EXPR

//////////////////////////////////////////////////////////////////////
/// @class ExprNodePtr ExprNode.h "ExprNode.h*
/// @brief ExprNode 用のスマートポインタ
//////////////////////////////////////////////////////////////////////
class ExprNodePtr
{
  friend class ExprNodePtrList;

public:

  /// @brief コンストラクタ
  /// @param[in] ptr Cポインタ
  ExprNodePtr(const ExprNode* ptr = NULL);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  ExprNodePtr(const ExprNodePtr& src);

  /// @brief 代入演算子
  /// @param[in] src 代入元のオブジェクト
  const ExprNodePtr&
  operator=(const ExprNodePtr& src);

  /// @brief デストラクタ
  ~ExprNodePtr();


public:

  /// @brief 中身を取り出す演算子
  operator const ExprNode*() const;

  /// @brief 中身を取り出す演算子
  const ExprNode*
  operator->() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のポインタ
  const ExprNode* mPtr;

};

typedef vector<ExprNodePtr> ExprNodeList;
typedef vector<ExprNodePtr>::iterator ExprNode_itr;
typedef vector<ExprNodePtr>::const_iterator ExprNode_citr;
typedef unordered_map<VarId, ExprNodePtr> VarExprNodeMap;



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ptr Cポインタ
inline
ExprNodePtr::ExprNodePtr(const ExprNode* ptr) :
  mPtr(ptr)
{
  if ( mPtr ) {
    mPtr->inc_ref();
  }
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
ExprNodePtr::ExprNodePtr(const ExprNodePtr& src) :
  mPtr(src.mPtr)
{
  if ( mPtr ) {
    mPtr->inc_ref();
  }
}

// @brief 代入演算子
// @param[in] src 代入元のオブジェクト
inline
const ExprNodePtr&
ExprNodePtr::operator=(const ExprNodePtr& src)
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
ExprNodePtr::~ExprNodePtr()
{
  if ( mPtr ) {
    mPtr->dec_ref();
  }
}

// @brief 中身を取り出す演算子
inline
ExprNodePtr::operator const ExprNode*() const
{
  return mPtr;
}

// @brief 中身を取り出す演算子
inline
const ExprNode*
ExprNodePtr::operator->() const
{
  return mPtr;
}

END_NAMESPACE_YM_EXPR

#endif // EXPRNODEPTR_H
