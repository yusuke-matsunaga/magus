#ifndef YMSLBUILTINFUNC_H
#define YMSLBUILTINFUNC_H

/// @file YmslBuiltinFunc.h
/// @brief YmslBuiltinFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslFunc.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslBuiltinFunc YmslBuiltinFunc.h "YmslBuiltinFunc.h"
/// @brief 組み込み関数を表すクラス
///
/// 実際にはこのクラスを継承したクラスを作り，execute() を実装する
/// 必要がある．
//////////////////////////////////////////////////////////////////////
class YmslBuiltinFunc :
  public YmslFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  YmslBuiltinFunc(ShString name,
		  const vector<YmslVar*>& arg_list);

  /// @brief デストラクタ
  virtual
  ~YmslBuiltinFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の時 true を返す．
  virtual
  bool
  is_builtin() const;

  /// @brief YMSL 関数の場合の局所変数の数を返す．
  virtual
  ymuint
  local_var_num() const;

  /// @brief YMSL 関数の場合の局所変数を返す．
  /// @param[in] lindex 変数インデックス
  virtual
  YmslVar*
  local_var(ymuint lindex) const;

  /// @brief YMSL 関数の場合のバイトコードのサイズを返す．
  virtual
  ymuint
  byte_codes_size() const;

  /// @brief YMSL 関数の場合のバイトコードの先頭アドレスを返す．
  virtual
  const ymuint8*
  byte_codes_top() const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLBUILTINFUNC_H
