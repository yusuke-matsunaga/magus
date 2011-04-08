#ifndef LIBYM_MVN_MVNLOGOP_H
#define LIBYM_MVN_MVNLOGOP_H

/// @file libym_mvn/MvnLogOp.h
/// @brief MvnLogOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnLogOp MvnLogOP.h "MvnLogOp.h"
/// @brief 論理演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnLogOp :
  public MvnNode
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
  MvnLogOp(MvnModule* module,
	   ymuint input_num,
	   ymuint bit_width);

  /// @brief デストラクタ
  virtual
  ~MvnLogOp();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnAnd MvnLogOp.h "MvnLogOp.h"
/// @brief AND 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnAnd :
  public MvnLogOp
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
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  MvnAnd(MvnModule* module,
	 ymuint input_num,
	 ymuint bit_width);

  /// @brief デストラクタ
  ~MvnAnd();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnOr MvnLogOp.h "MvnLogOp.h"
/// @brief OR 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnOr :
  public MvnLogOp
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
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  MvnOr(MvnModule* module,
	ymuint input_num,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvnOr();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnXor MvnLogOp.h "MvnLogOp.h"
/// @brief XOR 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnXor :
  public MvnLogOp
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
  /// @param[in] input_num 入力数
  /// @param[in] bit_width ビット幅
  MvnXor(MvnModule* module,
	 ymuint input_num,
	 ymuint bit_width);

  /// @brief デストラクタ
  ~MvnXor();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNLOGOP_H
