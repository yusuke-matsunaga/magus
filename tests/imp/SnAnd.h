#ifndef SNAND_H
#define SNAND_H

/// @file SnAnd.h
/// @brief SnAnd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "StrNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class SnAnd SnAnd.h "SnAnd.h"
/// @brief StrImp で用いられるノード
//////////////////////////////////////////////////////////////////////
class SnAnd :
  public StrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] node0 ファンイン0のノード
  /// @param[in] inv0 ファンイン0の極性
  /// @param[in] node1 ファンイン1のノード
  /// @param[in] inv1 ファンイン1の極性
  SnAnd(ymuint id,
	StrNode* node0,
	bool inv0,
	StrNode* node1,
	bool inv1);

  /// @brief デストラクタ
  virtual
  ~SnAnd();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND タイプのときに true を返す．
  virtual
  bool
  is_and() const;

  /// @brief 値を返す．
  virtual
  ymuint
  val() const;

  /// @brief ビットベクタ値の計算を行なう．
  virtual
  void
  calc_bitval();


public:
  //////////////////////////////////////////////////////////////////////
  // 含意を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態を初期化する．
  virtual
  void
  clear();

  /// @brief ファンイン0を0にする．
  virtual
  bool
  fwd0_imp0();

  /// @brief ファンイン0を1にする．
  virtual
  bool
  fwd0_imp1();

  /// @brief ファンイン1を0にする．
  virtual
  bool
  fwd1_imp0();

  /// @brief ファンイン1を1にする．
  virtual
  bool
  fwd1_imp1();

  /// @brief 出力を0にする．
  virtual
  bool
  bwd_imp0();

  /// @brief 出力を1にする．
  virtual
  bool
  bwd_imp1();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 状態
  // XX:X   0
  // XX:0   1
  // XX:1   -
  // X0:X   -
  // X0:0   2
  // X0:1   -
  // X1:X   3
  // X1:0   -
  // X1:1   -
  // 0X:X   -
  // 0X:0   4
  // 0X:1   -
  // 00:X   -
  // 00:0   5
  // 00:1   -
  // 01:X   -
  // 01:0   6
  // 01:1   -
  // 1X:X   7
  // 1X:0   -
  // 1X:1   -
  // 10:X   -
  // 10:0   8
  // 10:1   -
  // 11:X   -
  // 11:0   -
  // 11:1   9
  enum {
    kStXX_X = 0,
    kSt1X_X = 1,
    kStX1_X = 2,
    kStXX_0 = 3,
    kStX0_0 = 4,
    kSt0X_0 = 5,
    kSt00_0 = 6,
    kSt10_0 = 7,
    kSt01_0 = 8,
    kSt11_1 = 9
  } mState;

};

END_NAMESPACE_YM_NETWORKS

#endif // SNAND_H
