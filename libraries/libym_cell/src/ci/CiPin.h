#ifndef CIPIN_H
#define CIPIN_H

/// @file　CiPin.h
/// @brief CiPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/CellPin.h"
#include "YmCell/CellTime.h"
#include "YmCell/CellCapacitance.h"
#include "YmLogic/Expr.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

class CiCell;

//////////////////////////////////////////////////////////////////////
/// @class CellPinBase CiPin.h "CiPin.h"
/// @brief ピンの基底クラス
//////////////////////////////////////////////////////////////////////
class CiPin :
  public CellPin
{
  friend class CiLibrary;
  friend class CiPinHash;

protected:

  /// @brief コンストラクタ
  /// @param[in] cell 親のセル
  /// @param[in] name ピン名
  CiPin(CiCell* cell,
	const ShString& name);

  /// @brief デストラクタ
  virtual
  ~CiPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン番号を返す．
  virtual
  ymuint
  pin_id() const;

  /// @brief ピン名を返す．
  virtual
  string
  name() const;

  /// @brief 入力ピンの時に true を返す．
  virtual
  bool
  is_input() const;

  /// @brief 出力ピンの時に true を返す．
  virtual
  bool
  is_output() const;

  /// @brief 入出力ピンの時に true を返す．
  virtual
  bool
  is_inout() const;

  /// @brief 内部ピンの時に true を返す．
  virtual
  bool
  is_internal() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン番号を返す．
  /// @note 入力ピンもしくは入出力ピンの時のみ意味を持つ．
  virtual
  ymuint
  input_id() const;

  /// @brief 負荷容量を返す．
  virtual
  CellCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  CellCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  CellCapacitance
  fall_capacitance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピン番号を返す．
  /// @note 出力ピンもしくは入出力ピンの時のみ意味を持つ．
  virtual
  ymuint
  output_id() const;

  /// @brief 論理式を持っているときに true を返す．
  virtual
  bool
  has_function() const;

  /// @brief 機能を表す論理式を返す．
  virtual
  Expr
  function() const;

  /// @brief three_state 属性を持っているときに true を返す．
  virtual
  bool
  has_three_state() const;

  /// @brief three_state 論理式を返す．
  virtual
  Expr
  three_state() const;

  /// @brief 最大ファンアウト容量を返す．
  virtual
  CellCapacitance
  max_fanout() const;

  /// @brief 最小ファンアウト容量を返す．
  virtual
  CellCapacitance
  min_fanout() const;

  /// @brief 最大負荷容量を返す．
  virtual
  CellCapacitance
  max_capacitance() const;

  /// @brief 最小負荷容量を返す．
  virtual
  CellCapacitance
  min_capacitance() const;

  /// @brief 最大遷移時間を返す．
  virtual
  CellTime
  max_transition() const;

  /// @brief 最小遷移時間を返す．
  virtual
  CellTime
  min_transition() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部ピン番号を返す．
  /// @note 内部ピンの時のみ意味を持つ．
  virtual
  ymuint
  internal_id() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief dump 用の共通情報を出力する．
  void
  dump_common(ODO& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表のためのリンク
  CiPin* mLink;

  // 親のセル
  CiCell* mCell;

  // ピン番号
  ymuint32 mId;

  // 名前
  ShString mName;

};


//////////////////////////////////////////////////////////////////////
/// @class CiInputPin CiPin.h "CiPin.h"
/// @brief セルの入力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CiInputPin :
  public CiPin
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] cell 親のセル
  /// @param[in] name ピン名
  /// @param[in] capacitance 負荷容量
  /// @param[in] rise_capacitance 立ち上がり時の負荷容量
  /// @param[in] fall_capacitance 立ち下がり時の負荷容量
  CiInputPin(CiCell* cell,
	     const ShString& name,
	     CellCapacitance capacitance,
	     CellCapacitance rise_capacitance,
	     CellCapacitance fall_capacitance);

  /// @brief デストラクタ
  virtual
  ~CiInputPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tCellPinDirection
  direction() const;

  /// @brief 入力ピンの時に true を返す．
  virtual
  bool
  is_input() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン番号を返す．
  /// @note 入力ピンもしくは入出力ピンの時のみ意味を持つ．
  virtual
  ymuint
  input_id() const;

  /// @brief 負荷容量を返す．
  virtual
  CellCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  CellCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  CellCapacitance
  fall_capacitance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ODO& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン番号
  ymuint32 mInputId;

  // 負荷
  CellCapacitance mCapacitance;

  // rise 負荷
  CellCapacitance mRiseCapacitance;

  // fall 負荷
  CellCapacitance mFallCapacitance;

};


