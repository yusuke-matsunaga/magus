
/// @file SptUdp.cc
/// @brief SptUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SptUdp.h"
#include "SptFactory.h"

#include "PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// UDP を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptUdp::SptUdp(const FileRegion& file_region,
	       const char* name,
	       PtiPortArray port_array,
	       PtIOHeadArray iohead_array,
	       bool is_seq,
	       const PtExpr* init_value,
	       PtUdpEntryArray entry_array) :
  mFileRegion(file_region),
  mName(name),
  mPortArray(port_array),
  mIOHeadArray(iohead_array),
  mSeq(is_seq),
  mInitValue(init_value),
  mTableArray(entry_array)
{
}

// デストラクタ
SptUdp::~SptUdp()
{
}

// ファイル位置を返す．
FileRegion
SptUdp::file_region() const
{
  return mFileRegion;
}

// primitivie type を返す．
tVpiPrimType
SptUdp::prim_type() const
{
  if ( mSeq ) {
    return kVpiSeqPrim;
  }
  else {
    return kVpiCombPrim;
  }
}

// 名前を返す．
const char*
SptUdp::name() const
{
  return mName;
}

// ポート数を取り出す．
ymuint
SptUdp::port_num() const
{
  return mPortArray.size();
}

// ポートを取り出す．
const PtPort*
SptUdp::port(ymuint pos) const
{
  return mPortArray[pos];
}

/// @brief 入出力宣言ヘッダ配列の取得
PtIOHeadArray
SptUdp::iohead_array() const
{
  return mIOHeadArray;
}

// 初期値を取出す．
const PtExpr*
SptUdp::init_value() const
{
  return mInitValue;
}

// @brief テーブルを取り出す．
PtUdpEntryArray
SptUdp::table_array() const
{
  return mTableArray;
}


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの中身を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptUdpEntry::SptUdpEntry(const FileRegion& file_region,
			 PtUdpValueArray input_array,
			 const PtUdpValue* current,
			 const PtUdpValue* output) :
  mFileRegion(file_region),
  mInputArray(input_array),
  mCurrent(current),
  mOutput(output)
{
}

// デストラクタ
SptUdpEntry::~SptUdpEntry()
{
}

// ファイル位置を返す．
FileRegion
SptUdpEntry::file_region() const
{
  return mFileRegion;
}

// @brief 入力値の配列を取り出す．
PtUdpValueArray
SptUdpEntry::input_array() const
{
  return mInputArray;
}

// 現状態の値を取り出す．
const PtUdpValue*
SptUdpEntry::current() const
{
  return mCurrent;
}

// 出力の値を取り出す．
const PtUdpValue*
SptUdpEntry::output() const
{
  return mOutput;
}


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptUdpValue::SptUdpValue(const FileRegion& file_region,
			 char symbol1) :
  mFileRegion(file_region),
  mSymbol(symbol1)
{
}

// コンストラクタ
SptUdpValue::SptUdpValue(const FileRegion& file_region,
			 char symbol1,
			 char symbol2) :
  mFileRegion(file_region),
  mSymbol(symbol1, symbol2)
{
}

// デストラクタ
SptUdpValue::~SptUdpValue()
{
}

// ファイル位置を返す．
FileRegion
SptUdpValue::file_region() const
{
  return mFileRegion;
}

// 1つめのシンボルを取り出す．
VlUdpVal
SptUdpValue::symbol() const
{
  return mSymbol;
}


//////////////////////////////////////////////////////////////////////
// UDP 関係のオブジェクトの生成
//////////////////////////////////////////////////////////////////////

// @brief combinational UDP の生成
// @param[in] file_region ファイル位置情報
// @param[in] ai_top 属性インスタンスのリスト
// @param[in] name UDP名
// @param[in] port_array ポート名のリスト
// @param[in] iohead_array IOリスト
// @param[in] entry_top テーブルエントリのリスト
// @return 生成された UDP
const PtUdp*
SptFactory::new_CmbUdp(const FileRegion& file_region,
		       const char* name,
		       PtiPortArray port_array,
		       PtIOHeadArray iohead_array,
		       PtUdpEntryArray entry_array)
{
  void* p = alloc().get_memory(sizeof(SptUdp));
  return new (p) SptUdp(file_region,
			name,
			port_array,
			iohead_array,
			false, NULL,
			entry_array);
}

// @brief sequential UDP の生成
// @param[in] file_region ファイル位置の情報
// @param[in] ai_top 属性インスタンスのリスト
// @param[in] name UDP名
// @param[in] port_array ポート名のリスト
// @param[in] iohead_array IOリスト
// @param[in] init_value 初期値を表す式
// @param[in] entry_top テーブルエントリのリスト
// @return 生成された UDP
const PtUdp*
SptFactory::new_SeqUdp(const FileRegion& file_region,
		       const char* name,
		       PtiPortArray port_array,
		       PtIOHeadArray iohead_array,
		       const PtExpr* init_value,
		       PtUdpEntryArray entry_array)
{
  void* p = alloc().get_memory(sizeof(SptUdp));
  return new (p) SptUdp(file_region,
			name,
			port_array,
			iohead_array,
			true, init_value,
			entry_array);
}

// @brief combinational UDP 用のテーブルエントリの生成
// @param[in] file_region ファイル位置の情報
// @param[in] input_array 入力値のリスト
// @param[in] output 出力値のリスト
// @return 生成されたテーブルエントリ
const PtUdpEntry*
SptFactory::new_UdpEntry(const FileRegion& file_region,
			 PtUdpValueArray input_array,
			 const PtUdpValue* output)
{
  void* p = alloc().get_memory(sizeof(SptUdpEntry));
  return new (p) SptUdpEntry(file_region,
			     input_array,
			     NULL,
			     output);
}

// @brief sequential UDP 用のテーブルエントリの生成
// @param[in] file_region ファイル位置の情報
// @param[in] input_array 入力値のリスト
// @param[in] current 現状態値
// @param[in] output 出力値のリスト
// @return 生成されたテーブルエントリ
const PtUdpEntry*
SptFactory::new_UdpEntry(const FileRegion& file_region,
			 PtUdpValueArray input_array,
			 const PtUdpValue* current,
			 const PtUdpValue* output)
{
  void* p = alloc().get_memory(sizeof(SptUdpEntry));
  return new (p) SptUdpEntry(file_region,
			     input_array,
			     current,
			     output);
}

// @brief UDP のテーブルエントリの要素の値の生成
// @param[in] file_region ファイル位置の情報
// @param[in] symbol シンボル
// @return 生成された値
const PtUdpValue*
SptFactory::new_UdpValue(const FileRegion& file_region,
			 char symbol)
{
  void* p = alloc().get_memory(sizeof(SptUdpValue));
  return new (p) SptUdpValue(file_region, symbol);
}

// @brief UDP のテーブルエントリの要素の値の生成
// @param[in] fr ファイル位置の情報
// @param[in] symbol1, symbol2 シンボル
// @return 生成された値
const PtUdpValue*
SptFactory::new_UdpValue(const FileRegion& file_region,
			 char symbol1,
			 char symbol2)
{
  void* p = alloc().get_memory(sizeof(SptUdpValue));
  return new (p) SptUdpValue(file_region, symbol1, symbol2);
}

END_NAMESPACE_YM_VERILOG
