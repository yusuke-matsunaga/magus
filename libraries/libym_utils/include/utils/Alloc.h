#ifndef UTILS_ALLOC_H
#define UTILS_ALLOC_H

/// @file utils/Alloc.h
/// @brief Alloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Alloc Alloc.h "ym_utils/Alloc.h"
/// @brief メモリの管理を行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class Alloc
{
public:

  /// @brief コンストラクタ
  Alloc();

  /// @brief デストラクタ
  virtual
  ~Alloc();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name メモリの確保/開放を行う関数
  /// @{

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  void*
  get_memory(ymuint64 n);

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  void
  put_memory(ymuint64 n,
	     void* blk);

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  void
  destroy();

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name メモリ量の制限値に関する関数
  /// @{

  /// @brief メモリ量の制限値を設定する．
  /// @param[in] limit 制限値(単位はバイト)
  /// @note limit が 0 の時は制限なし
  void
  set_mem_limit(ymuint64 limit);

  /// @brief メモリ量の制限値を返す．
  ymuint64
  mem_limit() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 統計情報を返す関数
  /// @{

  /// @brief 使用されているメモリ量を返す．
  ymuint64
  used_size() const;

  /// @brief used_size() の今までの最大値を返す．
  ymuint64
  max_used_size() const;

  /// @brief 実際に確保したメモリ量を返す．
  ymuint64
  allocated_size() const;

  /// @brief 実際に確保した回数を返す．
  ymuint64
  allocated_count() const;

  /// @brief 内部状態を出力する．
  void
  print_stats(ostream& s) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 真のアロケート関数
  /// @param[in] n 確保するメモリ量(単位はバイト)
  /// @note 確保した総量が制限値を越えていたら 0 を返す．
  void*
  alloc(ymuint64 n);

  /// @brief 新のフリー関数
  /// @param[in] n 解放するメモリ量(単位はバイト)
  /// @param[in] blk 解放するメモリ領域
  void
  free(ymuint64 n,
       void* blk);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  // 継承クラスで実装する必要がある．
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際にメモリ領域の確保を行う関数
  /// @param[in] n 確保するメモリ量(単位はバイト)
  virtual
  void*
  _get_memory(ymuint64 n) = 0;

  /// @brief 実際にメモリ領域の開放を行う関数
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  virtual
  void
  _put_memory(ymuint64 n,
	      void* blk) = 0;

  /// @brief 実際に destory() の処理を行う関数
  virtual
  void
  _destroy() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ量の制限値
  ymuint64 mMemLimit;

  // 使用中のメモリサイズ
  ymuint64 mUsedSize;

  // 使用した最大のメモリサイズ
  ymuint64 mMaxUsedSize;

  // 確保したメモリサイズ
  ymuint64 mAllocSize;

  // 確保した回数
  ymuint64 mAllocCount;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief メモリ量の制限値を設定する．
// @param[in] limit 制限値(単位はバイト)
// @note limit が 0 の時は制限なし
inline
void
Alloc::set_mem_limit(ymuint64 limit)
{
  mMemLimit = limit;
}

// @brief メモリ量の制限値を返す．
inline
ymuint64
Alloc::mem_limit() const
{
  return mMemLimit;
}

// @brief 使用されているメモリ量を返す．
inline
ymuint64
Alloc::used_size() const
{
  return mUsedSize;
}

// @brief used_size() の今までの最大値を返す．
inline
ymuint64
Alloc::max_used_size() const
{
  return mMaxUsedSize;
}

// @brief 実際に確保したメモリ量を返す．
inline
ymuint64
Alloc::allocated_size() const
{
  return mAllocSize;
}

// @brief 実際に確保した回数を返す．
inline
ymuint64
Alloc::allocated_count() const
{
  return mAllocCount;
}

END_NAMESPACE_YM

#endif // UTILS_ALLOC_H
