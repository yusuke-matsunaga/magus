#ifndef BLIFDIC_H
#define BLIFDIC_H

/// @file BlifDic.h
/// @brief BlibDic のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/blif_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

/// @brief blif 形式のトークン
enum tToken {
  kTokenEOF,
  kTokenNL,
  kTokenSTRING,
  kTokenEQ,
  kTokenMODEL,
  kTokenINPUTS,
  kTokenOUTPUTS,
  kTokenCLOCK,
  kTokenEND,
  kTokenNAMES,
  kTokenEXDC,
  kTokenLATCH,
  kTokenGATE,
  kTokenMLATCH,
  kTokenSUBCKT,
  kTokenSEARCH,
  kTokenSTART_KISS,
  kTokenI,
  kTokenO,
  kTokenP,
  kTokenR,
  kTokenEND_KISS,
  kTokenLATCH_ORDER,
  kTokenCODE,
  kTokenCYCLE,
  kTokenCLOCK_EVENT,
  kTokenAREA,
  kTokenDELAY,
  kTokenWIRE_LOAD_SLOPE,
  kTokenWIRE,
  kTokenINPUT_ARRIVAL,
  kTokenDEFAULT_INPUT_ARRIVAL,
  kTokenOUTPUT_REQUIRED,
  kTokenDEFAULT_OUTPUT_REQUIRED,
  kTokenINPUT_DRIVE,
  kTokenDEFAULT_INPUT_DRIVE,
  kTokenOUTPUT_LOAD,
  kTokenDEFAULT_OUTPUT_LOAD
};


//////////////////////////////////////////////////////////////////////
/// @class BlifDic BlifDic.h "BlifDic.h"
/// @brief blif 用の予約語辞書
//////////////////////////////////////////////////////////////////////
class BlifDic
{
public:

  /// @brief コンストラクタ
  BlifDic();

  /// @brief デストラクタ
  ~BlifDic();


public:

  /// @brief str に対応したトークンを返す．
  tToken
  get_token(const char* str);

  /// @brief トークンの内容を出力する．
  static
  void
  dump_token(ostream& s,
	     tToken token);


private:

  struct Cell {

    // 予約語の文字列
    const char* mStr;

    // 予約語のトークン
    tToken mToken;

    // ハッシュ表の次の要素を指すポインタ
    Cell* mLink;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  Cell** mHashTable;

  // 全セルの先頭
  Cell* mCellBlock;

};

/// @relates tToken
/// @brief トークンを出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] token トークン
ostream&
operator<<(ostream& s,
	   tToken token);


END_NAMESPACE_YM_NETWORKS_BLIF

#endif // BLIFDIC_H
