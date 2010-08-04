#ifndef LIBYM_MVN_VERILOG_READERIMPL_H
#define LIBYM_MVN_VERILOG_READERIMPL_H

/// @file libym_mvn/verilog/ReaderImpl.h
/// @brief ReaderImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/VlMgr.h"
#include "ym_verilog/vl/VlFwd.h"
#include "ym_utils/MsgHandler.h"
#include "DeclMap.h"
#include "Driver.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

using namespace nsYm::nsVerilog;

//////////////////////////////////////////////////////////////////////
/// @class ReaderImpl ReaderImpl.h "ReaderImpl.h"
/// @brief 合成可能な Verilog 記述を読み込んで MVN に設定するクラス
//////////////////////////////////////////////////////////////////////
class ReaderImpl
{
public:

  /// @brief コンストラクタ
  ReaderImpl();

  /// @brief デストラクタ
  ~ReaderImpl();


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();
  
  /// @brief verilog 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @retval true 正常に読み込めた．
  /// @retval false 読込中にエラーが起こった．
  bool
  read(const string& filename,
       const SearchPathList& searchpath = SearchPathList(),
       const list<VlLineWatcher*> watcher_list = list<VlLineWatcher*>());

  /// @brief 今まで読み込んだ情報からネットワークを生成する．
  /// @param[in] mgr ネットワーク生成用のマネージャ
  /// @param[in] searchpath サーチパス
  /// @param[in] watcher_list 行番号ウォッチャーのリスト
  /// @retval true 正常に処理を行った．
  /// @retval false 生成中にエラーが起こった．
  bool
  gen_network(MvMgr& mgr);
  
  /// @brief メッセージハンドラを付加する．
  /// @param[in] msg_handler 登録するハンドラ
  /// @note このハンドラはこのオブジェクトの破壊に伴って破壊される．
  void
  add_msg_handler(MsgHandler* msg_handler);

  /// @brief フリップフロップのセル名，ピン名を設定する．
  /// @param[in] cell_name セル名
  /// @param[in] data_pin_name データ入力ピン名
  /// @param[in] clock_pin_name クロック入力ピン名
  /// @param[in] q_pin_name ノーマル出力ピン名
  /// @param[in] qn_pin_name 反転出力ピン名
  /// @param[in] set_pin_name セットピン名
  /// @param[in] reset_pin_name リセットピン名
  /// @note 存在しない場合には空文字列を渡す．
  void
  set_ffname(const string& cell_name,
	     const string& data_pin_name,
	     const string& clock_pin_name,
	     const string& q_pin_name,
	     const string& qn_pin_name,
	     const string& set_pin_name,
	     const string& reset_pin_name);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief module を生成する．
  /// @param[in] vl_module 対象のモジュール
  MvModule*
  gen_module(const VlModule* vl_module);

  /// @brief モジュール内部の要素を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] vl_module 対象のモジュール
  void
  gen_moduleitem(MvModule* module,
		 const VlModule* vl_module);

  /// @brief 宣言要素を生成する．
  /// @param[in] module モジュール
  /// @param[in] vl_scope 対象のスコープ
  /// @retval true 成功した．
  /// @retval false エラーが起こった．
  /// @note 内部に下位のスコープを含む場合には再帰する．
  bool
  gen_decl(MvModule* module,
	   const VlNamedObj* vl_scope);

  /// @brief 要素を生成する．
  /// @param[in] module モジュール
  /// @param[in] vl_scope 対象のスコープ
  /// @retval true 成功した．
  /// @retval false エラーが起こった．
  /// @note 内部に下位のスコープを含む場合には再帰する．
  bool
  gen_item(MvModule* module,
	   const VlNamedObj* vl_scope);

  /// @brief ポート参照式の実体化を行う．
  /// @param[in] expr 対象の式
  /// @param[out] node 対応するノードを格納する変数
  /// @param[out] msb ビット指定位置か範囲指定の MSB を格納する変数
  /// @param[out] lsb 範囲指定の LSB を格納する変数
  /// @retval 0 単純な形式だった．
  /// @retval 1 ビット指定形式だった．
  /// @retval 2 範囲指定形式だった．
  int
  gen_portref(const VlExpr* expr,
	      MvNode*& node,
	      ymuint& msb,
	      ymuint& lsb);

