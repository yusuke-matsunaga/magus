
/// @file VlwIO.cc
/// @brief VlwIO およびその派生クラスの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "VlwIO.h"


BEGIN_NAMESPACE_YM_NETWORKS

#if 0
//////////////////////////////////////////////////////////////////////
// クラス VlwIO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力ストリーム
VlwIO::VlwIO(VlWriter& writer) :
  VlwBase(writer)
{
}

// @brief デストラクタ
VlwIO::~VlwIO()
{
}

// @brief 要素を出力する．
// @param[in] name 名前
void
VlwIO::put_elem(const char* name)
{
  put_comma();
  put_idstr(name);
}

// @brief 要素を出力する．
// @param[in] name 名前
void
VlwIO::put_elem(const string& name)
{
  put_comma();
  put_idstr(name);
}

// @brief 要素を出力する(範囲付き)
// @param[in] name 名前
// @param[in] lsb LSB
// @param[in] msb MSB
void
VlwIO::put_elem(const char* name,
		int lsb,
		int msb)
{
  put_comma();
  put_idstr(name);
  put_str("[");
  put_num(lsb);
  put_str(":");
  put_num(msb);
  put_str("]");
}

// @brief 要素を出力する(範囲付き)
// @param[in] name 名前
// @param[in] lsb LSB
// @param[in] msb MSB
void
VlwIO::put_elem(const string& name,
		int lsb,
		int msb)
{
  put_comma();
  put_idstr(name);
  put_str("[");
  put_num(lsb);
  put_str(":");
  put_num(msb);
  put_str("]");
}

// @brief カンマを出力する．
// @note 初回は出力しない．
void
VlwIO::put_comma()
{
  put_str(mComma);
  mComma = ", ";
}
#endif

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

// @brief デストラクタ
VlwReg::~VlwReg()
{
  writer().end_reg();
}

END_NAMESPACE_YM_NETWORKS
