#ifndef LIBYM_NETWORKS_CMN_CMNNODELATCHIN_H
#define LIBYM_NETWORKS_CMN_CMNNODELATCHIN_H

/// @file libym_networks/cmn/CmnNodeLatchIn.h
/// @brief CmnNodeLatchIn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CmnNodeOutput.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnNodeLatchIn CmnNodeLatchIn.h "CmnNodeLatchIn.h"
/// @brief ラッチの入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeLatchIn :
  public CmnNodeOutput
{
  friend class CmnMgrImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] latch 親のラッチ
  CmnNodeLatchIn(CmnLatch* latch);

  /// @brief デストラクタ
  virtual
  ~CmnNodeLatchIn();


public:

  /// @brief 関連する ラッチを返す．
  /// @note ラッチに関連していない場合には NULL を返す．
  virtual
  const CmnLatch*
  latch() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関連する ラッチ
  const CmnLatch* mLatch;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeLatchData CmnNodeLatchIn.h "CmnNodeLatchIn.h"
/// @brief ラッチのデータ入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeLatchData :
  public CmnNodeLatchIn
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] latch 親のラッチ
  CmnNodeLatchData(CmnLatch* latch);

  /// @brief デストラクタ
  virtual
  ~CmnNodeLatchData();


public:

  /// @brief 出力ノードのサブタイプを得る．
  virtual
  tOutputType
  output_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeLatchClock CmnNodeLatchIn.h "CmnNodeLatchIn.h"
/// @brief ラッチのイネーブル入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeLatchEnable :
  public CmnNodeLatchIn
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] latch 親のラッチ
  CmnNodeLatchEnable(CmnLatch* latch);

  /// @brief デストラクタ
  virtual
  ~CmnNodeLatchEnable();


public:

  /// @brief 出力ノードのサブタイプを得る．
  virtual
  tOutputType
  output_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeLatchClear CmnNodeLatchIn.h "CmnNodeLatchIn.h"
/// @brief ラッチのクリア入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeLatchClear :
  public CmnNodeLatchIn
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] latch 親のラッチ
  CmnNodeLatchClear(CmnLatch* latch);

  /// @brief デストラクタ
  virtual
  ~CmnNodeLatchClear();


public:

  /// @brief 出力ノードのサブタイプを得る．
  virtual
  tOutputType
  output_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeLatchPreset CmnNodeLatchIn.h "CmnNodeLatchIn.h"
/// @brief ラッチのプリセット入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeLatchPreset :
  public CmnNodeLatchIn
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] latch 親のラッチ
  CmnNodeLatchPreset(CmnLatch* latch);

  /// @brief デストラクタ
  virtual
  ~CmnNodeLatchPreset();


public:

  /// @brief 出力ノードのサブタイプを得る．
  virtual
  tOutputType
  output_type() const;

};

END_NAMESPACE_YM_CMN

#endif // LIBYM_NETWORKS_CMN_CMNNODELATCHIN_H
