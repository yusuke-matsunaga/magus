/// @file libym_verilog/parser/main/Parser_misc.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser_misc.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Parser.h"

#include "ym_verilog/pt/PtMisc.h"

#include "PtMgr.h"
#include "PtiFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief ディレイコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] value 遅延を表す式
// @return 生成されたディレイコントロール
const PtControl*
Parser::new_DelayControl(const FileRegion& fr,
			 const PtExpr* value)
{
  return mFactory.new_DelayControl(fr, value);
}

// @brief イベントコントロールの生成
// @param[in] fr ファイル位置の情報
// @return 生成されたイベントコントロール
const PtControl*
Parser::new_EventControl(const FileRegion& fr)
{
  return mFactory.new_EventControl(fr);
}

// @brief イベントコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] event_name イベントを表す名前
// @return 生成されたイベントコントロール
const PtControl*
Parser::new_EventControl(const FileRegion& fr,
			 const char* event_name,
			 const FileRegion& name_loc)
{
  const PtExpr* expr = new_Primary(name_loc, event_name);
  return mFactory.new_EventControl(fr, expr);
}

// @brief イベントコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] event_name イベントを表す階層名
// @return 生成されたイベントコントロール
const PtControl*
Parser::new_EventControl(const FileRegion& fr,
			 PuHierName* event_name,
			 const FileRegion& name_loc)
{
  const PtExpr* expr = new_Primary(name_loc, event_name);
  return mFactory.new_EventControl(fr, expr);
}

// @brief イベントコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] event_list イベントのリスト
// @return 生成されたイベントコントロール
const PtControl*
Parser::new_EventControl(const FileRegion& fr,
			 PtrList<const PtExpr>* event_list)
{
  return mFactory.new_EventControl(fr, to_array(event_list));
}

// @brief リピートコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 繰り返し数を表す式
// @return 生成されたリピートコントロール
const PtControl*
Parser::new_RepeatControl(const FileRegion& fr,
			  const PtExpr* expr)
{
  return mFactory.new_RepeatControl(fr, expr);
}

// @brief リピートコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] rep 繰り返し数を表す式
// @param[in] event_name 繰り返しの単位となるイベント名
// @return 生成されたリピートコントロール
const PtControl*
Parser::new_RepeatControl(const FileRegion& fr,
			  const PtExpr* rep,
			  const char* event_name,
			  const FileRegion& name_loc)
{
  const PtExpr* expr = new_Primary(name_loc, event_name);
  return mFactory.new_RepeatControl(fr, rep, expr);
}

// @brief リピートコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] rep 繰り返し数を表す式
// @param[in] event 繰り返しの単位となるイベント階層名
// @return 生成されたリピートコントロール
const PtControl*
Parser::new_RepeatControl(const FileRegion& fr,
			  const PtExpr* rep,
			  PuHierName* event_name,
			  const FileRegion& name_loc)
{
  const PtExpr* expr = new_Primary(name_loc, event_name);
  return mFactory.new_RepeatControl(fr, rep, expr);
}

// @brief リピートコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] rep 繰り返し数を表す式
// @param[in] event_list 繰り返しの単位となるイベントのリスト
// @return 生成されたリピートコントロール
const PtControl*
Parser::new_RepeatControl(const FileRegion& fr,
			  const PtExpr* rep,
			  PtrList<const PtExpr>* event_list)
{
  return mFactory.new_RepeatControl(fr, rep, to_array(event_list));
}

// @brief 順序つき結合子の生成
// @param[in] expr 結合する式
// @return 生成された結合子
const PtConnection*
Parser::new_OrderedCon(const PtExpr* expr)
{
  return mFactory.new_OrderedCon(expr);
}

// @brief 順序つき結合子の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 結合する式
// @return 生成された結合子
const PtConnection*
Parser::new_OrderedCon(const FileRegion& fr,
		       const PtExpr* expr,
		       PtrList<const PtAttrInst>* ai_list)
{
  const PtConnection* con = mFactory.new_OrderedCon(fr, expr);
  reg_attrinst(con, ai_list);
  return con;
}

// @brief 名前付き結合子の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr 結合する式
// @return 生成された結合子
const PtConnection*
Parser::new_NamedCon(const FileRegion& fr,
		     const char* name,
		     const PtExpr* expr,
		     PtrList<const PtAttrInst>* ai_list)
{
  const PtConnection* con = mFactory.new_NamedCon(fr, name, expr);
  reg_attrinst(con, ai_list);
  return con;
}

// @brief strength の生成
// @param[in] fr ファイル位置の情報
// @param[in] value0 '0' の強度
// @param[in] value1 '1' の強度
// @return 生成された strength
const PtStrength*
Parser::new_Strength(const FileRegion& fr,
		     tVpiStrength value0,
		     tVpiStrength value1)
{
  return mFactory.new_Strength(fr, value0, value1);
}

// @brief charge strength の生成
// @param[in] fr ファイル位置の情報
// @param[in] value 強度
// @return 生成された strength
const PtStrength*
Parser::new_Strength(const FileRegion& fr,
		     tVpiStrength value)
{
  return mFactory.new_Strength(fr, value);
}

// @brief 遅延値の生成 (1つの値)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @return 生成された遅延値
const PtDelay*
Parser::new_Delay(const FileRegion& fr,
		  const PtExpr* value1)
{
  return mFactory.new_Delay(fr, value1);
}

// @brief 遅延値の生成 (2つの値)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @return 生成された遅延値
const PtDelay*
Parser::new_Delay(const FileRegion& fr,
		  const PtExpr* value1,
		  const PtExpr* value2)
{
  return mFactory.new_Delay(fr, value1, value2);
}

// @brief 遅延値の生成 (3つの値)
// @param[in] fr ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @return 生成された遅延値
const PtDelay*
Parser::new_Delay(const FileRegion& fr,
		  const PtExpr* value1,
		  const PtExpr* value2,
		  const PtExpr* value3)
{
  return mFactory.new_Delay(fr, value1, value2, value3);
}

// @brief attribute instance の生成
// @param[in] fr ファイル位置の情報
// @param[in] as_array attribute spec のリスト
// @return 生成された attribute instance
const PtAttrInst*
Parser::new_AttrInst(const FileRegion& fr,
		     PtrList<const PtAttrSpec>* as_list)
{
  return mFactory.new_AttrInst(fr, to_array(as_list));
}

// @brief attribute spec の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr 値
// @return 生成された attribute spec
const PtAttrSpec*
Parser::new_AttrSpec(const FileRegion& fr,
		     const char* name,
		     const PtExpr* expr)
{
  return mFactory.new_AttrSpec(fr, name, expr);
}

END_NAMESPACE_YM_VERILOG
