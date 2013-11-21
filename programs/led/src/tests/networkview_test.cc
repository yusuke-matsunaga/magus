
/// @file networkview_test.cc
/// @brief NetworkView のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "led_nsdef.h"
#include "GateObj.h"
#include "NetworkView.h"


BEGIN_NAMESPACE_YM_LED

int
networkview_test(int argc,
		 char** argv)
{

  QApplication app(argc, argv);

  NetworkView* nv = new NetworkView();

  nv->new_gate(20, 30, kGtInput);
  nv->new_gate(500, 30, kGtOutput);
  nv->new_gate(100, 30, kGtBuffer);
  nv->new_gate(100, 150, kGtNot);
  nv->new_gate(100, 300, kGtAnd, 5);
  nv->new_gate(200, 100, kGtOr, 7);
  nv->new_gate(200, 400, kGtXor, 9);

  QScrollArea* w = new QScrollArea();
  w->setWidget(nv);

  QSpinBox* spinbox = new QSpinBox;
  QSlider* slider = new QSlider(Qt::Horizontal);
  spinbox->setRange(10, 400);
  slider->setRange(10, 400);

  QObject::connect(spinbox, SIGNAL(valueChanged(int)),
		   slider, SLOT(setValue(int)));
  QObject::connect(slider, SIGNAL(valueChanged(int)),
		   spinbox, SLOT(setValue(int)));
  QObject::connect(spinbox, SIGNAL(valueChanged(int)),
		   nv, SLOT(setScale2(int)));
  spinbox->setValue(100);

  QCheckBox* gcheckbox = new QCheckBox("grid on");
  QObject::connect(gcheckbox, SIGNAL(stateChanged(int)),
		   nv, SLOT(enableGrid(int)));

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addWidget(spinbox);
  layout->addWidget(slider);
  layout->addWidget(gcheckbox);

  QVBoxLayout* vlayout = new QVBoxLayout;
  vlayout->addWidget(w);
  vlayout->addLayout(layout);

  QWidget* window = new QWidget;
  window->setLayout(vlayout);

  window->show();

  return app.exec();
}

END_NAMESPACE_YM_LED


int
main(int argc,
     char** argv)
{
  return nsYm::nsLed::networkview_test(argc, argv);
}
