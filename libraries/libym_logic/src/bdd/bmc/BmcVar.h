#ifndef BMCVAR_H
#define BMCVAR_H

/// @file BmcVar.h
/// @brief BmcVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/Bdd.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BmcVar BmcVar.h "Bmcvar.h"
/// @brief 変数の情報を格納しておくクラス
//////////////////////////////////////////////////////////////////////
class BmcVar
{
  friend class BddMgrClassic;

public:

  // レベルを得る．
  ymuint
  level() const
  {
    return mLevel;
  }


private:

  // コンストラクタ
  BmcVar(ymuint level) :
    mLevel(level),
    mMark(0),
    mLink(NULL)
  {
  }

  // デストラクタ
  ~BmcVar()
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // レベル
  ymuint mLevel;

  // 作業用のマーク
  int mMark;

  // compose用にBDDの枝を入れておくメンバ
  BddEdge mCompEdge;

  // 変数リスト中の次の要素を指すポインタ
  BmcVar* mNext;

  // ハッシュ表中の次の要素を指すポインタ
  BmcVar* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_BDD

#endif // BMCVAR_H
