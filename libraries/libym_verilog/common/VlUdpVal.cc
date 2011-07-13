
/// @file libym_verilog/common/VlUdpVal.cc
/// @brief VlUdpVal の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: udpval.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/VlUdpVal.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
/// @brief VlUdpVal の mData の符号化のための値
//////////////////////////////////////////////////////////////////////
enum tVpiUdpVal {
  kVpiUdpVal0  = 1 << 0,
  kVpiUdpVal1  = 1 << 1,
  kVpiUdpValX  = 1 << 2,
  kVpiUdpValB  = kVpiUdpVal0 | kVpiUdpVal1,
  kVpiUdpValQ  = kVpiUdpVal0 | kVpiUdpVal1 | kVpiUdpValX,

  kVpiUdpVal00 = kVpiUdpVal0 << 3,
  kVpiUdpVal01 = kVpiUdpVal1 << 3,
  kVpiUdpVal0X = kVpiUdpValX << 3,
  kVpiUdpVal0B = kVpiUdpVal00 | kVpiUdpVal01,
  kVpiUdpVal0Q = kVpiUdpVal00 | kVpiUdpVal01 | kVpiUdpVal0X,

  kVpiUdpVal10 = kVpiUdpVal0 << 6,
  kVpiUdpVal11 = kVpiUdpVal1 << 6,
  kVpiUdpVal1X = kVpiUdpValX << 6,
  kVpiUdpVal1B = kVpiUdpVal10 | kVpiUdpVal11,
  kVpiUdpVal1Q = kVpiUdpVal10 | kVpiUdpVal11 | kVpiUdpVal1X,

  kVpiUdpValX0 = kVpiUdpVal0 << 9,
  kVpiUdpValX1 = kVpiUdpVal1 << 9,
  kVpiUdpValXX = kVpiUdpValX << 9,
  kVpiUdpValXB = kVpiUdpValX0 | kVpiUdpValX1,
  kVpiUdpValXQ = kVpiUdpValX0 | kVpiUdpValX1 | kVpiUdpValXX,

  kVpiUdpValB0 = kVpiUdpVal00 | kVpiUdpVal10,
  kVpiUdpValB1 = kVpiUdpVal01 | kVpiUdpVal11,
  kVpiUdpValBX = kVpiUdpVal0X | kVpiUdpVal1X,
  kVpiUdpValBB = kVpiUdpValB0 | kVpiUdpValB1,
  kVpiUdpValBQ = kVpiUdpValBB | kVpiUdpValBX,

  kVpiUdpValQ0 = kVpiUdpValB0 | kVpiUdpValX0,
  kVpiUdpValQ1 = kVpiUdpValB1 | kVpiUdpValX1,
  kVpiUdpValQX = kVpiUdpValBX | kVpiUdpValXX,
  kVpiUdpValQB = kVpiUdpValQ0 | kVpiUdpValQ1,
  kVpiUdpValQQ = kVpiUdpValQB | kVpiUdpValQX,

  kVpiUdpValP  = kVpiUdpVal01 | kVpiUdpVal0X | kVpiUdpValX1,
  kVpiUdpValN  = kVpiUdpVal10 | kVpiUdpVal1X | kVpiUdpValX0,

  kVpiUdpValR  = kVpiUdpVal01,
  kVpiUdpValF  = kVpiUdpVal10,

  kVpiUdpValNC = 0
};

// シンボルを表す文字から値に変換する
inline
ymuint
conv(char symbol)
{
  switch ( symbol ) {
  case '-': return kVpiUdpValNC;

  case '0': return kVpiUdpVal0;
  case '1': return kVpiUdpVal1;
  case 'x': return kVpiUdpValX;

  case 'b': return kVpiUdpValB;
  case '?': return kVpiUdpValQ;

  case 'r': return kVpiUdpValR;
  case 'f': return kVpiUdpValF;
  case 'p': return kVpiUdpValP;
  case 'n': return kVpiUdpValN;
  case '*': return kVpiUdpValQQ;

  default:
    cerr << "Illegal symbol char: " << symbol
	 << " (" << static_cast<ymuint>(symbol) << ")" << endl;
    assert_not_reached(__FILE__, __LINE__);
  }

  return kVpiUdpValNC;
}

END_NONAMESPACE

// @brief 値を表す文字からのコンストラクタ
VlUdpVal::VlUdpVal(char symbol)
{
  mData = conv(symbol);
}

