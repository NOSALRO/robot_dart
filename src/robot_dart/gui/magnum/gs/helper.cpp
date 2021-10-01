#include "helper.hpp"

#include <Corrade/Containers/ArrayViewStl.h>
#include <Corrade/Containers/StridedArrayView.h>
#include <Corrade/Utility/Algorithms.h>

#include <Magnum/Math/Color.h>
#include <Magnum/Math/PackingBatch.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                Image rgb_from_image(Magnum::Image2D* image)
                {
                    Image img;

                    img.width = image->size().x();
                    img.height = image->size().y();
                    img.channels = 3;
                    img.data.resize(image->size().product() * sizeof(Magnum::Color3ub));
                    Corrade::Containers::StridedArrayView2D<const Magnum::Color3ub> src = image->pixels<Magnum::Color3ub>().flipped<0>();
                    Corrade::Containers::StridedArrayView2D<Magnum::Color3ub> dst{Corrade::Containers::arrayCast<Magnum::Color3ub>(Corrade::Containers::arrayView(img.data)), {std::size_t(image->size().y()), std::size_t(image->size().x())}};
                    Corrade::Utility::copy(src, dst);

                    return img;
                }

                GrayscaleImage depth_from_image(Magnum::Image2D* image, bool linearize, Magnum::Float near_plane, Magnum::Float far_plane)
                {
                    GrayscaleImage img;

                    img.width = image->size().x();
                    img.height = image->size().y();
                    img.data.resize(image->size().product() * sizeof(uint8_t));

                    std::vector<Magnum::Float> data = std::vector<Magnum::Float>(image->size().product());
                    Corrade::Containers::StridedArrayView2D<const Magnum::Float> src = image->pixels<Magnum::Float>().flipped<0>();
                    Corrade::Containers::StridedArrayView2D<Magnum::Float> dst{Corrade::Containers::arrayCast<Magnum::Float>(Corrade::Containers::arrayView(data)), {std::size_t(image->size().y()), std::size_t(image->size().x())}};
                    Corrade::Utility::copy(src, dst);

                    if (linearize) {
                        for (auto& depth : data)
                            depth = (2.f * near_plane) / (far_plane + near_plane - depth * (far_plane - near_plane));
                    }

                    Corrade::Containers::StridedArrayView2D<uint8_t> new_dst{Corrade::Containers::arrayCast<uint8_t>(Corrade::Containers::arrayView(img.data)), {std::size_t(image->size().y()), std::size_t(image->size().x())}};

                    Magnum::Math::packInto(dst, new_dst);

                    return img;
                }

                DepthImage depth_array_from_image(Magnum::Image2D* image, Magnum::Float near_plane, Magnum::Float far_plane)
                {
                    DepthImage img;
                    img.width = image->size().x();
                    img.height = image->size().y();

                    std::vector<Magnum::Float> data = std::vector<Magnum::Float>(image->size().product());
                    Corrade::Containers::StridedArrayView2D<const Magnum::Float> src = image->pixels<Magnum::Float>().flipped<0>();
                    Corrade::Containers::StridedArrayView2D<Magnum::Float> dst{Corrade::Containers::arrayCast<Magnum::Float>(Corrade::Containers::arrayView(data)), {std::size_t(image->size().y()), std::size_t(image->size().x())}};
                    Corrade::Utility::copy(src, dst);

                    img.data = std::vector<double>(data.begin(), data.end());

                    double zNear = static_cast<double>(near_plane);
                    double zFar = static_cast<double>(far_plane);

                    // zNear * zFar / (zFar + d * (zNear - zFar));
                    for (auto& depth : img.data)
                        // depth = (2. * zNear) / (zFar + zNear - depth * (zFar - zNear));
                        depth = (zNear * zFar) / (zFar - depth * (zFar - zNear));

                    return img;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart