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

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  VlwInput(VlWriter& writer,
	   int msb,
	   int lsb);

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

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  VlwOutput(VlWriter& writer,
	    int msb,
	    int lsb);

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

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  VlwInout(VlWriter& writer,
	   int msb,
	   int lsb);

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

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  VlwWire(VlWriter& writer,
	  int msb,
	  int lsb);

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

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  VlwReg(VlWriter& writer,
	 int msb,
	 int lsb);

  /// @brief デストラクタ
  ~VlwReg();

};

END_NAMESPACE_YM_NETWORKS

#endif // VLWINPUT_H
