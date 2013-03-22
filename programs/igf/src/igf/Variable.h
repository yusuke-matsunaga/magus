#ifndef VARIABLE_H
#define VARIABLE_H

/// @file Variable.h
/// @brief Variable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class Variable Variable.h "Variable.h"
/// @brief 通常の変数と合成変数を表すクラス
//////////////////////////////////////////////////////////////////////
class Variable
{
public:

  /// @brief 通常の変数用のコンストラクタ
  /// @param[in] vid 変数番号
  Variable(ymuint vid);

  /// @brief 合成変数用のコンストラクタ
  /// @param[in] vid_list 変数番号のリスト
  Variable(const vector<ymuint>& vid_list);

  /// @brief デストラクタ
  ~Variable();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 合成度を返す．
  ymuint
  compound_degree() const;

  /// @brief 通常の変数の場合に変数番号を返す．
  /// @note 合成変数の場合の動作は未定
  ymuint
  vid() const;

  /// @brief 合成変数の場合に変数番号のリストを返す．
  /// @note 通常の変数の場合の動作は未定
  const vector<ymuint>&
  vid_list() const;

  /// @brief ベクタを分類する．
  /// @param[in] vect 対象のベクタ
  ymuint
  classify(const RegVect* vect) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の変数番号
  ymuint mVid0;

  // 変数番号のリスト
  vector<ymuint> mVidList;

};

END_NAMESPACE_YM_IGF

#endif // VARIABLE_H
