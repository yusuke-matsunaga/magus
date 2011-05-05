#ifndef LIBYM_MISLIB_MISLIBPTIMPL2_H
#define LIBYM_MISLIB_MISLIBPTIMPL2_H

/// @file libym_mislib/MislibPtImpl2.h
/// @brief MislibPt の派生クラスのヘッダファイル(その2)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPtImpl2.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibBop MislibImpl2.h "MislibImpl2.h"
/// @brief 2項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibBop :
  public MislibNodeImpl
{
protected:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibBop(const FileRegion& loc,
	    const MislibNode* child1,
	    const MislibNode* child2);

  /// @brief デストラクタ
  virtual
  ~MislibBop();


public:

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 1番目の子供を取り出す．
  virtual
  const MislibNode*
  child1() const;

  /// @brief 2番目の子供を取り出す．
  virtual
  const MislibNode*
  child2() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 1番目の子供
  const MislibNode* mChild1;

  // 2番目の子供
  const MislibNode* mChild2;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibAnd MislibImpl2.h "MislibImpl2.h"
/// @brief AND論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibAnd :
  public MislibBop
{
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibAnd(const FileRegion& loc,
	    const MislibNode* child1,
	    const MislibNode* child2);

  /// @brief デストラクタ
  virtual
  ~MislibAnd();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 対応する論理式を生成する．
  /// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
  virtual
  LogExpr
  to_expr(const hash_map<ShString, ymuint>& name_map) const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibOr MislibImpl2.h "MislibImpl2.h"
/// @brief OR論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibOr :
  public MislibBop
{
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibOr(const FileRegion& loc,
	   const MislibNode* child1,
	   const MislibNode* child2);

  /// @brief デストラクタ
  virtual
  ~MislibOr();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 対応する論理式を生成する．
  /// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
  virtual
  LogExpr
  to_expr(const hash_map<ShString, ymuint>& name_map) const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibXor MislibImpl2.h "MislibImpl2.h"
/// @brief XOR論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibXor :
  public MislibBop
{
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibXor(const FileRegion& loc,
	    const MislibNode* child1,
	    const MislibNode* child2);

  /// @brief デストラクタ
  virtual
  ~MislibXor();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 対応する論理式を生成する．
  /// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
  virtual
  LogExpr
  to_expr(const hash_map<ShString, ymuint>& name_map) const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibPin MislibImpl2.h "MislibImpl2.h"
/// @brief ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPin :
  public MislibNodeImpl
{
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] name ピン名
  /// @param[in] phase 極性
  /// @param[in] input_load 入力負荷
  /// @param[in] max_load 最大駆動負荷
  /// @param[in] rise_block_delay 立ち上がり固定遅延
  /// @param[in] rise_fanout_delay 立ち上がり負荷依存遅延
  /// @param[in] fall_block_delay 立ち下がり固定遅延
  /// @param[in] fall_fanout_delay 立ち下がり負荷依存遅延
  MislibPin(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* phase,
	    const MislibNode* input_load,
	    const MislibNode* max_load,
	    const MislibNode* rise_block_delay,
	    const MislibNode* rise_fanout_delay,
	    const MislibNode* fall_block_delay,
	    const MislibNode* fall_fanout_delay);

  /// @brief デストラクタ
  virtual
  ~MislibPin();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief ピン名を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  name() const;

  /// @brief 極性情報を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  phase() const;

  /// @brief 入力負荷を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  input_load() const;

  /// @brief 最大駆動負荷を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  max_load() const;

  /// @brief 立ち上がり固定遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  rise_block_delay() const;

  /// @brief 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  rise_fanout_delay() const;

  /// @brief 立ち下がり固定遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  fall_block_delay() const;

  /// @brief 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  fall_fanout_delay() const;

  /// @brief 次のピンを設定する．
  virtual
  void
  set_next(const MislibNode* pin);

  /// @brief 次のピンを取り出す．
  virtual
  const MislibNode*
  next() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン名
  const MislibNode* mName;

  // 極性情報
  const MislibNode* mPhase;

  // 入力負荷
  const MislibNode* mInputLoad;

  // 最大駆動負荷
  const MislibNode* mMaxLoad;

  // 立ち上がり固有遅延
  const MislibNode* mRiseBlockDelay;

  // 立ち上がり負荷依存遅延係数
  const MislibNode* mRiseFanoutDelay;

  // 立ち下がり固有遅延
  const MislibNode* mFallBlockDelay;

  // 立ち下がり負荷依存遅延係数
  const MislibNode* mFallFanoutDelay;

  // 次のピン
  const MislibNode* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibGate MislibImpl2.h "MislibImpl2.h"
/// @brief ゲート(セル)を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibGate :
  public const MislibNode
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] name 名前を表すパース木
  /// @param[in] area 面積を表すパース木
  /// @param[in] opin_name 出力ピン名を表すパース木
  /// @param[in] opin_expr 出力の論理式を表すパース木
  /// @param[in] ipin_list 入力ピンのリストを表すパース木
  MislibGate(const FileRegion& loc,
	       const MislibNode* name,
	       const MislibNode* area,
	       const MislibNode* opin_name,
	       const MislibNode* opin_expr,
	       const MislibNode* ipin_list);

  /// @brief デストラクタ
  virtual
  ~MislibGate();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief ピン名/ゲート名を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  name() const;

  /// @brief 面積を表すオブジェクトを返す．
  virtual
  const MislibNode*
  area() const;

  /// @brief 出力ピン名を表すオブジェクトを返す．
  virtual
  const MislibNode*
  opin_name() const;

  /// @brief 出力の論理式を表すオブジェクトを返す．
  virtual
  const MislibNode*
  opin_expr() const;

  /// @brief 入力ピンのリストを表すオブジェクトを返す．
  virtual
  const MislibNode*
  ipin_list() const;

  /// @brief 次の要素を設定する．
  virtual
  void
  set_next(const MislibNode* next);

  /// @brief 次の要素を取り出す．
  virtual
  const MislibNode*
  next() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const MislibNode* mName;

  // 面積
  const MislibNode* mArea;

  // 出力ピン名
  const MislibNode* mOpinName;

  // 出力の論理式
  const MislibNode* mOpinExpr;

  // 入力ピンのリスト
  const MislibNode* mIpinList;

  // 次の要素
  const MislibNode* mNext;

};

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBPTIMPL2_H
