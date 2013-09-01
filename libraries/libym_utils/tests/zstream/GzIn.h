#ifndef GZIN_H
#define GZIN_H

/// @file GzIn.h
/// @brief GzIn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/zstream.h"


BEGIN_NAMESPACE_YM

class FileBuff;

//////////////////////////////////////////////////////////////////////
/// @class GzIn GzIn.h "GzIn.h"
/// @brief gzip'ed ファイルを読み込むクラス
//////////////////////////////////////////////////////////////////////
class GzIn
{
public:

  /// @brief コンストラクタ
  GzIn();

  /// @brief デストラクタ
  ~GzIn();


public:
  //////////////////////////////////////////////////////////////////////
  // 公開インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  open(const char* filename);

  /// @brief ファイルをクローズする．
  void
  close();

  /// @brief 圧縮されたデータを伸長する．
  /// @param[in] buff 伸長したデータを格納するバッファ
  /// @param[in] size バッファの空きサイズ
  ymuint
  decomp(ymuint8* buff,
	 ymuint size);


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

#endif // GZIN_H
