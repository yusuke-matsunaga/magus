#ifndef YM_UTILS_ALLOC_H
#define YM_UTILS_ALLOC_H

/// @file ym_utils/Alloc.h
/// @brief Alloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class AllocBase Alloc.h "ym_utils/Alloc.h"
/// @brief メモリの管理を行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class AllocBase
{
public:

  /// @brief デストラクタ
  virtual
  ~AllocBase() { }


public:
  //////////////////////////////////////////////////////////////////////
  /// @name メモリの確保/開放を行う関数
  /// @{

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  virtual
  void*
  get_memory(ymuint64 n) = 0;

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  virtual
  void
  put_memory(ymuint64 n,
	     void* blk) = 0;

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  destroy() = 0;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name メモリ量の制限値に関する関数
  /// @{

  /// @brief メモリ量の制限値を設定する．
  /// @param[in] limit 制限値(単位はバイト)
  /// @note limit が 0 の時は制限なし
  virtual
  void
  set_mem_limit(ymuint64 limit) = 0;

  /// @brief メモリ量の制限値を返す．
  virtual
  ymuint64
  mem_limit() const = 0;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 統計情報を返す関数
  /// @{

  /// @brief 使用されているメモリ量を返す．
  virtual
  ymuint64
  used_size() const = 0;

  /// @brief used_size() の今までの最大値を返す．
  virtual
  ymuint64
  max_used_size() const = 0;

  /// @brief 実際に確保したメモリ量を返す．
  virtual
  ymuint64
  allocated_size() const = 0;

  /// @brief 実際に確保した回数を返す．
  virtual
  ymuint64
  allocated_count() const = 0;

  /// @brief 内部状態を出力する．
  virtual
  void
  print_stats(ostream& s) const = 0;

  /// @}
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM

#endif // YM_UTILS_ALLOC_H
