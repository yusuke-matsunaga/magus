#ifndef PTIFACTORY_H
#define PTIFACTORY_H

/// @file PtiFactory.h
/// @brief PtiFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/pt/PtDecl.h"
#include "PtiFwd.h"
#include "YmUtils/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtiFactory PtiFactory.h "PtiFactory.h"
/// @brief Verilog-HDL のパース木を生成するファクトリクラス
///
/// このクラスは純粋仮想基底クラスなのでインターフェイスの定義しか
/// していない．実際にはこれを継承したクラスを用意する必要がある．
//////////////////////////////////////////////////////////////////////
class PtiFactory
{
public:

  /// @brief PtiFactory の実装クラスのオブジェクトを作る関数
  /// @param[in] type 実装クラスを指定する文字列
  /// @param[in] alloc メモリアロケータ
  static
  PtiFactory*
  make_obj(const string& type,
	   Alloc& alloc);

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  PtiFactory(Alloc& alloc);

  /// @brief デストラクタ
  virtual
  ~PtiFactory();


public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief combinational UDP の生成
  /// @param[in] fr ファイル位置情報
  /// @param[in] name UDP名
  /// @param[in] port_array ポート名の配列
  /// @param[in] io_array IOの配列
  /// @param[in] entry_array テーブルエントリの配列
  /// @return 生成された UDP
  virtual
  const PtUdp*
  new_CmbUdp(const FileRegion& fr,
	     const char* name,
	     PtiPortArray port_array,
	     PtIOHeadArray io_array,
	     PtUdpEntryArray entry_array) = 0;

  /// @brief sequential UDP の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name UDP名
  /// @param[in] port_array ポート名の配列
  /// @param[in] io_array IOの配列
  /// @param[in] init_value 初期値を表す式
  /// @param[in] entry_array テーブルエントリの配列
  /// @return 生成された UDP
  virtual
  const PtUdp*
  new_SeqUdp(const FileRegion& fr,
	     const char* name,
	     PtiPortArray port_array,
	     PtIOHeadArray io_array,
	     const PtExpr* init_value,
	     PtUdpEntryArray entry_array) = 0;

  /// @brief combinational UDP 用のテーブルエントリの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] input_array 入力値の配列
  /// @param[in] output 出力値
  /// @return 生成されたテーブルエントリ
  virtual
  const PtUdpEntry*
  new_UdpEntry(const FileRegion& fr,
	       PtUdpValueArray input_array,
	       const PtUdpValue* output) = 0;

