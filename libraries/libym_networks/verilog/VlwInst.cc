
/// @file VlwInst.cc
/// @brief VlwInst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "VlwInst.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス VlwInst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
// @param[in] def_name インスタンスの定義名
VlwInst::VlwInst(VlWriter& writer,
		 const char* def_name) :
  VlwBase(writer)
{
  writer.begin_inst(def_name);

  mFirst = true;
}

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
// @param[in] def_name インスタンスの定義名
VlwInst::VlwInst(VlWriter& writer,
		 const string& def_name) :
  VlwBase(writer)
{
  writer.begin_inst(def_name);

  mFirst = true;
}

// @brief デストラクタ
VlwInst::~VlwInst()
{
  writer().end_inst();
}

// @brief 要素間のデリミタを出力する．
void
VlwInst::put_delimiter()
{
  if ( mFirst ) {
    mFirst = false;
  }
  else {
    writer().put_str(",");
  }
  writer().put_sp();
}


//////////////////////////////////////////////////////////////////////
// クラス VlwInstElem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] inst 親の VlwInst
// @param[in] inst_name インスタンス名
VlwInstElem::VlwInstElem(VlwInst& inst,
			 const char* inst_name) :
  VlwBase(inst.writer())
{
  inst.put_delimiter();

  writer().begin_inst_elem(inst_name);

  mFirst = true;
}

// @brief コンストラクタ
// @param[in] inst 親の VlwInst
// @param[in] inst_name インスタンス名
VlwInstElem::VlwInstElem(VlwInst& inst,
			 const string& inst_name) :
  VlwBase(inst.writer())
{
  inst.put_delimiter();

  writer().begin_inst_elem(inst_name);

  mFirst = true;
}

// @brief デストラクタ
VlwInstElem::~VlwInstElem()
{
  writer().end_inst_elem();
}

// @brief ピン割り当てを出力する．
// @param[in] pin_name ピン名
// @param[in] net_name ピンに接続するネット名
void
VlwInstElem::put_pinassign(const char* pin_name,
			   const char* net_name)
{
  if ( mFirst ) {
    mFirst = false;
  }
  else {
    writer().put_str(",");
  }
  writer().put_sp();

  writer().put_str(".");
  writer().put_idstr(pin_name);
  writer().put_str("(");
  writer().put_idstr(net_name);
  writer().put_str(")");
}

// @brief ピン割り当てを出力する．
// @param[in] pin_name ピン名
// @param[in] net_name ピンに接続するネット名
void
VlwInstElem::put_pinassign(const char* pin_name,
			   const string& net_name)
{
  put_pinassign(pin_name, net_name.c_str());
}

// @brief ピン割り当てを出力する．
// @param[in] pin_name ピン名
// @param[in] net_name ピンに接続するネット名
void
VlwInstElem::put_pinassign(const string& pin_name,
			   const char* net_name)
{
  put_pinassign(pin_name.c_str(), net_name);
}

// @brief ピン割り当てを出力する．
// @param[in] pin_name ピン名
// @param[in] net_name ピンに接続するネット名
void
VlwInstElem::put_pinassign(const string& pin_name,
			   const string& net_name)
{
  put_pinassign(pin_name.c_str(), net_name.c_str());
}

END_NAMESPACE_YM_NETWORKS
