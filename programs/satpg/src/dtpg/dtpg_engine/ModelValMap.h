#ifndef MODELVALMAP_H
#define MODELVALMAP_H

/// @file ModelValMap.h
/// @brief ModelValMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ValMap.h"
#include "YmLogic/Bool3.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class ModelValMap ModelValMap.h "ModelValMap.h"
/// @brief SAT ソルバの model 配列から値を読みだす ValMap
//////////////////////////////////////////////////////////////////////
class ModelValMap :
  public ValMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] model SATソルバの作ったモデル
  ModelValMap(const vector<Bool3>& model);

  /// @brief デストラクタ
  virtual
  ~ModelValMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの正常値を返す．
  /// @param[in] node 対象のノード
  virtual
  Val3
  gval(TpgNode* node) const;

  /// @brief ノードの故障値を返す．
  /// @param[in] node 対象のノード
  virtual
  Val3
  fval(TpgNode* node) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モデル
  const vector<Bool3>& mModel;

};

END_NAMESPACE_YM_SATPG

#endif // GVALMODELMAP_H
