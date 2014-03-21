#ifndef UOPLIST_H
#define UOPLIST_H

/// @file UopList.h
/// @brief UopList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "UntestOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class UopList UopList.h "UopList.h"
/// @brief UntestOp のリストを持つクラス
//////////////////////////////////////////////////////////////////////
class UopList :
  public UntestOp
{
public:

  /// @brief コンストラクタ
  UopList();

  /// @brief デストラクタ
  virtual
  ~UopList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief uop を追加する．
  void
  add(UntestOp* uop);

  /// @brief テスト不能故障と判定された時の処理
  /// @param[in] f 故障
  virtual
  void
  operator()(TpgFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // uop のリスト
  vector<UntestOp*> mUopList;

};

END_NAMESPACE_YM_SATPG

#endif // UOPLIST_H
