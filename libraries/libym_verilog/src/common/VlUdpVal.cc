
/// @file VlUdpVal.cc
/// @brief VlUdpVal の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/VlUdpVal.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
/// @brief VlUdpVal の mData の符号化のための値
//////////////////////////////////////////////////////////////////////
enum tVpiUdpVal {
  // レベルシンボル
  kVpiUdpVal0 = 1 << 0,
  kVpiUdpVal1 = 1 << 1,
  kVpiUdpValX = 1 << 2,

  kVpiUdpValB = kVpiUdpVal0 | kVpiUdpVal1,
  kVpiUdpValQ = kVpiUdpVal0 | kVpiUdpVal1 | kVpiUdpValX,

  // 遷移シンボル
  kVpiUdpVal00 = 1 << 3,
  kVpiUdpVal01 = 1 << 4,
  kVpiUdpVal0X = 1 << 5,
  kVpiUdpVal10 = 1 << 6,
  kVpiUdpVal11 = 1 << 7,
  kVpiUdpVal1X = 1 << 8,
  kVpiUdpValX0 = 1 << 9,
  kVpiUdpValX1 = 1 << 10,
  kVpiUdpValXX = 1 << 11,

  kVpiUdpVal0B = kVpiUdpVal00 | kVpiUdpVal01,
  kVpiUdpVal0Q = kVpiUdpVal00 | kVpiUdpVal01 | kVpiUdpVal0X,

  kVpiUdpVal1B = kVpiUdpVal10 | kVpiUdpVal11,
  kVpiUdpVal1Q = kVpiUdpVal10 | kVpiUdpVal11 | kVpiUdpVal1X,

  kVpiUdpValXB = kVpiUdpValX0 | kVpiUdpValX1,
  kVpiUdpValXQ = kVpiUdpValX0 | kVpiUdpValX1 | kVpiUdpValXX,

  kVpiUdpValB0 = kVpiUdpVal00 | kVpiUdpVal10,
  kVpiUdpValB1 = kVpiUdpVal01 | kVpiUdpVal11,
  kVpiUdpValBX = kVpiUdpVal0X | kVpiUdpVal1X,
  kVpiUdpValBB = kVpiUdpVal0B | kVpiUdpVal1B,
  kVpiUdpValBQ = kVpiUdpVal0Q | kVpiUdpVal1Q,

  kVpiUdpValQ0 = kVpiUdpVal00 | kVpiUdpVal10 | kVpiUdpValX0,
  kVpiUdpValQ1 = kVpiUdpVal01 | kVpiUdpVal11 | kVpiUdpValX1,
  kVpiUdpValQX = kVpiUdpVal0X | kVpiUdpVal1X | kVpiUdpValXX,
  kVpiUdpValQB = kVpiUdpVal0B | kVpiUdpVal1B | kVpiUdpValXB,
  kVpiUdpValQQ = kVpiUdpVal0Q | kVpiUdpVal1Q | kVpiUdpValXQ,

  kVpiUdpValP  = kVpiUdpVal01 | kVpiUdpVal0X | kVpiUdpValX1,
  kVpiUdpValN  = kVpiUdpVal10 | kVpiUdpVal1X | kVpiUdpValX0,

  kVpiUdpValR  = kVpiUdpVal01,
  kVpiUdpValF  = kVpiUdpVal10,

  kVpiUdpValNC = 1 << 12
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
  mData = kVpiUdpValNC;

  switch ( symbol ) {
  case '-': mData = kVpiUdpValNC; break;
  case '0': mData = kVpiUdpVal0; break;
  case '1': mData = kVpiUdpVal1; break;
  case 'X':
  case 'x': mData = kVpiUdpValX; break;
  case 'B':
  case 'b': mData = kVpiUdpValB; break;
  case '?': mData = kVpiUdpValQ; break;
  case 'R':
  case 'r': mData = kVpiUdpValR; break;
  case 'F':
  case 'f': mData = kVpiUdpValF; break;
  case 'P':
  case 'p': mData = kVpiUdpValP; break;
  case 'N':
  case 'n': mData = kVpiUdpValN; break;
  case '*': mData = kVpiUdpValQQ; break;

  default:
    cerr << "Illegal symbol char: " << symbol
	 << " (" << static_cast<ymuint>(symbol) << ")" << endl;
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief 値を表す2つの文字からのコンストラクタ
VlUdpVal::VlUdpVal(char symbol1,
		   char symbol2)
{
  mData = kVpiUdpValNC;

  switch ( symbol1 ) {
  case '0':
    switch ( symbol2 ) {
    case '0': mData = kVpiUdpVal00; break;
    case '1': mData = kVpiUdpVal01; break;
    case 'X':
    case 'x': mData = kVpiUdpVal0X; break;
    case 'B':
    case 'b': mData = kVpiUdpVal0B; break;
    case '?': mData = kVpiUdpVal0Q; break;
    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    break;

  case '1':
    switch ( symbol2 ) {
    case '0': mData = kVpiUdpVal10; break;
    case '1': mData = kVpiUdpVal11; break;
    case 'X':
    case 'x': mData = kVpiUdpVal1X; break;
    case 'B':
    case 'b': mData = kVpiUdpVal1B; break;
    case '?': mData = kVpiUdpVal1Q; break;
    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    break;

  case 'X':
  case 'x':
    switch ( symbol2 ) {
    case '0': mData = kVpiUdpValX0; break;
    case '1': mData = kVpiUdpValX1; break;
    case 'X':
    case 'x': mData = kVpiUdpValXX; break;
    case 'B':
    case 'b': mData = kVpiUdpValXB; break;
    case '?': mData = kVpiUdpValXQ; break;
    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    break;

  case 'B':
  case 'b':
    switch ( symbol2 ) {
    case '0': mData = kVpiUdpValB0; break;
    case '1': mData = kVpiUdpValB1; break;
    case 'X':
    case 'x': mData = kVpiUdpValBX; break;
    case 'B':
    case 'b': mData = kVpiUdpValBB; break;
    case '?': mData = kVpiUdpValBQ; break;
    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    break;

  case '?':
    switch ( symbol2 ) {
    case '0': mData = kVpiUdpValQ0; break;
    case '1': mData = kVpiUdpValQ1; break;
    case 'X':
    case 'x': mData = kVpiUdpValQX; break;
    case 'B':
    case 'b': mData = kVpiUdpValQB; break;
    case '?': mData = kVpiUdpValQQ; break;
    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

// @brief 値を直接指定するコンストラクタ
VlUdpVal::VlUdpVal(ymuint16 val) :
  mData(val)
{
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
  return (mData & kVpiUdpValB) == kVpiUdpValB;
}

// @brief ストリーム出力
// @relates VlUdpVal
// @param[in] s 出力先のストリーム
// @param[in] val 値
ostream&
operator<<(ostream& s,
	   const VlUdpVal& val)
{
  s << val.to_string();
  return s;
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
