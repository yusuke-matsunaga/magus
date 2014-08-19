#ifndef RLIMP_H
#define RLIMP_H

/// @file RlImp.h
/// @brief RlImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BNetwork.h"
#include "ImpVal.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpMgr;
class ImpInfo;
class ImpNode;

//////////////////////////////////////////////////////////////////////
/// @class RlImp RlImp.h "RlImp.h"
/// @brief recursive learning を用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class RlImp
{
public:

  /// @brief コンストラクタ
  RlImp();

  /// @brief デストラクタ
  virtual
  ~RlImp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] imp_mgr マネージャ
  /// @param[out] imp_info 間接含意のリスト
  virtual
  void
  learning(ImpMgr& imp_mgr,
	   ImpInfo& imp_info);

  /// @brief ラーニングのレベルを設定する．
  void
  set_learning_level(ymuint level);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief recursive learning を行なう．
  /// @param[in] imp_mgr ImpMgr
  /// @param[in] node ノード
  /// @param[in] val 値
  /// @param[in] level レベル
  /// @param[out] imp_list 含意のリスト
  bool
  make_all_implication(ImpMgr& imp_mgr,
		       ImpNode* node,
		       ymuint val,
		       ymuint level,
		       vector<ImpVal>& imp_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラーニングのレベル
  ymuint32 mLevel;

};

END_NAMESPACE_YM_NETWORKS

#endif // RLIMP_H
