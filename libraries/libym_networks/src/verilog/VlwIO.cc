
/// @file VlwIO.cc
/// @brief VlwIO およびその派生クラスの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlwIO.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス VlwInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力ストリーム
VlwInput::VlwInput(VlWriter& writer) :
  VlwBase(writer)
{
  writer.begin_input();
}

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
VlwInput::VlwInput(VlWriter& writer,
		   int msb,
		   int lsb) :
  VlwBase(writer)
{
  writer.begin_input(msb, lsb);
}

// @brief デストラクタ
VlwInput::~VlwInput()
{
  writer().end_input();
}


//////////////////////////////////////////////////////////////////////
// クラス VlwOutput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力ストリーム
VlwOutput::VlwOutput(VlWriter& writer) :
  VlwBase(writer)
{
  writer.begin_output();
}

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
VlwOutput::VlwOutput(VlWriter& writer,
		     int msb,
		     int lsb) :
  VlwBase(writer)
{
  writer.begin_output(msb, lsb);
}


// @brief デストラクタ
VlwOutput::~VlwOutput()
{
  writer().end_output();
}


//////////////////////////////////////////////////////////////////////
// クラス VlwInout
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力ストリーム
VlwInout::VlwInout(VlWriter& writer) :
  VlwBase(writer)
{
  writer.begin_inout();
}

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
VlwInout::VlwInout(VlWriter& writer,
		   int msb,
		   int lsb) :
  VlwBase(writer)
{
  writer.begin_input(msb, lsb);
}

// @brief デストラクタ
VlwInout::~VlwInout()
{
  writer().end_inout();
}


//////////////////////////////////////////////////////////////////////
// クラス VlwWire
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力ストリーム
VlwWire::VlwWire(VlWriter& writer) :
  VlwBase(writer)
{
  writer.begin_wire();
}

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
VlwWire::VlwWire(VlWriter& writer,
		 int msb,
		 int lsb) :
  VlwBase(writer)
{
  writer.begin_wire(msb, lsb);
}

// @brief デストラクタ
VlwWire::~VlwWire()
{
  writer().end_wire();
}


//////////////////////////////////////////////////////////////////////
// クラス VlwReg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力ストリーム
VlwReg::VlwReg(VlWriter& writer) :
  VlwBase(writer)
{
  writer.begin_reg();
}

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
VlwReg::VlwReg(VlWriter& writer,
	       int msb,
	       int lsb) :
  VlwBase(writer)
{
  writer.begin_reg(msb, lsb);
}

// @brief デストラクタ
VlwReg::~VlwReg()
{
  writer().end_reg();
}

END_NAMESPACE_YM_NETWORKS