  /// @brief モジュールインスタンスの生成を行う．
  /// @param[in] vl_module モジュール
  /// @param[in] parent_module 親のモジュール
  void
  gen_moduleinst(const VlModule* vl_module,
		 MvModule* parent_module);
  
  /// @brief 左辺式に接続する．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] expr 左辺式
  /// @param[in] node 右辺に対応するノード
  void
  connect_lhs(MvModule* parent_module,
	      const VlExpr* expr,
	      MvNode* node);

  /// @brief 左辺式に接続する．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] expr 左辺式
  /// @param[in] node 右辺に対応するノード
  /// @param[in] offset node に対するオフセット
  void
  connect_lhs_sub(MvModule* parent_module,
		  const VlExpr* expr,
		  MvNode* node,
		  ymuint offset);
  
  /// @brief プリミティブインスタンスの生成を行う．
  /// @param[in] prim プリミティブ
  /// @param[in] parent_module 親のモジュール
  void
  gen_priminst(const VlPrimitive* prim,
	       MvModule* parent_module);

  /// @brief AND のバランス木を作る．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ピンを格納する配列
  /// @param[in] offset inputs のオフセット
  MvNode*
  gen_andtree(MvModule* parent_module,
	      ymuint ni,
	      vector<pair<MvNode*, ymuint> >& inputs,
	      ymuint offset);

  /// @brief OR のバランス木を作る．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ピンを格納する配列
  /// @param[in] offset inputs のオフセット
  MvNode*
  gen_ortree(MvModule* parent_module,
	     ymuint ni,
	     vector<pair<MvNode*, ymuint> >& inputs,
	     ymuint offset);

  /// @brief XOR のバランス木を作る．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ピンを格納する配列
  /// @param[in] offset inputs のオフセット
  MvNode*
  gen_xortree(MvModule* parent_module,
	      ymuint ni,
	      vector<pair<MvNode*, ymuint> >& inputs,
	      ymuint offset);

  /// @brief 式に対応したノードの木を作る．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] expr 式
  MvNode*
  gen_expr1(MvModule* parent_module,
	    const VlExpr* expr);

  /// @brief 宣言要素への参照に対応するノードを作る．
  /// @param[in] expr 式
  MvNode*
  gen_expr2(const VlExpr* expr);

  /// @brief ドライバーを登録する．
  /// @param[in] node 左辺に対応するノード
  /// @param[in] driver ドライバー
  void
  reg_driver(MvNode* node,
	     const Driver& driver);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief FF のセル名，ピン名を表す構造体
  struct FFInfo
  {
    /// @brief コンストラクタ
    FFInfo(const string& data_pin_name,
	   const string& clock_pin_name,
	   const string& q_pin_name,
	   const string& qn_pin_name,
	   const string& set_pin_name,
	   const string& reset_pin_name);
    
    /// @brief データ入力ピン名
    string mDataPinName;
    /// @brief クロック入力ピン名
    string mClockPinName;
    /// @brief ノーマル出力ピン名
    string mQPinName;
    /// @brief 反転出力ピン名
    string mQnPinName;
    /// @brief セットピン名
    string mSetPinName;
    /// @brief リセットピン名
    string mResetPinName;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // メッセージハンドラの管理者
  MsgMgr mMsgMgr;

  // Verilog を扱うマネージャ
  VlMgr mVlMgr;

  // FF セルの情報を保持するハッシュ表
  hash_map<string, FFInfo> mFFInfoDict;

  // MvNetwork を扱うマネージャ
  MvMgr* mMvMgr;

  // VlDecl と MvNode の対応付けをとるハッシュ表
  DeclMap mDeclMap;

  // VlDecl のドライバーのリスト
  vector<vector<Driver> > mDriverList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
ReaderImpl::FFInfo::FFInfo(const string& data_pin_name,
			   const string& clock_pin_name,
			   const string& q_pin_name,
			   const string& qn_pin_name,
			   const string& set_pin_name,
			   const string& reset_pin_name) :
  mDataPinName(data_pin_name),
  mClockPinName(clock_pin_name),
  mQPinName(q_pin_name),
  mQnPinName(qn_pin_name),
  mSetPinName(set_pin_name),
  mResetPinName(reset_pin_name)
{
}

END_NAMESPACE_YM_MVN_VERILOG

#endif // LIBYM_MVN_VERILOG_READERIMPL_H
