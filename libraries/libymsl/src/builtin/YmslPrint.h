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
  YmslPrint(ShString name);

  /// @brief デストラクタ
  virtual
  ~YmslPrint();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本当の実行関数
  /// @param[in] arg_list 引数のリスト
  /// @param[in] ret_val 返り値を格納する変数
  ///
  /// 実際の派生クラスが実装する必要がある．
  virtual
  void
  _execute(const vector<YmslValue>& arg_list,
	   YmslValue& ret_val) const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLPRINT_H
