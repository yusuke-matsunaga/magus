#ifndef YMSLFUNC_H
#define YMSLFUNC_H

/// @file YmslFunc.h
/// @brief YmslFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslFunc YmslFunc.h "YmslFunc.h"
/// @brief YMSL の関数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  YmslFunc(ShString name,
	   const vector<YmslVar*>& arg_list);

  /// @brief デストラクタ
  virtual
  ~YmslFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数名を返す．
  ShString
  name() const;

  /// @brief 引数の数
  ymuint
  arg_num() const;

  /// @brief 引数を得る．
  /// @param[in] pos 位置 ( 0 <= pos < arg_num() )
  YmslVar*
  arg(ymuint pos) const;

  /// @brief 組み込み関数の時 true を返す．
  virtual
  bool
  is_builtin() const = 0;

  /// @brief 組み込み関数の時の実行関数
  /// @param[in] exectable 実行環境
  virtual
  void
  execute(YmslExecutable& executable) const = 0;

  /// @brief YMSL 関数の場合の局所変数の数を返す．
  virtual
  ymuint
  local_var_num() const = 0;

  /// @brief YMSL 関数の場合の局所変数を返す．
  /// @param[in] lindex 変数インデックス
  virtual
  YmslVar*
  local_var(ymuint lindex) const = 0;

  /// @brief YMSL 関数の場合のバイトコードのサイズを返す．
  virtual
  ymuint
  byte_codes_size() const = 0;

  /// @brief YMSL 関数の場合のバイトコードの先頭アドレスを返す．
  virtual
  const ymuint8*
  byte_codes_top() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数名
  ShString mName;

  // 引数の数
  ymuint mArgNum;

  // 引数のリスト
  YmslVar** mArgList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLFUNC_H
