#ifndef COMPIN_H
#define COMPIN_H

/// @file CompIn.h
/// @brief CompIn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "CompBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class CompIn CompIn.h "CompIn.h"
/// @brief uncompress 用の CompBase
//////////////////////////////////////////////////////////////////////
class CompIn :
  public CompBase
{
public:

  /// @brief コンストラクタ
  CompIn();

  /// @brief デストラクタ
  ~CompIn();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
  /// @param[in] rbuff 展開したデータを格納するバッファ
  /// @param[in] num 読み出すデータ数(バイト)
  /// @return 実際に読み出したバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  ssize_t
  read(ymuint8* rbuff,
       ymuint64 num);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  code_int
  getcode();

  /// @brief スタックを初期化する．
  void
  init_stack();

  /// @brief スタックが空の時 true を返す．
  bool
  is_empty();

  /// @brief スタックにデータを積む．
  void
  push_stack(char_type data);

  /// @brief スタックからデータを取り出す．
  char_type
  pop_stack();

  u_short&
  tab_prefixof(ymuint i);

  char_type&
  tab_suffixof(ymuint i);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  char_type mStack[8000];

  char_type* mStackPtr;

  u_short mPrefix[k_HSIZE];

  char_type mSuffix[1 << k_BITS];

  ymint32 m_finchar;

  code_int m_oldcode;

  code_int m_incode;

  ymint32 m_roffset;

  ymint32 m_size;

  char_type m_gbuf[k_BITS];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

inline
u_short&
CompIn::tab_prefixof(ymuint i)
{
  return mPrefix[i];
}

inline
CompBase::char_type&
CompIn::tab_suffixof(ymuint i)
{
  return mSuffix[i];
}

// @brief スタックを初期化する．
inline
void
CompIn::init_stack()
{
  mStackPtr = &mStack[0];
}

// @brief スタックが空の時 true を返す．
inline
bool
CompIn::is_empty()
{
  return mStackPtr == &mStack[0];
}

// @brief スタックにデータを積む．
inline
void
CompIn::push_stack(char_type data)
{
  assert_cond( mStackPtr < &mStack[8000], __FILE__, __LINE__);
  *(mStackPtr ++) = data;
}

// @brief スタックからデータを取り出す．
inline
CompIn::char_type
CompIn::pop_stack()
{
  return *(-- mStackPtr);
}

END_NAMESPACE_YM

#endif // COMPIN_H
