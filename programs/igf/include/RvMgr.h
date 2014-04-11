#ifndef RVMGR_H
#define RVMGR_H

/// @file RvMgr.h
/// @brief RvMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "utils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class RvMgr RvMgr.h "RvMgr.h"
/// @brief RegVect を管理するクラス
///
/// ビット長が vect_size() のベクタの集合を表す．
/// インデックスのサイズとはベクタ数の log の ceil
//////////////////////////////////////////////////////////////////////
class RvMgr
{
public:

  /// @brief コンストラクタ
  /// @note ベクタのサイズは未定
  RvMgr();

  /// @brief デストラクタ
  /// @note このオブジェクトが確保したすべての RegVect が削除される．
  ~RvMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込む．
  /// @param[in] s 読み込み元のストリーム演算子
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_data(istream& s);

  /// @brief ベクタのサイズを得る．
  ///
  /// ベクタのサイズとはベクタのビット長
  ymuint
  vect_size() const;

  /// @brief ベクタのリストを得る．
  const vector<const RegVect*>&
  vect_list() const;

  /// @brief インデックスのサイズを得る．
  ///
  /// インデックスのサイズとはインデックスを2進符号化するのに
  /// 必要なビット数 = ceil(log (k + 1)): k はベクタ数
  ymuint
  index_size() const;

  /// @brief ベクタにハッシュ関数を適用した結果を作る．
  /// @param[in] hash_func ハッシュ関数
  /// @return ハッシュ値のベクタ
  FuncVect*
  gen_hash_vect(const InputFunc& hash_func) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ベクタのサイズを設定する．
  /// @note 1回以上呼ばれるとアボートする．
  /// @note 付随するいくつかの処理を行う．
  void
  set_size(ymuint size);

  /// @brief ベクタを作る．
  /// @param[in] index インデックス
  RegVect*
  new_vector(ymuint index);

  /// @brief ベクタを削除する．
  /// @param[in] vec 削除対象のベクタ
  void
  delete_vector(RegVect* vec);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベクタのサイズ
  ymuint32 mVectSize;

  // ブロックサイズ
  ymuint32 mBlockSize;

  // RegVect のサイズ
  ymuint32 mRvSize;

  // メモリを確保するためのオブジェクト
  UnitAlloc* mAlloc;

  // ベクタのリスト
  vector<const RegVect*> mVectList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ベクタのサイズを得る．
inline
ymuint
RvMgr::vect_size() const
{
  return mVectSize;
}

// @brief ベクタのリストを得る．
inline
const vector<const RegVect*>&
RvMgr::vect_list() const
{
  return mVectList;
}


END_NAMESPACE_YM_IGF

#endif // RVMGR_H
