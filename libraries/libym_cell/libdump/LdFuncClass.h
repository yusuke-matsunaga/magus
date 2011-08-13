#ifndef LDFUNCCLASS_H
#define LDFUNCCLASS_H

/// @file LdFuncClass.h
/// @brief LdFuncClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

class LdFunc;

//////////////////////////////////////////////////////////////////////
/// @class LdFuncClass LdFuncClass.h "LdFuncClass.h"
/// @brief NPN同値類の代表関数を表すクラス
//////////////////////////////////////////////////////////////////////
class LdFuncClass
{
  friend class LdFuncMgr;

private:

  /// @brief コンストラクタ
  LdFuncClass();

  /// @brief デストラクタ
  ~LdFuncClass();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 代表関数を返す．
  const TvFunc&
  rep_func() const;

  /// @brief 自分の同値類に属する関数の数を返す．
  ymuint
  func_num() const;

  /// @brief 関数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
  const LdFunc*
  func(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 代表関数
  TvFunc mFunc;

  // 関数のリスト
  vector<LdFunc*> mFuncList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LdFuncClass::LdFuncClass()
{
}

// @brief デストラクタ
inline
LdFuncClass::~LdFuncClass()
{
}

// @brief ID番号を返す．
inline
ymuint
LdFuncClass::id() const
{
  return mId;
}

// @brief 代表関数を返す．
inline
const TvFunc&
LdFuncClass::rep_func() const
{
  return mFunc;
}

// @brief 自分の同値類に属する関数の数を返す．
inline
ymuint
LdFuncClass::func_num() const
{
  return mFuncList.size();
}

// @brief 関数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
inline
const LdFunc*
LdFuncClass::func(ymuint pos) const
{
  return mFuncList[pos];
}

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDFUNCCLASS_H
