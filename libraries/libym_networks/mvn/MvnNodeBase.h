#ifndef MVNNODEBASE_H
#define MVNNODEBASE_H

/// @file MvnNodeBase.h
/// @brief MvnNodeBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnNode.h"


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
