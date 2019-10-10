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
import corrade
import magnum
import magnum_integration


def options(opt):
    opt.load('compiler_cxx')
    opt.load('compiler_c')
    opt.load('boost')
    opt.load('eigen')
    opt.load('dart')
    opt.load('hexapod_controller')
    opt.load('corrade')
    opt.load('magnum')
    opt.load('magnum_integration')

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
    conf.load('avx')
    conf.load('corrade')
    conf.load('magnum')
    conf.load('magnum_integration')

    conf.check_boost(lib='regex system filesystem unit_test_framework', min_version='1.46')
    conf.check_eigen(required=True)
    conf.check_dart(required=True)
    conf.check_hexapod_controller()
    conf.check_corrade(components='Utility PluginManager')
    conf.env['magnum_dep_libs'] = 'MeshTools Primitives Shaders SceneGraph Sdl2Application'
    if conf.env['DEST_OS'] == 'darwin':
        conf.env['magnum_dep_libs'] += ' WindowlessCglApplication'
    else:
        conf.env['magnum_dep_libs'] += ' WindowlessGlxApplication'
    conf.check_magnum(components=conf.env['magnum_dep_libs'])
    conf.check_magnum_integration(components='Dart')

    if len(conf.env.INCLUDES_MagnumIntegration) > 0:
        conf.get_env()['BUILD_MAGNUM'] = True

    avx_dart = conf.check_avx(lib='dart', required=['dart', 'dart-utils', 'dart-utils-urdf'])

    native = ''
    native_icc = ''
    if avx_dart:
        conf.msg('-march=native (AVX support)', 'yes', color='GREEN')
        native = '-march=native'
        native_icc = 'mtune=native'
    else:
        conf.msg('-march=native (AVX support)', 'no (optional)', color='YELLOW')

    conf.env['lib_type'] = 'cxxstlib'
    if conf.options.build_shared:
        conf.env['lib_type'] = 'cxxshlib'

    if conf.env.CXX_NAME in ["icc", "icpc"]:
        common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -xHost -unroll -g " + native_icc
    elif conf.env.CXX_NAME in ["clang"]:
        common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -g -faligned-new " + native
    else:
        gcc_version = int(conf.env['CC_VERSION'][0]+conf.env['CC_VERSION'][1])
        if gcc_version < 47:
            common_flags = "-Wall -std=c++0x"
        else:
            common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -g " + native
        if gcc_version >= 71:
            opt_flags = opt_flags + " -faligned-new"

    all_flags = common_flags + opt_flags
    conf.env['CXXFLAGS'] = conf.env['CXXFLAGS'] + all_flags.split(' ')
    if len(conf.env.CXXFLAGS_DART) > 0:
        if '-std=c++11' in conf.env['CXXFLAGS']:
            conf.env['CXXFLAGS'].remove('-std=c++11')
        if '-std=c++0x' in conf.env['CXXFLAGS']:
            conf.env['CXXFLAGS'].remove('-std=c++11')
        conf.env['CXXFLAGS'] = conf.env['CXXFLAGS'] + conf.env.CXXFLAGS_DART
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

    path = bld.path.abspath() + '/res'

    files = []
    magnum_files = []
    for root, dirnames, filenames in os.walk(bld.path.abspath()+'/src/robot_dart/'):
        for filename in fnmatch.filter(filenames, '*.cpp'):
            ffile = os.path.join(root, filename)
            if 'robot_dart/gui/magnum' in ffile:
                magnum_files.append(ffile)
            else:
                files.append(ffile)

    files = [f[len(bld.path.abspath())+1:] for f in files]
    robot_dart_srcs = " ".join(files)
    magnum_files = [f[len(bld.path.abspath())+1:] for f in magnum_files]
    robot_dart_magnum_srcs = " ".join(magnum_files)

    libs = 'BOOST EIGEN DART'
    libs_graphics = libs + ' DART_GRAPHIC'

    bld.program(features = 'cxx ' + bld.env['lib_type'],
                source = robot_dart_srcs,
                includes = './src',
                uselib = libs,
                target = 'RobotDARTSimu')

    shaders_resource = corrade.corrade_add_resource(bld, name = 'RobotDARTShaders', config_file = 'src/robot_dart/gui/magnum/resources/resources.conf')

    bld.program(features = 'cxx ' + bld.env['lib_type'],
                source = robot_dart_magnum_srcs + ' ' + shaders_resource,
                includes = './src',
                uselib = magnum.get_magnum_dependency_libs(bld, bld.env['magnum_dep_libs']) + magnum_integration.get_magnum_integration_dependency_libs(bld, 'Dart') + libs,
                use = 'RobotDARTSimu',
                target = 'RobotDARTMagnum')

    if bld.get_env()['BUILD_MAGNUM'] == True:
        bld.program(features = 'cxx',
                      install_path = None,
                      source = 'src/examples/magnum.cpp',
                      includes = './src',
                      uselib = magnum.get_magnum_dependency_libs(bld, bld.env['magnum_dep_libs']) + magnum_integration.get_magnum_integration_dependency_libs(bld, 'Dart') + libs,
                      use = 'RobotDARTSimu RobotDARTMagnum',
                      defines = ['RESPATH="' + path + '"'],
                      target = 'magnum')

        bld.env.LIB_PTHREAD = ['pthread']

        bld.program(features = 'cxx',
                      install_path = None,
                      source = 'src/examples/magnum_contexts.cpp',
                      includes = './src',
                      uselib = 'PTHREAD ' + magnum.get_magnum_dependency_libs(bld, bld.env['magnum_dep_libs']) + magnum_integration.get_magnum_integration_dependency_libs(bld, 'Dart') + libs,
                      use = 'RobotDARTSimu RobotDARTMagnum',
                      defines = ['RESPATH="' + path + '"'],
                      target = 'magnum_contexts')

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

        bld.program(features = 'cxx',
                      install_path = None,
                      source = 'src/examples/meshes.cpp',
                      includes = './src',
                      uselib = libs_graphics,
                      use = 'RobotDARTSimu',
                      defines = ['GRAPHIC', 'RESPATH="' + path + '"'],
                      target = 'meshes')

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

    bld.program(features = 'cxx',
                  install_path = None,
                  source = 'src/examples/meshes.cpp',
                  includes = './src',
                  uselib = libs,
                  use = 'RobotDARTSimu',
                  defines = ['RESPATH="' + path + '"'],
                  target = 'meshes_plain')

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
            if filename in ["stb_image_write.h", "create_compatibility_shader.hpp"]:
                continue
            install_files.append(os.path.join(root, filename))
    install_files = [f[len(bld.path.abspath())+1:] for f in install_files]

    for f in install_files:
        end_index = f.rfind('/')
        if end_index == -1:
            end_index = len(f)
        bld.install_files('${PREFIX}/include/' + f[4:end_index], f)
    if bld.env['lib_type'] == 'cxxstlib':
        bld.install_files('${PREFIX}/lib', blddir + '/libRobotDARTSimu.a')
        bld.install_files('${PREFIX}/lib', blddir + '/libRobotDARTMagnum.a')
    else:
        bld.install_files('${PREFIX}/lib', blddir + '/libRobotDARTSimu.so')
        bld.install_files('${PREFIX}/lib', blddir + '/libRobotDARTMagnum.so')
