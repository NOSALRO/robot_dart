#!/usr/bin/env python
# encoding: utf-8
import sys
import os
import fnmatch
import glob
sys.path.insert(0, sys.path[0]+'/waf_tools')

VERSION = '1.0.0'
APPNAME = 'robot_dart'

srcdir = '.'
blddir = 'build'

from waflib.Build import BuildContext
from waflib import Logs
from waflib.Tools import waf_unit_test
import dart
import boost
import eigen
import hexapod_controller


def options(opt):
    opt.load('compiler_cxx')
    opt.load('compiler_c')
    opt.load('boost')
    opt.load('eigen')
    opt.load('dart')
    opt.load('hexapod_controller')

    opt.add_option('--shared', action='store_true', help='build shared library', dest='build_shared')
    opt.add_option('--tests', action='store_true', help='compile tests or not', dest='tests')


def configure(conf):
    conf.get_env()['BUILD_GRAPHIC'] = False

    conf.load('compiler_cxx')
    conf.load('compiler_c')
    conf.load('waf_unit_test')
    conf.load('boost')
    conf.load('eigen')
    conf.load('dart')
    conf.load('hexapod_controller')

    conf.check_boost(lib='regex system filesystem unit_test_framework', min_version='1.46')
    conf.check_eigen(required=True)
    conf.check_dart(required=True)
    conf.check_hexapod_controller()

    conf.env['lib_type'] = 'cxxstlib'
    if conf.options.build_shared:
        conf.env['lib_type'] = 'cxxshlib'

    if conf.env.CXX_NAME in ["icc", "icpc"]:
        common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -xHost -mtune=native -unroll -g"
    elif conf.env.CXX_NAME in ["clang"]:
        common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -march=native -g "
    else:
        if int(conf.env['CC_VERSION'][0]+conf.env['CC_VERSION'][1]) < 47:
            common_flags = "-Wall -std=c++0x"
        else:
            common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -march=native -g "

    all_flags = common_flags + opt_flags
    conf.env['CXXFLAGS'] = conf.env['CXXFLAGS'] + all_flags.split(' ')
    print(conf.env['CXXFLAGS'])

def summary(bld):
    lst = getattr(bld, 'utest_results', [])
    total = 0
    tfail = 0
    if lst:
        total = len(lst)
        tfail = len([x for x in lst if x[1]])
    waf_unit_test.summary(bld)
    if tfail > 0:
        bld.fatal("Build failed, because some tests failed!")

def build(bld):
    if len(bld.env.INCLUDES_DART) == 0 or len(bld.env.INCLUDES_EIGEN) == 0 or len(bld.env.INCLUDES_BOOST) == 0:
        bld.fatal('Some libraries were not found! Cannot proceed!')

    if bld.options.tests:
        bld.recurse('src/tests')

    files = []
    for root, dirnames, filenames in os.walk(bld.path.abspath()+'/src/robot_dart/'):
        for filename in fnmatch.filter(filenames, '*.cpp'):
            files.append(os.path.join(root, filename))

    files = [f[len(bld.path.abspath())+1:] for f in files]
    robot_dart_srcs = " ".join(files)

    libs = 'BOOST EIGEN DART'
    libs_graphics = libs + ' DART_GRAPHIC'

    bld.program(features = 'cxx ' + bld.env['lib_type'],
                source = robot_dart_srcs,
                includes = './src',
                uselib = libs,
                target = 'RobotDARTSimu')

    if bld.get_env()['BUILD_GRAPHIC'] == True:
        bld.program(features = 'cxx',
                      install_path = None,
                      source = 'src/examples/pendulum.cpp',
                      includes = './src',
                      uselib = libs_graphics,
                      use = 'RobotDARTSimu',
                      defines = ['GRAPHIC'],
                      target = 'pendulum')

        bld.program(features = 'cxx',
                      install_path = None,
                      source = 'src/examples/arm.cpp',
                      includes = './src',
                      uselib = libs_graphics,
                      use = 'RobotDARTSimu',
                      defines = ['GRAPHIC'],
                      target = 'arm')

        bld.program(features = 'cxx',
                      install_path = None,
                      source = 'src/examples/cameras.cpp',
                      includes = './src',
                      uselib = libs_graphics,
                      use = 'RobotDARTSimu',
                      defines = ['GRAPHIC'],
                      target = 'cameras')

        bld.program(features = 'cxx',
                      install_path = None,
                      source = 'src/examples/tutorial.cpp',
                      includes = './src',
                      uselib = libs_graphics,
                      use = 'RobotDARTSimu',
                      defines = ['GRAPHIC'],
                      target = 'tutorial')

        # if we found the hexapod controller includes and Bullet collision
        if len(bld.env.INCLUDES_HEXAPOD_CONTROLLER) > 0 and 'BulletCollision' in bld.env.LIB_DART:
            bld.program(features = 'cxx',
                        install_path = None,
                        source = 'src/examples/hexapod.cpp',
                        includes = './src',
                        uselib = libs_graphics + ' HEXAPOD_CONTROLLER',
                        use = 'RobotDARTSimu',
                        defines = ['GRAPHIC'],
                        target = 'hexapod')

    bld.program(features = 'cxx',
                  install_path = None,
                  source = 'src/examples/pendulum.cpp',
                  includes = './src',
                  uselib = libs,
                  use = 'RobotDARTSimu',
                  target = 'pendulum_plain')

    bld.program(features = 'cxx',
                  install_path = None,
                  source = 'src/examples/arm.cpp',
                  includes = './src',
                  uselib = libs,
                  use = 'RobotDARTSimu',
                  target = 'arm_plain')

    bld.program(features = 'cxx',
                  install_path = None,
                  source = 'src/examples/tutorial.cpp',
                  includes = './src',
                  uselib = libs,
                  use = 'RobotDARTSimu',
                  target = 'tutorial_plain')

    # if we found the hexapod controller includes and Bullet collision
    if len(bld.env.INCLUDES_HEXAPOD_CONTROLLER) > 0 and 'BulletCollision' in bld.env.LIB_DART:
        bld.program(features = 'cxx',
                    install_path = None,
                    source = 'src/examples/hexapod.cpp',
                    includes = './src',
                    uselib = libs + ' HEXAPOD_CONTROLLER',
                    use = 'RobotDARTSimu',
                    target = 'hexapod_plain')

    bld.add_post_fun(summary)

    install_files = []
    for root, dirnames, filenames in os.walk(bld.path.abspath()+'/src/robot_dart/'):
        for filename in fnmatch.filter(filenames, '*.hpp'):
            install_files.append(os.path.join(root, filename))
    install_files = [f[len(bld.path.abspath())+1:] for f in install_files]

    for f in install_files:
        end_index = f.rfind('/')
        if end_index == -1:
            end_index = len(f)
        bld.install_files('${PREFIX}/include/' + f[4:end_index], f)
    if bld.env['lib_type'] == 'cxxstlib':
        bld.install_files('${PREFIX}/lib', blddir + '/libRobotDARTSimu.a')
    else:
        bld.install_files('${PREFIX}/lib', blddir + '/libRobotDARTSimu.so')
