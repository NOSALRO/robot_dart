#! /usr/bin/env python
# encoding: utf-8
# Konstantinos Chatzilygeroudis - 2015

"""
Quick n dirty DART detection
"""

import os
from copy import deepcopy
from waflib.Configure import conf


def options(opt):
    opt.add_option('--dart', type='string', help='path to DART physics engine/sim', dest='dart')

@conf
def check_dart(conf, *k, **kw):
    def fail(msg, required):
        if required:
            conf.fatal(msg)
        conf.end_msg(msg, 'RED')
    def get_directory(filename, dirs):
        res = conf.find_file(filename, dirs)
        return res[:-len(filename)-1]

    required = kw.get('required', False)

    # OSX/Mac uses .dylib and GNU/Linux .so
    suffix = 'dylib' if conf.env['DEST_OS'] == 'darwin' else 'so'

    if conf.options.dart:
        includes_check = [conf.options.dart + '/include']
        libs_check = [conf.options.dart + '/lib']
    else:
        includes_check = ['/usr/local/include', '/usr/include', '/opt/local/include', '/sw/include', '/opt/homebrew/include']
        libs_check = ['/usr/lib', '/usr/local/lib64', '/usr/local/lib', '/opt/local/lib', '/sw/lib', '/lib', '/usr/lib64', '/usr/lib/x86_64-linux-gnu/', '/usr/local/lib/x86_64-linux-gnu/', '/usr/lib/aarch64-linux-gnu/', '/usr/local/lib/aarch64-linux-gnu/', '/opt/homebrew/lib']

        if 'RESIBOTS_DIR' in os.environ:
            includes_check = [os.environ['RESIBOTS_DIR'] + '/include'] + includes_check
            libs_check = [os.environ['RESIBOTS_DIR'] + '/lib'] + libs_check

    # DART has some optional Bullet features
    bullet_check = ['/usr/local/include/bullet', '/usr/include/bullet', '/opt/local/include/bullet', '/sw/include/bullet', '/opt/homebrew/include/bullet', '/usr/local/include', '/usr/include', '/opt/local/include', '/sw/include', '/opt/homebrew/include']
    bullet_libs = ['/usr/lib', '/usr/local/lib64', '/usr/local/lib', '/opt/local/lib', '/sw/lib', '/lib', '/usr/lib64', '/usr/lib/x86_64-linux-gnu/', '/usr/local/lib/x86_64-linux-gnu/', '/usr/lib/aarch64-linux-gnu/', '/usr/local/lib/aarch64-linux-gnu/', '/opt/homebrew/lib']
    bullet_include = []
    bullet_lib = []
    bullet_found = False
    try:
        bullet_include = [get_directory('btBulletCollisionCommon.h', bullet_check)]
        bullet_lib = []
        bullet_lib.append(get_directory('libLinearMath.' + suffix, bullet_libs))
        bullet_lib.append(get_directory('libBulletCollision.' + suffix, bullet_libs))
        bullet_lib = list(set(bullet_lib))
        bullet_found = True
    except:
        bullet_found = False

    # DART has some optional ODE features
    ode_check = ['/usr/local/include', '/usr/include', '/opt/local/include', '/sw/include', '/opt/homebrew/include']
    ode_libs = ['/usr/lib', '/usr/local/lib64', '/usr/local/lib', '/opt/local/lib', '/sw/lib', '/lib', '/usr/lib64', '/usr/lib/x86_64-linux-gnu/', '/usr/local/lib/x86_64-linux-gnu/', '/usr/lib/aarch64-linux-gnu/', '/usr/local/lib/aarch64-linux-gnu/', '/opt/homebrew/lib']
    ode_include = []
    ode_lib = []
    ode_found = False
    try:
        ode_include = [get_directory('ode/collision.h', ode_check)]
        try:
            ode_lib = [get_directory('libode.' + suffix, ode_libs)]
        except:
            ode_lib = [get_directory('libode.a' , ode_libs)]
        ode_found = True
    except:
        ode_found = False

    # DART has some optional Octomap dependency
    octomap_check = ['/usr/local/include', '/usr/include', '/opt/local/include', '/sw/include', '/opt/homebrew/include']
    octomap_libs = ['/usr/lib', '/usr/local/lib64', '/usr/local/lib', '/opt/local/lib', '/sw/lib', '/lib', '/usr/lib64', '/usr/lib/x86_64-linux-gnu/', '/usr/local/lib/x86_64-linux-gnu/', '/usr/lib/aarch64-linux-gnu/', '/usr/local/lib/aarch64-linux-gnu/', '/opt/homebrew/lib']
    if 'ROS_DISTRO' in os.environ:
        octomap_check.append('/opt/ros/' + os.environ['ROS_DISTRO'] + '/include')
        octomap_libs.append('/opt/ros/' + os.environ['ROS_DISTRO'] + '/lib')
    octomap_include = []
    octomap_lib = []
    octomap_found = False
    try:
        octomap_include = [get_directory('octomap/octomap.h', octomap_check)]
        octomap_lib = [get_directory('liboctomap.' + suffix, octomap_libs)]
        octomap_found = True
    except:
        octomap_found = False

    # DART has some URDF dependency
    urdfdom_check = ['/usr/local/include', '/usr/include', '/opt/local/include', '/sw/include', '/opt/homebrew/include']
    urdfdom_check = urdfdom_check + [i + '/urdfdom_headers' for i in urdfdom_check]
    urdfdom_libs = ['/usr/lib', '/usr/local/lib64', '/usr/local/lib', '/opt/local/lib', '/sw/lib', '/lib', '/usr/lib64', '/usr/lib/x86_64-linux-gnu/', '/usr/local/lib/x86_64-linux-gnu/', '/usr/lib/aarch64-linux-gnu/', '/usr/local/lib/aarch64-linux-gnu/', '/opt/homebrew/lib']
    if 'ROS_DISTRO' in os.environ:
        urdfdom_check.append('/opt/ros/' + os.environ['ROS_DISTRO'] + '/include')
        urdfdom_libs.append('/opt/ros/' + os.environ['ROS_DISTRO'] + '/lib')
    urdfdom_include = []
    urdfdom_lib = []
    urdfdom_found = False
    try:
        urdfdom_include = [get_directory('urdf_model/model.h', urdfdom_check)]
        # urdfdom_lib = [get_directory('liburdfdom_model.' + suffix, urdfdom_libs)]
        urdfdom_found = True
    except:
        urdfdom_found = False

    dart_load_prefix = 'utils'
    dart_include = []
    dart_major = -1
    dart_minor = -1
    dart_patch = -1
    # DART Configurations
    dart_have_nlopt = False
    dart_have_ipopt = False
    dart_have_pagmo = False
    dart_have_snopt = False
    dart_have_bullet = False
    dart_have_ode = False
    dart_have_flann = False
    dart_have_octomap = False

    try:
        conf.start_msg('Checking for DART includes (including io/urdf)')
        config_file = conf.find_file('dart/config.hpp', includes_check)
        with open(config_file) as f:
            config_content = f.readlines()
        for line in config_content:
            major = line.find('#define DART_MAJOR_VERSION')
            minor = line.find('#define DART_MINOR_VERSION')
            patch = line.find('#define DART_PATCH_VERSION')
            if major > -1:
                dart_major = int(line.split(' ')[-1].strip())
            if minor > -1:
                dart_minor = int(line.split(' ')[-1].strip())
            if patch > -1:
                dart_patch = int(line.split(' ')[-1].strip())

            nlopt = line.find('#define HAVE_NLOPT')
            ipopt = line.find('#define HAVE_IPOPT')
            pagmo = line.find('#define HAVE_PAGMO')
            snopt = line.find('#define HAVE_SNOPT')
            bullet = line.find('#define HAVE_BULLET')
            ode = line.find('#define HAVE_ODE')
            flann = line.find('#define HAVE_FLANN')
            octomap = line.find('#define HAVE_OCTOMAP')

            if nlopt > -1:
                boolean = int(line.split(' ')[-1].strip())
                dart_have_nlopt = (boolean == 1)
            if ipopt > -1:
                boolean = int(line.split(' ')[-1].strip())
                dart_have_ipopt = (boolean == 1)
            if pagmo > -1:
                boolean = int(line.split(' ')[-1].strip())
                dart_have_pagmo = (boolean == 1)
            if snopt > -1:
                boolean = int(line.split(' ')[-1].strip())
                dart_have_snopt = (boolean == 1)
            if bullet > -1:
                boolean = int(line.split(' ')[-1].strip())
                dart_have_bullet = (boolean == 1)
            if ode > -1:
                boolean = int(line.split(' ')[-1].strip())
                dart_have_ode = (boolean == 1)
            if flann > -1:
                boolean = int(line.split(' ')[-1].strip())
                dart_have_flann = (boolean == 1)
            if octomap > -1:
                boolean = int(line.split(' ')[-1].strip())
                dart_have_octomap = (boolean == 1)

        if dart_major < 6:
            raise Exception('We need DART version at least 6.0.0')
        if dart_major > 6:
            dart_load_prefix = 'io'
        dart_cxx_flags = ''
        if dart_major == 6 and dart_minor >= 9:
            dart_cxx_flags = '-std=c++14'
        if dart_major > 6 or (dart_major == 6 and dart_minor >= 13):
            dart_cxx_flags = '-std=c++17'

        dart_include = []
        if urdfdom_found:
            dart_include += urdfdom_include
        else:
            fail('urdfdom_headers not found', required)
        dart_include.append(get_directory('dart/dart.hpp', includes_check))
        dart_include.append(get_directory('dart/'+dart_load_prefix+'/'+dart_load_prefix+'.hpp', includes_check))
        dart_include.append(get_directory('dart/'+dart_load_prefix+'/urdf/urdf.hpp', includes_check))
        dart_include = list(set(dart_include))
        conf.end_msg(str(dart_major)+'.'+str(dart_minor)+'.'+str(dart_patch)+' in '+dart_include[0])

        conf.start_msg('Checking for DART libs (including io/urdf)')

        dart_lib = []
        dart_lib.append(get_directory('libdart.' + suffix, libs_check))
        dart_lib.append(get_directory('libdart-'+dart_load_prefix+'.' + suffix, libs_check))
        dart_lib.append(get_directory('libdart-'+dart_load_prefix+'-urdf.' + suffix, libs_check))
        dart_lib = list(set(dart_lib))
        conf.env.INCLUDES_DART = dart_include
        conf.env.LIBPATH_DART = dart_lib
        conf.env.LIB_DART = ['dart', 'dart-'+dart_load_prefix, 'dart-'+dart_load_prefix+'-urdf']
        if len(dart_cxx_flags) > 0:
            conf.env.CXXFLAGS_DART = [dart_cxx_flags]
        conf.end_msg(conf.env.LIB_DART)


        if dart_have_bullet:
            conf.start_msg('DART: Checking for Bullet Collision libs')
            if bullet_found:
                try:
                    bullet_lib.append(get_directory('libdart-collision-bullet.'+suffix, libs_check))
                    conf.env.INCLUDES_DART = conf.env.INCLUDES_DART + bullet_include
                    conf.env.LIBPATH_DART =  conf.env.LIBPATH_DART + bullet_lib
                    conf.env.LIB_DART.append('LinearMath')
                    conf.env.LIB_DART.append('BulletCollision')
                    conf.env.LIB_DART.append('dart-collision-bullet')
                    conf.end_msg('libs: ' + str(conf.env.LIB_DART[-3:]) + ', bullet: ' + str(bullet_include[0]))
                except:
                    fail('Not found', required)
            else:
                fail('Not found', required)

        if dart_have_ode:
            conf.start_msg('DART: Checking for Ode Collision libs')
            if ode_found:
                try:
                    ode_lib.append(get_directory('libdart-collision-ode.'+suffix, libs_check))
                    conf.env.INCLUDES_DART = conf.env.INCLUDES_DART + ode_include
                    conf.env.LIBPATH_DART =  conf.env.LIBPATH_DART + ode_lib
                    conf.env.LIB_DART.append('ode')
                    conf.env.LIB_DART.append('dart-collision-ode')
                    conf.end_msg('libs: ' + str(conf.env.LIB_DART[-2:]) + ', ode: ' + str(ode_include[0]))
                except:
                    fail('Not found', required)
            else:
                fail('Not found', required)

        if dart_have_octomap:
            conf.start_msg('DART: Checking for Octomap libs')
            if octomap_found:
                try:
                    conf.env.INCLUDES_DART = conf.env.INCLUDES_DART + octomap_include
                    conf.env.LIBPATH_DART =  conf.env.LIBPATH_DART + octomap_lib
                    conf.env.LIB_DART.append('octomap')
                    conf.end_msg('libs: ' + str(conf.env.LIB_DART[-1:]) + ', octomap: ' + str(octomap_include[0]))
                except:
                    fail('Not found', required)
            else:
                fail('Not found', required)

        # remove duplicates
        conf.env.INCLUDES_DART = list(set(conf.env.INCLUDES_DART))
        conf.env.LIBPATH_DART = list(set(conf.env.LIBPATH_DART))
    except:
        if dart_major < 6 and dart_major > -1:
            fail('We need DART >= 6.0.0', required)
        else:
            fail('Not found', required)
        return
    return 1
