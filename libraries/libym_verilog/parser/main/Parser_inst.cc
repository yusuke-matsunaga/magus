/// @file libym_verilog/parser/main/Parser_inst.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser_item.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Parser.h"

#include "PtiFactory.h"
#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtStmt.h"
#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// gate/module/UDP instance の生成
//////////////////////////////////////////////////////////////////////

// @brief gate instance 文のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] type primitive の型
PtItem*
Parser::new_GateH(const FileRegion& fr,
		  tVpiPrimType type)
{
  return mFactory.new_GateH(fr, type, get_inst_array());
}

// @brief gate instance 文のヘッダの生成 (strength付き)
// @param[in] fr ファイル位置の情報
// @param[in] type primitive の型
// @param[in] strength 信号強度
PtItem*
Parser::new_GateH(const FileRegion& fr,
		  tVpiPrimType type,
		  PtStrength* strength)
{
  return mFactory.new_GateH(fr, type, strength, get_inst_array());
}

// @brief gate instance 文のヘッダの生成 (遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] type primitive の型
// @param[in] delay 遅延値
PtItem*
Parser::new_GateH(const FileRegion& fr,
		  tVpiPrimType type,
		  PtDelay* delay)
{
  return mFactory.new_GateH(fr, type, delay, get_inst_array());
}

// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] type primitive の型
// @param[in] strength 信号強度
// @param[in] delay 遅延値
PtItem*
Parser::new_GateH(const FileRegion& fr,
		  tVpiPrimType type,
		  PtStrength* strength,
		  PtDelay* delay)
{
  return mFactory.new_GateH(fr, type, strength, delay, get_inst_array());
}

