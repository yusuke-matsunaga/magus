#ifndef LED_NSDEF_H
#define LED_NSDEF_H

/// @file led_nsdef.h
/// @brief LED の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include <QtGui>


//////////////////////////////////////////////////////////////////////
// 名前空間の定義
//////////////////////////////////////////////////////////////////////

#define BEGIN_NAMESPACE_YM_LED			\
  BEGIN_NAMESPACE_YM				\
  BEGIN_NAMESPACE(nsLed)

#define END_NAMESPACE_YM_LED			\
  END_NAMESPACE(nsLEd)				\
  END_NAMESPACE_YM

#endif // LED_NSDEF_H
