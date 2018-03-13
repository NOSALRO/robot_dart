#! /usr/bin/env python
# encoding: utf-8
# JB Mouret - 2009
# Federico Allocati - 2015

"""
Quick n dirty eigen3 detection
"""

import os, glob, types
import subprocess
from waflib.Configure import conf


def options(opt):
    opt.add_option('--eigen', type='string', help='path to eigen', dest='eigen')
    opt.add_option('--lapacke_blas', action='store_true', help='enable lapacke/blas if found (required Eigen>=3.3)', dest='lapacke_blas')


@conf
def check_eigen(conf):
    conf.start_msg('Checking for Eigen')
    includes_check = ['/usr/include/eigen3', '/usr/local/include/eigen3', '/usr/include', '/usr/local/include']

    if conf.options.eigen:
        includes_check = [conf.options.eigen]

    try:
        res = conf.find_file('Eigen/Core', includes_check)
        incl = res[:-len('Eigen/Core')-1]
        conf.env.INCLUDES_EIGEN = [incl]
        conf.end_msg(incl)
        if conf.options.lapacke_blas:
            conf.start_msg('Checking for LAPACKE/BLAS (optional)')
            p1 = subprocess.Popen(["cat", incl+"/Eigen/src/Core/util/Macros.h"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            p2 = subprocess.Popen(["grep", "#define EIGEN_WORLD_VERSION"], stdin=p1.stdout, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            p1.stdout.close()
            out1, err = p2.communicate()
            p1 = subprocess.Popen(["cat", incl+"/Eigen/src/Core/util/Macros.h"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            p2 = subprocess.Popen(["grep", "#define EIGEN_MAJOR_VERSION"], stdin=p1.stdout, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            p1.stdout.close()
            out2, err = p2.communicate()
            out1 = out1.decode('UTF-8')
            out2 = out2.decode('UTF-8')
            world_version = int(out1.strip()[-1])
            major_version = int(out2.strip()[-1])

            if world_version == 3 and major_version >= 3:
                # Check for lapacke and blas
                extra_libs = ['/usr/lib', '/usr/local/lib', '/usr/local/opt/openblas/lib']
                blas_libs = ['blas', 'openblas']
                blas_lib = ''
                blas_path = ''
                for b in blas_libs:
                    try:
                        if conf.env['DEST_OS']=='darwin':
                            res = conf.find_file('lib'+b+'.dylib', extra_libs)
                            blas_path = res[:-len('lib'+b+'.dylib')-1]
                        else:
                            res = conf.find_file('lib'+b+'.so', extra_libs)
                            blas_path = res[:-len('lib'+b+'.so')-1]
                    except:
                        continue
                    blas_lib = b
                    break

                lapacke = False
                lapacke_path = ''
                try:
                    if conf.env['DEST_OS']=='darwin':
                            res = conf.find_file('liblapacke.dylib', extra_libs)
                            lapacke_path = res[:-len('liblapacke.dylib')-1]
                    else:
                            res = conf.find_file('liblapacke.so', extra_libs)
                            lapacke_path = res[:-len('liblapacke.so')-1]
                    lapacke = True
                except:
                    lapacke = False

                if lapacke or blas_lib != '':
                    conf.env.DEFINES_EIGEN = []
                    if lapacke_path != blas_path:
                        conf.env.LIBPATH_EIGEN = [lapacke_path, blas_path]
                    else:
                        conf.env.LIBPATH_EIGEN = [lapacke_path]
                    conf.env.LIB_EIGEN = []
                    conf.end_msg('LAPACKE: \'%s\', BLAS: \'%s\'' % (lapacke_path, blas_path))
                elif lapacke:
                    conf.end_msg('Found only LAPACKE: %s' % lapacke_path, 'YELLOW')
                elif blas_lib != '':
                    conf.end_msg('Found only BLAS: %s' % blas_path, 'YELLOW')
                else:
                    conf.end_msg('Not found in %s' % str(extra_libs), 'RED')
                if lapacke:
                    conf.env.DEFINES_EIGEN.append('EIGEN_USE_LAPACKE')
                    conf.env.LIB_EIGEN.append('lapacke')
                if blas_lib != '':
                    conf.env.DEFINES_EIGEN.append('EIGEN_USE_BLAS')
                    conf.env.LIB_EIGEN.append(blas_lib)
            else:
                conf.end_msg('LAPACKE/BLAS can be used only with Eigen>=3.3', 'RED')
    except:
        conf.fatal('Not found in %s' % str(includes_check))
    return 1
