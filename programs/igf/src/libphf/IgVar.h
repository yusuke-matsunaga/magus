#ifndef IGVAR_H
#define IGVAR_H

/// @file IgVar.h
/// @brief IgVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "RegVect.h"
#include "ym_logic/bdd_nsdef.h"
#include "ym_logic/Bdd.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class IgVar IgVar.h "IgVar.h"
/// @brief 通常の変数と合成変数を表すクラス
//////////////////////////////////////////////////////////////////////
class IgVar
{
public:

  /// @brief コンストラクタ
  /// @param[in] var もとの変数
  /// @param[in] ds_func distinguished set を表す BDD
  IgVar(const Variable* var,
	Bdd ds_func,
	ymuint ds_count);

  /// @brief デストラクタ
  ~IgVar();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief もとの Variable を返す．
  const Variable*
  variable() const;

  /// @brief distinguished set を返す．
  Bdd
  ds_func() const;

  /// @brief distinguished set の要素数を返す．
  ymuint
  ds_count() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // もとの変数
  const Variable* mVariable;

  // この変数で区別できるペアのリストを表す BDD
  Bdd mDsFunc;

  // mDsFunc の要素数
  ymuint32 mDsCount;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var もとの変数
// @param[in] ds_func distinguished set を表す BDD
inline
IgVar::IgVar(const Variable* var,
	     Bdd ds_func,
	     ymuint ds_count) :
  mVariable(var),
  mDsFunc(ds_func),
  mDsCount(ds_count)
{
}

// @brief デストラクタ
inline
IgVar::~IgVar()
{
}

// @brief もとの Variable を返す．
inline
const Variable*
IgVar::variable() const
{
  return mVariable;
}

// @brief distinguished set を返す．
inline
Bdd
IgVar::ds_func() const
{
  return mDsFunc;
}

// @brief distinguished set の要素数を返す．
inline
ymuint
IgVar::ds_count() const
{
  return mDsCount;
}

END_NAMESPACE_YM_IGF

#endif // IGVAR_H
