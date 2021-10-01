#ifndef ROBOT_DART_UTILS_HEADERS_DART_COLLISION_HPP
#define ROBOT_DART_UTILS_HEADERS_DART_COLLISION_HPP

#pragma GCC system_header

#include <dart/config.hpp>

#include <dart/collision/CollisionFilter.hpp>
#include <dart/collision/CollisionObject.hpp>
#include <dart/collision/dart/DARTCollisionDetector.hpp>
#include <dart/collision/fcl/FCLCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>

#if (HAVE_BULLET == 1)
#include <dart/collision/bullet/BulletCollisionDetector.hpp>
#endif

#if (HAVE_ODE == 1)
#include <dart/collision/ode/OdeCollisionDetector.hpp>
#endif

#endif
