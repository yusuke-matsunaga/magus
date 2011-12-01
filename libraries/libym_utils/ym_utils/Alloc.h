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
  // メモリの確保/開放を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  virtual
  void*
  get_memory(ymuint n) = 0;

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  virtual
  void
  put_memory(ymuint n,
	     void* blk) = 0;

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  destroy() = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 統計情報を返す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用されているメモリ量を返す．
  virtual
  ymuint
  used_size() const = 0;

  /// @brief used_size() の最大値を返す．
  virtual
  ymuint
  max_used_size() const = 0;

  /// @brief 実際に確保したメモリ量を返す．
  virtual
  ymuint
  allocated_size() const = 0;

  /// @brief 実際に確保した回数を返す．
  virtual
  ymuint
  allocated_count() const = 0;

  /// @brief 内部状態を出力する．
  virtual
  void
  print_stats(ostream& s) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class SimpleAlloc Alloc.h "ym_utils/Alloc.h"
/// @brief 単純なメモリ管理
//////////////////////////////////////////////////////////////////////
class SimpleAlloc :
  public AllocBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size このオブジェクトが管理する最大サイズ
  /// @note max_size 以上のメモリ領域はデフォルトのアロケーターを
  /// 使用する．
  SimpleAlloc(ymuint max_size);

  /// @brief デストラクタ
  virtual
  ~SimpleAlloc();


public:
  //////////////////////////////////////////////////////////////////////
  // メモリの確保/開放を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  virtual
  void*
  get_memory(ymuint n);

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  virtual
  void
  put_memory(ymuint n,
	     void* blk);

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  destroy();


