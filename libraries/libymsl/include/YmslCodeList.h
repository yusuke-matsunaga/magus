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

  /// @brief 命令を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  Ymsl_CODE
  read_opcode(Ymsl_INT& addr);

  /// @brief INT を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  Ymsl_INT
  read_int(Ymsl_INT& addr);

  /// @brief FLOAT を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  Ymsl_FLOAT
  read_float(Ymsl_INT& addr);

  /// @brief OBJPTR を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  Ymsl_OBJPTR
  read_objptr(Ymsl_INT& addr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サイズ
  ymuint mSize;

  // 実体
  Ymsl_CODE* mBody;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLCODELIST_H
