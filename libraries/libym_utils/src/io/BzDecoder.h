#ifndef BZDECODER_H
#define BZDECODER_H

/// @file BzDecoder.h
/// @brief BzDecoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FileDecoder.h"
#include "BzEngine.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BzDecoder BzDecoder.h "BzDecoder.h"
/// @brief bzip2 形式の伸長器
//////////////////////////////////////////////////////////////////////
class BzDecoder :
  public FileDecoder
{
public:

  /// @brief コンストラクタ
  BzDecoder();

  /// @brief デストラクタ
  virtual
  ~BzDecoder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しない．
  ///  - ファイルに対する読み出し許可がない．
  ///  - ファイルの形式が異なる．
  virtual
  bool
  open(const char* filename);

  /// @brief ファイルを閉じる．
  virtual
  void
  close();

  /// @brief 読み出せるデータがある時に true を返す．
  virtual
  bool
  is_ready() const;

  /// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
  /// @param[in] rbuff 展開したデータを格納するバッファ
  /// @param[in] num 読み出すデータ数(バイト)
  /// @return 実際に読み出したバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  virtual
  ymint64
  read(ymuint8* rbuff,
       ymuint64 num);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ファイル用のバッファ
  FileBuff mBuff;

  // 伸長エンジン
  BzEngine mDecompEngine;

};

END_NAMESPACE_YM

#endif // BZDECODER_H
