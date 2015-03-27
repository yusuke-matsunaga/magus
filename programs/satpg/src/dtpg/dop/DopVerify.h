#ifndef DOPVERIFY_H
#define DOPVERIFY_H

/// @file DopVerify.h
/// @brief DopVerify のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DopVerify DopVerify.h "DopVerify.h"
/// @brief 故障シミュレーションを行なった故障ドロップを行なうクラス
//////////////////////////////////////////////////////////////////////
class DopVerify :
  public DetectOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  DopVerify(Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~DopVerify();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった時の処理
  /// @param[in] f 故障
  /// @param[in] assign_list 値割当のリスト
  virtual
  void
  operator()(TpgFault* f,
	     const NodeValList& assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

};

END_NAMESPACE_YM_SATPG

#endif // DOPVERIFY_H
