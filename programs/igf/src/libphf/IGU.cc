
/// @file IGU.cc
/// @brief IGU の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "IGU.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス IGU
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_width 入力のビット幅
// @param[in] signature_width シグネチャのビット幅
// @param[in] index_width インデックスのビット幅
IGU::IGU(ymuint input_width,
	 ymuint signature_width,
	 ymuint index_width) :
  mInputWidth(input_width),
  mSignatureWidth(signature_width),
  mIndexWidth(index_width),
  mVectTable(1U << signature_width)
{
}

// @brief デストラクタ
IGU::~IGU()
{
}

// @brief 入力のビット幅
ymuint
IGU::input_width() const
{
  return mInputWidth;
}

// @brief シグネチャのビット幅
ymuint
IGU::signature_width() const
{
  return mSignatureWidth;
}

// @brief インデックスのビット幅
ymuint
IGU::index_width() const
{
  return mIndexWidth;
}

// @brief シグネチャからインデックスを取り出す．
ymuint
IGU::index(ymuint signature) const
{
  return mVectTable[signature]->index();
}

// @brief シグネチャからベクタを取り出す．
const RegVect*
IGU::get_vector(ymuint signature) const
{
  return mVectTable[signature];
}

// @brief ベクタを登録する．
void
IGU::set_vector(ymuint signature,
		const RegVect* vect)
{
  mVectTable[signature] = vect;
}

END_NAMESPACE_YM_IGF

