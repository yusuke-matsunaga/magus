#ifndef CPTFACTORY_H
#define CPTFACTORY_H

/// @file CptFactory.h
/// @brief CptFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PtiFactory.h"
#include "YmUtils/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

class CptDefStub;

//////////////////////////////////////////////////////////////////////
/// @class CptFactory CptFactory.h "CptFactory.h"
/// @brief 省メモリ版 Pt クラス(Cpt クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class CptFactory :
  public PtiFactory
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリ確保用のアロケータ
  CptFactory(Alloc& alloc);

  /// @brief デストラクタ
  virtual
  ~CptFactory();


public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief combinational UDP の生成
  virtual
  const PtUdp*
  new_CmbUdp(const FileRegion& fr,
	     const char* name,
	     PtiPortArray port_array,
	     PtIOHeadArray io_array,
	     PtUdpEntryArray entry_array);

  /// @brief sequential UDP の生成
  virtual
  const PtUdp*
  new_SeqUdp(const FileRegion& fr,
	     const char* name,
	     PtiPortArray port_array,
	     PtIOHeadArray io_array,
	     const PtExpr* init_value,
	     PtUdpEntryArray entry_array);

  /// @brief combinational UDP 用のテーブルエントリの生成
  virtual
  const PtUdpEntry*
  new_UdpEntry(const FileRegion& fr,
	       PtUdpValueArray input_array,
	       const PtUdpValue* output);

  /// @brief sequential UDP 用のテーブルエントリの生成
  virtual
  const PtUdpEntry*
  new_UdpEntry(const FileRegion& fr,
	       PtUdpValueArray input_array,
	       const PtUdpValue* current,
	       const PtUdpValue* output);

  /// @brief UDP のテーブルエントリの要素の値の生成
  virtual
  const PtUdpValue*
  new_UdpValue(const FileRegion& fr,
	       char symbol);

  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] symbol1, symbol2 シンボル
  /// @return 生成された値
  virtual
  const PtUdpValue*
  new_UdpValue(const FileRegion& fr,
	       char symbol1,
	       char symbol2);


  //////////////////////////////////////////////////////////////////////
  // モジュール関係
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールの生成
  virtual
  const PtModule*
  new_Module(const FileRegion& fr,
	     const char* name,
	     bool macro,
	     bool is_cell,
	     bool is_protected,
	     int time_unit,
	     int time_precision,
	     tVpiNetType net_type,
	     tVpiUnconnDrive unconn,
	     tVpiDefDelayMode delay,
	     int decay,
	     bool explicit_name,
	     bool portfaults,
	     bool suppress_faults,
	     const string& config,
	     const string& library,
	     const string& cell,
	     PtDeclHeadArray paramport_array,
	     PtiPortArray port_array,
	     PtIOHeadArray iohead_array,
	     PtDeclHeadArray declhead_array,
	     PtItemArray item_array);


  //////////////////////////////////////////////////////////////////////
  // ポート関係
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] ext_name ポート名 (空文字列の場合もある)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(const FileRegion& file_region,
	   const char* ext_name = NULL);

  /// @brief ポートの生成 (ポート参照式があるポート)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] portref ポートに接続している式 (ポート参照式)
  /// @param[in] ext_name ポート名 (空文字列の場合もある)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(const FileRegion& fr,
	   const PtExpr* portref,
	   const char* ext_name);

  /// @brief ポートの生成 (連結型のポート参照式があるポート)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] portref ポートに接続している式 (ポート参照式)
  /// @param[in] portref_array ポートに接続している式 (ポート参照式)の配列
  /// @param[in] ext_name ポート名 (空文字列の場合もある)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(const FileRegion& fr,
	   const PtExpr* portref,
	   PtExprArray port_ref_array,
	   const char* ext_name);


  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief IO 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_IOHead(const FileRegion& fr,
	     tPtIOType type,
	     bool sign);

  /// @brief IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_RegIOHead(const FileRegion& fr,
		tPtIOType type,
		bool sign);

  /// @brief IO 宣言のヘッダの生成 (ネット型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] net_type 補助的なネット型
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_NetIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiNetType net_type,
		bool sign);

  /// @brief IO 宣言のヘッダの生成 (変数型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] var_type 補助的な変数型
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_VarIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiVarType var_type);

  /// @brief 範囲付きの IO 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_IOHead(const FileRegion& fr,
	     tPtIOType type,
	     bool sign,
	     const PtExpr* left,
	     const PtExpr* right);

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_RegIOHead(const FileRegion& fr,
		tPtIOType type,
		bool sign,
		const PtExpr* left,
		const PtExpr* right);

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] net_type 補助的なネット型
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_NetIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiNetType net_type,
		bool sign,
		const PtExpr* left,
		const PtExpr* right);

  /// @brief IO 宣言の要素の生成
  /// @param[in] file_region ファイル上の位置
  /// @param[in] name 名前
  virtual
  PtIOItem*
  new_IOItem(const FileRegion& file_region,
	     const char* name);

  /// @brief 初期値付き IO 宣言の要素の生成
  /// @param[in] file_region ファイル上の位置
  /// @param[in] name 名前
  /// @param[in] init_value 初期値
  virtual
  PtIOItem*
  new_IOItem(const FileRegion& file_region,
	     const char* name,
	     const PtExpr* init_value);


  //////////////////////////////////////////////////////////////////////
  // その他の宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ宣言のヘッダの生成 (型指定なし)
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& fr);

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& fr,
	     bool sign,
	     const PtExpr* left,
	     const PtExpr* right);

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& fr,
	     tVpiVarType var_type);

  /// @brief local param 宣言のヘッダの生成 (型指定なし)
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr);

  /// @brief 範囲指定型 local param 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr,
		  bool sign,
		  const PtExpr* left,
		  const PtExpr* right);

  /// @brief 組み込み型 local param 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr,
		  tVpiVarType var_type);

  /// @brief specparam 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_SpecParamH(const FileRegion& fr);

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_SpecParamH(const FileRegion& fr,
		 const PtExpr* left,
		 const PtExpr* right);

  /// @brief イベント宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_EventH(const FileRegion& fr);

  /// @brief genvar 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_GenvarH(const FileRegion& fr);

  /// @brief 変数宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_VarH(const FileRegion& fr,
	   tVpiVarType var_type);

  /// @brief 1ビット型 reg 宣言のヘッダを生成の生成
  virtual
  PtiDeclHead*
  new_RegH(const FileRegion& fr,
	   bool sign);

  /// @brief ビットベクタ型 reg 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_RegH(const FileRegion& fr,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right);

  /// @brief 1ビット型 net 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign);

  /// @brief 1ビット型 net 宣言のヘッダの生成(strength あり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   const PtStrength* strength);

  /// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   const PtDelay* delay);

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   const PtStrength* strength,
	   const PtDelay* delay);

  /// @brief 範囲指定型 net 宣言のヘッダの生成
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtStrength* strength);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtDelay* delay);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtStrength* strength,
	   const PtDelay* delay);

  /// @brief 宣言要素の生成
  virtual
  const PtDeclItem*
  new_DeclItem(const FileRegion& fr,
	       const char* name);

  /// @brief 初期値付き宣言要素の生成
  virtual
  const PtDeclItem*
  new_DeclItem(const FileRegion& fr,
	       const char* name,
	       const PtExpr* init_value);

  /// @brief 配列型宣言要素の生成
  virtual
  const PtDeclItem*
  new_DeclItem(const FileRegion& fr,
	       const char* name,
	       PtRangeArray range_array);

  /// @brief 範囲の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] msb MSB を表す式
  /// @param[in] lsb LSB を表す式
  virtual
  const PtRange*
  new_Range(const FileRegion& fr,
	    const PtExpr* msb,
	    const PtExpr* lsb);


  //////////////////////////////////////////////////////////////////////
  // item 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  virtual
  const PtItem*
  new_DefParamH(const FileRegion& fr,
		PtDefParamArray elem_array);

  /// @brief defparam 文の要素の生成
  virtual
  const PtDefParam*
  new_DefParam(const FileRegion& fr,
	       const char* name,
	       const PtExpr* value);

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  virtual
  const PtDefParam*
  new_DefParam(const FileRegion& fr,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       const PtExpr* value);

  /// @brief continuous assign 文のヘッダの生成
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文のヘッダの生成 (strengthつき)
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  const PtStrength* strength,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文のヘッダの生成 (遅延付き)
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  const PtDelay* delay,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  const PtStrength* strength,
		  const PtDelay* delay,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文の生成
  virtual
  const PtContAssign*
  new_ContAssign(const FileRegion& fr,
		 const PtExpr* lhs,
		 const PtExpr* rhs);

  /// @brief initial 文の生成
  virtual
  const PtItem*
  new_Initial(const FileRegion& fr,
	      const PtStmt* body);

  /// @brief always 文の生成
  virtual
  const PtItem*
  new_Always(const FileRegion& fr,
	     const PtStmt* body);

  /// @brief task 文の生成
  virtual
  const PtItem*
  new_Task(const FileRegion& fr,
	   const char* name,
	   bool automatic,
	   PtIOHeadArray iohead_array,
	   PtDeclHeadArray declhead_array,
	   const PtStmt* stmt);

  /// @brief 1ビット型 function 文の生成
  virtual
  const PtItem*
  new_Function(const FileRegion& fr,
	       const char* name,
	       bool automatic,
	       bool sign,
	       PtIOHeadArray iohead_array,
	       PtDeclHeadArray declhead_array,
	       const PtStmt* stmt);

  /// @brief 範囲指定型 function 文の生成
  virtual
  const PtItem*
  new_SizedFunc(const FileRegion& fr,
		const char* name,
		bool automatic,
		bool sign,
		const PtExpr* left,
		const PtExpr* right,
		PtIOHeadArray iohead_array,
		PtDeclHeadArray declhead_array,
		const PtStmt* stmt);

  /// @brief 組み込み型 function 文の生成
  virtual
  const PtItem*
  new_TypedFunc(const FileRegion& fr,
		const char* name,
		bool automatic,
		bool sign,
		tVpiVarType func_type,
		PtIOHeadArray iohead_array,
		PtDeclHeadArray declhead_array,
		const PtStmt* stmt);

  /// @brief gate instance 文のヘッダの生成
  virtual
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    PtInstArray elem_array);

  /// @brief gate instance 文のヘッダの生成 (strength付き)
  virtual
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    const PtStrength* strength,
	    PtInstArray elem_array);

  /// @brief gate instance 文のヘッダの生成 (遅延付き)
  virtual
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    const PtDelay* delay,
	    PtInstArray elem_array);

  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  virtual
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    const PtStrength* strength,
	    const PtDelay* delay,
	    PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成
  virtual
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
  virtual
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  const PtStrength* strength,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
  virtual
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  const PtDelay* delay,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  virtual
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  const PtStrength* strength,
	  const PtDelay* delay,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成
  virtual
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtConnectionArray con_array,
	  PtInstArray elem_array);

  /// @brief module instance/UDP/gate instance の要素の生成
  virtual
  const PtInst*
  new_Inst(const FileRegion& fr,
	   PtConnectionArray con_array);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  virtual
  const PtInst*
  new_InstN(const FileRegion& fr,
	    const char* name,
	    PtConnectionArray con_array);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  virtual
  const PtInst*
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    PtConnectionArray con_array);

  /// @brief generate 文の生成
  virtual
  const PtItem*
  new_Generate(const FileRegion& fr,
	       PtDeclHeadArray declhead_array,
	       PtItemArray item_array);

  /// @brief generate block 文の生成
  virtual
  const PtItem*
  new_GenBlock(const FileRegion& fr,
	       PtDeclHeadArray declhead_array,
	       PtItemArray item_array);

  /// @brief 名前付き generate block 文の生成
  virtual
  const PtItem*
  new_GenBlock(const FileRegion& fr,
	       const char* name,
	       PtDeclHeadArray declhead_array,
	       PtItemArray item_array);

  /// @brief generate if 文の生成
  virtual
  const PtItem*
  new_GenIf(const FileRegion& fr,
	    const PtExpr* cond,
	    PtDeclHeadArray then_declhead_array,
	    PtItemArray then_item_array,
	    PtDeclHeadArray else_declhead_array,
	    PtItemArray else_item_array);

  /// @brief generate case 文の生成
  virtual
  const PtItem*
  new_GenCase(const FileRegion& fr,
	      const PtExpr* expr,
	      PtGenCaseItemArray item_array);

  /// @brief generate case の要素の生成
  virtual
  const PtGenCaseItem*
  new_GenCaseItem(const FileRegion& fr,
		  PtExprArray label_array,
		  PtDeclHeadArray declhead_array,
		  PtItemArray item_array);

  /// @brief generate for 文の生成
  virtual
  const PtItem*
  new_GenFor(const FileRegion& fr,
	     const char* loop_var,
	     const PtExpr* init_expr,
	     const PtExpr* cond,
	     const PtExpr* next_expr,
	     const char* block_name,
	     PtDeclHeadArray declhead_array,
	     PtItemArray item_array);

  /// @brief specify block item の生成
  virtual
  const PtItem*
  new_SpecItem(const FileRegion& fr,
	       tVpiSpecItemType id,
	       PtExprArray terminal_top);

  /// @brief path 仕様を生成する．
  virtual
  const PtItem*
  new_SpecPath(const FileRegion& fr,
	       tVpiSpecPathType id,
	       const PtExpr* expr,
	       const PtPathDecl* path_decl);

  /// @brief パス記述の生成
  virtual
  const PtPathDecl*
  new_PathDecl(const FileRegion& fr,
	       int edge,
	       PtExprArray input_array,
	       int input_pol,
	       int op,
	       PtExprArray output_array,
	       int output_pol,
	       const PtExpr* expr,
	       const PtPathDelay* path_delay);

  /// @brief path delay value の生成 (値が1個)
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value);

  /// @brief path delay value の生成 (値が2個)
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2);

  /// @brief path delay value の生成 (値が3個)
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2,
		const PtExpr* value3);

  /// @brief path delay value の生成 (値が6個)
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2,
		const PtExpr* value3,
		const PtExpr* value4,
		const PtExpr* value5,
		const PtExpr* value6);

  /// @brief path delay value の生成 (値が12個)
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2,
		const PtExpr* value3,
		const PtExpr* value4,
		const PtExpr* value5,
		const PtExpr* value6,
		const PtExpr* value7,
		const PtExpr* value8,
		const PtExpr* value9,
		const PtExpr* value10,
		const PtExpr* value11,
		const PtExpr* value12);


  //////////////////////////////////////////////////////////////////////
  // statement 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief disable 文の生成
  virtual
  const PtStmt*
  new_Disable(const FileRegion& fr,
	      const char* name);

  /// @brief disable 文の生成 (階層付き識別子)
  virtual
  const PtStmt*
  new_Disable(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name);

  /// @brief enable 文の生成
  virtual
  const PtStmt*
  new_Enable(const FileRegion& fr,
	     const char* name,
	     PtExprArray arg_array);

  /// @brief enable 文の生成 (階層付き識別子)
  virtual
  const PtStmt*
  new_Enable(const FileRegion& fr,
	     PtNameBranchArray nb_array,
	     const char* tail_name,
	     PtExprArray arg_array);

  /// @brief system task enable 文の生成
  virtual
  const PtStmt*
  new_SysEnable(const FileRegion& fr,
		const char* task_name,
		PtExprArray arg_array);

  /// @brief delay control 文の生成
  virtual
  const PtStmt*
  new_DcStmt(const FileRegion& fr,
	     const PtControl* delay,
	     const PtStmt* body);

  /// @brief event control 文の生成
  virtual
  const PtStmt*
  new_EcStmt(const FileRegion& fr,
	     const PtControl* event,
	     const PtStmt* body);

  /// @brief wait 文の生成
  virtual
  const PtStmt*
  new_Wait(const FileRegion& fr,
	   const PtExpr* cond,
	   const PtStmt* body);

  /// @brief assign 文の生成
  virtual
  const PtStmt*
  new_Assign(const FileRegion& fr,
	     const PtExpr* lhs,
	     const PtExpr* rhs);

  /// @brief control 付き assign 文の生成
  virtual
  const PtStmt*
  new_Assign(const FileRegion& fr,
	     const PtExpr* lhs,
	     const PtExpr* rhs,
	     const PtControl* control);

  /// @brief nonblocking assign 文の生成
  virtual
  const PtStmt*
  new_NbAssign(const FileRegion& fr,
	       const PtExpr* lhs,
	       const PtExpr* rhs);

  /// @brief control 付き nonblocking assign 文の生成
  virtual
  const PtStmt*
  new_NbAssign(const FileRegion& fr,
	       const PtExpr* lhs,
	       const PtExpr* rhs,
	       const PtControl* control);

  /// @brief event 文の生成
  virtual
  const PtStmt*
  new_EventStmt(const FileRegion& fr,
		const PtExpr* event);

  /// @brief null 文の生成
  virtual
  const PtStmt*
  new_NullStmt(const FileRegion& fr);

  /// @brief if 文の生成
  virtual
  const PtStmt*
  new_If(const FileRegion& fr,
	 const PtExpr* expr,
	 const PtStmt* then_body);

  /// @brief if 文の生成
  virtual
  const PtStmt*
  new_If(const FileRegion& fr,
	 const PtExpr* expr,
	 const PtStmt* then_body,
	 const PtStmt* else_body);

  /// @brief case 文の生成
  virtual
  const PtStmt*
  new_Case(const FileRegion& fr,
	   const PtExpr* expr,
	   PtCaseItemArray caseitem_array);

  /// @brief casex 文の生成
  virtual
  const PtStmt*
  new_CaseX(const FileRegion& fr,
	    const PtExpr* expr,
	    PtCaseItemArray caseitem_array);

  /// @brief casez 文の生成
  virtual
  const PtStmt*
  new_CaseZ(const FileRegion& fr,
	    const PtExpr* expr,
	    PtCaseItemArray caseitem_array);

  /// @brief case item の生成
  virtual
  const PtCaseItem*
  new_CaseItem(const FileRegion& fr,
	       PtExprArray label_array,
	       const PtStmt* body);

  /// @brief forever 文の生成
  virtual
  const PtStmt*
  new_Forever(const FileRegion& fr,
	      const PtStmt* body);

  /// @brief repeat 文の生成
  virtual
  const PtStmt*
  new_Repeat(const FileRegion& fr,
	     const PtExpr* expr,
	     const PtStmt* body);

  /// @brief while 文の生成
  virtual
  const PtStmt*
  new_While(const FileRegion& fr,
	    const PtExpr* cond,
	    const PtStmt* body);

  /// @brief for 文の生成
  virtual
  const PtStmt*
  new_For(const FileRegion& fr,
	  const PtStmt* init,
	  const PtExpr* cond,
	  const PtStmt* next,
	  const PtStmt* body);

  /// @brief procedural assign 文の生成
  virtual
  const PtStmt*
  new_PcAssign(const FileRegion& fr,
	       const PtExpr* lhs,
	       const PtExpr* rhs);

  /// @brief deassign 文の生成
  virtual
  const PtStmt*
  new_Deassign(const FileRegion& fr,
	       const PtExpr* lhs);

  /// @brief force 文の生成
  virtual
  const PtStmt*
  new_Force(const FileRegion& fr,
	    const PtExpr* lhs,
	    const PtExpr* rhs);

  /// @brief release 文の生成
  virtual
  const PtStmt*
  new_Release(const FileRegion& fr,
	      const PtExpr* lhs);

  /// @brief parallel block の生成
  virtual
  const PtStmt*
  new_ParBlock(const FileRegion& fr,
	       PtStmtArray stmt_array);

  /// @brief 名前付き parallel block の生成
  virtual
  const PtStmt*
  new_NamedParBlock(const FileRegion& fr,
		    const char* name,
		    PtDeclHeadArray declhead_array,
		    PtStmtArray stmt_array);

  /// @brief sequential block の生成
  virtual
  const PtStmt*
  new_SeqBlock(const FileRegion& fr,
	       PtStmtArray stmt_array);

  /// @brief 名前付き sequential block の生成
  virtual
  const PtStmt*
  new_NamedSeqBlock(const FileRegion& fr,
		    const char* name,
		    PtDeclHeadArray declhead_array,
		    PtStmtArray stmt_array);


  //////////////////////////////////////////////////////////////////////
  // expression 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  virtual
  const PtExpr*
  new_Opr(const FileRegion& fr,
	  tVlOpType type,
	  const PtExpr* opr);

  /// @brief 二項演算子の生成
  virtual
  const PtExpr*
  new_Opr(const FileRegion& fr,
	  tVlOpType type,
	  const PtExpr* opr1,
	  const PtExpr* opr2);

  /// @brief 三項演算子の生成
  virtual
  const PtExpr*
  new_Opr(const FileRegion& fr,
	  tVlOpType type,
	  const PtExpr* opr1,
	  const PtExpr* opr2,
	  const PtExpr* opr3);

  /// @brief concatination 演算子の生成
  virtual
  const PtExpr*
  new_Concat(const FileRegion& fr,
	     PtExprArray expr_top);

  /// @brief multi-concatination 演算子の生成
  virtual
  const PtExpr*
  new_MultiConcat(const FileRegion& fr,
		  PtExprArray expr_top);

  /// @brief min/typ/max delay 演算子の生成
  virtual
  const PtExpr*
  new_MinTypMax(const FileRegion& fr,
		const PtExpr* val0,
		const PtExpr* val1,
		const PtExpr* val2);

  /// @brief primary の生成
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name);

  /// @brief インデックス付き primary の生成
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      PtExprArray index_array);

  /// @brief 範囲指定付き primary の生成
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief インデックスと範囲指定付き primary の生成
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      PtExprArray index_array,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief primary の生成 (階層付き)
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name);

  /// @brief インデックス付き primary の生成 (階層付き)
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      PtExprArray index_array);

  /// @brief 範囲指定付き primary の生成 (階層付き)
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      PtExprArray index_array,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief constant primary の生成
  virtual
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       PtExprArray index_array);

  /// @brief 範囲指定付き constant primary の生成
  virtual
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       tVpiRangeMode mode,
	       const PtExpr* left,
	       const PtExpr* right);

  /// @brief インデックス付き constant primary の生成 (階層付き)
  virtual
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       PtExprArray index_array);

  /// @brief function call の生成
  virtual
  const PtExpr*
  new_FuncCall(const FileRegion& fr,
	       const char* name,
	       PtExprArray arg_array);

  /// @brief function call の生成 (階層付き)
  virtual
  const PtExpr*
  new_FuncCall(const FileRegion& fr,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       PtExprArray arg_array);

  /// @brief system function call の生成
  virtual
  const PtExpr*
  new_SysFuncCall(const FileRegion& fr,
		  const char* name,
		  PtExprArray arg_array);

  /// @brief 整数型の定数の生成
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       ymuint value);

  /// @brief 整数型の定数の生成
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       const char* value);

  /// @brief 基底付き整数型の定数の生成
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       tVpiConstType const_type,
	       const char* value);

  /// @brief サイズと基底付き定数の生成
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       ymuint size,
	       tVpiConstType const_type,
	       const char* value);

  /// @brief 実数型の定数の生成
  virtual
  const PtExpr*
  new_RealConst(const FileRegion& fr,
		double value);

  /// @brief 文字列型の定数の生成
  virtual
  const PtExpr*
  new_StringConst(const FileRegion& fr,
		  const char* value);


  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  virtual
  const PtControl*
  new_DelayControl(const FileRegion& fr,
		   const PtExpr* value);

  /// @brief イベントコントロールの生成
  virtual
  const PtControl*
  new_EventControl(const FileRegion& fr,
		   PtExprArray event_array);

  /// @brief リピートコントロールの生成
  virtual
  const PtControl*
  new_RepeatControl(const FileRegion& fr,
		    const PtExpr* expr,
		    PtExprArray event_array);

  /// @brief 順序つき結合子の生成
  virtual
  const PtConnection*
  new_OrderedCon(const FileRegion& fr,
		 const PtExpr* expr);

  /// @brief 順序つき結合子の生成
  virtual
  const PtConnection*
  new_OrderedCon(const PtExpr* expr);

  /// @brief 名前付き結合子の生成
  virtual
  const PtConnection*
  new_NamedCon(const FileRegion& fr,
	       const char* name,
	       const PtExpr* expr);

  /// @brief drive strength の生成
  virtual
  const PtStrength*
  new_Strength(const FileRegion& fr,
	       tVpiStrength value1,
	       tVpiStrength value2);

  /// @brief charge strength の生成
  virtual
  const PtStrength*
  new_Strength(const FileRegion& fr,
	       tVpiStrength value);

  /// @brief 遅延値の生成 (1つの値)
  virtual
  const PtDelay*
  new_Delay(const FileRegion& fr,
	    const PtExpr* value1);

  /// @brief 遅延値の生成 (2つの値)
  virtual
  const PtDelay*
  new_Delay(const FileRegion& fr,
	    const PtExpr* value1,
	    const PtExpr* value2);

  /// @brief 遅延値の生成 (3つの値)
  virtual
  const PtDelay*
  new_Delay(const FileRegion& fr,
	    const PtExpr* value1,
	    const PtExpr* value2,
	    const PtExpr* value3);

  /// @brief 階層名の生成
  virtual
  const PtNameBranch*
  new_NameBranch(const char* name);

  /// @brief インデックス付きの階層名の生成
  virtual
  const PtNameBranch*
  new_NameBranch(const char* name,
		 int index);


  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  virtual
  const PtAttrInst*
  new_AttrInst(const FileRegion& fr,
	       PtAttrSpecArray as_array);

  /// @brief attribute spec の生成
  virtual
  const PtAttrSpec*
  new_AttrSpec(const FileRegion& fr,
	       const char* name,
	       const PtExpr* expr);


  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列領域の確保
  virtual
  const char*
  new_string(const char* str);


