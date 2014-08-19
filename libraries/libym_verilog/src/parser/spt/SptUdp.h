#ifndef SPTUDP_H
#define SPTUDP_H

/// @file SptUdp.h
/// @brief SptUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/pt/PtUdp.h"
#include "YmVerilog/pt/PtDecl.h"
#include "YmVerilog/VlUdpVal.h"
#include "YmUtils/FileRegion.h"
#include "PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// UDP を表すノード
//////////////////////////////////////////////////////////////////////
class SptUdp :
  public PtUdp
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptUdp(const FileRegion& file_region,
	 const char* name,
	 PtiPortArray port_array,
	 PtIOHeadArray iohead_array,
	 bool is_seq,
	 const PtExpr* init_value,
	 PtUdpEntryArray entry_array);

  // デストラクタ
  virtual
  ~SptUdp();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdp の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  // primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  // 名前を返す．
  virtual
  const char*
  name() const;

  // ポート数を取り出す．
  virtual
  ymuint
  port_num() const;

  // ポートを返す．
  virtual
  const PtPort*
  port(ymuint pos) const;

  /// @brief 入出力宣言ヘッダ配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const;

  // 初期値を取出す．
  virtual
  const PtExpr*
  init_value() const;

  /// @brief テーブルを取り出す．
  virtual
  PtUdpEntryArray
  table_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // プリミティブ名
  const char* mName;

  // ポートの配列
  PtiPortArray mPortArray;

  // 入出力宣言の配列
  PtIOHeadArray mIOHeadArray;

  // sequential primitive の時 true
  bool mSeq;

  // 初期値
  const PtExpr* mInitValue;

  // テーブル要素の配列
  PtUdpEntryArray mTableArray;

};


//////////////////////////////////////////////////////////////////////
// UDP の中身のテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class SptUdpEntry :
  public PtUdpEntry
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptUdpEntry(const FileRegion& file_region,
	      PtUdpValueArray input_array,
	      const PtUdpValue* current,
	      const PtUdpValue* output);

  // デストラクタ
  virtual
  ~SptUdpEntry();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 入力値の配列を取り出す．
  virtual
  PtUdpValueArray
  input_array() const;

  // 現状態の値を取り出す．
  virtual
  const PtUdpValue*
  current() const;

  // 出力の値を取り出す．
  virtual
  const PtUdpValue*
  output() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 入力パタンの配列
  PtUdpValueArray mInputArray;

  // 現状態のパタン
  const PtUdpValue* mCurrent;

  // 出力のパタン
  const PtUdpValue* mOutput;

};


//////////////////////////////////////////////////////////////////////
// UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////
class SptUdpValue :
  public PtUdpValue
{
  friend class SptFactory;

private:

  // コンストラクタ
  SptUdpValue(const FileRegion& file_region,
	      char symbol);

  // コンストラクタ
  SptUdpValue(const FileRegion& file_region,
	      char symbol1,
	      char symbol2);

  // デストラクタ
  virtual
  ~SptUdpValue();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpValue の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  // シンボルを取り出す．
  virtual
  VlUdpVal
  symbol() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // シンボル
  VlUdpVal mSymbol;

};

END_NAMESPACE_YM_VERILOG

#endif // SPTCUDP_H
