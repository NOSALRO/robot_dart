#!/usr/bin/env python
# encoding: utf-8

"""
Quick n dirty pybind11 detection
"""

import os, glob, types
import subprocess
from waflib.Configure import conf


def options(opt):
    opt.add_option('--pybind11', type='string', help='path to pybind11', dest='pybind11')

@conf
def check_pybind11(conf, *k, **kw):
    required = kw.get('required', False)

    conf.start_msg('Checking for pybind11')
    includes_check = ['/usr/include/', '/usr/local/include/']

    if conf.options.pybind11:
        includes_check = [conf.options.pybind11]
    try:
        res = conf.find_file('pybind11/pybind11.h', includes_check)
        incl = res[:-len('pybind11/pybind11.h')-1]
        conf.env.INCLUDES_PYBIND11 = [incl]
        conf.end_msg(incl)
    except:
        if required:
            conf.fatal('Not found in %s' % str(includes_check))
        conf.end_msg('Not found in %s' % str(includes_check), 'RED')
    return 1