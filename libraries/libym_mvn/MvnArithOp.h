#ifndef LIBYM_MVN_MVNARITHOP_H
#define LIBYM_MVN_MVNARITHOP_H

/// @file libym_mvn/MvnArithOp.h
/// @brief MvnArithOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnAdd MvnArithOp.h "MvnArithOp.h"
/// @brief ADD 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnAdd :
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
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnAdd(MvnModule* module,
	 ymuint bit_width1,
	 ymuint bit_width2,
	 ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnAdd();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnSub MvnArithOp.h "MvnArithOp.h"
/// @brief SUB 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnSub :
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
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnSub(MvnModule* module,
	 ymuint bit_width1,
	 ymuint bit_width2,
	 ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnSub();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnMult MvnArithOp.h "MvnArithOp.h"
/// @brief MULT 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnMult :
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
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnMult(MvnModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnMult();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnDiv MvnArithOp.h "MvnArithOp.h"
/// @brief DIV 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnDiv :
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
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnDiv(MvnModule* module,
	 ymuint bit_width1,
	 ymuint bit_width2,
	 ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnDiv();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnMod MvnArithOp.h "MvnArithOp.h"
/// @brief MOD 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnMod :
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
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnMod(MvnModule* module,
	 ymuint bit_width1,
	 ymuint bit_width2,
	 ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnMod();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnPow MvnArithOp.h "MvnArithOp.h"
/// @brief POW 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnPow :
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
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnPow(MvnModule* module,
	 ymuint bit_width1,
	 ymuint bit_width2,
	 ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnPow();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnSll MvnArithOp.h "MvnArithOp.h"
/// @brief SLL 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnSll :
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
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnSll(MvnModule* module,
	 ymuint bit_width1,
	 ymuint bit_width2,
	 ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnSll();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnSrl MvnArithOp.h "MvnArithOp.h"
/// @brief SRL 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnSrl :
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
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnSrl(MvnModule* module,
	 ymuint bit_width1,
	 ymuint bit_width2,
	 ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnSrl();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnSla MvnArithOp.h "MvnArithOp.h"
/// @brief Sla 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnSla :
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
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnSla(MvnModule* module,
	 ymuint bit_width1,
	 ymuint bit_width2,
	 ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnSla();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnSra MvnArithOp.h "MvnArithOp.h"
/// @brief SRA 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnSra :
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
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnSra(MvnModule* module,
	 ymuint bit_width1,
	 ymuint bit_width2,
	 ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnSra();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNARITHOP_H
