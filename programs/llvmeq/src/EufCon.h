#ifndef EUFCON_H
#define EUFCON_H

/// @file EufCon.h
/// @brief EufCon のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufBin.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufCon EufCon.h "EufCon.h"
/// @brief conjunctive ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class EufCon :
  public EufBin
{
  friend class EufMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] left, right 左辺と右辺の式
  EufCon(EufNode* left,
	 EufNode* right);

  /// @brief デストラクタ
  virtual
  ~EufCon();


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
