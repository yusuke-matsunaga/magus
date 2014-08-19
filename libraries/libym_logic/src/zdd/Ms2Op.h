#ifndef MS2OP_H
#define MS2OP_H

/// @file Ms2Op.h
/// @brief Ms2Op のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ZddOp.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class Ms2Op Ms2Op.h "Ms2Op.h"
/// @brief minimum set を求める演算クラス
//////////////////////////////////////////////////////////////////////
class Ms2Op :
  public ZddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  Ms2Op(ZddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~Ms2Op();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief minimum set 演算を行う関数
  ZddEdge
  apply(ZddEdge left);

  /// @brief 次回のGCで回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief apply() の下請け関数
  /// @param[in] e 対象の枝
  /// @param[out] nelem 要素数
  /// @return minimum set の枝を返す．
  ZddEdge
  apply_step(ZddEdge e,
	     ymuint& nelem);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  typedef unordered_map<ZddEdge, pair<ZddEdge, ymuint32> > MinimumSetMap;

  // 枝をキーとしてminimum set を保持するハッシュ表
  MinimumSetMap mHash;

};


END_NAMESPACE_YM_ZDD

#endif // MS2OP_H
