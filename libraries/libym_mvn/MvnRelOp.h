#ifndef LIBYM_MVN_MVNRELOP_H
#define LIBYM_MVN_MVNRELOP_H

/// @file libym_mvn/MvnRelOp.h
/// @brief MvnRelOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnEq MvnRelOp.h "MvnRelOp.h"
/// @brief equal 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnEq :
  public MvnBinaryOp
{
  friend class MvnMgr;

public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width ビット幅
  MvnEq(MvnModule* module,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvnEq();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnLt MvnRelOp.h "MvnRelOp.h"
/// @brief less than 比較演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnLt :
  public MvnBinaryOp
{
  friend class MvnMgr;

public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width ビット幅
  MvnLt(MvnModule* module,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvnLt();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNRELOP_H
