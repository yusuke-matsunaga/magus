#! /usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import CombiGenTest
import PermGenTest

unittest.TextTestRunner(verbosity=2).run(CombiGenTest.suite)
unittest.TextTestRunner(verbosity=2).run(PermGenTest.suite)
