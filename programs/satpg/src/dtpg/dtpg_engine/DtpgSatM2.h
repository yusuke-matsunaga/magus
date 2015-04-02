#ifndef DTPGSATM2_H
#define DTPGSATM2_H

/// @file DtpgSatM2.h
/// @brief DtpgSatM2 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatBaseM.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSatM2 DtpgSatM2.h "DtpgSatM2.h"
/// @brief 複数の故障の検出に使える CNF 式を生成するタイプの SatEngine
//////////////////////////////////////////////////////////////////////
class DtpgSatM2 :
  public DtpgSatBaseM
{
public:

  /// @brief コンストラクタ
  /// @param[in] th_val しきい値
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  DtpgSatM2(ymuint th_val,
	    const string& sat_type,
	    const string& sat_option,
	    ostream* sat_outp,
	    BackTracer& bt,
	    DetectOp& dop,
	    UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~DtpgSatM2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] node_set 対象のノード集合
  /// @param[in] fnode_list 対象の故障を持つノードのリスト
  /// @param[in] flist 対象の故障リスト
  virtual
  void
  run_multi(const NodeSet& node_set,
	    const vector<TpgNode*>& fnode_list,
	    const vector<TpgFault*>& flist);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // しきい値
  ymuint32 mThVal;

  // 作業用のノードリスト
  vector<TpgNode*> mTmpNodeList;

  // 処理済みのノードのマーク
  vector<ymuint> mMark;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGSATM2_H
