#ifndef MISLIBNODEIMPL_H
#define MISLIBNODEIMPL_H

/// @file MislibNodeImpl.h
/// @brief MislibNodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNode.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibNodeImpl MislibNodeImpl.h "MislibNodeImpl.h"
/// @brief MislibNode の実装クラス
//////////////////////////////////////////////////////////////////////
class MislibNodeImpl :
  public MislibNode
{
  friend class MislibParserImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibNodeImpl(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibNodeImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 位置情報を取り出す．
  virtual
  FileRegion
  loc() const;

  /// @brief 論理式を表す型のときに true を返す．
  /// @note 文字列や定数0と定数1も論理式とみなす．
  /// @note デフォルトでは false を返す．
  virtual
  bool
  is_expr() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 文字列型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列を取り出す
  /// @note デフォルトでは空文字列を返す．
  virtual
  ShString
  str() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 数値型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 数値を取り出す
  /// @note デフォルトでは 0.0 を返す．
  virtual
  double
  num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理式型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1番目の子供を取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  child1() const;

  /// @brief 2番目の子供を取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  child2() const;

  /// @brief 対応する論理式を生成する．
  /// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
  virtual
  LogExpr
  to_expr(const NameMap& name_map) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ピン型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン名/ゲート名を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  name() const;

  /// @brief 極性情報を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  phase() const;

  /// @brief 入力負荷を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  input_load() const;

  /// @brief 最大駆動負荷を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  max_load() const;

  /// @brief 立ち上がり固定遅延を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  rise_block_delay() const;

  /// @brief 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  rise_fanout_delay() const;

  /// @brief 立ち下がり固定遅延を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  fall_block_delay() const;

  /// @brief 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  fall_fanout_delay() const;

  /// @brief 次の要素を設定する．
  /// @note デフォルトでは何もしない．
  virtual
  void
  set_next(MislibNodeImpl* pin);

  /// @brief 次の要素を取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  next() const;


public:
  //////////////////////////////////////////////////////////////////////
  // リスト型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 末尾に要素を追加する．
  /// @note デフォルトでは何もしない．
  virtual
  void
  push_back(MislibNodeImpl* pin);

  /// @brief 先頭の要素を取り出す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  top() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ゲート型のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積を表すオブジェクトを返す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  area() const;

  /// @brief 出力ピン名を表すオブジェクトを返す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  opin_name() const;

  /// @brief 出力の論理式を表すオブジェクトを返す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  opin_expr() const;

  /// @brief 入力ピンのリストを表すオブジェクトを返す．
  /// @note デフォルトでは NULL を返す．
  virtual
  const MislibNode*
  ipin_list() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 位置を出力する．
  void
  dump_loc(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 位置情報
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_MISLIB

#endif // MISLIBNODEIMPL_H
