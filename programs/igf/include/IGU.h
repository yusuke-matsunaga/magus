#ifndef IGU_H
#define IGU_H

/// @file IGU.h
/// @brief IGU のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class IGU IGU.h "IGU.h"
/// @brief IGU を表すクラス
//////////////////////////////////////////////////////////////////////
class IGU
{
public:

  /// @brief コンストラクタ
  /// @param[in] input_width 入力のビット幅
  /// @param[in] signature_width シグネチャのビット幅
  /// @param[in] index_width インデックスのビット幅
  IGU(ymuint input_width,
      ymuint signature_width,
      ymuint index_width);

  /// @brief デストラクタ
  ~IGU();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力のビット幅
  ymuint
  input_width() const;

  /// @brief シグネチャのビット幅
  ymuint
  signature_width() const;

  /// @brief インデックスのビット幅
  ymuint
  index_width() const;

  /// @brief シグネチャからインデックスを取り出す．
  ymuint
  index(ymuint signature) const;

  /// @brief シグネチャからベクタを取り出す．
  const RegVect*
  get_vector(ymuint signature) const;

  /// @brief ベクタを登録する．
  void
  set_vector(ymuint signature,
	     const RegVect* vect);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力のビット幅
  ymuint32 mInputWidth;

  // シグネチャのビット幅
  ymuint32 mSignatureWidth;

  // インデックスのビット幅
  ymuint32 mIndexWidth;

  // ベクタ表
  // サイズは 2^mSignatureWidth
  vector<const RegVect*> mVectTable;

};

END_NAMESPACE_YM_IGF

#endif // IGU_H
