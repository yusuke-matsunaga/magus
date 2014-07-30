#ifndef MISLIBNODE_H
#define MISLIBNODE_H

/// @file MislibNode.h
/// @brief MislibNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"

#include "YmUtils/ShString.h"
#include "YmUtils/FileRegion.h"
#include "YmLogic/Expr.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibNode MislibNode.h "MislibNode.h"
/// @brief トークンを表す基底クラス
//////////////////////////////////////////////////////////////////////
class MislibNode
{
public:

  typedef unordered_map<ShString, ymuint> NameMap;

public:

  /// @brief ノードの種類
  enum tType {
    /// @brief 文字列
    kStr,
    /// @brief 数値
    kNum,
    /// @brief NONINV 極性
    kNoninv,
    /// @brief INV 極性
    kInv,
    /// @brief UNKOWN 極性
    kUnknown,
    /// @brief 定数0
    kConst0,
    /// @brief 定数1
    kConst1,
    /// @brief リスト
    kList,
    /// @brief NOT論理式
    kNot,
    /// @brief AND論理式
    kAnd,
    /// @brief OR論理式
    kOr,
    /// @brief XOR論理式
    kXor,
    /// @brief 入力ピン
    kPin,
    /// @brief ゲート
    kGate
  };

protected:

  /// @brief デストラクタ
  virtual
  ~MislibNode() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 共通な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 位置情報を取り出す．
  virtual
  FileRegion
  loc() const = 0;

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const = 0;

  /// @brief 論理式を表す型のときに true を返す．
  /// @note 文字列や定数0と定数1も論理式とみなす．
  virtual
  bool
  is_expr() const = 0;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 文字列型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列を取り出す
  virtual
  ShString
  str() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 数値型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 数値を取り出す
  virtual
  double
  num() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理式型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1番目の子供を取り出す．
  virtual
  const MislibNode*
  child1() const = 0;

  /// @brief 2番目の子供を取り出す．
  virtual
  const MislibNode*
  child2() const = 0;

  /// @brief 対応する論理式を生成する．
  /// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
  virtual
  Expr
  to_expr(const NameMap& name_map) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ピン型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン名/ゲート名を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  name() const = 0;

  /// @brief 極性情報を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  phase() const = 0;

  /// @brief 入力負荷を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  input_load() const = 0;

  /// @brief 最大駆動負荷を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  max_load() const = 0;

  /// @brief 立ち上がり固定遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  rise_block_delay() const = 0;

  /// @brief 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  rise_fanout_delay() const = 0;

  /// @brief 立ち下がり固定遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  fall_block_delay() const = 0;

  /// @brief 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  fall_fanout_delay() const = 0;

  /// @brief 次の要素を取り出す．
  virtual
  const MislibNode*
  next() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // リスト型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 先頭の要素を取り出す．
  virtual
  const MislibNode*
  top() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ゲート型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積を表すオブジェクトを返す．
  virtual
  const MislibNode*
  area() const = 0;

  /// @brief 出力ピン名を表すオブジェクトを返す．
  virtual
  const MislibNode*
  opin_name() const = 0;

  /// @brief 出力の論理式を表すオブジェクトを返す．
  virtual
  const MislibNode*
  opin_expr() const = 0;

  /// @brief 入力ピンのリストを表すオブジェクトを返す．
  virtual
  const MislibNode*
  ipin_list() const = 0;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBNODE_H
