#ifndef YMSLPRINT_H
#define YMSLPRINT_H

/// @file YmslPrint.h
/// @brief YmslPrint のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslBuiltinFunc.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslPrint YmslPrint.h "YmslPrint.h"
/// @brief print 関数
//////////////////////////////////////////////////////////////////////
class YmslPrint :
  public YmslBuiltinFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  YmslPrint(ShString name,
	    const vector<YmslVar*>& arg_list);

  /// @brief デストラクタ
  virtual
  ~YmslPrint();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の時の実行関数
  /// @param[in] exectable 実行環境
  virtual
  void
  execute(YmslExecutable& executable) const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLPRINT_H
