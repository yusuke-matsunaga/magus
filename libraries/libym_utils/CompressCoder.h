#ifndef COMPRESSDECODER_H
#define COMPRESSDECODER_H

/// @file CompressDecoder.h
/// @brief CompressDecoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileCoder.h"
#include "CompOut.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class CompressCoder CompressCoder.h "CompressCoder.h"
/// @brief compress(Z) 形式の圧縮器
//////////////////////////////////////////////////////////////////////
class CompressCoder :
  public FileCoder
{
public:

  /// @brief コンストラクタ
  CompressCoder();

  /// @brief デストラクタ
  virtual
  ~CompressCoder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] level 圧縮レベル
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  virtual
  bool
  open(const char* filename,
       ymuint level = 0);

  /// @brief ファイルを閉じる．
  virtual
  void
  close();

  /// @brief 書き込み可能の時に true を返す．
  virtual
  bool
  is_ready() const;

  /// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
  /// @param[in] wbuff 圧縮するデータを格納するバッファ
  /// @param[in] num 書き込むデータ数(バイト)
  /// @return 実際に書き込んだバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  virtual
  ssize_t
  write(const ymuint8* wbuff,
	ymuint64 num);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際に圧縮を行うクラス
  CompOut mOut;

};


END_NAMESPACE_YM

#endif // COMPRESSDECODER_H
