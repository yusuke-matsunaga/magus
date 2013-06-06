#ifndef RVMGR_H
#define RVMGR_H

/// @file RvMgr.h
/// @brief RvMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "ym_utils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class RvMgr RvMgr.h "RvMgr.h"
/// @brief RegVect を管理するクラス
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
  ymuint
  vect_size() const;

  /// @brief ベクタのリストを得る．
  const vector<const RegVect*>&
  vect_list() const;

  /// @brief インデックスのサイズを得る．
  ymuint
  index_size() const;

  /// @brief 内容を出力する．
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
