
// LzmaEngine のテスト


#include "ym_utils/LzmaEngine.h"
#include <fcntl.h>


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

string
LZMA_ACTION(lzma action)
{
  switch ( action ) {
  case LZMA_RUN:              return "LZMA_RUN";
  case LZMA_FLUSH:            return "LZMA_FLUSH";
  case LZMA_FINISH:           return "LZMA_FINISH";
  default:
    {
      ostringstream buf;
      buf << "Undefined action: " << action;
      return buf.str();
    }
  }
  return string();
}

string
LZMA_RET(int code)
{
  switch ( code ) {
  case LZMA_OK:               return "LZMA_OK";
  case LZMA_RUN_OK:           return "LZMA_RUN_OK";
  case LZMA_FLUSH_OK:         return "LZMA_FLUSH_OK";
  case LZMA_FINISH_OK:        return "LZMA_FINISH_OK";
  case LZMA_STREAM_END:       return "LZMA_STREAM_END";
  case LZMA_SEQUENCE_ERROR:   return "LZMA_SEQUENCE_ERROR";
  case LZMA_PARAM_ERROR:      return "LZMA_PARAM_ERROR";
  case LZMA_MEM_ERROR:        return "LZMA_MEM_ERROR";
  case LZMA_DATA_ERROR:       return "LZMA_DATA_ERROR";
  case LZMA_DATA_ERROR_MAGIC: return "LZMA_DATA_ERROR_MAGIC";
  case LZMA_IO_ERROR:         return "LZMA_IO_ERROR";
  case LZMA_UNEXPECTED_EOF:   return "LZMA_UNEXPECTED_EOF";
  case LZMA_OUTBUFF_FULL:     return "LZMA_OUTBUFF_FULL";
  case LZMA_CONFIG_ERROR:     return "LZMA_CONFIG_ERROR";
  default:
    {
      ostringstream buf;
      buf << "Undefined code: " << code;
      return buf.str();
    }
  }
  return string();
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

  int ret = lze.easy_encoder();
  if ( ret != LZMA_OK ) {
    cerr << "error in LzmaEngine::easy_encoder()" << endl;
    return -1;
  }

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

    lze.set_inbuf(rbuff, static_cast<ymuint>(res));
    do {
      lze.set_outbuf(wbuff, BUFF_SIZE);

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

  ymuint64 wr = BUFF_SIZE - bce.avail_out();
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
    //assert(strm.avail_in != 0);
    do {
      ret = lze.code(LZMA_RUN);
      if ( ret != LZMA_OK && ret != LZMA_STREAM_END ) {
	cerr << "error in LzmaStream::decompress()" << endl;
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
