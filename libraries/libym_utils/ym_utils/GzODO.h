#ifndef YM_UTILS_GZODO_H
#define YM_UTILS_GZODO_H

/// @file ym_utils/GzODO.h
/// @brief GzODO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/ODO.h"
#include "ym_utils/FileCoder.h"


BEGIN_NAMESPACE_YM

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
  GzODO();

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
  explicit
  GzODO(const char* filename,
	ymuint level = 0);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
  explicit
  GzODO(const string& filename,
	ymuint level = 0);

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
  /// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  open(const char* filename,
       ymuint level = 0);

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  open(const string& filename,
       ymuint level = 0);

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

  // 実際の処理を行うエンジン
  FileCoder* mCoder;

};

END_NAMESPACE_YM

#endif // YM_UTILS_GZODO_H
