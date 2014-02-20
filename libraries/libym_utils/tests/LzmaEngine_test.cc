
// LzmaEngine のテスト


#include "io/LzmaEngine.h"
#include <fcntl.h>


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

string
LZMA_ACTION(lzma_action action)
{
  if ( action == LZMA_RUN ) {
    return "LZMA_RUN";
  }
  if ( action == LZMA_SYNC_FLUSH ) {
    return "LZMA_SYNC_FLUSH";
  }
  if ( action == LZMA_FULL_FLUSH ) {
    return "LZMA_FULL_FLUSH";
  }
  if ( action == LZMA_FINISH ) {
    return "LZMA_FINISH";
  }

  ostringstream buf;
  buf << "Undefined action: " << static_cast<int>(action);
  return buf.str();
}

string
LZMA_RET(lzma_ret code)
{
  if ( code == LZMA_OK ) {
    return "LZMA_OK";
  }
  if ( code == LZMA_STREAM_END ) {
    return "LZMA_STREAM_END";
  }
  if ( code == LZMA_NO_CHECK ) {
    return "LZMA_NO_CHECK";
  }
  if ( code == LZMA_UNSUPPORTED_CHECK ) {
    return "LZMA_UNSUPPORTED_CHECK";
  }
  if ( code == LZMA_GET_CHECK ) {
    return "LZMA_GET_CHECK";
  }
  if ( code == LZMA_MEM_ERROR ) {
    return "LZMA_MEM_ERROR";
  }
  if ( code == LZMA_MEMLIMIT_ERROR ) {
    return "LZMA_MEMLIMIT_ERROR";
  }
  if ( code == LZMA_FORMAT_ERROR ) {
    return "LZMA_FORMAT_ERROR";
  }
  if ( code == LZMA_OPTIONS_ERROR ) {
    return "LZMA_OPTIONS_ERROR";
  }
  if ( code == LZMA_DATA_ERROR ) {
    return "LZMA_DATA_ERROR";
  }
  if ( code == LZMA_BUF_ERROR ) {
    return "LZMA_BUF_ERROR";
  }
  if ( code == LZMA_PROG_ERROR ) {
    return "LZMA_PROG_ERROR";
  }

  ostringstream buf;
  buf << "Undefined return code: " << static_cast<int>(code);
  return buf.str();
}

char
hex_char(ymuint8 x)
{
  if ( x <= 9 ) {
    return '0' + x;
  }
  else {
    return 'A' + (x - 10);
  }
}

string
hex_str(ymuint8 x)
{
  ymuint8 h = x >> 4;
  ymuint8 l = x % 15;
  string ans;
  ans += hex_char(h);
  ans += hex_char(l);
  return ans;
}

END_NONAMESPACE

int
compress(const char* infile,
	 const char* outfile)
{
  LzmaEngine lze;

  int i_fd = open(infile, O_RDONLY);
  if ( i_fd < 0 ) {
    cerr << "Could not open " << infile << endl;
    return -1;
  }

  int o_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if ( o_fd < 0 ) {
    cerr << "Could not create " << outfile << endl;
    return -1;
  }

  ymuint BUFF_SIZE = 4096;
  ymuint8 rbuff[BUFF_SIZE];
  ymuint8 wbuff[BUFF_SIZE];

  lzma_ret ret = lze.easy_encoder(6, LZMA_CHECK_CRC64);
  if ( ret != LZMA_OK ) {
    cerr << "error in LzmaEngine::easy_encoder()" << endl;
    return -1;
  }

  lze.set_outbuf(wbuff, BUFF_SIZE);
  lzma_action action = LZMA_RUN;
  do {
    ssize_t res = read(i_fd, rbuff, BUFF_SIZE);
    if ( res < 0 ) {
      cerr << "error in read()" << endl;
      return -1;
    }
    cerr << "read " << res << " bytes" << endl;

    if ( res == 0 ) {
      action = LZMA_FINISH;
    }

    assert_cond( lze.avail_in() == 0, __FILE__, __LINE__);
    lze.set_inbuf(rbuff, static_cast<ymuint>(res));
    do {
      ret = lze.code(action);
      cerr << "code(" << LZMA_ACTION(action) << ") => " << LZMA_RET(ret) << endl;
      if ( ret != LZMA_OK && ret != LZMA_STREAM_END ) {
	cerr << "error in LzmaEngine::code()" << endl;;
      }

      if ( lze.avail_out() == 0 ) {
	res = write(o_fd, wbuff, BUFF_SIZE);
	if ( res < 0 ) {
	  cerr << "error in write()" << endl;
	  return -1;
	}
	cerr << "write " << res << " bytes" << endl;
	lze.set_outbuf(wbuff, BUFF_SIZE);
      }
    } while (lze.avail_in() > 0);
  } while ( ret != LZMA_STREAM_END );

  ymuint64 wr = BUFF_SIZE - lze.avail_out();
  if ( wr > 0 ) {
    ssize_t res = write(o_fd, wbuff, wr);
    if ( res < 0 ) {
      cerr << "error in write()" << endl;
      return -1;
    }
    cerr << "write " << res << " bytes" << endl;
  }

  lze.end();

  close(i_fd);
  close(o_fd);

  return 0;
}

