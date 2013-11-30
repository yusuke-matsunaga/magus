#ifndef ZDDVAR_H
#define ZDDVAR_H

/// @file ZddVar.h
/// @brief ZddVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Zdd.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddVar ZddVar.h "ZddVar.h"
/// @brief 変数の情報を格納しておくクラス
//////////////////////////////////////////////////////////////////////
class ZddVar
{
  friend class ZddMgrImpl;

private:

  // コンストラクタ
  ZddVar(VarId id);

  // デストラクタ
  ~ZddVar();


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
  ZddVar* mNext;

  // ハッシュ表中の次の要素を指すポインタ
  ZddVar* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
ZddVar::ZddVar(VarId id) :
  mId(id),
  mLevel(id.val()),
  mMark(0),
  mLink(NULL)
{
}

// デストラクタ
inline
ZddVar::~ZddVar()
{
}

// 変数番号を得る．
inline
VarId
ZddVar::varid() const
{
  return mId;
}

// レベルを得る．
inline
ymuint
ZddVar::level() const
{
  return mLevel;
}

END_NAMESPACE_YM_ZDD

#endif // ZDDVAR_H