public:
  //////////////////////////////////////////////////////////////////////
  // おもにデバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保したオブジェクトに関する情報を出力する．
  virtual
  void
  dump_profile(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 確保したオブジェクトの数
  int mNumDefStub;
  int mNumUdp;
  int mNumUdpEntry;
  int mNumUdpEntryS;
  int mNumUdpValue;
  int mNumModule;
  int mNumPort;
  int mNumPortRef;
  int mNumPortRefI;
  int mNumPortRefR;
  int mNumIOH;
  int mNumIOHV;
  int mNumIOItem;
  int mNumIOItemI;
  int mNumParamH;
  int mNumParamHV;
  int mNumParamHT;
  int mNumLocalParamH;
  int mNumLocalParamHV;
  int mNumLocalParamHT;
  int mNumSpecParamH;
  int mNumSpecParamHV;
  int mNumEventH;
  int mNumGenvarH;
  int mNumVarH;
  int mNumRegH;
  int mNumRegHV;
  int mNumNetH;
  int mNumNetHS;
  int mNumNetHD;
  int mNumNetHSD;
  int mNumNetHV;
  int mNumNetHVS;
  int mNumNetHVD;
  int mNumNetHVSD;
  int mNumDeclItem;
  int mNumDeclItemI;
  int mNumDeclItemR;
  int mNumDefParamH;
  int mNumDefParam;
  int mNumContAssignH;
  int mNumContAssignHS;
  int mNumContAssignHD;
  int mNumContAssignHSD;
  int mNumContAssign;
  int mNumInitial;
  int mNumAlways;
  int mNumTask;
  int mNumFunction;
  int mNumSizedFunc;
  int mNumTypedFunc;
  int mNumGateH;
  int mNumGateHS;
  int mNumGateHD;
  int mNumGateHSD;
  int mNumMuH;
  int mNumMuHS;
  int mNumMuHD;
  int mNumMuHSD;
  int mNumMuHP;
  int mNumInst;
  int mNumInstN;
  int mNumInstR;
  int mNumGenerate;
  int mNumGenBlock;
  int mNumGenBlockN;
  int mNumGenIf;
  int mNumGenCase;
  int mNumGenCaseItem;
  int mNumGenFor;
  int mNumSpecItem;
  int mNumSpecPath;
  int mNumPathDecl;
  int mNumPathDelay;
  int mNumDisable;
  int mNumDisableH;
  int mNumEnable;
  int mNumEnableH;
  int mNumSysEnable;
  int mNumDcStmt;
  int mNumEcStmt;
  int mNumWait;
  int mNumAssign;
  int mNumAssignC;
  int mNumNbAssign;
  int mNumNbAssignC;
  int mNumEventStmt;
  int mNumNullStmt;
  int mNumIf;
  int mNumCase;
  int mNumCaseX;
  int mNumCaseZ;
  int mNumCaseItem;
  int mNumForever;
  int mNumRepeat;
  int mNumWhile;
  int mNumFor;
  int mNumPcAssign;
  int mNumDeassign;
  int mNumForce;
  int mNumRelease;
  int mNumParBlock;
  int mNumParBlockN;
  int mNumSeqBlock;
  int mNumSeqBlockN;
  int mNumOpr1;
  int mNumOpr2;
  int mNumOpr3;
  int mNumConcat;
  int mNumMultiConcat;
  int mNumMinTypMax3;
  int mNumPrimary;
  int mNumPrimaryI;
  int mNumPrimaryR;
  int mNumPrimaryIR;
  int mNumPrimaryH;
  int mNumPrimaryHI;
  int mNumPrimaryHR;
  int mNumPrimaryHIR;
  int mNumPrimaryCI;
  int mNumPrimaryCR;
  int mNumPrimaryHCI;
  int mNumFuncCall;
  int mNumFuncCallH;
  int mNumSysFuncCall;
  int mNumIntConstant1;
  int mNumIntConstant2;
  int mNumIntConstant3;
  int mNumRealConstant;
  int mNumStringConstant;
  int mNumDelayControl;
  int mNumEventControl;
  int mNumRepeatControl;
  int mNumOrderedCon;
  int mNumAiOrderedCon;
  int mNumNamedCon;
  int mNumAiNamedCon;
  int mNumStrength;
  int mNumDelay;
  int mNumNameBranch;
  int mNumNameBranchI;
  int mNumAttrInst;
  int mNumAttrSpec;
};

END_NAMESPACE_YM_VERILOG

#endif // CPTFACTORY_H