// @brief module instance/UDP instance 文のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] def_name 定義名
PtItem*
Parser::new_MuH(const FileRegion& fr,
		const char* def_name)
{
  reg_defname(def_name);
  return mFactory.new_MuH(fr, def_name, get_inst_array());
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
// @param[in] fr ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] strength 信号強度
PtItem*
Parser::new_MuH(const FileRegion& fr,
		const char* def_name,
		PtStrength* strength)
{
  reg_defname(def_name);
  return mFactory.new_MuH(fr, def_name, strength, get_inst_array());
}

// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] delay 遅延値
PtItem*
Parser::new_MuH(const FileRegion& fr,
		const char* def_name,
		PtDelay* delay)
{
  reg_defname(def_name);
  return mFactory.new_MuH(fr, def_name, delay, get_inst_array());
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
// @param[in] fr ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] strength 信号強度
// @param[in] delay 遅延値
PtItem*
Parser::new_MuH(const FileRegion& fr,
		const char* def_name,
		PtStrength* strength,
		PtDelay* delay)
{
  reg_defname(def_name);
  return mFactory.new_MuH(fr, def_name, strength, delay, get_inst_array());
}

// @brief module instance/UDP instance 文のヘッダの生成
// @param[in] fr ファイル位置の情報
// @param[in] def_name 定義名
// @param[in] con_array ポート割り当てリスト
PtItem*
Parser::new_MuH(const FileRegion& fr,
		const char* def_name,
		PtrList<PtConnection>* con_list)
{
  reg_defname(def_name);
  return mFactory.new_MuH(fr, def_name, to_array(con_list), get_inst_array());
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] con_list ポート割り当ての配列
void
Parser::new_Inst(const FileRegion& fr,
		 PtrList<PtConnection>* con_list)
{
  add_inst( mFactory.new_Inst(fr, to_array(con_list)) );
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1 ポート割り当て
void
Parser::new_Inst(const FileRegion& fr,
		 PtExpr* expr1)
{
  add_inst( mFactory.new_Inst(fr, expr1) );
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2 ポート割り当て
void
Parser::new_Inst(const FileRegion& fr,
		 PtExpr* expr1,
		 PtExpr* expr2)
{
  add_inst( mFactory.new_Inst(fr, expr1, expr2) );
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2, expr3 ポート割り当て
void
Parser::new_Inst(const FileRegion& fr,
		 PtExpr* expr1,
		 PtExpr* expr2,
		 PtExpr* expr3)
{
  add_inst( mFactory.new_Inst(fr, expr1, expr2, expr3) );
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
void
Parser::new_Inst(const FileRegion& fr,
		 PtExpr* expr1,
		 PtExpr* expr2,
		 PtExpr* expr3,
		 PtExpr* expr4)
{
  add_inst( mFactory.new_Inst(fr, expr1, expr2, expr3, expr4) );
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] con_list ポート割り当ての配列
void
Parser::new_InstN(const FileRegion& fr,
		  const char* name,
		  PtrList<PtConnection>* con_list)
{
  add_inst( mFactory.new_InstN(fr, name, to_array(con_list)) );
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1 ポート割り当て
void
Parser::new_InstN(const FileRegion& fr,
		  const char* name,
		  PtExpr* expr1)
{
  add_inst( mFactory.new_InstN(fr, name, expr1) );
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2 ポート割り当て
void
Parser::new_InstN(const FileRegion& fr,
		  const char* name,
		  PtExpr* expr1,
		  PtExpr* expr2)
{
  add_inst( mFactory.new_InstN(fr, name, expr1, expr2) );
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2, expr3 ポート割り当て
void
Parser::new_InstN(const FileRegion& fr,
		  const char* name,
		  PtExpr* expr1,
		  PtExpr* expr2,
		  PtExpr* expr3)
{
  add_inst( mFactory.new_InstN(fr, name, expr1, expr2, expr3) );
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
void
Parser::new_InstN(const FileRegion& fr,
		  const char* name,
		  PtExpr* expr1,
		  PtExpr* expr2,
		  PtExpr* expr3,
		  PtExpr* expr4)
{
  add_inst( mFactory.new_InstN(fr, name, expr1, expr2, expr3, expr4) );
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] con_list ポート割り当ての配列
void
Parser::new_InstV(const FileRegion& fr,
		  const char* name,
		  PtExpr* left,
		  PtExpr* right,
		  PtrList<PtConnection>* con_list)
{
  add_inst( mFactory.new_InstV(fr, name, left, right, to_array(con_list)) );
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1 ポート割り当て
void
Parser::new_InstV(const FileRegion& fr,
		  const char* name,
		  PtExpr* left,
		  PtExpr* right,
		  PtExpr* expr1)
{
  add_inst( mFactory.new_InstV(fr, name, left, right, expr1) );
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2 ポート割り当て
void
Parser::new_InstV(const FileRegion& fr,
		  const char* name,
		  PtExpr* left,
		  PtExpr* right,
		  PtExpr* expr1,
		  PtExpr* expr2)
{
  add_inst( mFactory.new_InstV(fr, name, left, right, expr1, expr2) );
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2, expr3 ポート割り当て
void
Parser::new_InstV(const FileRegion& fr,
		  const char* name,
		  PtExpr* left,
		  PtExpr* right,
		  PtExpr* expr1,
		  PtExpr* expr2,
		  PtExpr* expr3)
{
  add_inst( mFactory.new_InstV(fr, name, left, right, expr1, expr2, expr3) );
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
void
Parser::new_InstV(const FileRegion& fr,
		  const char* name,
		  PtExpr* left,
		  PtExpr* right,
		  PtExpr* expr1,
		  PtExpr* expr2,
		  PtExpr* expr3,
		  PtExpr* expr4)
{
  add_inst( mFactory.new_InstV(fr, name, left, right,
			       expr1, expr2, expr3, expr4) );
}

// @brief instance リストに要素を追加する．
inline
void
Parser::add_inst(PtInst* inst)
{
  mInstList.push_back(inst);
}

// @brief instance リストを配列に変換する．
inline
PtInstArray
Parser::get_inst_array()
{
  return mInstList.to_array(mAlloc);
}

END_NAMESPACE_YM_VERILOG
