#ifndef LIBYM_VERILOG_PT_SIMPLE_SPTFACTORY_H
#define LIBYM_VERILOG_PT_SIMPLE_SPTFACTORY_H

/// @file libym_verilog/pt_simple/SptFactory.h
/// @brief SptFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptFactory.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PtiFactory.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// 簡易版 Pt クラス(Spt クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class SptFactory :
  public PtiFactory
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリ確保用のアロケータ
  SptFactory(AllocBase& alloc);

  /// @brief デストラクタ
  /// @note このオブジェクトが生成したすべての Pt オブジェクトを破壊する．
  virtual
  ~SptFactory();


public:

  //////////////////////////////////////////////////////////////////////
  // UDP 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief combinational UDP の生成
  /// @param[in] file_region ファイル位置情報
  /// @param[in] name UDP名
  /// @param[in] port_array ポート名のリスト
  /// @param[in] iohead_array IOリスト
  /// @param[in] entry_array テーブルエントリのリスト
  /// @return 生成された UDP
  virtual
  const PtUdp*
  new_CmbUdp(const FileRegion& file_region,
	     const char* name,
	     PtiPortArray port_array,
	     PtIOHeadArray iohead_array,
	     PtUdpEntryArray entry_array);

  /// @brief sequential UDP の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name UDP名
  /// @param[in] port_array ポート名のリスト
  /// @param[in] iohead_array IOリスト
  /// @param[in] init_value 初期値を表す式
  /// @param[in] entry_array テーブルエントリのリスト
  /// @return 生成された UDP
  virtual
  const PtUdp*
  new_SeqUdp(const FileRegion& file_region,
	     const char* name,
	     PtiPortArray port_array,
	     PtIOHeadArray iohead_array,
	     const PtExpr* init_value,
	     PtUdpEntryArray entry_array);

  /// @brief combinational UDP 用のテーブルエントリの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] input_array 入力値のリスト
  /// @param[in] output 出力値のリスト
  /// @return 生成されたテーブルエントリ
  virtual
  const PtUdpEntry*
  new_UdpEntry(const FileRegion& file_region,
	       PtUdpValueArray input_array,
	       const PtUdpValue* output);

  /// @brief sequential UDP 用のテーブルエントリの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] input_array 入力値のリスト
  /// @param[in] current 現状態値
  /// @param[in] output 出力値のリスト
  /// @return 生成されたテーブルエントリ
  virtual
  const PtUdpEntry*
  new_UdpEntry(const FileRegion& file_region,
	       PtUdpValueArray input_array,
	       const PtUdpValue* current,
	       const PtUdpValue* output);

  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] symbol シンボル
  /// @return 生成された値
  virtual
  const PtUdpValue*
  new_UdpValue(const FileRegion& file_region,
	       tVpiUdpVal symbol);


  //////////////////////////////////////////////////////////////////////
  // モジュール関係
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name モジュール名
  /// @param[in] macro macromodule の時 true となるフラグ
  /// @param[in] is_cell `celldefine - `endcelldefine で囲まれていたときに
  ///   true となるフラグ
  /// @param[in] is_protected 保護されているときに true となるフラグ．
  ///   verilog の構文にこんな情報はない．
  /// @param[in] time_unit 時刻の単位を表す数値．
  /// @param[in] time_precision 時刻の精度を表す数値．
  ///   有効な値は 2 - -15 で 2 の時に 100秒を表す．
  ///   以下，1減るごとに10分の1になる．-16 で未定義を表す．
  /// @param[in] net_type 未定義の識別子から暗黙のネットを生成するときのネット型．
  /// @param[in] unconn unconnected_drive の値．
  /// @param[in] delay delay_mode の値．
  /// @param[in] decay default_decay_time の値．
  ///   意味のある値は正の整数もしくは無限大をあらわす -1
  /// @param[in] explicit_name ポートがすべて名前付きのときに true となるフラグ
  /// @param[in] portfaults Verifault 用．
  /// @param[in] suppress_faults Verifault 用
  /// @param[in] config config 情報
  /// @param[in] library library 情報
  /// @param[in] cell cell 情報
  /// @param[in] paramdecl_port_array パラメータポートのリスト
  /// @param[in] port_array ポートのリスト
  /// @param[in] iodecl_array 入出力のリスト
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成されたモジュール
  /// @note paramdecl_port_array の内容と paramdecl_array の内容は重複しない．
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
	     PtIOHeadArray iodecl_array,
	     PtDeclHeadArray decl_array,
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

  /// @brief ポートの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] portref ポートに接続している式 (ポート参照式)
  /// @param[in] ext_name ポート名 (空文字列の場合もある)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(const FileRegion& file_region,
	   const PtExpr* portref,
	   const char* ext_name);

  /// @brief ポートの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] portref ポートに接続している式 (ポート参照式)
  /// @param[in] portref_array ポートに接続している式 (ポート参照式) のリスト
  /// @param[in] ext_name ポート名 (空文字列の場合もある)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(const FileRegion& fr,
	   const PtExpr* portref,
	   PtExprArray portref_array,
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
  new_IOHead(const FileRegion& file_region,
	     tPtIOType type,
	     bool sign);

  /// @brief IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type IO の種類
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_RegIOHead(const FileRegion& file_region,
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
  new_NetIOHead(const FileRegion& file_region,
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
  new_VarIOHead(const FileRegion& file_region,
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
  new_IOHead(const FileRegion& file_region,
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
  new_RegIOHead(const FileRegion& file_region,
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
  new_NetIOHead(const FileRegion& file_region,
		tPtIOType type,
		tVpiNetType net_type,
		bool sign,
		const PtExpr* left,
		const PtExpr* right);

  /// @brief IO 宣言の要素の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 要素名
  /// @return 生成された要素
  virtual
  const PtIOItem*
  new_IOItem(const FileRegion& file_region,
	     const char* name);

  /// @brief 初期値付き IO 宣言の要素の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 要素名
  /// @param[in] init_value 初期値を表す式
  /// @return 生成された要素
  virtual
  const PtIOItem*
  new_IOItem(const FileRegion& file_region,
	     const char* name,
	     const PtExpr* init_value);


  //////////////////////////////////////////////////////////////////////
  // その他の宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ宣言のヘッダの生成 (型指定なし)
  /// @param[in] file_region ファイル位置の情報
  /// @return 生成されたパラメータ
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& file_region);

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成されたパラメータ
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& file_region,
	     bool sign,
	     const PtExpr* left,
	     const PtExpr* right);

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] var_type データ型
  /// @return 生成されたパラメータ
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& file_region,
	     tVpiVarType var_type);

  /// @brief local param 宣言のヘッダの生成 (型指定なし)
  /// @param[in] file_region ファイル位置の情報
  /// @return 生成された localparam
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& file_region);

  /// @brief 範囲指定型 local param 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された localparam
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& file_region,
		  bool sign,
		  const PtExpr* left,
		  const PtExpr* right);

  /// @brief 組み込み型 local param 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] var_type データ型
  /// @return 生成された localparam
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& file_region,
		  tVpiVarType var_type);

  /// @brief specparam 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @return 生成された specparam
  virtual
  PtiDeclHead*
  new_SpecParamH(const FileRegion& file_region);

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された specparam
  virtual
  PtiDeclHead*
  new_SpecParamH(const FileRegion& file_region,
		 const PtExpr* left,
		 const PtExpr* right);

  /// @brief イベント宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @return 生成されたイベント
  virtual
  PtiDeclHead*
  new_EventH(const FileRegion& file_region);

  /// @brief genvar 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @return 生成された genvar
  virtual
  PtiDeclHead*
  new_GenvarH(const FileRegion& file_region);

  /// @brief 変数宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] var_type データ型
  /// @return 生成された変数
  virtual
  PtiDeclHead*
  new_VarH(const FileRegion& file_region,
	   tVpiVarType var_type);

  /// @brief 1ビット型 reg 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] sign 符号の有無を表すフラグ
  /// @return 生成された reg
  virtual
  PtiDeclHead*
  new_RegH(const FileRegion& file_region,
	   bool sign);

  /// @brief 範囲指定型 reg 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] sign 符号付きの時 true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された reg
  virtual
  PtiDeclHead*
  new_RegH(const FileRegion& file_region,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right);

  /// @brief 1ビット型 net 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& file_region,
	   tVpiNetType type,
	   bool sign);

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength あり)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] strength 信号強度
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& file_region,
	   tVpiNetType type,
	   bool sign,
	   const PtStrength* strength);

  /// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] delay 遅延
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& file_region,
	   tVpiNetType type,
	   bool sign,
	   const PtDelay* delay);

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& file_region,
	   tVpiNetType type,
	   bool sign,
	   const PtStrength* strength,
	   const PtDelay* delay);

  /// @brief 範囲指定型 net 宣言のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& file_region,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& file_region,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtStrength* strength);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] delay 遅延
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& file_region,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtDelay* delay);

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& file_region,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtStrength* strength,
	   const PtDelay* delay);

  /// @brief 宣言要素の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @return 生成された要素
  virtual
  const PtDeclItem*
  new_DeclItem(const FileRegion& file_region,
	       const char* name);

  /// @brief 初期値付き宣言要素の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] init_value 初期値を表す式
  /// @return 生成された要素
  virtual
  const PtDeclItem*
  new_DeclItem(const FileRegion& file_region,
	       const char* name,
	       const PtExpr* init_value);

  /// @brief 配列型宣言要素の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] range_array 配列の各次元の範囲のリスト
  /// @return 生成された要素
  virtual
  const PtDeclItem*
  new_DeclItem(const FileRegion& file_region,
	       const char* name,
	       PtRangeArray range_array);

  /// @brief 範囲の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] msb MSB を表す式
  /// @param[in] lsb LSB を表す式
  virtual
  const PtRange*
  new_Range(const FileRegion& file_region,
	    const PtExpr* msb,
	    const PtExpr* lsb);


  //////////////////////////////////////////////////////////////////////
  // item 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された defparam ヘッダ
  virtual
  const PtItem*
  new_DefParamH(const FileRegion& file_region,
		PtDefParamArray elem_array);

  /// @brief defparam 文の要素の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] value 値を表す式
  /// @return 生成された要素
  virtual
  const PtDefParam*
  new_DefParam(const FileRegion& file_region,
	       const char* name,
	       const PtExpr* value);

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] nb_array 階層名のリスト
  /// @param[in] tail_name 名前
  /// @param[in] value 値を表す式
  /// @return 生成された要素
  virtual
  const PtDefParam*
  new_DefParam(const FileRegion& file_region,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       const PtExpr* value);

  /// @brief continuous assign 文のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された continuous assign 文のヘッダ
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& file_region,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文のヘッダの生成 (strengthつき)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] strength 信号強度
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された continuous assign 文のヘッダ
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& file_region,
		  const PtStrength* strength,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文のヘッダの生成 (遅延付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された continuous assign 文のヘッダ
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& file_region,
		  const PtDelay* delay,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された continuous assign 文のヘッダ
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& file_region,
		  const PtStrength* strength,
		  const PtDelay* delay,
		  PtContAssignArray elem_array);

  /// @brief continuous assign 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された continuous assign 文
  virtual
  const PtContAssign*
  new_ContAssign(const FileRegion& file_region,
		 const PtExpr* lhs,
		 const PtExpr* rhs);

  /// @brief initial 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] body 本体のステートメント
  /// @return 生成された initial 文
  virtual
  const PtItem*
  new_Initial(const FileRegion& file_region,
	      const PtStmt* body);

  /// @brief always 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] body 本体のステートメント
  /// @return 生成された always 文
  virtual
  const PtItem*
  new_Always(const FileRegion& file_region,
	     const PtStmt* body);

  /// @brief task 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name task 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] iohead_array ポート宣言のリスト
  /// @param[in] decl_array 宣言のリスト (IO宣言を含む)
  /// @param[in] stmt 本体のステートメント
  /// @return 生成された task 文
  virtual
  const PtItem*
  new_Task(const FileRegion& file_region,
	   const char* name,
	   bool automatic,
	   PtIOHeadArray iohead_array,
	   PtDeclHeadArray decl_array,
	   const PtStmt* stmt);

  /// @brief 1ビット型 function 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign 符号つきの時に true となるフラグ
  /// @param[in] iohead_array ポート宣言のリスト
  /// @param[in] decl_array 宣言のリスト (IO宣言を含む)
  /// @param[in] stmt 本体のステートメント
  /// @return 生成された func 文
  virtual
  const PtItem*
  new_Function(const FileRegion& file_region,
	       const char* name,
	       bool automatic,
	       bool sign,
	       PtIOHeadArray iohead_array,
	       PtDeclHeadArray decl_array,
	       const PtStmt* stmt);

  /// @brief 範囲指定型 function 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign 符号つきの時に true にするフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] iohead_array ポート宣言のリスト
  /// @param[in] decl_array 宣言のリスト (IO宣言を含む)
  /// @param[in] stmt 本体のステートメント
  /// @return 生成された func 文
  virtual
  const PtItem*
  new_SizedFunc(const FileRegion& file_region,
		const char* name,
		bool automatic,
		bool sign,
		const PtExpr* left,
		const PtExpr* right,
		PtIOHeadArray iohead_array,
		PtDeclHeadArray decl_array,
		const PtStmt* stmt);

  /// @brief 組み込み型 function 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign 符号つきの時に true にするフラグ
  /// @param[in] func_type 関数の戻り値の型
  /// @param[in] iohead_array ポート宣言のリスト
  /// @param[in] paramhead_array parameter 宣言のリスト
  /// @param[in] lparamhead_array local parmeter 宣言のリスト
  /// @param[in] decl_array 宣言のリスト (IO宣言を含む)
  /// @param[in] stmt 本体のステートメント
  /// @return 生成された func 文
  virtual
  const PtItem*
  new_TypedFunc(const FileRegion& file_region,
		const char* name,
		bool automatic,
		bool sign,
		tVpiVarType func_type,
		PtIOHeadArray iohead_array,
		PtDeclHeadArray decl_array,
		const PtStmt* stmt);

  /// @brief gate instance 文のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された gate instance 文のヘッダ
  virtual
  const PtItem*
  new_GateH(const FileRegion& file_region,
	    tVpiPrimType type,
	    PtInstArray elem_array);

  /// @brief gate instance 文のヘッダの生成 (strength付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] strength 信号強度
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された gate instance 文のヘッダ
  virtual
  const PtItem*
  new_GateH(const FileRegion& file_region,
	    tVpiPrimType type,
	    const PtStrength* strength,
	    PtInstArray elem_array);

  /// @brief gate instance 文のヘッダの生成 (遅延付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された gate instance 文のヘッダ
  virtual
  const PtItem*
  new_GateH(const FileRegion& file_region,
	    tVpiPrimType type,
	    const PtDelay* delay,
	    PtInstArray elem_array);

  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された gate instance 文のヘッダ
  virtual
  const PtItem*
  new_GateH(const FileRegion& file_region,
	    tVpiPrimType type,
	    const PtStrength* strength,
	    const PtDelay* delay,
	    PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(const FileRegion& file_region,
	  const char* def_name,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] strength 信号強度
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(const FileRegion& file_region,
	  const char* def_name,
	  const PtStrength* strength,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(const FileRegion& file_region,
	  const char* def_name,
	  const PtDelay* delay,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(const FileRegion& file_region,
	  const char* def_name,
	  const PtStrength* strength,
	  const PtDelay* delay,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance 文のヘッダの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] con_array ポート割り当てリスト
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(const FileRegion& file_region,
	  const char* def_name,
	  PtConnectionArray con_array,
	  PtInstArray elem_array);

  /// @brief module instance/UDP instance の要素の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] con_array ポート割り当てリスト
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_Inst(const FileRegion& file_region,
	   PtConnectionArray con_array);

  /// @brief 名前付き module instance/UDP instance の要素の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] con_array ポート割り当てリスト
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstN(const FileRegion& file_region,
	    const char* name,
	    PtConnectionArray con_array);

  /// @brief 名前と範囲付き module instance/UDP instance の要素の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] con_array ポート割り当てリスト
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstV(const FileRegion& file_region,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    PtConnectionArray con_array);

  /// @brief generate 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] declhead_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成された generate 文
  virtual
  const PtItem*
  new_Generate(const FileRegion& file_region,
	       PtDeclHeadArray declhead_array,
	       PtItemArray item_array);

  /// @brief generate block 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] declhead_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成された generate block 文
  virtual
  const PtItem*
  new_GenBlock(const FileRegion& file_region,
	       PtDeclHeadArray declhead_array,
	       PtItemArray item_array);

  /// @brief 名前付き generate block 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] declhead_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成された generate block 文
  virtual
  const PtItem*
  new_GenBlock(const FileRegion& file_region,
	       const char* name,
	       PtDeclHeadArray declhead_array,
	       PtItemArray item_array);

  /// @brief generate if 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] cond 条件を表す式
  /// @param[in] then_declhead_array 条件が成り立った時に生成する宣言のリスト
  /// @param[in] then_item_array 条件が成り立った時に生成する要素のリスト
  /// @param[in] else_declhead_array 条件が成り立たなかった時に生成する宣言のリスト
  /// @param[in] else_item_array 条件が成り立たなかった時に生成する要素のリスト
  /// @return 生成された generate if 文
  /// generate if 文の生成
  virtual
  const PtItem*
  new_GenIf(const FileRegion& file_region,
	    const PtExpr* cond,
	    PtDeclHeadArray then_declhead_array,
	    PtItemArray then_item_array,
	    PtDeclHeadArray else_declhead_array,
	    PtItemArray else_item_array);

  /// @brief generate case 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr 選択式
  /// @param[in] item_array generate case item のリスト
  /// @return 生成された generate case 文
  virtual
  const PtItem*
  new_GenCase(const FileRegion& file_region,
	      const PtExpr* expr,
	      PtGenCaseItemArray item_array);

  /// @brief generate case の要素の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] label 比較式
  /// @param[in] declhead_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成された generate case item
  virtual
  const PtGenCaseItem*
  new_GenCaseItem(const FileRegion& file_region,
		  PtExprArray label_array,
		  PtDeclHeadArray declhead_array,
		  PtItemArray item_array);

  /// @brief generate for 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] loop_var ループ変数
  /// @param[in] init_expr 初期化式
  /// @param[in] cond ループ条件式
  /// @param[in] next_expr 増加式
  /// @param[in] block_name ブロック名
  /// @param[in] declhead_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成された generate for 文
  virtual
  const PtItem*
  new_GenFor(const FileRegion& file_region,
	     const char* loop_var,
	     const PtExpr* init_expr,
	     const PtExpr* cond,
	     const PtExpr* next_expr,
	     const char* block_name,
	     PtDeclHeadArray declhead_array,
	     PtItemArray item_array);

  /// @brief specify block item の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] id specify block item の種類
  /// @param[in] terminal_array 端子のリスト
  /// @return 生成された specify block item
  virtual
  const PtItem*
  new_SpecItem(const FileRegion& file_region,
	       tVpiSpecItemType id,
	       PtExprArray terminal_array);

  /// @brief path 仕様を生成する．
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] id spec path の種類
  /// @param[in] expr 条件式
  /// @param[in] path_decl パス記述
  /// @return 生成された spec path
  virtual
  const PtItem*
  new_SpecPath(const FileRegion& file_region,
	       tVpiSpecPathType id,
	       const PtExpr* expr,
	       const PtPathDecl* path_decl);

  /// @brief パス記述の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] edge
  /// @param[in] input_array
  /// @param[in] input_pol
  /// @param[in] op
  /// @param[in] output_array
  /// @param[in] output_pol
  /// @param[in] expr
  /// @param[in] path_delay
  /// @return 生成されたパス記述
  virtual
  const PtPathDecl*
  new_PathDecl(const FileRegion& file_region,
	       int edge,
	       PtExprArray input_array,
	       int input_pol,
	       int op,
	       PtExprArray output_array,
	       int output_pol,
	       const PtExpr* expr,
	       const PtPathDelay* path_delay);

  /// @brief path delay value の生成 (値が1つ)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value 値
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& file_region,
		const PtExpr* value);

  /// @brief path delay value の生成 (値が2つ)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& file_region,
		const PtExpr* value1,
		const PtExpr* value2);

  /// @brief path delay value の生成 (値が3つ)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& file_region,
		const PtExpr* value1,
		const PtExpr* value2,
		const PtExpr* value3);

  /// @brief path delay value の生成 (値が6つ)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @param[in] value4 値4
  /// @param[in] value5 値5
  /// @param[in] value6 値6
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& file_region,
		const PtExpr* value1,
		const PtExpr* value2,
		const PtExpr* value3,
		const PtExpr* value4,
		const PtExpr* value5,
		const PtExpr* value6);

  /// @brief path delay value の生成 (値が12個)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @param[in] value4 値4
  /// @param[in] value5 値5
  /// @param[in] value6 値6
  /// @param[in] value7 値7
  /// @param[in] value8 値8
  /// @param[in] value9 値9
  /// @param[in] value10 値10
  /// @param[in] value11 値11
  /// @param[in] value12 値12
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& file_region,
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
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 停止するスコープ名
  /// @return 生成された disable 文
  virtual
  const PtStmt*
  new_Disable(const FileRegion& file_region,
	      const char* name);

  /// @brief disable 文の生成 (階層付き識別子)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] nb_array 階層名のリスト
  /// @param[in] tail_name 末尾の名前
  /// @return 生成された disable 文
  virtual
  const PtStmt*
  new_Disable(const FileRegion& file_region,
	      PtNameBranchArray nb_array,
	      const char* tail_name);

  /// @brief enable 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 起動するタスク名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された enable 文
  virtual
  const PtStmt*
  new_Enable(const FileRegion& file_region,
	     const char* name,
	     PtExprArray arg_array);

  /// @brief enable 文の生成 (階層付き識別子)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] nb_array 階層名のリスト
  /// @param[in] tail_name 末尾の名前
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された enable 文
  virtual
  const PtStmt*
  new_Enable(const FileRegion& file_region,
	     PtNameBranchArray nb_array,
	     const char* tail_name,
	     PtExprArray arg_array);

  /// @brief system task enable 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 起動するシステムタスク名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された system task enable 文
  virtual
  const PtStmt*
  new_SysEnable(const FileRegion& file_region,
		const char* name,
		PtExprArray arg_array);

  /// @brief delay control 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] delay delay control (delay control 文とは別物)
  /// @param[in] body 本体のステートメント
  /// @return 生成された delay control 文
  virtual
  const PtStmt*
  new_DcStmt(const FileRegion& file_region,
	     const PtControl* delay,
	     const PtStmt* body);

  /// @brief event control 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] event delay control (event control 文とは別物)
  /// @param[in] body 本体のステートメント
  /// @return 生成された event control 文
  virtual
  const PtStmt*
  new_EcStmt(const FileRegion& file_region,
	     const PtControl* event,
	     const PtStmt* body);

  /// @brief wait 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] cond 条件式
  /// @param[in] body 本体のステートメント
  /// @return 生成された wait 文
  virtual
  const PtStmt*
  new_Wait(const FileRegion& file_region,
	   const PtExpr* cond,
	   const PtStmt* body);

  /// @brief assign 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された assign 文
  virtual
  const PtStmt*
  new_Assign(const FileRegion& file_region,
	     const PtExpr* lhs,
	     const PtExpr* rhs);

  /// @brief control 付き assign 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @param[in] control 制御式
  /// @return 生成された assign 文
  virtual
  const PtStmt*
  new_Assign(const FileRegion& file_region,
	     const PtExpr* lhs,
	     const PtExpr* rhs,
	     const PtControl* control);

  /// @brief nonblocking assign 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された nonblocking assign 文
  virtual
  const PtStmt*
  new_NbAssign(const FileRegion& file_region,
	       const PtExpr* lhs,
	       const PtExpr* rhs);

  /// @brief control 付き nonblocking assign 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @param[in] control 制御式
  /// @return 生成された nonblocking assign 文
  virtual
  const PtStmt*
  new_NbAssign(const FileRegion& file_region,
	       const PtExpr* lhs,
	       const PtExpr* rhs,
	       const PtControl* control);

  /// @brief event 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] event イベント名
  /// @return 生成された event 文
  virtual
  const PtStmt*
  new_EventStmt(const FileRegion& file_region,
		const PtExpr* event);

  /// @brief null 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @return 生成された null 文
  virtual
  const PtStmt*
  new_NullStmt(const FileRegion& file_region);

  /// @brief if 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] then_body 成り立ったときに実行されるステートメント
  /// @return 生成された if 文
  virtual
  const PtStmt*
  new_If(const FileRegion& file_region,
	 const PtExpr* expr,
	 const PtStmt* then_body);

  /// @brief if 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] then_body 成り立ったときに実行されるステートメント
  /// @param[in] else_body 成り立たなかったときに実行されるステートメント
  /// @return 生成された if 文
  virtual
  const PtStmt*
  new_If(const FileRegion& file_region,
	 const PtExpr* expr,
	 const PtStmt* then_body,
	 const PtStmt* else_body);

  /// @brief case 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_array case item のリスト
  /// @return 生成された case 文
  virtual
  const PtStmt*
  new_Case(const FileRegion& file_region,
	   const PtExpr* expr,
	   PtCaseItemArray caseitem_array);

  /// @brief casex 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_array case item のリスト
  /// @return 生成された casex 文
  virtual
  const PtStmt*
  new_CaseX(const FileRegion& file_region,
	    const PtExpr* expr,
	    PtCaseItemArray caseitem_array);

  /// @brief casez 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_array case item のリスト
  /// @return 生成された casez 文
  virtual
  const PtStmt*
  new_CaseZ(const FileRegion& file_region,
	    const PtExpr* expr,
	    PtCaseItemArray caseitem_array);

  /// @brief case item の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] label ラベルのリスト
  /// @param[in] body 本体のステートメント
  /// @return 生成された case item
  virtual
  const PtCaseItem*
  new_CaseItem(const FileRegion& file_region,
	       PtExprArray label_array,
	       const PtStmt* body);

  /// @brief forever 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] body 本体のステートメント
  /// @return 生成された forever 文
  virtual
  const PtStmt*
  new_Forever(const FileRegion& file_region,
	      const PtStmt* body);

  /// @brief repeat 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr 繰り返し数を表す式
  /// @param[in] body 本体のステートメント
  /// @return 生成された repeat 文
  virtual
  const PtStmt*
  new_Repeat(const FileRegion& file_region,
	     const PtExpr* expr,
	     const PtStmt* body);

  /// @brief while 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] cond 繰り返し条件式
  /// @param[in] body 本体のステートメント
  /// @return 生成された while 文
  virtual
  const PtStmt*
  new_While(const FileRegion& file_region,
	    const PtExpr* cond,
	    const PtStmt* body);

  /// @brief for 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] init 初期化文
  /// @param[in] cond 繰り返し条件式
  /// @param[in] next 増加文
  /// @param[in] body 本体のステートメント
  /// @return 生成された for 文
  virtual
  const PtStmt*
  new_For(const FileRegion& file_region,
	  const PtStmt* init,
	  const PtExpr* cond,
	  const PtStmt* next,
	  const PtStmt* body);

  /// @brief procedural assign 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された procedural assign 文
  virtual
  const PtStmt*
  new_PcAssign(const FileRegion& file_region,
	       const PtExpr* lhs,
	       const PtExpr* rhs);

  /// @brief deassign 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @return 生成された deassign 文
  virtual
  const PtStmt*
  new_Deassign(const FileRegion& file_region,
	       const PtExpr* lhs);

  /// @brief force 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された force 文
  virtual
  const PtStmt*
  new_Force(const FileRegion& file_region,
	    const PtExpr* lhs,
	    const PtExpr* rhs);

  /// @brief release 文の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @return 生成された release 文
  virtual
  const PtStmt*
  new_Release(const FileRegion& file_region,
	      const PtExpr* lhs);

  /// @brief parallel block の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] stmt_array 本体のステートメントのリスト
  /// @return 生成された parallel block
  virtual
  const PtStmt*
  new_ParBlock(const FileRegion& file_region,
	       const PtStmtArray stmt_array);

  /// @brief 名前付き parallel block の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name ブロック名
  /// @param[in] declhead_array 宣言のリスト
  /// @param[in] stmt_array 本体のステートメントのリスト
  /// @return 生成された parallel block
  virtual
  const PtStmt*
  new_NamedParBlock(const FileRegion& file_region,
		    const char* name,
		    PtDeclHeadArray declhead_array,
		    PtStmtArray stmt_array);

  /// @brief sequential block の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] stmt_array 本体のステートメントのリスト
  /// @return 生成された sequential block
  virtual
  const PtStmt*
  new_SeqBlock(const FileRegion& file_region,
	       PtStmtArray stmt_array);

  /// @brief 名前付き sequential block の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name ブロック名
  /// @param[in] declhead_array 宣言のリスト
  /// @param[in] stmt_array 本体のステートメントのリスト
  /// @return 生成された sequential block
  virtual
  const PtStmt*
  new_NamedSeqBlock(const FileRegion& file_region,
		    const char* name,
		    PtDeclHeadArray declhead_array,
		    PtStmtArray stmt_array);


  //////////////////////////////////////////////////////////////////////
  // expression 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr オペランド
  /// @return 生成された演算子
  virtual
  const PtExpr*
  new_Opr(const FileRegion& file_region,
	  tVpiOpType type,
	  const PtExpr* opr);

  /// @brief 二項演算子の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @return 生成された演算子
  virtual
  const PtExpr*
  new_Opr(const FileRegion& file_region,
	  tVpiOpType type,
	  const PtExpr* opr1,
	  const PtExpr* opr2);

  /// @brief 三項演算子の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @param[in] opr3 オペランド3
  /// @return 生成された演算子
  virtual
  const PtExpr*
  new_Opr(const FileRegion& file_region,
	  tVpiOpType type,
	  const PtExpr* opr1,
	  const PtExpr* opr2,
	  const PtExpr* opr3);

  /// @brief concatination 演算子の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr_array オペランドのリスト
  /// @return 生成された concatination 演算子
  virtual
  const PtExpr*
  new_Concat(const FileRegion& file_region,
	     PtExprArray expr_array);

  /// @brief multi-concatination 演算子の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr_array 結合するオペランドのリスト
  /// @return 生成された multi-concatination 演算子
  virtual
  const PtExpr*
  new_MultiConcat(const FileRegion& file_region,
		  PtExprArray expr_array);

  /// @brief min/typ/max delay 演算子の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] val0 minimum 値
  /// @param[in] val1 typical 値
  /// @param[in] val2 maximum 値
  /// @return 生成された min/typ/max 演算子
  virtual
  const PtExpr*
  new_MinTypMax(const FileRegion& file_region,
		const PtExpr* val0,
		const PtExpr* val1,
		const PtExpr* val2);

  /// @brief primary の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 識別子名
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& file_region,
	      const char* name);

  /// @brief インデックス付き primary の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_array インデックスのリスト
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& file_region,
	      const char* name,
	      PtExprArray index_array);

  /// @brief 範囲指定付き primary の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& file_region,
	      const char* name,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief インデックスと範囲指定付き primary の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_array インデックスのリスト
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& file_region,
	      const char* name,
	      PtExprArray index_array,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief primary の生成 (階層付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] nb_array 識別子の階層部分のリスト
  /// @param[in] tail_name 識別子の末尾
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& file_region,
	      PtNameBranchArray nb_array,
	      const char* tail_name);

  /// @brief インデックス付き primary の生成 (階層付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] nb_array 識別子の階層部分のリスト
  /// @param[in] tail_name 識別子の末尾
  /// @param[in] index_array インデックスのリスト
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& file_region,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      PtExprArray index_array);

  /// @brief 範囲指定付き primary の生成 (階層付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] nb_array 識別子の階層部分のリスト
  /// @param[in] tail_name 識別子の末尾
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& file_region,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] nb_array 識別子の階層部分のリスト
  /// @param[in] tail_name 識別子の末尾
  /// @param[in] index_array インデックスのリスト
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& file_region,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      PtExprArray index_array,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right);

  /// @brief constant primary の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_array インデックスのリスト
  /// @return 生成された const primary
  virtual
  const PtExpr*
  new_CPrimary(const FileRegion& file_region,
	       const char* name,
	       PtExprArray index_array);

  /// @brief 範囲指定付き constant primary の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された constant primary
  virtual
  const PtExpr*
  new_CPrimary(const FileRegion& file_region,
	       const char* name,
	       tVpiRangeMode mode,
	       const PtExpr* left,
	       const PtExpr* right);

  /// @brief インデックス付き constant primary の生成 (階層付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] nb_array 識別子の階層部分のリスト
  /// @param[in] tail_name 識別子の末尾
  /// @param[in] index_array インデックスのリスト
  /// @return 生成された constant primary
  virtual
  const PtExpr*
  new_CPrimary(const FileRegion& file_region,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       PtExprArray index_array);

  /// @brief function call の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 関数名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された function call
  virtual
  const PtExpr*
  new_FuncCall(const FileRegion& file_region,
	       const char* name,
	       PtExprArray arg_array);

  /// @brief function call の生成 (階層付き)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] nb_array 関数名の階層部分のリスト
  /// @param[in] tail_name 関数名の末尾
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された function call
  virtual
  const PtExpr*
  new_FuncCall(const FileRegion& file_region,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       PtExprArray arg_array);

  /// @brief system function call の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 関数名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された function call
  virtual
  const PtExpr*
  new_SysFuncCall(const FileRegion& file_region,
		  const char* name,
		  PtExprArray arg_array);

  /// @brief 整数型の定数の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& file_region,
	       ymuint value);

  /// @brief 整数型の定数の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& file_region,
	       const char* value);

  /// @brief 基底付き整数型の定数の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] const_type 定数の種類
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& file_region,
	       tVpiConstType const_type,
	       const char* value);

  /// @brief サイズと基底付き定数の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] size サイズ
  /// @param[in] const_type 定数の種類
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& file_region,
	       ymuint size,
	       tVpiConstType const_type,
	       const char* value);

  /// @brief 実数型の定数の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_RealConst(const FileRegion& file_region,
		double value);

  /// @brief 文字列型の定数の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value 文字列
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_StringConst(const FileRegion& file_region,
		  const char* value);


  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value 遅延を表す式
  /// @return 生成されたディレイコントロール
  virtual
  const PtControl*
  new_DelayControl(const FileRegion& file_region,
		   const PtExpr* value);

  /// @brief イベントコントロールの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] event_array イベントのリスト
  /// @return 生成されたイベントコントロール
  virtual
  const PtControl*
  new_EventControl(const FileRegion& file_region,
		   PtExprArray event_array);

  /// @brief リピートコントロールの生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr 繰り返し数を表す式
  /// @param[in] event_array 繰り返しの単位となるイベントのリスト
  /// @return 生成されたリピートコントロール
  virtual
  const PtControl*
  new_RepeatControl(const FileRegion& file_region,
		    const PtExpr* expr,
		    PtExprArray event_array);

  /// @brief 順序つき結合子の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  virtual
  const PtConnection*
  new_OrderedCon(const FileRegion& file_region,
		 const PtExpr* expr);

  /// @brief 順序つき結合子の生成
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  virtual
  const PtConnection*
  new_OrderedCon(const PtExpr* expr);

  /// @brief 名前付き結合子の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  virtual
  const PtConnection*
  new_NamedCon(const FileRegion& file_region,
	       const char* name,
	       const PtExpr* expr);

  /// @brief strength の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value0 '0' の強度
  /// @param[in] value1 '1' の強度
  /// @return 生成された strength
  virtual
  const PtStrength*
  new_Strength(const FileRegion& file_region,
	       tVpiStrength value0,
	       tVpiStrength value1);

  /// @brief charge strength の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value 強度
  /// @return 生成された strength
  virtual
  const PtStrength*
  new_Strength(const FileRegion& file_region,
	       tVpiStrength value);

  /// @brief 遅延値の生成 (1つの値)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value1 値1
  /// @return 生成された遅延値
  virtual
  const PtDelay*
  new_Delay(const FileRegion& file_region,
	    const PtExpr* value1);

  /// @brief 遅延値の生成 (2つの値)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @return 生成された遅延値
  virtual
  const PtDelay*
  new_Delay(const FileRegion& file_region,
	    const PtExpr* value1,
	    const PtExpr* value2);

  /// @brief 遅延値の生成 (3つの値)
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @return 生成された遅延値
  virtual
  const PtDelay*
  new_Delay(const FileRegion& file_region,
	    const PtExpr* value1,
	    const PtExpr* value2,
	    const PtExpr* value3);

  /// @brief 階層名の生成
  /// @param[in] name 名前
  /// @return 生成された階層名
  virtual
  const PtNameBranch*
  new_NameBranch(const char* name);

  /// @brief インデックス付きの階層名の生成
  /// @param[in] name 名前
  /// @param[in] index インデックス
  /// @return 生成された階層名
  virtual
  const PtNameBranch*
  new_NameBranch(const char* name,
		 int index);


  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] as_array attribute spec のリスト
  /// @return 生成された attribute instance
  virtual
  const PtAttrInst*
  new_AttrInst(const FileRegion& file_region,
	       PtAttrSpecArray as_array);

  /// @brief attribute spec の生成
  /// @param[in] file_region ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr 値
  /// @return 生成された attribute spec
  virtual
  const PtAttrSpec*
  new_AttrSpec(const FileRegion& file_region,
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

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_SIMPLE_SPTFACTORY_H
