#ifndef BZCODER_H
#define BZCODER_H

/// @file BzCoder.h
/// @brief BzCoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileCoder.h"
#include "ym_utils/BzEngine.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BzCoder BzCoder.h "BzCoder.h"
/// @brief bzip2 形式の圧縮器
//////////////////////////////////////////////////////////////////////
class BzCoder :
  public FileCoder
{
public:

  /// @brief コンストラクタ
  BzCoder();

  /// @brief デストラクタ
  virtual
  ~BzCoder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] mode ファイル作成用のモード
  /// @param[in] level 圧縮レベル ( 0 - 9 )
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 圧縮レベルは実際には 1 - 9 の整数
  /// 0 の場合にはデフォルト値(9)が用いられる．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  virtual
  bool
  open(const char* filename,
       mode_t mode = 0666,
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

  // 出力用のバッファ
  FileBuff mBuff;

  // 圧縮エンジン
  BzEngine mCompEngine;

  // 次の動作コード
  int mNextAction;

};

END_NAMESPACE_YM

#endif // BZCODER_H
