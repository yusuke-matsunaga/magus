#ifndef SNXOR_H
#define SNXOR_H

/// @file SnXor.h
/// @brief SnXor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "StrNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class SnXor SnXor.h "SnXor.h"
/// @brief StrImp で用いられるノード
//////////////////////////////////////////////////////////////////////
class SnXor :
  public StrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] node0 ファンイン0のノード
  /// @param[in] inv0 ファンイン0の極性
  /// @param[in] node1 ファンイン1のノード
  /// @param[in] inv1 ファンイン1の極性
  SnXor(ymuint id,
	StrNode* node0,
	bool inv0,
	StrNode* node1,
	bool inv1);

  /// @brief デストラクタ
  ~SnXor();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief XOR タイプのときに true を返す．
  virtual
  bool
  is_xor() const;

  /// @brief 値を返す．
  virtual
  ymuint
  val() const;


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
  // XX:1   2
  // X0:X   3
  // X0:0   -
  // X0:1   -
  // X1:X   4
  // X1:0   -
  // X1:1   -
  // 0X:X   5
  // 0X:0   -
  // 0X:1   -
  // 00:X   -
  // 00:0   6
  // 00:1   -
  // 01:X   -
  // 01:0   -
  // 01:1   7
  // 1X:X   8
  // 1X:0   -
  // 1X:1   -
  // 10:X   -
  // 10:0   -
  // 10:1   9
  // 11:X   -
  // 11:0   10
  // 11:1   -
  enum tState {
    kStXX_X = 0,
    kStX0_X = 1,
    kStX1_X = 2,
    kSt0X_X = 3,
    kSt1X_X = 4,
    kStXX_0 = 5,
    kSt00_0 = 6,
    kSt11_0 = 7,
    kStXX_1 = 8,
    kSt01_1 = 9,
    kSt10_1 = 10
  } mState;

};

END_NAMESPACE_YM_NETWORKS

#endif // SNXOR_H
