#ifndef MISLIBPIN_H
#define MISLIBPIN_H

/// @file MislibPin.h
/// @brief MislibPin の派生クラスのヘッダファイル(その2)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibPin MislibPin.h "MislibPin.h"
/// @brief ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPin :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] name ピン名
  /// @param[in] phase 極性
  /// @param[in] input_load 入力負荷
  /// @param[in] max_load 最大駆動負荷
  /// @param[in] rise_block_delay 立ち上がり固定遅延
  /// @param[in] rise_fanout_delay 立ち上がり負荷依存遅延
  /// @param[in] fall_block_delay 立ち下がり固定遅延
  /// @param[in] fall_fanout_delay 立ち下がり負荷依存遅延
  MislibPin(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* phase,
	    const MislibNode* input_load,
	    const MislibNode* max_load,
	    const MislibNode* rise_block_delay,
	    const MislibNode* rise_fanout_delay,
	    const MislibNode* fall_block_delay,
	    const MislibNode* fall_fanout_delay);

  /// @brief デストラクタ
  virtual
  ~MislibPin();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief ピン名を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  name() const;

  /// @brief 極性情報を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  phase() const;

  /// @brief 入力負荷を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  input_load() const;

  /// @brief 最大駆動負荷を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  max_load() const;

  /// @brief 立ち上がり固定遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  rise_block_delay() const;

  /// @brief 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  rise_fanout_delay() const;

  /// @brief 立ち下がり固定遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  fall_block_delay() const;

  /// @brief 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  fall_fanout_delay() const;

  /// @brief 次のピンを設定する．
  virtual
  void
  set_next(MislibNodeImpl* pin);

  /// @brief 次のピンを取り出す．
  virtual
  const MislibNode*
  next() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン名
  const MislibNode* mName;

  // 極性情報
  const MislibNode* mPhase;

  // 入力負荷
  const MislibNode* mInputLoad;

  // 最大駆動負荷
  const MislibNode* mMaxLoad;

  // 立ち上がり固有遅延
  const MislibNode* mRiseBlockDelay;

  // 立ち上がり負荷依存遅延係数
  const MislibNode* mRiseFanoutDelay;

  // 立ち下がり固有遅延
  const MislibNode* mFallBlockDelay;

  // 立ち下がり負荷依存遅延係数
  const MislibNode* mFallFanoutDelay;

  // 次のピン
  MislibNodeImpl* mNext;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBPIN_H
