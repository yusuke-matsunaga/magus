#ifndef LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFUNCREP_H
#define LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFUNCREP_H

/// @file libym_techmap/cellmap/libdump/LdFuncRep.h
/// @brief LdFuncRep のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM_CELLMAP_LIBDUMP

class LdFunc;

//////////////////////////////////////////////////////////////////////
/// @class LdFuncRep LdFuncRep.h "LdFuncRep.h"
/// @brief NPN同値類の代表関数を表すクラス
//////////////////////////////////////////////////////////////////////
class LdFuncRep
{
  friend class LdFuncMgr;

private:

  /// @brief コンストラクタ
  LdFuncRep();

  /// @brief デストラクタ
  ~LdFuncRep();


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
LdFuncRep::LdFuncRep()
{
}

// @brief デストラクタ
inline
LdFuncRep::~LdFuncRep()
{
}

// @brief ID番号を返す．
inline
ymuint
LdFuncRep::id() const
{
  return mId;
}

// @brief 代表関数を返す．
inline
const TvFunc&
LdFuncRep::rep_func() const
{
  return mFunc;
}

// @brief 自分の同値類に属する関数の数を返す．
inline
ymuint
LdFuncRep::func_num() const
{
  return mFuncList.size();
}

// @brief 関数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
inline
const LdFunc*
LdFuncRep::func(ymuint pos) const
{
  return mFuncList[pos];
}

END_NAMESPACE_YM_CELLMAP_LIBDUMP

#endif // LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFUNCREP_H
