#ifndef MVNNODEBASE_H
#define MVNNODEBASE_H

/// @file MvnNodeBase.h
/// @brief MvnNodeBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/MvnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnNodeBase MvnNodeBase.h "MvnNodeBase.h"
/// @brief MvnNode の実装用基底クラス
//////////////////////////////////////////////////////////////////////
class MvnNodeBase :
  public MvnNode
{
  friend class MvnMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] type 型
  /// @param[in] input_num 入力数
  MvnNodeBase(MvnModule* parent,
	      tType type,
	      ymuint input_num);

  /// @brief デストラクタ
  virtual
  ~MvnNodeBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief 入力ピン数を得る．
  virtual
  ymuint
  input_num() const;

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  virtual
  const MvnInputPin*
  input(ymuint pos) const;

  /// @brief クロック信号の極性を得る．
  /// @retval 1 正極性(posedge)
  /// @retval 0 負極性(negedge)
  /// @note type() が kDff の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  clock_pol() const;

  /// @brief 非同期セット信号の極性を得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @retval 1 正極性(posedge)
  /// @retval 0 負極性(negedge)
  /// @note type() が kDff の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  control_pol(ymuint pos) const;

  /// @brief 非同期セットの値を表す定数ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @note デフォルトの実装では NULL を返す．
  virtual
  const MvnNode*
  control_val(ymuint pos) const;

  /// @brief ビット位置を得る．
  /// @note type() が kConstBitSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  bitpos() const;

  /// @brief 範囲指定の MSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  msb() const;

  /// @brief 範囲指定の LSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  lsb() const;

  /// @brief 定数値を得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kConst の時のみ意味を持つ．
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  const_value(vector<ymuint32>& val) const;

  /// @brief Xマスクを得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kCaseEq の時のみ意味を持つ．
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  xmask(vector<ymuint32>& val) const;

  /// @brief セルを得る．
  /// @note type() が kCell の時のみ意味を持つ．
  /// @note デフォルトの実装では NULL を返す．
  virtual
  const Cell*
  cell() const;

  /// @brief セルの出力ピン番号を返す．
  /// @note type() が kCell の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  cell_opin_pos() const;

  /// @brief 多出力セルノードの場合の代表ノードを返す．
  /// @note type() が kCell の時のみ意味を持つ．
  /// @note 1出力セルノードの時には自分自身を返す．
  /// @note デフォルトの実装では NULL を返す．
  virtual
  const MvnNode*
  cell_node() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  virtual
  MvnInputPin*
  _input(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  tType mType;

  // 入力数
  ymuint32 mInputNum;

  // 入力ピンの配列
  MvnInputPin* mInputArray;

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // MVNNODEBASE_H
