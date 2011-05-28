#ifndef YM_MVN_MVNNODEBASE_H
#define YM_MVN_MVNNODEBASE_H

/// @file libym_networks/MvnNodeBase.h
/// @brief MvnNodeBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnNode.h"


BEGIN_NAMESPACE_YM_MVN

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
  /// @param[in] output_num 出力数
  MvnNodeBase(MvnModule* parent,
	      tType type,
	      ymuint input_num,
	      ymuint output_num);

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

  /// @brief 出力ピン数を得る．
  virtual
  ymuint
  output_num() const;

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  virtual
  const MvnOutputPin*
  output(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  virtual
  MvnInputPin*
  _input(ymuint pos);

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  virtual
  MvnOutputPin*
  _output(ymuint pos);


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

  // 出力数
  ymuint32 mOutputNum;

  // 出力ピンの配列
  MvnOutputPin* mOutputArray;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNNODEBASE_H
