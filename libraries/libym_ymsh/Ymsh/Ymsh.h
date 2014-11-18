﻿#ifndef YMSH_YMSH_H
#define YMSH_YMSH_H

/// @file Ymsh/Ymsh.h
/// @brief Ymsh のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Ymsh/ymsh_nsdef.h"


BEGIN_NAMESPACE_YM_YMSH

class YmshImpl;

//////////////////////////////////////////////////////////////////////
/// @class YmshBase Ymsh.h "Ymsh/Ymsh.h"
/// @brief 簡単なインタプリタを実現するクラス
//////////////////////////////////////////////////////////////////////
class YmshBase
{
public:

  /// @brief コンストラクタ
  YmshBase();

  /// @brief デストラクタ
  ~YmshBase();


public:
  //////////////////////////////////////////////////////////////////////
  // メイン関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンドループを実行する．
  void
  run();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実装クラスを返す．
  YmshImpl*
  impl();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  YmshImpl* mImpl;

};


//////////////////////////////////////////////////////////////////////
/// @class Ymsh Ymsh.h "Ymsh/Ymsh.h"
/// @brief 標準的な機能を組み込んだインタプリタを実現するクラス
//////////////////////////////////////////////////////////////////////
class Ymsh :
  public YmshBase
{
public:

  /// @brief コンストラクタ
  Ymsh();

  /// @brief デストラクタ
  ~Ymsh();

};

END_NAMESPACE_YM_YMSH

#endif // YMSH_YMSH_H