
/// @file VsmCodeList.cc
/// @brief VsmCodeList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmCodeList.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス VsmCodeList::Builder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VsmCodeList::Builder::Builder()
{
}

// @brief デストラクタ
VsmCodeList::Builder::~Builder()
{
}

// @brief 命令を追加する．
// @param[in] op 命令
void
VsmCodeList::Builder::write_opcode(Ymsl_CODE op)
{
  mBody.push_back(op);
}

// @brief INT を追加する
// @param[in] val 値
void
VsmCodeList::Builder::write_int(Ymsl_INT val)
{
  ASSERT_COND( sizeof(Ymsl_INT) == sizeof(Ymsl_CODE) );
  mBody.push_back(static_cast<Ymsl_CODE>(val));
}

// @brief FLOAT を追加する．
// @param[in] val 値
void
VsmCodeList::Builder::write_float(Ymsl_FLOAT val)
{
  // ちょっとキタナイコード
  const ymuint n = sizeof(Ymsl_FLOAT) / sizeof(Ymsl_CODE);
  union buf_type {
    Ymsl_CODE codes[n];
    Ymsl_FLOAT fval;
  } buf;

  buf.fval = val;
  for (ymuint i = 0; i < n; ++ i) {
    mBody.push_back(buf.codes[i]);
  }
}

// @brief OBJPTR を追加する．
// @param[in] val 値
void
VsmCodeList::Builder::write_objptr(Ymsl_OBJPTR val)
{
  // ちょっとキタナイコード
  const ymuint n = sizeof(Ymsl_OBJPTR) / sizeof(Ymsl_CODE);
  union buf_type {
    Ymsl_CODE codes[n];
    Ymsl_OBJPTR pval;
  } buf;

  buf.pval = val;
  for (ymuint i = 0; i < n; ++ i) {
    mBody.push_back(buf.codes[i]);
  }
}

// @brief サイズを得る．
Ymsl_INT
VsmCodeList::Builder::size() const
{
  return mBody.size();
}

// @brief 命令を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
Ymsl_CODE
VsmCodeList::Builder::read_opcode(Ymsl_INT addr) const
{
  ASSERT_COND( 0 <= addr && addr < size() );
  return mBody[addr];
}


//////////////////////////////////////////////////////////////////////
// クラス VsmCodeList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] builder 初期化用オブジェクト
VsmCodeList::VsmCodeList(const Builder& builder)
{
  mSize = builder.size();
  mBody = new Ymsl_CODE[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mBody[i] = builder.read_opcode(i);
  }
}

// @brief デストラクタ
VsmCodeList::~VsmCodeList()
{
  delete [] mBody;
}


END_NAMESPACE_YM_YMSL
