
/// @file libym_utils/tests/IODO_test.cc
/// @brief IDOO のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileIDO.h"
#include "ym_utils/FileODO.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

ymuint8 data_8[] = {
  0U,
  1U,
  3U,
  127U,
  240U,
  255U
};
ymuint n_8 = sizeof(data_8) / sizeof(ymuint8);

ymuint16 data_16[] = {
  127U,
  255U,
  32767U,
  65535U
};
ymuint n_16 = sizeof(data_16) / sizeof(ymuint16);

ymuint32 data_32[] = {
  127UL,
  255UL,
  32727UL,
  65535UL,
  (1UL << 31) - 1UL,
  (1UL << 31) + 1UL
};
ymuint n_32 = sizeof(data_32) / sizeof(ymuint32);

ymuint64 data_64[] = {
  127ULL,
  255ULL,
  32727ULL,
  65535ULL,
  (1ULL << 31) - 1UL,
  (1ULL << 31) + 1UL,
  (1ULL << 63) - 1UL,
  (1ULL << 63) + 1UL
};
ymuint n_64 = sizeof(data_64) / sizeof(ymuint64);

END_NONAMESPACE

bool
IODO_test()
{
  const char* datafile = "IODO_test.bin";

  FileODO bo(datafile);
  if ( !bo ) {
    // エラー
    cerr << "Could not create " << datafile << endl;
  }

  for (ymuint i = 0; i < n_8; ++ i) {
    bo.write_8(data_8[i]);
  }
  for (ymuint i = 0; i < n_16; ++ i) {
    bo.write_16(data_16[i]);
  }
  for (ymuint i = 0; i < n_32; ++ i) {
    bo.write_32(data_32[i]);
  }
  for (ymuint i = 0; i < n_64; ++ i) {
    bo.write_64(data_64[i]);
  }
  bo.close();

  FileIDO bi(datafile);
  if ( !bi ) {
    // エラー
    cerr << "Could not open " << datafile << endl;
    return false;
  }

  ymuint nerr = 0;
  for (ymuint i = 0; i < n_8; ++ i) {
    ymuint8 val = bi.read_8();
    if ( val != data_8[i] ) {
      cerr << "data mismatch at data_8[" << i << "]" << endl
	   << "write data = " << static_cast<ymuint>(data_8[i]) << endl
	   << "read data  = " << static_cast<ymuint>(val) << endl;
      ++ nerr;
    }
  }
  for (ymuint i = 0; i < n_16; ++ i) {
    ymuint16 val = bi.read_16();
    if ( val != data_16[i] ) {
      cerr << "data mismatch at data_16[" << i << "]" << endl
	   << "write data = " << static_cast<ymuint>(data_16[i]) << endl
	   << "read data  = " << static_cast<ymuint>(val) << endl;
      ++ nerr;
    }
  }
  for (ymuint i = 0; i < n_32; ++ i) {
    ymuint32 val = bi.read_32();
    if ( val != data_32[i] ) {
      cerr << "data mismatch at data_32[" << i << "]" << endl
	   << "write data = " << static_cast<ymuint>(data_32[i]) << endl
	   << "read data  = " << static_cast<ymuint>(val) << endl;
      ++ nerr;
    }
  }
  for (ymuint i = 0; i < n_64; ++ i) {
    ymuint64 val = bi.read_64();
    if ( val != data_64[i] ) {
      cerr << "data mismatch at data_64[" << i << "]" << endl
	   << "write data = " << static_cast<ymuint>(data_64[i]) << endl
	   << "read data  = " << static_cast<ymuint>(val) << endl;
      ++ nerr;
    }
  }
  bi.close();

  if ( nerr ) {
    cerr << "Total " << nerr << "error";
    if ( nerr > 1 ) {
      cerr << "s";
    }
  }
  return nerr == 0;
}

bool
IODO_test2()
{
  const char* datafile = "IODO_test2.bin";

  {
    FileODO bo(datafile);
    if ( !bo ) {
      // エラー
      cerr << "Could not create " << datafile << endl;
      return false;
    }

    for (ymuint i = 0; i < n_8; ++ i) {
      bo << data_8[i];
    }
    for (ymuint i = 0; i < n_16; ++ i) {
      bo << data_16[i];
    }
    for (ymuint i = 0; i < n_32; ++ i) {
      bo << data_32[i];
    }
    for (ymuint i = 0; i < n_64; ++ i) {
      bo << data_64[i];
    }
  }

  ymuint nerr = 0;
  {
    FileIDO bi(datafile);
    if ( !bi ) {
      // エラー
      cerr << "Could not open " << datafile << endl;
      return false;
    }

    for (ymuint i = 0; i < n_8; ++ i) {
      ymuint8 val;
      bi >> val;
      if ( val != data_8[i] ) {
	cerr << "data mismatch at data_8[" << i << "]" << endl
	     << "write data = " << static_cast<ymuint>(data_8[i]) << endl
	     << "read data  = " << static_cast<ymuint>(val) << endl;
	++ nerr;
      }
    }
    for (ymuint i = 0; i < n_16; ++ i) {
      ymuint16 val;
      bi >> val;
      if ( val != data_16[i] ) {
	cerr << "data mismatch at data_16[" << i << "]" << endl
	     << "write data = " << static_cast<ymuint>(data_16[i]) << endl
	     << "read data  = " << static_cast<ymuint>(val) << endl;
	++ nerr;
      }
    }
    for (ymuint i = 0; i < n_32; ++ i) {
      ymuint32 val;
      bi >> val;
      if ( val != data_32[i] ) {
	cerr << "data mismatch at data_32[" << i << "]" << endl
	     << "write data = " << static_cast<ymuint>(data_32[i]) << endl
	     << "read data  = " << static_cast<ymuint>(val) << endl;
	++ nerr;
      }
    }
    for (ymuint i = 0; i < n_64; ++ i) {
      ymuint64 val;
      bi >> val;
      if ( val != data_64[i] ) {
	cerr << "data mismatch at data_64[" << i << "]" << endl
	     << "write data = " << static_cast<ymuint>(data_64[i]) << endl
	     << "read data  = " << static_cast<ymuint>(val) << endl;
	++ nerr;
      }
    }
  }

  if ( nerr ) {
    cerr << "Total " << nerr << "error";
    if ( nerr > 1 ) {
      cerr << "s";
    }
  }
  return nerr == 0;
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  bool stat = nsYm::IODO_test();
  if ( !stat ) {
    return 1;
  }

  stat = nsYm::IODO_test2();
  if ( !stat ) {
    return 2;
  }

  return 0;
}
