#ifndef VLWINPUT_H
#define VLWINPUT_H

/// @file VlwIO.h
/// @brief VlwIOおよびその派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "VlwBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

#if 0
//////////////////////////////////////////////////////////////////////
/// @class VlwIO VlwIO.h "VlwIO.h"
/// @brief VlwInput/VlwOutput に共通の基底クラス
//////////////////////////////////////////////////////////////////////
class VlwIO :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  VlwIO(VlWriter& writer);

  /// @brief デストラクタ
  ~VlwIO();

};
#endif

//////////////////////////////////////////////////////////////////////
/// @class VlwInput VlwIO.h "VlwIO.h"
/// @brief 入力宣言のためのクラス
//////////////////////////////////////////////////////////////////////
class VlwInput :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  VlwInput(VlWriter& writer);

  /// @brief デストラクタ
  ~VlwInput();

};


//////////////////////////////////////////////////////////////////////
/// @class VlwOutput VlwIO.h "VlwIO.h"
/// @brief 出力宣言のためのクラス
//////////////////////////////////////////////////////////////////////
class VlwOutput :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  VlwOutput(VlWriter& writer);

  /// @brief デストラクタ
  ~VlwOutput();

};


//////////////////////////////////////////////////////////////////////
/// @class VlwInout VlwIO.h "VlwIO.h"
/// @brief 入出力宣言のためのクラス
//////////////////////////////////////////////////////////////////////
class VlwInout :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  VlwInout(VlWriter& writer);

  /// @brief デストラクタ
  ~VlwInout();

};


//////////////////////////////////////////////////////////////////////
/// @class VlwWire VlwIO.h "VlwIO.h"
/// @brief wire宣言のためのクラス
//////////////////////////////////////////////////////////////////////
class VlwWire :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  VlwWire(VlWriter& writer);

  /// @brief デストラクタ
  ~VlwWire();

};


//////////////////////////////////////////////////////////////////////
/// @class VlwReg VlwIO.h "VlwIO.h"
/// @brief reg宣言のためのクラス
//////////////////////////////////////////////////////////////////////
class VlwReg :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  VlwReg(VlWriter& writer);

  /// @brief デストラクタ
  ~VlwReg();

};

END_NAMESPACE_YM_NETWORKS

#endif // VLWINPUT_H