  /// @brief sequential UDP 用のテーブルエントリの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] input_array 入力値の配列
  /// @param[in] current 現状態値
  /// @param[in] output 出力値
  /// @return 生成されたテーブルエントリ
  virtual
  const PtUdpEntry*
  new_UdpEntry(const FileRegion& fr,
	       PtUdpValueArray input_array,
	       const PtUdpValue* current,
	       const PtUdpValue* output) = 0;

  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] symbol シンボル
  /// @return 生成された値
  virtual
  const PtUdpValue*
  new_UdpValue(const FileRegion& fr,
	       char symbol) = 0;

  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] symbol1, symbol2 シンボル
  /// @return 生成された値
  virtual
  const PtUdpValue*
  new_UdpValue(const FileRegion& fr,
	       char symbol1,
	       char symbol2) = 0;


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
	     PtItemArray item_array) = 0;


  //////////////////////////////////////////////////////////////////////
  // ポート関係
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートの生成 (空のポート / 名前のみのポート)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] ext_name ポート名
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(const FileRegion& fr,
	   const char* ext_name = NULL) = 0;

  /// @brief ポートの生成 (ポート参照式があるポート)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] portref ポートに接続している式 (ポート参照式)
  /// @param[in] ext_name ポート名 (空文字列の場合もある)
  /// @return 生成されたポート
  virtual
  PtiPort*
  new_Port(const FileRegion& fr,
	   const PtExpr* portref,
	   const char* ext_name) = 0;

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
	   PtExprArray portref_array,
	   const char* ext_name) = 0;


  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief IO 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類 (input, inout, output)
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_IOHead(const FileRegion& fr,
	     tPtIOType type,
	     bool sign) = 0;

  /// @brief IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類 (input, inout, output)
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_RegIOHead(const FileRegion& fr,
		tPtIOType type,
		bool sign) = 0;

  /// @brief IO 宣言のヘッダの生成 (ネット型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類 (input, inout, output)
  /// @param[in] net_type 補助的なネット型
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_NetIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiNetType net_type,
		bool sign) = 0;

  /// @brief IO 宣言のヘッダの生成 (変数型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類 (input, inout, output)
  /// @param[in] var_type 補助的な変数型
  /// @return 生成された IO宣言ヘッダ
  virtual
  PtiIOHead*
  new_VarIOHead(const FileRegion& fr,
		tPtIOType type,
		tVpiVarType var_type) = 0;

  /// @brief 範囲付きの IO 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類 (input, inout, output)
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
	     const PtExpr* right) = 0;

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類 (input, inout, output)
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
		const PtExpr* right) = 0;

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type IO の種類 (input, inout, output)
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
		const PtExpr* right) = 0;

  /// @brief IO 宣言の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 要素名
  /// @return 生成された要素
  virtual
  const PtIOItem*
  new_IOItem(const FileRegion& fr,
	     const char* name) = 0;


  /// @brief 初期値付き IO 宣言の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 要素名
  /// @param[in] init_value 初期値を表す式
  /// @return 生成された要素
  virtual
  const PtIOItem*
  new_IOItem(const FileRegion& fr,
	     const char* name,
	     const PtExpr* init_value) = 0;


  //////////////////////////////////////////////////////////////////////
  // その他の宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ宣言のヘッダの生成 (型指定なし)
  /// @param[in] fr ファイル位置の情報
  /// @return 生成されたパラメータ
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& fr) = 0;

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成されたパラメータ
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& fr,
	     bool sign,
	     const PtExpr* left,
	     const PtExpr* right) = 0;

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] var_type データ型
  /// @return 生成されたパラメータ
  virtual
  PtiDeclHead*
  new_ParamH(const FileRegion& fr,
	     tVpiVarType var_type) = 0;

  /// @brief local param 宣言のヘッダの生成 (型指定なし)
  /// @param[in] fr ファイル位置の情報
  /// @return 生成された localparam
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr) = 0;

  /// @brief 範囲指定型 local param 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きのとき true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された localparam
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr,
		  bool sign,
		  const PtExpr* left,
		  const PtExpr* right) = 0;

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] var_type データ型
  /// @return 生成されたパラメータ
  virtual
  PtiDeclHead*
  new_LocalParamH(const FileRegion& fr,
		  tVpiVarType var_type) = 0;

  /// @brief specparam 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成された specparam
  virtual
  PtiDeclHead*
  new_SpecParamH(const FileRegion& fr) = 0;

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された specparam
  virtual
  PtiDeclHead*
  new_SpecParamH(const FileRegion& fr,
		 const PtExpr* left,
		 const PtExpr* right) = 0;

  /// @brief イベント宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成されたイベント
  virtual
  PtiDeclHead*
  new_EventH(const FileRegion& fr) = 0;

  /// @brief genvar 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成された genvar
  virtual
  PtiDeclHead*
  new_GenvarH(const FileRegion& fr) = 0;

  /// @brief 変数宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] var_type データ型
  /// @return 生成された変数
  virtual
  PtiDeclHead*
  new_VarH(const FileRegion& fr,
	   tVpiVarType var_type) = 0;

  /// @brief 1ビット型 reg 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きの時 true となるフラグ
  /// @return 生成された reg
  virtual
  PtiDeclHead*
  new_RegH(const FileRegion& fr,
	   bool sign) = 0;

  /// @brief 範囲指定型 reg 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] sign 符号付きの時 true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された reg
  virtual
  PtiDeclHead*
  new_RegH(const FileRegion& fr,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right) = 0;

  /// @brief 1ビット型 net 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign) = 0;

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] strength 信号強度
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   const PtStrength* strength) = 0;

  /// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] delay 遅延
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   const PtDelay* delay) = 0;

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   bool sign,
	   const PtStrength* strength,
	   const PtDelay* delay) = 0;

  /// @brief 範囲指定型 net 宣言のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right) = 0;

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] strength 信号強度
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtStrength* strength) = 0;

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type net の型
  /// @param[in] vstype vector/scalar 指定
  /// @param[in] sign 符号の有無を表すフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] delay 遅延
  /// @return 生成されたネット
  virtual
  PtiDeclHead*
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtDelay* delay) = 0;

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
  /// @param[in] fr ファイル位置の情報
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
  new_NetH(const FileRegion& fr,
	   tVpiNetType type,
	   tVpiVsType vstype,
	   bool sign,
	   const PtExpr* left,
	   const PtExpr* right,
	   const PtStrength* strength,
	   const PtDelay* delay) = 0;

  /// @brief 宣言要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @return 生成された要素
  virtual
  const PtDeclItem*
  new_DeclItem(const FileRegion& fr,
	       const char* name) = 0;

  /// @brief 初期値付き宣言要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] init_value 初期値を表す式
  /// @return 生成された要素
  virtual
  const PtDeclItem*
  new_DeclItem(const FileRegion& fr,
	       const char* name,
	       const PtExpr* init_value) = 0;

  /// @brief 配列型宣言要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] range_array 配列の各次元の範囲のリスト
  /// @return 生成された要素
  virtual
  const PtDeclItem*
  new_DeclItem(const FileRegion& fr,
	       const char* name,
	       PtRangeArray range_array) = 0;

  /// @brief 範囲の生成
  /// @param[in] msb MSB を表す式
  /// @param[in] lsb LSB を表す式
  virtual
  const PtRange*
  new_Range(const FileRegion& fr,
	    const PtExpr* msb,
	    const PtExpr* lsb) = 0;


  //////////////////////////////////////////////////////////////////////
  // item 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された defparam ヘッダ
  virtual
  const PtItem*
  new_DefParamH(const FileRegion& fr,
		PtDefParamArray elem_array) = 0;

  /// @brief defparam 文の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] value 値を表す式
  /// @return 生成された要素
  virtual
  const PtDefParam*
  new_DefParam(const FileRegion& fr,
	       const char* name,
	       const PtExpr* value) = 0;

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] nb_array 階層名のリスト
  /// @param[in] tail_name 名前
  /// @param[in] value 値を表す式
  /// @return 生成された要素
  virtual
  const PtDefParam*
  new_DefParam(const FileRegion& fr,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       const PtExpr* value) = 0;

  /// @brief continuous assign 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された continuous assign 文のヘッダ
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  PtContAssignArray elem_array) = 0;

  /// @brief continuous assign 文のヘッダの生成 (strengthつき)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] strength 信号強度
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された continuous assign 文のヘッダ
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  const PtStrength* strength,
		  PtContAssignArray elem_array) = 0;

  /// @brief continuous assign 文のヘッダの生成 (遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された continuous assign 文のヘッダ
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  const PtDelay* delay,
		  PtContAssignArray elem_array) = 0;

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素のリスト
  /// @return 生成された continuous assign 文のヘッダ
  virtual
  const PtItem*
  new_ContAssignH(const FileRegion& fr,
		  const PtStrength* strength,
		  const PtDelay* delay,
		  PtContAssignArray elem_array) = 0;

  /// @brief continuous assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された continuous assign 文
  virtual
  const PtContAssign*
  new_ContAssign(const FileRegion& fr,
		 const PtExpr* lhs,
		 const PtExpr* rhs) = 0;

  /// @brief initial 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] body 本体のステートメント
  /// @return 生成された initial 文
  virtual
  const PtItem*
  new_Initial(const FileRegion& fr,
	      const PtStmt* body) = 0;

  /// @brief always 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] body 本体のステートメント
  /// @return 生成された always 文
  virtual
  const PtItem*
  new_Always(const FileRegion& fr,
	     const PtStmt* body) = 0;

  /// @brief task 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name task 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] iodecl_array IO宣言のリスト
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] stmt 本体のステートメント
  /// @return 生成された task
  virtual
  const PtItem*
  new_Task(const FileRegion& fr,
	   const char* name,
	   bool automatic,
	   PtIOHeadArray iodecl_array,
	   PtDeclHeadArray decl_array,
	   const PtStmt* stmt) = 0;

  /// @brief 1ビット型 function 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign signed 属性がついていたら true となるフラグ
  /// @param[in] iodecl_array IO宣言のリスト
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] stmt 本体のステートメント
  /// @return 生成された function
  virtual
  const PtItem*
  new_Function(const FileRegion& fr,
	       const char* name,
	       bool automatic,
	       bool sign,
	       PtIOHeadArray iodecl_array,
	       PtDeclHeadArray decl_array,
	       const PtStmt* stmt) = 0;

  /// @brief 範囲指定型 function 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign 符号つきの時に true となるフラグ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] iodecl_array IO宣言のリスト
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] stmt 本体のステートメント
  /// @return 生成された function
  virtual
  const PtItem*
  new_SizedFunc(const FileRegion& fr,
		const char* name,
		bool automatic,
		bool sign,
		const PtExpr* left,
		const PtExpr* right,
		PtIOHeadArray iodecl_array,
		PtDeclHeadArray decl_array,
		const PtStmt* stmt) = 0;

  /// @brief 組み込み型 function 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name function 名
  /// @param[in] automatic automatic task の時に true となるフラグ
  /// @param[in] sign signed 属性がついていたら true となるフラグ
  /// @param[in] func_type 関数の戻値の型
  /// @param[in] iodecl_array ポート宣言のリスト
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] stmt 本体のステートメント
  /// @return 生成された task 文
  virtual
  const PtItem*
  new_TypedFunc(const FileRegion& fr,
		const char* name,
		bool automatic,
		bool sign,
		tVpiVarType func_type,
		PtIOHeadArray iodecl_array,
		PtDeclHeadArray decl_array,
		const PtStmt* stmt) = 0;

  /// @brief gate instance 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] elem_array 要素の配列
  /// @return 生成された gate instance 文のヘッダ
  virtual
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    PtInstArray elem_array) = 0;

  /// @brief gate instance 文のヘッダの生成 (strength付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] strength 信号強度
  /// @param[in] elem_array 要素の配列
  /// @return 生成された gate instance 文のヘッダ
  virtual
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    const PtStrength* strength,
	    PtInstArray elem_array) = 0;

  /// @brief gate instance 文のヘッダの生成 (遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素の配列
  /// @return 生成された gate instance 文のヘッダ
  virtual
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    const PtDelay* delay,
	    PtInstArray elem_array) = 0;

  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type primitive の型
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素の配列
  /// @return 生成された gate instance 文のヘッダ
  virtual
  const PtItem*
  new_GateH(const FileRegion& fr,
	    tVpiPrimType type,
	    const PtStrength* strength,
	    const PtDelay* delay,
	    PtInstArray elem_array) = 0;

  /// @brief module instance/UDP instance 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] elem_array 要素の配列
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtInstArray elem_array) = 0;

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] strength 信号強度
  /// @param[in] elem_array 要素の配列
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  const PtStrength* strength,
	  PtInstArray elem_array) = 0;

  /// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素の配列
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  const PtDelay* delay,
	  PtInstArray elem_array) = 0;

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] strength 信号強度
  /// @param[in] delay 遅延値
  /// @param[in] elem_array 要素の配列
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  const PtStrength* strength,
	  const PtDelay* delay,
	  PtInstArray elem_array) = 0;

  /// @brief module instance/UDP instance 文のヘッダの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] def_name 定義名
  /// @param[in] con_array ポート割り当てリスト
  /// @param[in] elem_array 要素の配列
  /// @return 生成された module instance/UDP instance 文のヘッダ
  virtual
  const PtItem*
  new_MuH(const FileRegion& fr,
	  const char* def_name,
	  PtConnectionArray con_array,
	  PtInstArray elem_array) = 0;

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] con_array ポート割り当ての配列
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_Inst(const FileRegion& fr,
	   PtConnectionArray con_array) = 0;

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_Inst(const FileRegion& fr,
	   const PtExpr* expr1);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1, expr2 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_Inst(const FileRegion& fr,
	   const PtExpr* expr1,
	   const PtExpr* expr2);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1, expr2, expr3 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_Inst(const FileRegion& fr,
	   const PtExpr* expr1,
	   const PtExpr* expr2,
	   const PtExpr* expr3);

  /// @brief module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_Inst(const FileRegion& fr,
	   const PtExpr* expr1,
	   const PtExpr* expr2,
	   const PtExpr* expr3,
	   const PtExpr* expr4);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] con_array ポート割り当ての配列
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstN(const FileRegion& fr,
	    const char* name,
	    PtConnectionArray con_array) = 0;

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstN(const FileRegion& fr,
	    const char* name,
	    const PtExpr* expr1);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1, expr2 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstN(const FileRegion& fr,
	    const char* name,
	    const PtExpr* expr1,
	    const PtExpr* expr2);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1, expr2, expr3 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstN(const FileRegion& fr,
	    const char* name,
	    const PtExpr* expr1,
	    const PtExpr* expr2,
	    const PtExpr* expr3);

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstN(const FileRegion& fr,
	    const char* name,
	    const PtExpr* expr1,
	    const PtExpr* expr2,
	    const PtExpr* expr3,
	    const PtExpr* expr4);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] con_array ポート割り当ての配列
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    PtConnectionArray con_array) = 0;

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    const PtExpr* expr1);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1, expr2 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    const PtExpr* expr1,
	    const PtExpr* expr2);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1, expr2, expr3 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    const PtExpr* expr1,
	    const PtExpr* expr2,
	    const PtExpr* expr3);

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
  /// @return 生成された module instance/UDP instance の要素
  virtual
  const PtInst*
  new_InstV(const FileRegion& fr,
	    const char* name,
	    const PtExpr* left,
	    const PtExpr* right,
	    const PtExpr* expr1,
	    const PtExpr* expr2,
	    const PtExpr* expr3,
	    const PtExpr* expr4);

  /// @brief generate 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成された generate 文
  virtual
  const PtItem*
  new_Generate(const FileRegion& fr,
	       PtDeclHeadArray decl_array,
	       PtItemArray item_array) = 0;

  /// @brief generate block 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成された generate block 文
  virtual
  const PtItem*
  new_GenBlock(const FileRegion& fr,
	       PtDeclHeadArray decl_array,
	       PtItemArray item_array) = 0;

  /// @brief 名前付き generate block 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成された generate block 文
  virtual
  const PtItem*
  new_GenBlock(const FileRegion& fr,
	       const char* name,
	       PtDeclHeadArray decl_array,
	       PtItemArray item_array) = 0;

  /// @brief generate if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 条件を表す式
  /// @param[in] then_decl_array 条件が成り立った時に生成する宣言のリスト
  /// @param[in] then_item_array 条件が成り立った時に生成する要素のリスト
  /// @param[in] else_decl_array 条件が成り立たなかった時に生成する宣言のリスト
  /// @param[in] else_item_array 条件が成り立たなかった時に生成する要素のリスト
  /// @return 生成された generate if 文
  virtual
  const PtItem*
  new_GenIf(const FileRegion& fr,
	    const PtExpr* cond,
	    PtDeclHeadArray then_decl_array,
	    PtItemArray then_item_array,
	    PtDeclHeadArray else_decl_array,
	    PtItemArray else_item_array) = 0;

  /// @brief generate case 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 選択式
  /// @param[in] item_array generate case item のリスト
  /// @return 生成された generate case 文
  virtual
  const PtItem*
  new_GenCase(const FileRegion& fr,
	      const PtExpr* expr,
	      PtGenCaseItemArray item_array) = 0;

  /// @brief generate case の要素の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] label_array 比較式のリスト
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成された generate case item
  virtual
  const PtGenCaseItem*
  new_GenCaseItem(const FileRegion& fr,
		  PtExprArray label_array,
		  PtDeclHeadArray decl_array,
		  PtItemArray item_array) = 0;

  /// @brief generate for 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] loop_var ループ変数
  /// @param[in] init_expr 初期化式
  /// @param[in] cond ループ条件式
  /// @param[in] next_expr 増加式
  /// @param[in] block_name ブロック名
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] item_array 要素のリスト
  /// @return 生成された generate for 文
  virtual
  const PtItem*
  new_GenFor(const FileRegion& fr,
	     const char* loop_var,
	     const PtExpr* init_expr,
	     const PtExpr* cond,
	     const PtExpr* next_expr,
	     const char* block_name,
	     PtDeclHeadArray decl_array,
	     PtItemArray item_array) = 0;

  /// @brief specify block item の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] id specify block item の種類
  /// @param[in] terminal_array 端子のリスト
  /// @return 生成された specify block item
  virtual
  const PtItem*
  new_SpecItem(const FileRegion& fr,
	       tVpiSpecItemType id,
	       PtExprArray terminal_array) = 0;

  /// @brief path 仕様を生成する．
  /// @param[in] fr ファイル位置の情報
  /// @param[in] id spec path の種類
  /// @param[in] expr 条件式
  /// @param[in] path_decl パス記述
  /// @return 生成された spec path
  virtual
  const PtItem*
  new_SpecPath(const FileRegion& fr,
	       tVpiSpecPathType id,
	       const PtExpr* expr,
	       const PtPathDecl* path_decl) = 0;

  /// @brief パス記述の生成
  /// @param[in] fr ファイル位置の情報
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
  new_PathDecl(const FileRegion& fr,
	       int edge,
	       PtExprArray input_array,
	       int input_pol,
	       int op,
	       PtExprArray output_array,
	       int output_pol,
	       const PtExpr* expr,
	       const PtPathDelay* path_delay) = 0;

  /// @brief パス記述の生成
  /// @param[in] fr ファイル位置の情報
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
  new_PathDecl(const FileRegion& fr,
	       int edge,
	       PtExprArray input_array,
	       int input_pol,
	       int op,
	       const PtExpr* output,
	       int output_pol,
	       const PtExpr* expr,
	       const PtPathDelay* path_delay);

  /// @brief path delay value の生成 (値が1個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 値
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value) = 0;

  /// @brief path delay value の生成 (値が2個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2) = 0;

  /// @brief path delay value の生成 (値が3個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2,
		const PtExpr* value3) = 0;

  /// @brief path delay value の生成 (値が6個)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @param[in] value4 値4
  /// @param[in] value5 値5
  /// @param[in] value6 値6
  /// @return 生成された path delay value
  virtual
  const PtPathDelay*
  new_PathDelay(const FileRegion& fr,
		const PtExpr* value1,
		const PtExpr* value2,
		const PtExpr* value3,
		const PtExpr* value4,
		const PtExpr* value5,
		const PtExpr* value6) = 0;

  /// @brief path delay value の生成 (値が12個)
  /// @param[in] fr ファイル位置の情報
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
		const PtExpr* value12) = 0;


  //////////////////////////////////////////////////////////////////////
  // statement 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief disable 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 停止するスコープ名
  /// @return 生成された disable 文
  virtual
  const PtStmt*
  new_Disable(const FileRegion& fr,
	      const char* name) = 0;

  /// @brief disable 文の生成 (階層付き識別子)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] nb_array 階層名のリスト
  /// @param[in] name 末尾の名前
  /// @return 生成された disable 文
  virtual
  const PtStmt*
  new_Disable(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* name) = 0;

  /// @brief enable 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 起動するタスク名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された enable 文
  virtual
  const PtStmt*
  new_Enable(const FileRegion& fr,
	     const char* name,
	     PtExprArray arg_array) = 0;

  /// @brief enable 文の生成 (階層付き識別子)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] nb_array 階層名のリスト
  /// @param[in] name 末尾の名前
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された enable 文
  virtual
  const PtStmt*
  new_Enable(const FileRegion& fr,
	     PtNameBranchArray nb_array,
	     const char* name,
	     PtExprArray arg_array) = 0;

  /// @brief system task enable 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 起動するシステムタスク名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された system task enable 文
  virtual
  const PtStmt*
  new_SysEnable(const FileRegion& fr,
		const char* name,
		PtExprArray arg_array) = 0;

  /// @brief delay control 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] delay delay control (delay control 文とは別物)
  /// @param[in] body 本体のステートメント
  /// @return 生成された delay control 文
  virtual
  const PtStmt*
  new_DcStmt(const FileRegion& fr,
	     const PtControl* delay,
	     const PtStmt* body) = 0;

  /// @brief event control 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event event control (event control 文とは別物)
  /// @param[in] body 本体のステートメント
  /// @return 生成された event control 文
  virtual
  const PtStmt*
  new_EcStmt(const FileRegion& fr,
	     const PtControl* event,
	     const PtStmt* body) = 0;

  /// @brief wait 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 条件式
  /// @param[in] body 本体のステートメント
  /// @return 生成された wait 文
  virtual
  const PtStmt*
  new_Wait(const FileRegion& fr,
	   const PtExpr* cond,
	   const PtStmt* body) = 0;

  /// @brief assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された assign 文
  virtual
  const PtStmt*
  new_Assign(const FileRegion& fr,
	     const PtExpr* lhs,
	     const PtExpr* rhs) = 0;

  /// @brief control 付き assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @param[in] control 制御式
  /// @return 生成された assign 文
  virtual
  const PtStmt*
  new_Assign(const FileRegion& fr,
	     const PtExpr* lhs,
	     const PtExpr* rhs,
	     const PtControl* control) = 0;

  /// @brief nonblocking assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された nonblocking assign 文
  virtual
  const PtStmt*
  new_NbAssign(const FileRegion& fr,
	       const PtExpr* lhs,
	       const PtExpr* rhs) = 0;

  /// @brief control 付き nonblocking assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @param[in] control 制御式
  /// @return 生成された nonblocking assign 文
  virtual
  const PtStmt*
  new_NbAssign(const FileRegion& fr,
	       const PtExpr* lhs,
	       const PtExpr* rhs,
	       const PtControl* control) = 0;

  /// @brief event 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event イベント名
  /// @return 生成された event 文
  virtual
  const PtStmt*
  new_EventStmt(const FileRegion& fr,
		const PtExpr* event) = 0;

  /// @brief null 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成された null 文
  virtual
  const PtStmt*
  new_NullStmt(const FileRegion& fr) = 0;

  /// @brief if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] then_body 成り立ったときに実行されるステートメント
  /// @return 生成された if 文
  virtual
  const PtStmt*
  new_If(const FileRegion& fr,
	 const PtExpr* expr,
	 const PtStmt* then_body) = 0;

  /// @brief if 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] then_body 成り立ったときに実行されるステートメント
  /// @param[in] else_body 成り立たなかったときに実行されるステートメント
  /// @return 生成された if 文
  virtual
  const PtStmt*
  new_If(const FileRegion& fr,
	 const PtExpr* expr,
	 const PtStmt* then_body,
	 const PtStmt* else_body) = 0;

  /// @brief case 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_array case item のリスト
  /// @return 生成された case 文
  virtual
  const PtStmt*
  new_Case(const FileRegion& fr,
	   const PtExpr* expr,
	   PtCaseItemArray caseitem_array) = 0;

  /// @brief casex 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_array case item のリスト
  /// @return 生成された case 文
  virtual
  const PtStmt*
  new_CaseX(const FileRegion& fr,
	    const PtExpr* expr,
	    PtCaseItemArray caseitem_array) = 0;

  /// @brief casez 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 条件式
  /// @param[in] caseitem_array case item のリスト
  /// @return 生成された case 文
  virtual
  const PtStmt*
  new_CaseZ(const FileRegion& fr,
	    const PtExpr* expr,
	    PtCaseItemArray caseitem_array) = 0;

  /// @brief case item の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] label_array ラベルのリスト
  /// @param[in] body 本体のステートメント
  /// @return 生成された case item
  virtual
  const PtCaseItem*
  new_CaseItem(const FileRegion& fr,
	       PtExprArray label_array,
	       const PtStmt* body) = 0;

  /// @brief forever 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] body 本体のステートメント
  /// @return 生成された forever 文
  virtual
  const PtStmt*
  new_Forever(const FileRegion& fr,
	      const PtStmt* body) = 0;

  /// @brief repeat 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 繰り返し数を表す式
  /// @param[in] body 本体のステートメント
  /// @return 生成された repeat 文
  virtual
  const PtStmt*
  new_Repeat(const FileRegion& fr,
	     const PtExpr* expr,
	     const PtStmt* body) = 0;

  /// @brief while 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] cond 繰り返し条件式
  /// @param[in] body 本体のステートメント
  /// @return 生成された while 文
  virtual
  const PtStmt*
  new_While(const FileRegion& fr,
	    const PtExpr* cond,
	    const PtStmt* body) = 0;

  /// @brief for 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] init 初期化文
  /// @param[in] cond 繰り返し条件式
  /// @param[in] next 増加文
  /// @param[in] body 本体のステートメント
  /// @return 生成された for 文
  virtual
  const PtStmt*
  new_For(const FileRegion& fr,
	  const PtStmt* init,
	  const PtExpr* cond,
	  const PtStmt* next,
	  const PtStmt* body) = 0;

  /// @brief procedural assign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された procedural assign 文
  virtual
  const PtStmt*
  new_PcAssign(const FileRegion& fr,
	       const PtExpr* lhs,
	       const PtExpr* rhs) = 0;

  /// @brief deassign 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @return 生成された deassign 文
  virtual
  const PtStmt*
  new_Deassign(const FileRegion& fr,
	       const PtExpr* lhs) = 0;

  /// @brief force 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  /// @return 生成された force 文
  virtual
  const PtStmt*
  new_Force(const FileRegion& fr,
	    const PtExpr* lhs,
	    const PtExpr* rhs) = 0;

  /// @brief release 文の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] lhs 左辺式
  /// @return 生成された release 文
  virtual
  const PtStmt*
  new_Release(const FileRegion& fr,
	      const PtExpr* lhs) = 0;

  /// @brief parallel block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] stmt_array 本体のステートメントのリスト
  /// @return 生成された parallel block
  virtual
  const PtStmt*
  new_ParBlock(const FileRegion& fr,
	       PtStmtArray stmt_lit) = 0;

  /// @brief 名前付き parallel block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ブロック名
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] stmt_array 本体のステートメントのリスト
  /// @return 生成された parallel block
  virtual
  const PtStmt*
  new_NamedParBlock(const FileRegion& fr,
		    const char* name,
		    PtDeclHeadArray decl_array,
		    PtStmtArray stmt_lit) = 0;

  /// @brief sequential block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] stmt_array 本体のステートメントのリスト
  /// @return 生成された sequential block
  virtual
  const PtStmt*
  new_SeqBlock(const FileRegion& fr,
	       PtStmtArray stmt_lit) = 0;

  /// @brief 名前付き sequential block の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name ブロック名
  /// @param[in] decl_array 宣言のリスト
  /// @param[in] stmt_array 本体のステートメントのリスト
  /// @return 生成された sequential block
  virtual
  const PtStmt*
  new_NamedSeqBlock(const FileRegion& fr,
		    const char* name,
		    PtDeclHeadArray decl_array,
		    PtStmtArray stmt_lit) = 0;


  //////////////////////////////////////////////////////////////////////
  // expression 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr オペランド
  /// @return 生成された演算子
  virtual
  const PtExpr*
  new_Opr(const FileRegion& fr,
	  tVlOpType type,
	  const PtExpr* opr) = 0;

  /// @brief 二項演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @return 生成された演算子
  virtual
  const PtExpr*
  new_Opr(const FileRegion& fr,
	  tVlOpType type,
	  const PtExpr* opr1,
	  const PtExpr* opr2) = 0;

  /// @brief 三項演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] type 演算の種類
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  /// @param[in] opr3 オペランド3
  /// @return 生成された演算子
  virtual
  const PtExpr*
  new_Opr(const FileRegion& fr,
	  tVlOpType type,
	  const PtExpr* opr1,
	  const PtExpr* opr2,
	  const PtExpr* opr3) = 0;

  /// @brief concatination 演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr_array オペランドのリスト
  /// @return 生成された concatination 演算子
  virtual
  const PtExpr*
  new_Concat(const FileRegion& fr,
	     PtExprArray expr_array) = 0;

  /// @brief multi-concatination 演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr_array 結合するオペランドのリスト
  /// @return 生成された multi-concatination 演算子
  virtual
  const PtExpr*
  new_MultiConcat(const FileRegion& fr,
		  PtExprArray expr_array) = 0;

  /// @brief min/typ/max delay 演算子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] val0 minimum 値
  /// @param[in] val1 typical 値
  /// @param[in] val2 maximum 値
  /// @return 生成された min/typ/max 演算子
  virtual
  const PtExpr*
  new_MinTypMax(const FileRegion& fr,
		const PtExpr* val0,
		const PtExpr* val1,
		const PtExpr* val2) = 0;

  /// @brief primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name) = 0;

  /// @brief インデックス付き primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_array インデックスのリスト
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      PtExprArray index_array) = 0;

  /// @brief 範囲指定付き primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right) = 0;

  /// @brief インデックスと範囲指定付き primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_array インデックスのリスト
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      const char* name,
	      PtExprArray index_array,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right) = 0;

  /// @brief primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] nb_array 識別子の階層部分のリスト
  /// @param[in] tail_name 識別子の末尾
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name) = 0;

  /// @brief インデックス付き primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] nb_array 識別子の階層部分のリスト
  /// @param[in] tail_name 識別子の末尾
  /// @param[in] index_array インデックスのリスト
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      PtExprArray index_array) = 0;

  /// @brief 範囲指定付き primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] nb_array 識別子の階層部分のリスト
  /// @param[in] tail_name 識別子の末尾
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right) = 0;

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] nb_array 識別子の階層部分のリスト
  /// @param[in] tail_name 識別子の末尾
  /// @param[in] index_array インデックスのリスト
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された primary
  virtual
  const PtExpr*
  new_Primary(const FileRegion& fr,
	      PtNameBranchArray nb_array,
	      const char* tail_name,
	      PtExprArray index_array,
	      tVpiRangeMode mode,
	      const PtExpr* left,
	      const PtExpr* right) = 0;

  /// @brief constant primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index インデックス
  /// @return 生成された const primary
  virtual
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       const PtExpr* index);

  /// @brief constant primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] index_array インデックスのリスト
  /// @return 生成された const primary
  virtual
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       PtExprArray index_array) = 0;

  /// @brief 範囲指定付き constant primary の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 識別子名
  /// @param[in] mode 範囲指定のモード
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @return 生成された constant primary
  virtual
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       const char* name,
	       tVpiRangeMode mode,
	       const PtExpr* left,
	       const PtExpr* right) = 0;

  /// @brief インデックス付き constant primary の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] nb_array 識別子の階層部分のリスト
  /// @param[in] tail_name 識別子の末尾
  /// @param[in] index_array インデックスのリスト
  /// @return 生成された constant primary
  virtual
  const PtExpr*
  new_CPrimary(const FileRegion& fr,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       PtExprArray index_array) = 0;

  /// @brief function call の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 関数名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された function call
  virtual
  const PtExpr*
  new_FuncCall(const FileRegion& fr,
	       const char* name,
	       PtExprArray arg_array) = 0;

  /// @brief function call の生成 (階層付き)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] nb_array 関数名の階層部分のリスト
  /// @param[in] tail_name 関数名の末尾
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された function call
  virtual
  const PtExpr*
  new_FuncCall(const FileRegion& fr,
	       PtNameBranchArray nb_array,
	       const char* tail_name,
	       PtExprArray arg_array) = 0;

  /// @brief system function call の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 関数名
  /// @param[in] arg_array 引数のリスト
  /// @return 生成された function call
  virtual
  const PtExpr*
  new_SysFuncCall(const FileRegion& fr,
		  const char* name,
		  PtExprArray arg_array) = 0;

  /// @brief 整数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 定数を表す整数値
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       ymuint value) = 0;

  /// @brief 整数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       const char* value) = 0;

  /// @brief 基底付き整数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] const_type 定数の種類
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       tVpiConstType const_type,
	       const char* value) = 0;

  /// @brief サイズと基底付き定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] size サイズ
  /// @param[in] const_type 定数の種類
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_IntConst(const FileRegion& fr,
	       ymuint size,
	       tVpiConstType const_type,
	       const char* value) = 0;

  /// @brief 実数型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 定数の文字列表現
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_RealConst(const FileRegion& fr,
		double value) = 0;

  /// @brief 文字列型の定数の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 文字列
  /// @return 生成された定数
  virtual
  const PtExpr*
  new_StringConst(const FileRegion& fr,
		  const char* value) = 0;


  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 遅延を表す式
  /// @return 生成されたディレイコントロール
  virtual
  const PtControl*
  new_DelayControl(const FileRegion& fr,
		   const PtExpr* value) = 0;

  /// @brief イベントコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @return 生成されたイベントコントロール
  virtual
  const PtControl*
  new_EventControl(const FileRegion& fr);

  /// @brief イベントコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event イベント
  /// @return 生成されたイベントコントロール
  virtual
  const PtControl*
  new_EventControl(const FileRegion& fr,
		   const PtExpr* event);

  /// @brief イベントコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] event_array イベントのリスト
  /// @return 生成されたイベントコントロール
  virtual
  const PtControl*
  new_EventControl(const FileRegion& fr,
		   PtExprArray event_array) = 0;

  /// @brief リピートコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 繰り返し数を表す式
  /// @return 生成されたリピートコントロール
  virtual
  const PtControl*
  new_RepeatControl(const FileRegion& fr,
		    const PtExpr* expr);

  /// @brief リピートコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 繰り返し数を表す式
  /// @param[in] event 繰り返しの単位となるイベント
  /// @return 生成されたリピートコントロール
  virtual
  const PtControl*
  new_RepeatControl(const FileRegion& fr,
		    const PtExpr* expr,
		    const PtExpr* event);

  /// @brief リピートコントロールの生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 繰り返し数を表す式
  /// @param[in] event_array 繰り返しの単位となるイベントのリスト
  /// @return 生成されたリピートコントロール
  virtual
  const PtControl*
  new_RepeatControl(const FileRegion& fr,
		    const PtExpr* expr,
		    PtExprArray event_array) = 0;

  /// @brief 順序つき結合子の生成
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  virtual
  const PtConnection*
  new_OrderedCon(const PtExpr* expr) = 0;

  /// @brief 順序つき結合子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  virtual
  const PtConnection*
  new_OrderedCon(const FileRegion& fr,
		 const PtExpr* expr) = 0;

  /// @brief 名前付き結合子の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr 結合する式
  /// @return 生成された結合子
  virtual
  const PtConnection*
  new_NamedCon(const FileRegion& fr,
	       const char* name,
	       const PtExpr* expr) = 0;

  /// @brief strength の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value0 '0' の強度
  /// @param[in] value1 '1' の強度
  /// @return 生成された strength
  virtual
  const PtStrength*
  new_Strength(const FileRegion& fr,
	       tVpiStrength value0,
	       tVpiStrength value1) = 0;

  /// @brief charge strength の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value 強度
  /// @return 生成された strength
  virtual
  const PtStrength*
  new_Strength(const FileRegion& fr,
	       tVpiStrength value) = 0;

  /// @brief 遅延値の生成 (1つの値)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @return 生成された遅延値
  virtual
  const PtDelay*
  new_Delay(const FileRegion& fr,
	    const PtExpr* value1) = 0;

  /// @brief 遅延値の生成 (2つの値)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @return 生成された遅延値
  virtual
  const PtDelay*
  new_Delay(const FileRegion& fr,
	    const PtExpr* value1,
	    const PtExpr* value2) = 0;

  /// @brief 遅延値の生成 (3つの値)
  /// @param[in] fr ファイル位置の情報
  /// @param[in] value1 値1
  /// @param[in] value2 値2
  /// @param[in] value3 値3
  /// @return 生成された遅延値
  virtual
  const PtDelay*
  new_Delay(const FileRegion& fr,
	    const PtExpr* value1,
	    const PtExpr* value2,
	    const PtExpr* value3) = 0;

  /// @brief 階層名の生成
  /// @param[in] name 名前
  /// @return 生成された階層名
  virtual
  const PtNameBranch*
  new_NameBranch(const char* name) = 0;

  /// @brief インデックス付きの階層名の生成
  /// @param[in] name 名前
  /// @param[in] index インデックス
  /// @return 生成された階層名
  virtual
  const PtNameBranch*
  new_NameBranch(const char* name,
		 int index) = 0;

  /// @brief 順序付き結合子の配列の生成
  /// @param[in] expr1 結合する式
  PtConnectionArray
  ConArray(const PtExpr* expr1);

  /// @brief 順序付き結合子の配列の生成
  /// @param[in] expr1 結合する式
  /// @param[in] expr2 結合する式
  PtConnectionArray
  ConArray(const PtExpr* expr1,
	   const PtExpr* expr2);

  /// @brief 順序付き結合子の配列の生成
  /// @param[in] expr1 結合する式
  /// @param[in] expr2 結合する式
  /// @param[in] expr3 結合する式
  PtConnectionArray
  ConArray(const PtExpr* expr1,
	   const PtExpr* expr2,
	   const PtExpr* expr3);

  /// @brief 順序付き結合子の配列の生成
  /// @param[in] expr1 結合する式
  /// @param[in] expr2 結合する式
  /// @param[in] expr3 結合する式
  /// @param[in] expr4 結合する式
  PtConnectionArray
  ConArray(const PtExpr* expr1,
	   const PtExpr* expr2,
	   const PtExpr* expr3,
	   const PtExpr* expr4);


  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] as_array attribute spec のリスト
  /// @return 生成された attribute instance
  virtual
  const PtAttrInst*
  new_AttrInst(const FileRegion& fr,
	       PtAttrSpecArray as_array) = 0;

  /// @brief attribute spec の生成
  /// @param[in] fr ファイル位置の情報
  /// @param[in] name 名前
  /// @param[in] expr 値
  /// @return 生成された attribute spec
  virtual
  const PtAttrSpec*
  new_AttrSpec(const FileRegion& fr,
	       const char* name,
	       const PtExpr* expr) = 0;


  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列領域の確保
  virtual
  const char*
  new_string(const char* str) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // おもにデバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保したオブジェクトに関する情報を出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump_profile(ostream& s) const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部と継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief T の配列を確保する．
  /// @param[in] size 配列のサイズ
  template<typename T>
  T*
  alloc_array(ymuint size);


  /// @brief メモリアロケータを得る．
  Alloc&
  alloc();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  Alloc& mAlloc;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

/// @brief T の配列を確保する．
/// @param[in] size 配列のサイズ
template<typename T>
inline
T*
PtiFactory::alloc_array(ymuint size)
{
  void* p = mAlloc.get_memory(sizeof(T) * size);
  return new (p) T[size];
}

END_NAMESPACE_YM_VERILOG

#endif // PTFACTORY_H
