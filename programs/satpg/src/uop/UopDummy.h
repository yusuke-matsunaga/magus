#ifndef UOPDUMMY_H
#define UOPDUMMY_H

/// @file UopDummy.h
/// @brief UopDummy のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "UntestOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class UopDummy UopDummy.h "UopDummy.h"
/// @brief UntestOp の基本的な動作を行なうクラス
//////////////////////////////////////////////////////////////////////
class UopDummy :
  public UntestOp
{
public:

  /// @brief コンストラクタ
  UopDummy();

  /// @brief デストラクタ
  virtual
  ~UopDummy();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト不能故障と判定された時の処理
  /// @param[in] f 故障
  virtual
  void
  operator()(TpgFault* f);

};

END_NAMESPACE_YM_SATPG

#endif // UOPDUMMY_H
