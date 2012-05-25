#ifndef YM_GDS_GDSDUMPER_H
#define YM_GDS_GDSDUMPER_H

/// @file ym_gds/GdsDumper.h
/// @brief GdsDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"
#include "ym_gds/GdsRecord.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsDumper GdsDumper.h "ym_gds/GdsDumper.h"
/// @brief GDS-II ファイルの中身をダンプするためのクラス
//////////////////////////////////////////////////////////////////////
class GdsDumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] os 出力先のストリーム
  explicit
  GdsDumper(ostream& os);

  /// @brief デストラクタ
  ~GdsDumper();


public:

  /// @brief record の内容を出力する．
  /// @param[in] record レコード
  void
  operator()(const GdsRecord& record);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief HEADER の出力
  /// @param[in] data データ
  void
  dump_HEADER(const ymuint8 data[]);

  /// @brief BGNLIB の出力
  /// @param[in] data データ
  void
  dump_BGNLIB(const ymuint8 data[]);

  /// @brief UNITS の出力
  /// @param[in] data データ
  void
  dump_UNITS(const ymuint8 data[]);

  /// @brief BGNSTR の出力
  /// @param[in] data データ
  void
  dump_BGNSTR(const ymuint8 data[]);

  /// @brief XY の出力
  /// @param[in] data データ
  void
  dump_XY(const ymuint8 data[]);

  /// @brief COLROW の出力
  /// @param[in] data データ
  void
  dump_COLROW(const ymuint8 data[]);

  /// @brief PRESENTATION の出力
  /// @param[in] data データ
  void
  dump_PRESENTATION(const ymuint8 data[]);

  /// @brief STRANS の出力
  /// @param[in] data データ
  void
  dump_STRANS(const ymuint8 data[]);

  /// @brief ELFLAGS の出力
  /// @param[in] data データ
  void
  dump_ELFLAGS(const ymuint8 data[]);

  /// @brief LIBSECUR の出力
  /// @param[in] data データ
  void
  dump_LIBSECUR(const ymuint8 data[]);

  /// @brief data type が 2 byte integer 一つの場合の出力
  /// @param[in] data データ
  void
  dump_2int(const ymuint8 data[]);

  /// @brief data type が 4 byte integer 一つの場合の出力
  /// @param[in] data データ
  void
  dump_4int(const ymuint8 data[]);

  /// @brief data type が 8 byte real 一つの場合の出力
  /// @param[in] data データ
  /// @param[in] s_form 科学形式で出力する時 true にするフラグ
  void
  dump_8real(const ymuint8 data[],
	     bool s_form);

  /// @brief data type が ASCII String の場合の出力
  /// @param[in] data データ
  void
  dump_string(const ymuint8 data[]);

  /// @brief 時刻のデータを出力する．
  void
  dump_date(ymuint16 buf[]);

  /// @brief 2桁の整数を0つきで出力する．
  void
  dump_2digit(ymuint num);

  /// @brief 1バイトのデータを出力する．
  void
  dump_byte(ymuint8 byte);

  /// @brief data を 2バイト整数の配列とみなして pos 番めの要素を返す．
  /// @param[in] data データ
  /// @param[in] pos 位置
  static
  ymint16
  conv_2byte_int(const ymuint8 data[],
		 ymuint pos);

  /// @brief pos 番目の 4バイトのデータを符号つき数(2の補数表現)に変換する．
  /// @param[in] data データ
  /// @param[in] pos 位置
  static
  ymint32
  conv_4byte_int(const ymuint8 data[],
		 ymuint32 pos);

  /// @brief pos 番目の 4バイトのデータを浮動小数点数に変換する．
  /// @param[in] data データ
  /// @param[in] pos 位置
  static
  double
  conv_4byte_real(const ymuint8 data[],
		  ymuint32 pos);

  /// @brief pos 番目の 8バイトのデータを浮動小数点数に変換する．
  /// @param[in] data データ
  /// @param[in] pos 位置
  static
  double
  conv_8byte_real(const ymuint8 data[],
		  ymuint32 pos);

  /// @brief データを文字列に変換する．
  /// @param[in] data データ
  /// @param[in] n データサイズ
  static
  string
  conv_string(const ymuint8 data[],
	      ymuint32 n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  ostream& mOs;

};

END_NAMESPACE_YM_GDS

#endif