// @brief 値を表す2つの文字からのコンストラクタ
VlUdpVal::VlUdpVal(char symbol1,
		   char symbol2)
{
  ymuint val1 = conv(symbol1);
  ymuint val2 = conv(symbol2);
  assert_cond( val1 > 0 && val1 < 8, __FILE__, __LINE__);
  assert_cond( val2 > 0 && val2 < 8, __FILE__, __LINE__);

  ymuint new_val = 0U;
  if ( val1 & kVpiUdpVal0 ) {
    if ( val2 & kVpiUdpVal0 ) {
      new_val |= kVpiUdpVal00;
    }
    if ( val2 & kVpiUdpVal1 ) {
      new_val |= kVpiUdpVal01;
    }
    if ( val2 & kVpiUdpValX ) {
      new_val |= kVpiUdpVal0X;
    }
  }
  if ( val1 & kVpiUdpVal1 ) {
    if ( val2 & kVpiUdpVal0 ) {
      new_val |= kVpiUdpVal10;
    }
    if ( val2 & kVpiUdpVal1 ) {
      new_val |= kVpiUdpVal11;
    }
    if ( val2 & kVpiUdpValX ) {
      new_val |= kVpiUdpVal1X;
    }
  }
  if ( val1 & kVpiUdpValX ) {
    if ( val2 & kVpiUdpVal0 ) {
      new_val |= kVpiUdpValX0;
    }
    if ( val2 & kVpiUdpVal1 ) {
      new_val |= kVpiUdpValX1;
    }
    if ( val2 & kVpiUdpValX ) {
      new_val |= kVpiUdpValXX;
    }
  }

  mData = new_val;
}

// @brief デストラクタ
VlUdpVal::~VlUdpVal()
{
}

// @brief レベルシンボルの時に true を返す．
bool
VlUdpVal::is_level_symbol() const
{
  return mData < 8U;
}

// @brief 遷移シンボル(エッジシンボル)の時に true を返す．
bool
VlUdpVal::is_edge_symbol() const
{
  return mData >= 8U;
}

// @brief No Change シンボルの時に true を返す．
bool
VlUdpVal::is_nc_symbol() const
{
  return mData == kVpiUdpValNC;
}

// @brief 複合値の時に true を返す．
bool
VlUdpVal::is_composite_symbol() const
{
  return (mData & kVpiUdpValB) != 0U;
}

// @brief 文字列を返す．
string
VlUdpVal::to_string() const
{
  switch ( mData ) {
  case kVpiUdpVal0:  return "0";
  case kVpiUdpVal1:  return "1";
  case kVpiUdpValX:  return "x";
  case kVpiUdpValB:  return "b";
  case kVpiUdpValQ:  return "?";

  case kVpiUdpVal00: return "00";
  case kVpiUdpVal01: return "01";
  case kVpiUdpVal0X: return "0x";
  case kVpiUdpVal0B: return "0b";
  case kVpiUdpVal0Q: return "0?";

  case kVpiUdpVal10: return "10";
  case kVpiUdpVal11: return "11";
  case kVpiUdpVal1X: return "1x";
  case kVpiUdpVal1B: return "1b";
  case kVpiUdpVal1Q: return "1?";

  case kVpiUdpValX0: return "x0";
  case kVpiUdpValX1: return "x1";
  case kVpiUdpValXX: return "xx";
  case kVpiUdpValXB: return "xb";
  case kVpiUdpValXQ: return "x?";

  case kVpiUdpValB0: return "b0";
  case kVpiUdpValB1: return "b1";
  case kVpiUdpValBX: return "bx";
  case kVpiUdpValBB: return "bb";
  case kVpiUdpValBQ: return "b?";

  case kVpiUdpValQ0: return "?0";
  case kVpiUdpValQ1: return "?1";
  case kVpiUdpValQX: return "?x";
  case kVpiUdpValQB: return "?b";
  case kVpiUdpValQQ: return "??";

  case kVpiUdpValP:  return "p";
  case kVpiUdpValN:  return "n";

  case kVpiUdpValNC: return "-";

  default:
    break;
  }

  if ( mData < 8 ) {
    // 予想外の1値
    return "@";
  }
  // 予想外の2値
  return "@@";
}

END_NAMESPACE_YM_VERILOG
