#ifndef YMSLCOMPILER_H
#define YMSLCOMPILER_H

/// @file YmslCompiler.h
/// @brief YmslCompiler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/File.h"
#include "YmUtils/IDO.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslCompiler YmslCompiler.h "YmslCompiler.h"
/// @brief YMSL 用のコンパイラ
//////////////////////////////////////////////////////////////////////
class YmslCompiler
{
public:

  /// @brief コンストラクタ
  YmslCompiler();

  /// @brief デストラクタ
  ~YmslCompiler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コンパイルする．
  /// @param[in] ido 入力データ
  /// @param[in] name モジュール名
  /// @return コンパイルしたモジュールを返す．
  ///
  /// エラーが起きたら NULL を返す．
  VsmModule*
  compile(IDO& ido,
	  ShString name);

  /// @brief モジュールを import する．
  /// @param[in] name モジュール名
  /// @return モジュールを返す．
  ///
  /// エラーが起きたら NULL を返す．
  VsmModule*
  import(ShString name);

  /// @brief サーチパスの先頭に path を追加する．
  /// @param[in] path 追加するパス
  /// @note path は ':' を含んでいても良い
  void
  add_searchpath_top(const string& path);

  /// @brief サーチパスの末尾に path を追加する．
  /// @param[in] path 追加するパス
  /// @note path は ':' を含んでいても良い
  void
  add_searchpath_end(const string& path);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュールのサーチパスリスト
  SearchPathList mPathList;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLCOMPILER_H