public:
  //////////////////////////////////////////////////////////////////////
  // 統計情報を返す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用されているメモリ量を返す．
  virtual
  ymuint
  used_size() const;

  /// @brief used_size() の最大値を返す．
  virtual
  ymuint
  max_used_size() const;

  /// @brief 実際に確保したメモリ量を返す．
  virtual
  ymuint
  allocated_size() const;

  /// @brief 実際に確保した回数を返す．
  virtual
  ymuint
  allocated_count() const;

  /// @brief 内部状態を出力する．
  virtual
  void
  print_stats(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief アラインメントを考慮してサイズを調節する．
  static
  ymuint
  align(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コンストラクタの引数
  ymuint32 mMaxSize;

  // mMaxSize を越えない2の巾乗の数．
  ymuint32 mMaxPowerSize;

  // mMaxPowerSize のログ
  ymuint32 mMaxLogSize;

  // 現在利用可能なメモリ領域
  char* mCurBlock;

  // mCurBlock の次に利用可能な位置
  ymuint32 mNextPos;

  // 確保して使用中のメモリ領域のリスト
  list<char*> mAllocList;

  // 使用中のメモリサイズ
  ymuint32 mUsedSize;

  // 使用した最大のメモリサイズ
  ymuint32 mMaxUsedSize;

  // 確保したメモリサイズ
  ymuint32 mAllocSize;

  // 確保した回数
  ymuint32 mAllocCount;

};


//////////////////////////////////////////////////////////////////////
/// @class FragAlloc Alloc.h "ym_utils/Alloc.h"
/// @brief 2の巾乗の単位で管理する
//////////////////////////////////////////////////////////////////////
class FragAlloc :
  public AllocBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size このオブジェクトが管理する最大サイズ
  /// @note max_size 以上のメモリ領域はデフォルトのアロケーターを
  /// 使用する．
  FragAlloc(ymuint max_size);

  /// @brief デストラクタ
  ~FragAlloc();


public:
  //////////////////////////////////////////////////////////////////////
  // メモリの確保/開放を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  virtual
  void*
  get_memory(ymuint n);

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  virtual
  void
  put_memory(ymuint n,
	     void* blk);

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  destroy();


public:
  //////////////////////////////////////////////////////////////////////
  // 統計情報を返す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用されているメモリ量を返す．
  virtual
  ymuint
  used_size() const;

  /// @brief used_size() の最大値を返す．
  virtual
  ymuint
  max_used_size() const;

  /// @brief 実際に確保したメモリ量を返す．
  virtual
  ymuint
  allocated_size() const;

  /// @brief 実際に確保した回数を返す．
  virtual
  ymuint
  allocated_count() const;

  /// @brief 内部状態を出力する．
  virtual
  void
  print_stats(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  // サイズ 2^p のブロックを確保する．
  char*
  alloc_block(ymuint p);

  // サイズ 2^p のブロックがあれば返す．
  // なければ NULL を返す．
  char*
  get_block(ymuint p);

  // サイズ 2^p のブロックをリストに戻す．
  void
  put_block(ymuint p,
	    char* block);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 利用可能なメモリ領域を管理するための構造体
  struct Block
  {
    // 次の要素を指すポインタ
    Block* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 最小のサイズ
  ymuint32 mMinSize;

  // mMinSize の log
  ymuint32 mMinLogSize;

  // コンストラクタの引数
  ymuint32 mMaxSize;

  // mMaxSize を越えない2の巾乗の数．
  ymuint32 mMaxPowerSize;

  // mMaxPowerSize の log
  ymuint32 mMaxLogSize;

  // サイズごとに分けられたブロックリストの配列
  // 配列の大きさは mMaxLogSize + 1
  Block** mBlockListArray;

  // 確保して使用中のメモリ領域のリスト
  list<char*> mAllocList;

  // 使用中のメモリサイズ
  ymuint32 mUsedSize;

  // 使用した最大のメモリサイズ
  ymuint32 mMaxUsedSize;

  // 確保したメモリサイズ
  ymuint32 mAllocSize;

  // 確保した回数
  ymuint32 mAllocCount;

};


//////////////////////////////////////////////////////////////////////
/// @class UnitAlloc Alloc.h "ym_utils/Alloc.h"
/// @brief 単一サイズのメモリ領域の管理を行うクラス
//////////////////////////////////////////////////////////////////////
class UnitAlloc :
  public AllocBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] unit_size メモリ割り当ての単位となるサイズ
  /// @param[in] block_size 一度に確保する個数
  UnitAlloc(ymuint32 unit_size,
	    ymuint32 block_size);

  /// @brief デストラクタ
  virtual
  ~UnitAlloc();


public:
  //////////////////////////////////////////////////////////////////////
  // メモリの確保/開放を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  /// @note n != unit_size の場合にはデフォルトアロケータを用いる．
  virtual
  void*
  get_memory(ymuint n);

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  /// @note n != unit_size の場合にはデフォルトアロケータを用いる．
  virtual
  void
  put_memory(ymuint n,
	     void* blk);

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  destroy();


public:
  //////////////////////////////////////////////////////////////////////
  // 統計情報を返す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用されているメモリ量を返す．
  virtual
  ymuint
  used_size() const;

  /// @brief used_size() の最大値を返す．
  virtual
  ymuint
  max_used_size() const;

  /// @brief 実際に確保したメモリ量を返す．
  virtual
  ymuint
  allocated_size() const;

  /// @brief 実際に確保した回数を返す．
  virtual
  ymuint
  allocated_count() const;

  /// @brief 内部状態を出力する．
  virtual
  void
  print_stats(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズ 2^p のブロックがあれば返す．
  /// @note なければ NULL を返す．
  char*
  get_block(ymuint p);

  /// @brief サイズ 2^p のブロックをリストに戻す．
  void
  put_block(ymuint p,
	    char* block);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 利用可能なメモリ領域を管理するための構造体
  struct Block
  {
    // 次の要素を指すポインタ
    Block* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 単位サイズ
  ymuint32 mUnitSize;

  // ブロックサイズ
  ymuint32 mBlockSize;

  // 利用可能なメモリ領域のリスト
  Block* mAvailTop;

  // 確保して使用中のメモリ領域のリスト
  list<char*> mAllocList;

  // 使用中のメモリサイズ
  ymuint32 mUsedSize;

  // 使用した最大のメモリサイズ
  ymuint32 mMaxUsedSize;

  // 確保したメモリサイズ
  ymuint32 mAllocSize;

  // 確保した回数
  ymuint32 mAllocCount;

};

END_NAMESPACE_YM

#endif // YM_UTILS_ALLOC_H
