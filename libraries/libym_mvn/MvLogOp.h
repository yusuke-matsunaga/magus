#ifndef LIBYM_MVN_MVLOGOP_H
#define LIBYM_MVN_MVLOGOP_H

/// @file libym_mvn/MvLogOp.h
/// @brief MvLogOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvLogOp MvLogOP.h "MvLogOp.h"
/// @brief 論理演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvLogOp :
  public MvNode
{
protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  /// @note ビット幅はすべての入力，出力で同一
  MvLogOp(MvModule* module,
	  ymuint input_num,
	  ymuint bit_width);

  /// @brief デストラクタ
  virtual
  ~MvLogOp();

};


//////////////////////////////////////////////////////////////////////
/// @class MvAnd MvLogOp.h "MvLogOp.h"
/// @brief AND 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvAnd :
  public MvLogOp
{
  friend class MvMgr;

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
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  MvAnd(MvModule* module,
	ymuint input_num,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvAnd();

};


//////////////////////////////////////////////////////////////////////
/// @class MvOr MvLogOp.h "MvLogOp.h"
/// @brief OR 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvOr :
  public MvLogOp
{
  friend class MvMgr;

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
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  MvOr(MvModule* module,
       ymuint input_num,
       ymuint bit_width);

  /// @brief デストラクタ
  ~MvOr();

};


//////////////////////////////////////////////////////////////////////
/// @class MvXor MvLogOp.h "MvLogOp.h"
/// @brief XOR 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvXor :
  public MvLogOp
{
  friend class MvMgr;

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
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  MvXor(MvModule* module,
	ymuint input_num,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvXor();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVLOGOP_H
