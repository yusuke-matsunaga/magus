#ifndef SKIPUNTESTOP_H
#define SKIPUNTESTOP_H

/// @file SkipUntestOp.h
/// @brief SkipUntestOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "UntestOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SkipUntestOp SkipUntestOp.h "SkipUntestOp.h"
/// @brief 指定した回数だけ検出不能となったらスキップフラグをセットする．
//////////////////////////////////////////////////////////////////////
class SkipUntestOp :
  public UntestOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] threshold しきい値
  SkipUntestOp(ymuint threshold);

  /// @brief デストラクタ
  virtual
  ~SkipUntestOp();


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

#endif // SKIPUNTESTOP_H
