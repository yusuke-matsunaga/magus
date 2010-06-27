#ifndef LIBYM_MVN_MVTERNARYOP_H
#define LIBYM_MVN_MVTERNARYOP_H

/// @file libym_mvn/MvTernaryOp.h
/// @brief MvTernaryOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvTernaryOp MvTernaryOp.h "MvTernaryOp.h"
/// @brief 3項演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvTernaryOp :
  public MvNode
{
public:

  /// @brief 入力ピン数を得る．
  virtual
  ymuint
  input_num() const;

  /// @brief 出力ピン数を得る．
  virtual
  ymuint
  output_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  virtual
  MvInputPin*
  input(ymuint pos);

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  virtual
  MvOutputPin*
  output(ymuint pos);


protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 入力3のビット幅
  /// @param[in] bit_width4 出力のビット幅
  MvTernaryOp(MvModule* module,
	      ymuint bit_width1,
	      ymuint bit_width2,
	      ymuint bit_width3,
	      ymuint bit_width4);

  /// @brief デストラクタ
  ~MvTernaryOp();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン
  MvInputPin mInput[3];
  
  // 出力ピン
  MvOutputPin mOutput;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVTERNARYOP_H
