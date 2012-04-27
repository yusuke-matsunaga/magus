#ifndef IMPREC_H
#define IMPREC_H

/// @file ImpRec.h
/// @brief ImpRec のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BNetwork.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpNode;

//////////////////////////////////////////////////////////////////////
/// @class ImpRec ImpRec.h "ImpRec.h"
/// @brief 含意結果を記録するための基底クラス
//////////////////////////////////////////////////////////////////////
class ImpRec
{
public:

  /// @brief デストラクタ
  virtual
  ~ImpRec() { }


public:

  /// @brief 含意結果を記録する仮想関数
  virtual
  void
  record(ImpNode* node,
	 ymuint val) = 0;

};

END_NAMESPACE_YM_NETWORKS

#endif // IMPREC_H
