#ifndef RLIMP_H
#define RLIMP_H

/// @file RlImp.h
/// @brief RlImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpBase.h"
#include "ImpInfo.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpMgr;
class StrNode;

//////////////////////////////////////////////////////////////////////
/// @class RlImp RlImp.h "RlImp.h"
/// @brief 構造を用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class RlImp :
  public ImpBase
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
  /// @param[in] network 対象のネットワーク
  /// @param[in] imp_info 間接含意のリスト
  virtual
  void
  learning(const BdnMgr& network,
	   ImpInfo& imp_info);


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
		       StrNode* node,
		       ymuint val,
		       ymuint level,
		       vector<ImpCell>& imp_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 含意用のノードの配列
  vector<StrNode*> mNodeArray;

};

END_NAMESPACE_YM_NETWORKS

#endif // RLIMP_H
