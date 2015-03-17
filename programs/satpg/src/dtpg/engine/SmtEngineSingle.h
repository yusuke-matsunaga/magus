#ifndef SMTENGINESINGLE_H
#define SMTENGINESINGLE_H

/// @file SmtEngineSingle.h
/// @brief SmtEngineSingle のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SmtEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SmtEngineSingle SmtEngineSingle.h "SmtEngineSingle.h"
/// @brief 1つの故障を対象とした CNF を生成する SmtEngine
//////////////////////////////////////////////////////////////////////
class SmtEngineSingle :
  public SmtEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] network 対象のネットワーク
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  SmtEngineSingle(const string& sat_type,
		  const string& sat_option,
		  ostream* sat_outp,
		  const TpgNetwork& network,
		  BackTracer& bt,
		  DetectOp& dop,
		  UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~SmtEngineSingle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] f_tgt 対象の故障
  virtual
  void
  run(TpgFault* f_tgt);

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  virtual
  void
  run(const vector<TpgFault*>& flist);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_SATPG

#endif // SATENGINESINGLE_H
