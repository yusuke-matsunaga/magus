#ifndef YM_MVN_MVMGR_H
#define YM_MVN_MVMGR_H

/// @file ym_mvn/MvMgr.h
/// @brief MvMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvMgr MvMgr.h "ym_mvn/MvMgr.h"
/// @brief 多値ネットワークの生成/設定を行うクラス
//////////////////////////////////////////////////////////////////////
class MvMgr
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvMgr();

  /// @brief デストラクタ
  ~MvMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief トップレベルモジュールのリストを得る．
  /// @param[out] module_list モジュールを格納するリスト
  /// @return 要素数を返す．
  /// @note この関数はトップモジュール数ではなく全モジュール数に比例した
  /// 実行時間を要する．
  ymuint
  topmodule_list(list<MvModule*>& module_list) const;

  /// @brief モジュール番号の最大値+1を得る．
  ymuint
  max_module_id() const;

  /// @brief モジュールIDをキーにしてモジュールにアクセスする．
  /// @param[in] id モジュールID ( 0 <= id < max_module_id() )
  /// @return 該当するモジュールを返す．
  /// @note 該当するモジュールがない場合は NULL を返す．
  MvModule*
  module(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報の設定を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールを生成する．
  /// @param[in] name 名前
  /// @param[in] np ポート数
  /// @param[in] ni 入力ノード数
  /// @param[in] no 出力ノード数
  /// @return 生成したモジュールを返す．
  /// @note 入出力ノードのビット幅は1で初期化される．
  MvModule*
  new_module(const char* name,
	     ymuint np,
	     ymuint ni,
	     ymuint no);

  /// @brief モジュールを生成する．
  /// @param[in] name 名前
  /// @param[in] np ポート数
  /// @param[in] ibitwidth_array 入力のビット幅の配列
  /// @param[in] obitwidth_array 出力のビット幅の配列
  /// @return 生成したモジュールを返す．
  MvModule*
  new_module(const char* name,
	     ymuint np,
	     const vector<ymuint>& ibitwidth_array,
	     const vector<ymuint>& obitwidth_array);

  /// @brief モジュールを削除する．
  /// @param[in] module 対象のモジュール
  /// @note モジュールインスタンスとして使われているモジュールは削除できない．
  void
  delete_module(MvModule* module);

  /// @brief through ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_through(MvModule* module,
	      ymuint bit_width = 1);

  /// @brief not ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_not(MvModule* module,
	  ymuint bit_width = 1);

  /// @brief and ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_and(MvModule* module,
	  ymuint bit_width = 1);
  
  /// @brief or ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_or(MvModule* module,
	 ymuint bit_width = 1);
  
  /// @brief xor ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_xor(MvModule* module,
	  ymuint bit_width = 1);

  /// @brief reduction and ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_rand(MvModule* module,
	   ymuint bit_width = 1);

  /// @brief reduction or ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_ror(MvModule* module,
	  ymuint bit_width = 1);

  /// @brief reduction xor ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_rxor(MvModule* module,
	   ymuint bit_width = 1);

  /// @brief equal ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_equal(MvModule* module,
	    ymuint bit_width);

  /// @brief less than ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_lt(MvModule* module,
	 ymuint bit_width);

  /// @brief shift left logical ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_sll(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief shift right logical ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_srl(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief shift left arithmetic ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_sla(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief shift right arithmetic ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_sra(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief add ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_add(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);


  /// @brief sub ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_sub(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief mult ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_mult(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief div ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_div(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief modulo ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_mod(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);
  
  /// @brief power ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_pow(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief condition ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_ite(MvModule* module,
	  ymuint bit_width);

  /// @brief concatenate ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] ibitwidth_array 入力のビット幅の配列
  /// @return 生成したノードを返す．
  /// @note 出力のビット幅は入力のビット幅の和となる．
  MvNode*
  new_concat(MvModule* module,
	     const vector<ymuint>& ibitwidth_arary);

  /// @brief bit-selectノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bitpos ビット位置
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_bitselect(MvModule* module,
		ymuint bitpos,
		ymuint bit_width);

  /// @brief part-select ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] msb 範囲指定の MSB
  /// @param[in] lsb 範囲指定の LSB
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_partselect(MvModule* module,
		 ymuint msb,
		 ymuint lsb,
		 ymuint bit_width);

  /// @brief module instance ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] submodule 下位モジュール
  /// @return 生成したノードを返す．
  MvNode*
  new_inst(MvModule* module,
	   MvModule* submodule);

  /// @brief constant ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @param[in] val 値
  MvNode*
  new_const(MvModule* module,
	    ymuint bit_width,
	    const vector<ymuint32>& val);

  /// @brief ノードを削除する．
  /// @param[in] node 対象のノード
  /// @note 入力ノード, 出力ノードは削除できない
  /// @note 接続が残っているノードは削除できない．
  void
  delete_node(MvNode* node);
  
  /// @brief ピンとピンを接続する．
  /// @param[in] src_pin 入力元のピン
  /// @param[in] dst_pin 出力先のピン
  /// @return 接続を表すネットを返す．
  /// @note 接続が失敗したら NULLを返す．
  /// @note 接続が失敗するのは，
  ///  - ピンが異なるモジュールに属していた．
  ///  - ピンのビット幅が異なっていた．
  MvNet*
  connect(MvOutputPin* src_pin,
	  MvInputPin* dst_pin);

  /// @brief 接続を取り除く
  /// @param[in] net 接続を表すネット
  void
  disconnect(MvNet* net);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを登録する．
  /// @param[in] node 対象のノード
  void
  reg_node(MvNode* node);

  /// @brief ノードの登録を削除する．
  /// @param[in] node 対象のノード
  void
  unreg_node(MvNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュールを納めた配列
  // ID番号をキーにしている．
  // 場合によっては穴が空いている．
  vector<MvModule*> mModuleArray;

  // ID番号を管理するためのオブジェクト
  ItvlMgr mItvlMgr;
  
};


/// @brief 内容を出力する
/// @param[in] s 出力先のストリーム
/// @param[in] mgr MvMgr
/// @note デバッグ用
void
dump(ostream& s,
     const MvMgr& mgr);

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVMGR_H
