#ifndef LDLOGICCLASS_H
#define LDLOGICCLASS_H

/// @file LdLogicClass.h
/// @brief LdLogicClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

class LdLogicGroup;

//////////////////////////////////////////////////////////////////////
/// @class LdLogicClass LdLogicClass.h "LdLogicClass.h"
/// @brief NPN同値類の代表関数を表すクラス
//////////////////////////////////////////////////////////////////////
class LdLogicClass
{
  friend class LdLogicMgr;

private:

  /// @brief コンストラクタ
  LdLogicClass();

  /// @brief デストラクタ
  ~LdLogicClass();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 入力ピン数の取得
  ymuint
  input_num() const;

  /// @brief 出力ピン数の取得
  ymuint
  output_num() const;

  /// @brief 出力の論理関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  TvFunc
  logic_function(ymuint pos) const;

  /// @brief 代表関数を返す．
  const TvFunc&
  rep_func() const;

  /// @brief このクラスに属しているグループのリストを返す．
  const vector<LdLogicGroup*>&
  group_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 入力数
  ymuint32 mInputNum;

  // 出力数
  ymuint32 mOutputNum;

  // 出力の論理関数の配列
  TvFunc* mLogicFunctionArray;

  // グループのリスト
  vector<LdLogicGroup*> mGroupList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LdLogicClass::LdLogicClass()
{
}

// @brief デストラクタ
inline
LdLogicClass::~LdLogicClass()
{
}

// @brief ID番号を返す．
inline
ymuint
LdLogicClass::id() const
{
  return mId;
}

// @brief 入力ピン数の取得
inline
ymuint
LdLogicClass::input_num() const
{
  return mInputNum;
}

// @brief 出力ピン数の取得
inline
ymuint
LdLogicClass::output_num() const
{
  return mOutputNum;
}

// @brief 代表関数を返す．
inline
const TvFunc&
LdLogicClass::rep_func() const
{
  return mFunc;
}

// @brief このクラスに属しているグループのリストを返す．
inline
const vector<LdLogicGroup*>&
LdLogicClass::group_list() const
{
  return mGroupList;
}

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDLOGICCLASS_H