//////////////////////////////////////////////////////////////////////
/// @class CiOutputPin CiPin.h "CiPin.h"
/// @brief セルの出力ピンと入出力ピンの基底クラス
//////////////////////////////////////////////////////////////////////
class CiOutputPinBase :
  public CiPin
{
  friend class CiLibrary;

protected:

  /// @brief コンストラクタ
  /// @param[in] cell 親のセル
  /// @param[in] name ピン名
  /// @param[in] has_logic 論理式を持つとき true にするフラグ
  /// @param[in] logic_expr 論理式
  /// @param[in] tristate_expr tristate 条件式
  /// @param[in] max_fanout 最大ファンアウト容量
  /// @param[in] min_fanout 最小ファンアウト容量
  /// @param[in] max_capacitance 最大負荷容量
  /// @param[in] min_capacitance 最小負荷容量
  /// @param[in] max_transition 最大遷移時間
  /// @param[in] min_transition 最小遷移時間
  CiOutputPinBase(CiCell* cell,
		  const ShString& name,
		  bool has_logic,
		  const Expr& logic_expr,
		  const Expr& tristate_expr,
		  CellCapacitance max_fanout,
		  CellCapacitance min_fanout,
		  CellCapacitance max_capacitance,
		  CellCapacitance min_capacitance,
		  CellTime max_transition,
		  CellTime min_transition);

  /// @brief デストラクタ
  virtual
  ~CiOutputPinBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピン番号を返す．
  /// @note 出力ピンもしくは入出力ピンの時のみ意味を持つ．
  virtual
  ymuint
  output_id() const;

  /// @brief 論理式を持っているときに true を返す．
  virtual
  bool
  has_function() const;

  /// @brief 機能を表す論理式を返す．
  virtual
  Expr
  function() const;

  /// @brief three_state 属性を持っているときに true を返す．
  virtual
  bool
  has_three_state() const;

  /// @brief three_state 論理式を返す．
  virtual
  Expr
  three_state() const;

  /// @brief 最大ファンアウト容量を返す．
  virtual
  CellCapacitance
  max_fanout() const;

  /// @brief 最小ファンアウト容量を返す．
  virtual
  CellCapacitance
  min_fanout() const;

  /// @brief 最大負荷容量を返す．
  virtual
  CellCapacitance
  max_capacitance() const;

  /// @brief 最小負荷容量を返す．
  virtual
  CellCapacitance
  min_capacitance() const;

  /// @brief 最大遷移時間を返す．
  virtual
  CellTime
  max_transition() const;

  /// @brief 最小遷移時間を返す．
  virtual
  CellTime
  min_transition() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ピン番号
  ymuint32 mOutputId;

  // 論理式を持っているかどうかを表すフラグ
  ymuint32 mHasFunction;

  // 出力の論理式
  Expr mFunction;

  // three_state 属性の論理式
  Expr mThreeState;

  // ファンアウトの負荷
  CellCapacitance mFanoutLoad;

  // 最大ファンアウト容量
  CellCapacitance mMaxFanout;

  // 最小ファンアウト容量
  CellCapacitance mMinFanout;

  // 最大負荷容量
  CellCapacitance mMaxCapacitance;

  // 最小負荷容量
  CellCapacitance mMinCapacitance;

  // 最大遷移時間
  CellTime mMaxTransition;

  // 最小遷移時間
  CellTime mMinTransition;

};


