#ifndef GZIDO_H
#define GZIDO_H

/// @file GzIDO.h
/// @brief GzIDO のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/IDO.h"
#include "ym_utils/zstream.h"


BEGIN_NAMESPACE_YM

class FileBuff;

//////////////////////////////////////////////////////////////////////
/// @class GzIDO GzIDO.h "GzIDO.h"
/// @brief gzip'ed ファイルを読み込む IDO の継承クラスクラス
//////////////////////////////////////////////////////////////////////
class GzIDO :
  public IDO
{
public:

  /// @brief コンストラクタ
  GzIDO();

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @note 意味的にはコンストラクタ + open()
  explicit
  GzIDO(const char* filename);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @note 意味的にはコンストラクタ + open()
  explicit
  GzIDO(const string& filename);

  /// @brief デストラクタ
  ~GzIDO();


public:
  //////////////////////////////////////////////////////////////////////
  // 公開インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み出し可能なら true を返す．
  operator bool() const;

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  open(const char* filename);

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  open(const string& filename);

  /// @brief ファイルをクローズする．
  void
  close();


public:
  //////////////////////////////////////////////////////////////////////
  // IDO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮されたデータを伸長してバッファに書き込む．
  /// @param[in] buff 伸長したデータを格納するバッファ
  /// @param[in] size バッファの空きサイズ
  /// @return バッファに書き込まれたデータサイズを返す．
  /// @note エラーが起こった場合には負の値が返される．
  virtual
  ssize_t
  read(ymuint8* buff,
       size_t size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ファイル用のバッファ
  FileBuff* mBuff;

  // zstream
  zstream mZS;

  // CRC コード
  ymuint32 mCRC;

  // 出力バッファに書き込まれたサイズ
  ymuint32 mOutSize;

};

END_NONAMESPACE

#endif // GZIDO_H
