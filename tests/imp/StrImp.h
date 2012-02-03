#ifndef STRIMP_H
#define STRIMP_H

/// @file StrImp.h
/// @brief StrImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

class StrNode;

//////////////////////////////////////////////////////////////////////
/// @class StrImp StrImp.h "StrImp.h"
/// @brief 構造を用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class StrImp :
  public ImpBase
{
public:

  /// @brief コンストラクタ
  StrImp();

  /// @brief デストラクタ
  virtual
  ~StrImp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] network 対象のネットワーク
  /// @param[in] imp_list 間接含意のリスト
  virtual
  void
  learning(const BdnMgr& network,
	   vector<ImpInfo>& imp_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 含意用のノードの配列
  vector<StrNode*> mNodeArray;

};

END_NAMESPACE_YM_NETWORKS

#endif // STRIMP_H
