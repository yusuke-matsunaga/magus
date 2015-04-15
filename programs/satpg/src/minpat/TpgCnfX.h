#ifndef TPGCNFX_H
#define TPGCNFX_H

/// @file TpgCnfX.h
/// @brief TpgCnfX のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "SatEngine.h"
#include "GenVidMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgCnfX TpgCnfX.h "TpgCnfX.h"
/// @brief X個の故障解析器
//////////////////////////////////////////////////////////////////////
class TpgCnfX
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  TpgCnfX(const string& sat_type,
	  const string& sat_option,
	  ostream* sat_outp);

  /// @brief デストラクタ
  ~TpgCnfX();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障グループが両立か調べる．
  /// @param[in] fault_group 故障のリスト
  /// @param[in] max_id ノード番号の最大値
  bool
  check_compat(const vector<TpgFault*>& fault_group,
	       ymuint max_id);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATエンジン
  SatEngine mEngine;

  // ノード番号の最大値
  ymuint mMaxId;

  // 正常値のマップ
  GenVidMap mGvarMap;

  // 故障値のマップの配列
  vector<GenVidMap> mFvarMapArray;

  // 故障伝搬値のマップの配列
  vector<GenVidMap> mDvarMapArray;

};

END_NAMESPACE_YM_SATPG

#endif // TPGCNFX_H