//////////////////////////////////////////////////////////////////////
/// @class CiOutputPin CiPin.h "CiPin.h"
/// @brief セルの出力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CiOutputPin :
  public CiOutputPinBase
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] cell 親のセル
  /// @param[in] name ピン名
  /// @param[in] has_logic 論理式を持つとき true にするフラグ
  /// @param[in] logic_expr 論理式
  /// @param[in] tristate_expr tristate 条件式
  /// @param[in] max_fanout 最大ファンアウト容量
  /// @param[in] min_fanout 最小ファンアウト容量
  /// @param[in] max_capacitance 最大負荷容量
  /// @param[in] min_capacitance 最小負荷容量
  /// @param[in] max_transition 最大遷移時間
  /// @param[in] min_transition 最小遷移時間
  CiOutputPin(CiCell* cell,
	      const ShString& name,
	      bool has_logic,
	      const Expr& logic_expr,
	      const Expr& tristate_expr,
	      CellCapacitance max_fanout,
	      CellCapacitance min_fanout,
	      CellCapacitance max_capacitance,
	      CellCapacitance min_capacitance,
	      CellTime max_transition,
	      CellTime min_transition);

  /// @brief デストラクタ
  virtual
  ~CiOutputPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tCellPinDirection
  direction() const;

  /// @brief 出力ピンの時に true を返す．
  virtual
  bool
  is_output() const;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ODO& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class CiInoutPin CiPin.h "CiPin.h"
/// @brief セルの入出力ピンを表すクラス
/// @note 多重継承はオーバーヘッドがかかるので愚直な実装を用いている．
//////////////////////////////////////////////////////////////////////
class CiInoutPin :
  public CiOutputPinBase
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] cell 親のセル
  /// @param[in] name ピン名
  /// @param[in] has_logic 論理式を持つとき true にするフラグ
  /// @param[in] logic_expr 論理式
  /// @param[in] tristate_expr tristate 条件式
  /// @param[in] capacitance 負荷容量
  /// @param[in] rise_capacitance 立ち上がり時の負荷容量
  /// @param[in] fall_capacitance 立ち下がり時の負荷容量
  /// @param[in] max_fanout 最大ファンアウト容量
  /// @param[in] min_fanout 最小ファンアウト容量
  /// @param[in] max_capacitance 最大負荷容量
  /// @param[in] min_capacitance 最小負荷容量
  /// @param[in] max_transition 最大遷移時間
  /// @param[in] min_transition 最小遷移時間
  CiInoutPin(CiCell* cell,
	     const ShString& name,
	     bool has_logic,
	     const Expr& logic_expr,
	     const Expr& tristate_expr,
	     CellCapacitance capacitance,
	     CellCapacitance rise_capacitance,
	     CellCapacitance fall_capacitance,
	     CellCapacitance max_fanout,
	     CellCapacitance min_fanout,
	     CellCapacitance max_capacitance,
	     CellCapacitance min_capacitance,
	     CellTime max_transition,
	     CellTime min_transition);

  /// @brief デストラクタ
  virtual
  ~CiInoutPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tCellPinDirection
  direction() const;

  /// @brief 入出力ピンの時に true を返す．
  virtual
  bool
  is_inout() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン番号を返す．
  /// @note 入力ピンもしくは入出力ピンの時のみ意味を持つ．
  virtual
  ymuint
  input_id() const;

  /// @brief 負荷容量を返す．
  virtual
  CellCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  CellCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  CellCapacitance
  fall_capacitance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ODO& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン番号
  ymuint32 mInputId;

  // 負荷
  CellCapacitance mCapacitance;

  // rise 負荷
  CellCapacitance mRiseCapacitance;

  // fall 負荷
  CellCapacitance mFallCapacitance;

};


//////////////////////////////////////////////////////////////////////
/// @class CiInternalPin CiPin.h "CiPin.h"
/// @brief セルの内部ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CiInternalPin :
  public CiPin
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] cell 親のセル
  /// @param[in] name ピン名
  CiInternalPin(CiCell* cell,
		const ShString& name);

  /// @brief デストラクタ
  virtual
  ~CiInternalPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tCellPinDirection
  direction() const;

  /// @brief 内部ピンの時に true を返す．
  virtual
  bool
  is_internal() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部ピン番号を返す．
  /// @note 内部ピンの時のみ意味を持つ．
  virtual
  ymuint
  internal_id() const;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ODO& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内部ピン番号
  ymuint32 mInternalId;

};

END_NAMESPACE_YM_CELL

#endif // CIPIN_H