int
decompress(const char* infile,
	   const char* outfile)
{
  LzmaEngine lze;

  int i_fd = open(infile, O_RDONLY);
  if ( i_fd < 0 ) {
    cerr << "Could not open " << infile << endl;
    return -1;
  }

  int o_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if ( o_fd < 0 ) {
    cerr << "Could not create " << outfile << endl;
    return -1;
  }

  const ymuint BUFF_SIZE = 4096;
  ymuint8 rbuff[BUFF_SIZE];
  ymuint8 wbuff[BUFF_SIZE];

  int ret = lze.stream_decoder();
  if ( ret != LZMA_OK ) {
    cerr << "error in LzmaEngine::stream_decoder()" << endl;
    return -1;
  }

  lze.set_outbuf(wbuff, BUFF_SIZE);
  do {
    ssize_t res = read(i_fd, rbuff, 4096);
    if ( res < 0 ) {
      cerr << "error in read()" << endl;
      return -1;
    }
    lze.set_inbuf(rbuff, static_cast<ymint>(res));
    do {
      ret = lze.code(LZMA_RUN);
      if ( ret != LZMA_OK && ret != LZMA_STREAM_END ) {
	cerr << "error in LzmaStream::code()" << endl;
	return -1;
      }

      if ( lze.avail_out() == 0 ) {
	// 出力バッファの中身を処理
	res = write(o_fd, wbuff, BUFF_SIZE);
	if ( res < 0 ) {
	  cerr << "error in write()" << endl;
	  return -1;
	}
	lze.set_outbuf(wbuff, BUFF_SIZE);
      }
    } while (lze.avail_in() > 0);
  } while ( ret != LZMA_STREAM_END );
  ymuint64 wr = BUFF_SIZE - lze.avail_out();
  if ( wr > 0 ) {
    ssize_t res = write(o_fd, wbuff, wr);
    if ( res < 0 ) {
      cerr << "error in write()" << endl;
      return -1;
    }
  }

  lze.end();

  close(i_fd);
  close(o_fd);

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace nsYm;

  if ( argc != 4 ) {
    cerr << "usage: " << argv[0] << " c|d input-file output-file" << endl;
    return 250;
  }
  if ( strlen(argv[1]) != 1 || argv[1][0] != 'c' && argv[1][0] != 'd' ) {
    cerr << "argv[1] should be \"c\" or \"d\"" << endl;
    return 251;
  }
  try {
    if ( argv[1][0] == 'c' ) {
      // compress mode
      return compress(argv[2], argv[3]);
    }
    else if ( argv[1][0] == 'd' ) {
      // decompress mode
      return decompress(argv[2], argv[3]);
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }
  catch ( ... ) {
    cerr << "unknown exception" << endl;
    return 254;
  }

  return 0;
}
