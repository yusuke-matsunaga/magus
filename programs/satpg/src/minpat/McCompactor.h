#ifndef MCCOMPACTOR_H
#define MCCOMPACTOR_H

/// @file Compactor.h
/// @brief Compactor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Compactor Compactor.h "Compactor.h"
/// @brief 故障グループを圧縮するクラス
//////////////////////////////////////////////////////////////////////
class McCompactor
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  McCompactor(Fsim& fsim);


  /// @brief デストラクタ
  ~McCompactor();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストセットの最小被覆問題を解く．
  /// @param[in] fault_list 故障リスト
  /// @param[in] orig_tv_list もとのテストセット
  /// @param[out] tv_list 圧縮されたテストセット
  void
  run(const vector<const TpgFault*>& fault_list,
      const vector<TestVector*>& orig_tv_list,
      vector<TestVector*>& tv_list);

  /// @brief verbose フラグを設定する．
  /// @param[in] verbose 指定する値
  void
  set_verbose(ymuint verbose);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // verbose フラグ
  ymuint mVerbose;

};

END_NAMESPACE_YM_SATPG

#endif // MCCOMPACTOR_H
