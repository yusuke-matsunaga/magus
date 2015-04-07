#ifndef TPGCNF2_H
#define TPGCNF2_H

/// @file TpgCnf2.h
/// @brief TpgCnf2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "SatEngine.h"
#include "GenVidMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgCnf2 TpgCnf2.h "TpgCnf2.h"
/// @brief 単一の故障解析器
//////////////////////////////////////////////////////////////////////
class TpgCnf2
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  TpgCnf2(const string& sat_type,
	  const string& sat_option,
	  ostream* sat_outp);

  /// @brief デストラクタ
  ~TpgCnf2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief f1 が f2 を支配しているか調べる．
  /// @param[in] f1, f2 故障
  /// @param[in] max_id ノード番号の最大値
  bool
  check_dominance(TpgFault* f1,
		  TpgFault* f2,
		  ymuint max_id);

  /// @brief f1 と f2 がコンフリクトしているか調べる．
  /// @param[in] f1, f2 故障
  /// @param[in] max_id ノード番号の最大値
  bool
  check_conflict(TpgFault* f1,
		 TpgFault* f2,
		 ymuint max_id);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常回路と故障回路のCNFを作る．
  /// @param[in] f1, f2 故障
  /// @param[in] detect_f2 f2 を検出するとき true とするフラグ
  /// @param[in] max_id ノードの最大番号
  void
  make_fval_cnf(TpgFault* f1,
		TpgFault* f2,
		bool detect_f2,
		ymuint max_id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATエンジン
  SatEngine mEngine;

  // ノード番号の最大値
  ymuint mMaxId;

  // 正常値
  GenVidMap mGvarMap;

  // 故障値
  GenVidMap mFvar1Map;

  // 故障伝搬値
  GenVidMap mDvar1Map;

  // 故障値
  GenVidMap mFvar2Map;

  // 故障伝搬値
  GenVidMap mDvar2Map;

};

END_NAMESPACE_YM_SATPG

#endif // TPGCNF2_H
