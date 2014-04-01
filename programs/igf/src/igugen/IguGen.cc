
/// @file IguGen.cc
/// @brief IguGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IguGen.h"


BEGIN_NAMESPACE_YM_IGF

// @brief コンストラクタ
IguGen::IguGen()
{
}

// @brief デストラクタ
IguGen::~IguGen()
{
}

// @brief 登録ベクタを読み込む．
// @param[in] s 読み込み元のストリーム演算子
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
IguGen::read_data(istream& s)
{
  return mRvMgr.read_data(s);
}

// @brief ベクタのサイズを得る．
//
// ベクタのサイズとはベクタのビット長
ymuint
IguGen::vect_size() const
{
  return mRvMgr.vect_size();
}

// @brief ベクタのリストを得る．
const vector<const RegVect*>&
IguGen::vect_list() const
{
  return mRvMgr.vect_list();
}

// @brief インデックスのサイズを得る．
//
// インデックスのサイズとはインデックスを2進符号化するのに
// 必要なビット数 = ceil(log (k + 1)): k はベクタ数
ymuint
IguGen::index_size() const
{
  return mRvMgr.index_size();
}

END_NAMESPACE_YM_IGF
