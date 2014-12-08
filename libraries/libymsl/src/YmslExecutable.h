#ifndef YMSLEXECUTABLE_H
#define YMSLEXECUTABLE_H

/// @file YmslExecutable.h
/// @brief YmslExecutable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslExecutalbe YmslExecutable.h "YmslExecutable.h"
/// @brief YMSL の実行環境を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslExecutable
{
public:

  /// @brief コンストラクタ
  YmslExecutable();

  /// @brief デストラクタ
  ~YmslExecutable();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief top-level の関数を得る．
  YmslFunc*
  top_level() const;

  /// @brief 関数を得る．
  /// @param[in] findex 関数インデックス
  YmslFunc*
  func(ymuint findex) const;

  /// @brief グローバル変数を得る．
  /// @param[in] gindex 変数インデックス
  YmslVar*
  global_var(ymuint gindex) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数テーブル
  vector<YmslFunc*> mFuncTable;

  // グローバル変数テーブル
  vector<YmslVar*> mGlobalVarTable;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLEXECUTABLE_H
