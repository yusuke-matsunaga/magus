#ifndef YM_UTILS_GZODO_H
#define YM_UTILS_GZODO_H

/// @file ym_utils/GzODO.h
/// @brief GzODO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/ODO.h"


BEGIN_NAMESPACE_YM

class GzOut;

//////////////////////////////////////////////////////////////////////
/// @class GzODO GzODO.h "ym_utils/GzODO.h"
/// @ingroup YmUtils
/// @brief Gzress エンジンを用いた ODO の継承クラス
//////////////////////////////////////////////////////////////////////
class GzODO :
  public ODO
{
public:

  /// @brief 空のコンストラクタ
  /// @param[in] bits 初期ビットサイズ (0 でデフォルト値を用いる)
  explicit
  GzODO(ymuint bits = 0);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] bits 初期ビットサイズ (0 でデフォルト値を用いる)
  explicit
  GzODO(const char* filename,
	  ymuint bits = 0);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] bits 初期ビットサイズ (0 でデフォルト値を用いる)
  explicit
  GzODO(const string& filename,
	  ymuint bits = 0);

  /// @brief デストラクタ
  virtual
  ~GzODO();


public:
  //////////////////////////////////////////////////////////////////////
  // GzODO の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 書き込み可能なら true を返す．
  operator bool() const;

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  open(const char* filename);

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  open(const string& filename);

  /// @brief ファイルを閉じる．
  /// @note 以降の書き込みは行われない．
  void
  close();


public:
  //////////////////////////////////////////////////////////////////////
  // ODO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを書き出す．
  /// @param[in] buff データを収めた領域のアドレス
  /// @param[in] n データサイズ
  /// @return 実際に書き出した量を返す．
  virtual
  ssize_t
  write(const ymuint8* buff,
	ymuint64 n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力用のバッファ
  FileBuff* mBuff;

  // zstream
  zstream mZ;

  // CRC コード
  ymuint32 mCRC;

  // 出力されたデータサイズ
  ymuint32 mOutSize;

};

END_NAMESPACE_YM

#endif // YM_UTILS_GZODO_H
