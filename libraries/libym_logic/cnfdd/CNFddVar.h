#ifndef CNFDDVAR_H
#define CNFDDVAR_H

/// @file CNFddVar.h
/// @brief CNFddVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/CNFdd.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class CNFddVar CNFddVar.h "CNFddVar.h"
/// @brief 変数の情報を格納しておくクラス
//////////////////////////////////////////////////////////////////////
class CNFddVar
{
  friend class CNFddMgrImpl;

private:

  // コンストラクタ
  CNFddVar(VarId id);

  // デストラクタ
  ~CNFddVar();


public:

  // 変数番号を得る．
  VarId
  varid() const;

  // レベルを得る．
  ymuint
  level() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号
  VarId mId;

  // レベル
  ymuint32 mLevel;

  // 作業用のマーク
  int mMark;

  // 変数リスト中の次の要素を指すポインタ
  CNFddVar* mNext;

  // ハッシュ表中の次の要素を指すポインタ
  CNFddVar* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
CNFddVar::CNFddVar(VarId id) :
  mId(id),
  mLevel(id.val()),
  mMark(0),
  mLink(NULL)
{
}

// デストラクタ
inline
CNFddVar::~CNFddVar()
{
}

// 変数番号を得る．
inline
VarId
CNFddVar::varid() const
{
  return mId;
}

// レベルを得る．
inline
ymuint
CNFddVar::level() const
{
  return mLevel;
}

END_NAMESPACE_YM_CNFDD

#endif // CNFDDVAR_H
