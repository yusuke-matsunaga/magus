#ifndef UOPSKIP_H
#define UOPSKIP_H

/// @file UopSkip.h
/// @brief UopSkip のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "UntestOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class UopSkip UopSkip.h "UopSkip.h"
/// @brief 指定した回数だけ検出不能となったらスキップフラグをセットする．
//////////////////////////////////////////////////////////////////////
class UopSkip :
  public UntestOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] threshold しきい値
  UopSkip(ymuint threshold);

  /// @brief デストラクタ
  virtual
  ~UopSkip();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト不能故障と判定された時の処理
  /// @param[in] f 故障
  virtual
  void
  operator()(TpgFault* f);

  /// @brief 検出不能回数とスキップフラグをクリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // スキップフラグをセットするしきい値
  ymuint32 mThreshold;

  // 検出不能となった故障を保持するリスト
  vector<TpgFault*> mUntestList;

  // スキップフラグがセットされた故障を保持するリスト
  vector<TpgFault*> mSkipList;

};

END_NAMESPACE_YM_SATPG

#endif // UOPSKIP_H
