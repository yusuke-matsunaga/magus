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

  /// @brief 関数を得る．
  /// @param[in] findex 関数インデックス
  const Function*
  func(Ymsl_INT findex) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数テーブル
  vector<const Function*> mFuncTable;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLEXECUTABLE_H
