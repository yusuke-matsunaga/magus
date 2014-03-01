
/// @file VlwAssign.cc
/// @brief VlwAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlwAssign.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス VlwAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
VlwAssign::VlwAssign(VlWriter& writer) :
  VlwBase(writer)
{
  writer.begin_assign();
  writer.put_sp();
}

// @brief デストラクタ
VlwAssign::~VlwAssign()
{
  writer().end_assign();
}

// @brief 左辺を出力する．
// @param[in] name 左辺の信号線名
void
VlwAssign::put_lhs(const char* name)
{
  writer().put_idstr(name);
  writer().put_str(" = ");
}

// @brief 左辺を出力する．
// @param[in] name 左辺の信号線名
void
VlwAssign::put_lhs(const string& name)
{
  put_lhs(name.c_str());
}

// @brief 左辺を出力する．
// @param[in] name 左辺の信号線名
// @param[in] lsb 範囲の LSB
// @param[in] msb 範囲の MSB
void
VlwAssign::put_lhs(const char* name,
		   int lsb,
		   int msb)
{
  writer().put_idstr(name);
  writer().put_str("[");
  writer().put_num(lsb);
  writer().put_str(":");
  writer().put_num(msb);
  writer().put_str("] = ");
}

// @brief 左辺を出力する．
// @param[in] name 左辺の信号線名
// @param[in] lsb 範囲の LSB
// @param[in] msb 範囲の MSB
void
VlwAssign::put_lhs(const string& name,
		   int lsb,
		   int msb)
{
  put_lhs(name.c_str(), lsb, msb);
}

// @brief 右辺を出力する．
// @param[in] expr 論理式
// @param[in] name_map 変数名の対応表
// @note expr 中の i 番めの変数の名前が name_map[i] に入っている．
void
VlwAssign::put_rhs(const Expr& expr,
		   const VarStrMap& name_map)
{
  writer().put_expr(expr, name_map);
}

END_NAMESPACE_YM_NETWORKS
