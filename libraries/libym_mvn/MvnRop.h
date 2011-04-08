#ifndef LIBYM_MVN_MVNROP_H
#define LIBYM_MVN_MVNROP_H

/// @file libym_mvn/MvnRop.h
/// @brief MvnRop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnRand MvnRop.h "MvnRop.h"
//////////////////////////////////////////////////////////////////////
class MvnRand :
  public MvnUnaryOp
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
  MvnRand(MvnModule* module,
	  ymuint bit_width);

  /// @brief デストラクタ
  ~MvnRand();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnRor MvnRop.h "MvnRop.h"
//////////////////////////////////////////////////////////////////////
class MvnRor :
  public MvnUnaryOp
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
  MvnRor(MvnModule* module,
	 ymuint bit_width);

  /// @brief デストラクタ
  ~MvnRor();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnRxor MvnRop.h "MvnRop.h"
//////////////////////////////////////////////////////////////////////
class MvnRxor :
  public MvnUnaryOp
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
  MvnRxor(MvnModule* module,
	  ymuint bit_width);

  /// @brief デストラクタ
  ~MvnRxor();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNROP_H
