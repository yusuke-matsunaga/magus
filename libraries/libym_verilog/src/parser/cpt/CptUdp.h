#ifndef CPTUDP_H
#define CPTUDP_H

/// @file CptUdp.h
/// @brief CptUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/pt/PtUdp.h"
#include "YmVerilog/pt/PtArray.h"
#include "YmVerilog/pt/PtP.h"
#include "YmVerilog/VlUdpVal.h"
#include "YmUtils/FileRegion.h"

#include "PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief UDP を表すノード
//////////////////////////////////////////////////////////////////////
class CptUdp :
  public PtUdp
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptUdp(const FileRegion& file_region,
	 const char* name,
	 PtiPortArray port_array,
	 PtIOHeadArray iohead_array,
	 bool is_seq,
	 const PtExpr* init_value,
	 PtUdpEntryArray entry_array);

  /// @brief デストラクタ
  virtual
  ~CptUdp();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdp の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief primitivie type を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  /// @brief 名前を返す．
  virtual
  const char*
  name() const;

  /// @brief ポート数を取り出す．
  virtual
  ymuint
  port_num() const;

  /// @brief ポートを取り出す．
  virtual
  const PtPort*
  port(ymuint pos) const;

  /// @brief 入出力宣言ヘッダ配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const;

  /// @brief 初期値を取出す．
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
/// @brief UDP の中身のテーブルを表すクラス(組合せ回路用)
//////////////////////////////////////////////////////////////////////
class CptUdpEntry :
  public PtUdpEntry
{
  friend class CptFactory;

protected:

  /// @brief コンストラクタ
  CptUdpEntry(const FileRegion& file_region,
	      PtUdpValueArray input_array,
	      const PtUdpValue* output);

  /// @brief デストラクタ
  virtual
  ~CptUdpEntry();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief 入力値の配列を取り出す．
  virtual
  PtUdpValueArray
  input_array() const;

  /// @brief 現状態の値を取り出す．
  /// @note このクラスでは NULL を返す．
  virtual
  const PtUdpValue*
  current() const;

  /// @brief 出力の値を取り出す．
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

  // 出力のパタン
  const PtUdpValue* mOutput;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP の中身のテーブルを表すクラス(順序回路用)
//////////////////////////////////////////////////////////////////////
class CptUdpEntryS :
  public CptUdpEntry
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptUdpEntryS(const FileRegion& file_region,
	       PtUdpValueArray input_array,
	       const PtUdpValue* current,
	       const PtUdpValue* output);

  /// @brief デストラクタ
  virtual
  ~CptUdpEntryS();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現状態の値を取り出す．
  virtual
  const PtUdpValue*
  current() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現状態のパタン
  const PtUdpValue* mCurrent;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////
class CptUdpValue :
  public PtUdpValue
{
  friend class CptFactory;

private:

  /// @brief コンストラクタ
  CptUdpValue(const FileRegion& file_region,
	      char symbol);

  /// @brief コンストラクタ
  CptUdpValue(const FileRegion& file_region,
	      char symbol1,
	      char symbol2);

  /// @brief デストラクタ
  virtual
  ~CptUdpValue();


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpValue の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief シンボルを取り出す．
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

#endif // CPTUDP_H
