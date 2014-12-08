#ifndef YMSLUSERFUNC_H
#define YMSLUSERFUNC_H

/// @file YmslUserFunc.h
/// @brief YmslUserFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslFunc.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslUserFunc YmslUserFunc.h "YmslUserFunc.h"
/// @brief ユーザー関数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslUserFunc :
  public YmslFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  /// @param[in] local_var_list ローカル変数のリスト
  /// @param[in] byte_codes バイトコード
  YmslUserFunc(ShString name,
	       const vector<YmslVar*>& arg_list,
	       const vector<YmslVar*>& local_var_list,
	       const vector<ymuint8>& byte_codes);

  /// @brief デストラクタ
  virtual
  ~YmslUserFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の時 true を返す．
  virtual
  bool
  is_builtin() const;

  /// @brief 組み込み関数の時の実行関数
  virtual
  void
  execute() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ローカル変数の数
  ymuint mLocalVarNum;

  // ローカル変数のテーブル
  YmslVar** mLocalVarTable;

  // バイトコードのサイズ
  ymuint mByteCodesSize;

  // バイトコードの本体
  ymuint8* mByteCodes;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLUSERFUNC_H
