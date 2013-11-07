#ifndef LIBYM_VERILOG_PARSER_SCANNER_INPUTFILE_H
#define LIBYM_VERILOG_PARSER_SCANNER_INPUTFILE_H

/// @file libym_verilog/parser/scanner/InputFile.h
/// @brief InputFile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"

#include "ym_utils/Scanner.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/FileInfo.h"
#include "ym_utils/StrBuff.h"

#include "RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class InputFile InputFile.h "InputFile.h"
/// @ingroup VlParser
/// @brief 入力ファイルを表すクラス
///
/// このクラスはファイルを一文字単位で読み出す．\n
/// ただし, 改行コードの処理系依存部分を吸収して常に '\\n' だけを
/// 返すようにしている．
/// @sa FileInfo RawLex
//////////////////////////////////////////////////////////////////////
class InputFile :
  public Scanner
{
  friend class InputMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] ido 入力データ
  /// @param[in] lex 親の Lex
  InputFile(IDO& ido,
	    RawLex* lex);

  /// @brief デストラクタ
  ~InputFile();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンの読み出しを行う．
  /// @param[out] buff 結果の文字列を格納するバッファ
  /// @param[out] token_loc トークンの位置情報
  int
  read_token(StrBuff& buff,
	     FileRegion& token_loc);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_token() の下請け関数
  /// @param[out] buff 結果の文字列を格納するバッファ
  int
  _read_token(StrBuff& buff);

  /// @brief 2進数モードの読み込みを行う．
  /// @param[in] c 最初の文字
  /// @param[out] buff 結果を格納するバッファ
  /// @return トークンを返す．
  int
  read_bin_str(int c,
	       StrBuff& buff);

  /// @brief 8進数モードの読み込みを行う．
  /// @param[in] c 最初の文字
  /// @param[out] buff 結果を格納するバッファ
  /// @return トークンを返す．
  int
  read_oct_str(int c,
	       StrBuff& buff);

  /// @brief 10進数モードの読み込みを行う．
  /// @param[in] c 最初の文字
  /// @param[out] buff 結果を格納するバッファ
  /// @return トークンを返す．
  int
  read_dec_str(int c,
	       StrBuff& buff);

  /// @brief 16進数モードの読み込みを行う．
  /// @param[in] c 最初の文字
  /// @param[out] buff 結果を格納するバッファ
  /// @return トークンを返す．
  int
  read_hex_str(int c,
	       StrBuff& buff);

  /// @brief 識別子に用いられる文字([a-zA-Z0-9_$])が続く限り読みつづける．
  /// @param[out] buff 結果を格納する文字列バッファ
  void
  read_str(StrBuff& buff);

  /// @brief 二重引用符用の読み込み
  /// @param[out] buff 結果を格納する文字列バッファ
  /// @return トークン番号を返す．
  /// @note 可能性のあるトークンは
  ///  - STRING
  ///  - ERROR
  int
  read_dq_str(StrBuff& buff);

  /// @brief escaped identifier 用の読み込み
  /// @param[out] buff 結果を格納する文字列バッファ
  /// @return トークン番号を返す．
  /// @note 可能性のあるトークンは
  ///  - SPACE
  ///  - IDENTIFIER
  ///  - ERROR
  int
  read_esc_str(StrBuff& buff);

  /// @brief 数字を読み込む．
  /// @param[out] buff 結果を格納する文字列バッファ
  /// @return トークン番号を返す．
  /// @note 可能性のあるトークンは
  ///  - UNUM_INT
  ///  - RNUMBER
  ///  - ERROR
  int
  read_num(StrBuff& buff);

  /// @brief 空白文字を読み飛ばす
  void
  read_space();

  /// @brief '/' を読み込んだ後の処理
  /// @param[out] buff コメントを格納する文字列バッファ
  /// @return トークン番号を返す．
  /// @note 可能性のあるトークンは
  ///  - COMMENT1
  ///  - COMMENT2
  ///  - '/'
  ///  - ERROR
  int
  read_comment(StrBuff& buff);


protected:
  //////////////////////////////////////////////////////////////////////
  // Scanner の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 改行を読み込んだ時に起動する関数
  /// @param[in] line 行番号
  virtual
  void
  check_line(ymuint line);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の Lex
  RawLex* mLex;

};

//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief トークンの読み出しを行う．
// @param[out] buff 結果の文字列を格納するバッファ
// @param[out] token_loc トークンの位置情報
inline
int
InputFile::read_token(StrBuff& buff,
		      FileRegion& token_loc)
{
  int id = _read_token(buff);
  token_loc = cur_loc();
  return id;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_SCANNER_INPUTFILE_H
