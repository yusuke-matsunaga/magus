#ifndef SNINPUT_H
#define SNINPUT_H

/// @file SnInput.h
/// @brief SnInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "StrNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class SnInput SnInput.h "SnInput.h"
/// @brief StrImp で用いられるノード
//////////////////////////////////////////////////////////////////////
class SnInput :
  public StrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  SnInput(ymuint id);

  /// @brief デストラクタ
  virtual
  ~SnInput();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力のときに true を返す．
  virtual
  bool
  is_input() const;

  /// @brief 値を返す．
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
  // X: 0
  // 0: 1
  // 1: 2
  enum {
    kStX = 0,
    kSt0 = 1,
    kSt1 = 2
  } mState;
};

END_NAMESPACE_YM_NETWORKS

#endif // SNAND_H
