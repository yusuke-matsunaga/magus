#ifndef EUFDIS_H
#define EUFDIS_H

/// @file EufDis.h
/// @brief EufDis のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufBin.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufDis EufDis.h "EufDis.h"
/// @brief disjunctive ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class EufDis :
  public EufBin
{
  friend class EufMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] left, right 左辺と右辺の式
  EufDis(EufNode* left,
	 EufNode* right);

  /// @brief デストラクタ
  virtual
  ~EufDis();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  tType
  type() const;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFCON_H
