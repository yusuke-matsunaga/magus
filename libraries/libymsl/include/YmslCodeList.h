#ifndef YMSLCODELIST_H
#define YMSLCODELIST_H

/// @file YmslCodeList.h
/// @brief YmslCodeList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslCodeList YmslCodeList.h "YmslCodeList.h"
/// @brief YMVM用の命令コードのリスト
//////////////////////////////////////////////////////////////////////
class YmslCodeList
{
public:

  /// @brief コンストラクタ
  /// @param[in] size コードサイズ
  YmslCodeList(ymuint size);

  /// @brief デストラクタ
  ~YmslCodeList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 命令を書き込む．
  /// @param[inout] addr アドレス
  /// @param[in] op 命令
  ///
  /// addr は次の位置を指すように変更される．
  void
  write_opcode(Ymsl_INT& addr,
	       Ymsl_CODE op);

  /// @brief INT を書き込む．
  /// @param[inout] addr アドレス
  /// @param[in] val 値
  ///
  /// addr は次の位置を指すように変更される．
  void
  write_int(Ymsl_INT& addr,
	    Ymsl_INT val);

  /// @brief FLOAT を書き込む．
  /// @param[inout] addr アドレス
  /// @param[in] val 値
  ///
  /// addr は次の位置を指すように変更される．
  void
  write_float(Ymsl_INT& addr,
	      Ymsl_FLOAT val);

  /// @brief OBJPTR を書き込む．
  /// @param[inout] addr アドレス
  /// @param[in] val 値
  ///
  /// addr は次の位置を指すように変更される．
  void
  write_objptr(Ymsl_INT& addr,
	       Ymsl_OBJPTR val);

  /// @brief サイズを得る．
  Ymsl_INT
  size() const;

  /// @brief 命令を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  Ymsl_CODE
  read_opcode(Ymsl_INT& addr) const;

  /// @brief INT を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  Ymsl_INT
  read_int(Ymsl_INT& addr) const;

  /// @brief FLOAT を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  Ymsl_FLOAT
  read_float(Ymsl_INT& addr) const;

  /// @brief OBJPTR を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  Ymsl_OBJPTR
  read_objptr(Ymsl_INT& addr) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サイズ
  ymuint mSize;

  // 実体
  Ymsl_CODE* mBody;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] size コードサイズ
inline
YmslCodeList::YmslCodeList(ymuint size)
{
  mSize = size;
  mBody = new Ymsl_CODE[size];
}

// @brief デストラクタ
inline
YmslCodeList::~YmslCodeList()
{
  delete [] mBody;
}

// @brief 命令を書き込む．
// @param[inout] addr アドレス
// @param[in] op 命令
//
// addr は次の位置を指すように変更される．
inline
void
YmslCodeList::write_opcode(Ymsl_INT& addr,
			   Ymsl_CODE op)
{
  mBody[addr] = op;
  ++ addr;
}

// @brief INT を書き込む．
// @param[inout] addr アドレス
// @param[in] val 値
//
// addr は次の位置を指すように変更される．
inline
void
YmslCodeList::write_int(Ymsl_INT& addr,
			Ymsl_INT val)
{
  ASSERT_COND( sizeof(Ymsl_INT) == sizeof(Ymsl_CODE) );
  mBody[addr] = val;
  ++ addr;
}

// @brief FLOAT を書き込む．
// @param[inout] addr アドレス
// @param[in] val 値
//
// addr は次の位置を指すように変更される．
inline
void
YmslCodeList::write_float(Ymsl_INT& addr,
			  Ymsl_FLOAT val)
{
  Ymsl_CODE* c_ptr = mBody + addr;
  Ymsl_FLOAT* f_ptr = reinterpret_cast<Ymsl_FLOAT*>(c_ptr);
  *f_ptr = val;
  addr += sizeof(Ymsl_FLOAT) / sizeof(Ymsl_CODE);
}

// @brief OBJPTR を書き込む．
// @param[inout] addr アドレス
// @param[in] val 値
//
// addr は次の位置を指すように変更される．
inline
void
YmslCodeList::write_objptr(Ymsl_INT& addr,
			   Ymsl_OBJPTR val)
{
  Ymsl_CODE* c_ptr = mBody + addr;
  Ymsl_OBJPTR* o_ptr = reinterpret_cast<Ymsl_OBJPTR*>(c_ptr);
  *o_ptr = val;
  addr += sizeof(Ymsl_OBJPTR) / sizeof(Ymsl_CODE);
}

// @brief サイズを得る．
inline
Ymsl_INT
YmslCodeList::size() const
{
  return mSize;
}

// @brief 命令を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
inline
Ymsl_CODE
YmslCodeList::read_opcode(Ymsl_INT& addr) const
{
  Ymsl_CODE op = mBody[addr];
  ++ addr;
  return op;
}

// @brief INT を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
inline
Ymsl_INT
YmslCodeList::read_int(Ymsl_INT& addr) const
{
  ASSERT_COND( sizeof(Ymsl_INT) == sizeof(Ymsl_CODE) );
  Ymsl_INT val = mBody[addr];
  ++ addr;
  return val;
}

// @brief FLOAT を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
inline
Ymsl_FLOAT
YmslCodeList::read_float(Ymsl_INT& addr) const
{
  Ymsl_CODE* c_ptr = mBody + addr;
  Ymsl_FLOAT* f_ptr = reinterpret_cast<Ymsl_FLOAT*>(c_ptr);
  Ymsl_FLOAT val = *f_ptr;
  addr += sizeof(Ymsl_FLOAT) / sizeof(Ymsl_CODE);
  return val;
}

// @brief OBJPTR を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
inline
Ymsl_OBJPTR
YmslCodeList::read_objptr(Ymsl_INT& addr) const
{
  Ymsl_CODE* c_ptr = mBody + addr;
  Ymsl_OBJPTR* o_ptr = reinterpret_cast<Ymsl_OBJPTR*>(c_ptr);
  Ymsl_OBJPTR val = *o_ptr;
  addr += sizeof(Ymsl_OBJPTR) / sizeof(Ymsl_CODE);
  return val;
}

END_NAMESPACE_YM_YMSL


#endif // YMSLCODELIST_H
