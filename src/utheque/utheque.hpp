#ifndef UTHEQUE_HPP_
#define UTHEQUE_HPP_

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp> // will move to std::filesystem
#include <exception>
#include <string>

namespace utheque {
#ifndef UTHEQUE_PREFIX
    static constexpr char* UTHEQUE_DEFAULT_PREFIX = const_cast<char*>("/usr/local");
#else
    static constexpr char* UTHEQUE_DEFAULT_PREFIX = const_cast<char*>(UTHEQUE_PREFIX);
#endif
    /// return the directory  where to find the urdf (or urdf package) from the utheque (URDF library)
    /// if start by /, do nothing
    /// otherwise, search (in this order):
    ///    - in the current directory
    ///    - in <current_directory>/utheque/
    ///    - $UTHEQUE_PATH
    ///    - in <prefix>/share/utheque
    /// The default prefix is got :
    ///     - from the cmake config (which defines UTHEQUE_PREFIX)
    ///     - otherwise from -DUTHEQUE_PREFIX="..."
    ///     - if no UTHEQUE_PREFIX, then use /usr/local
    /// @arg urdf or package name (e.g. talos/talos.urdf or talos_description)
    /// @arg prefix (default to UTHEQUE_PREFIX)
    /// @arg verbose print search paths
    /// @return the full (absolute) path where to find the URDF (e.g. /usr/local/share/utheque/)
    static std::string directory(const std::string& filename, const std::string prefix = UTHEQUE_DEFAULT_PREFIX, bool verbose = false)
    {
        namespace fs = boost::filesystem;
        fs::path model_file(boost::trim_copy(filename));
        if (verbose)
            std::cout << "utheque: searching for [" << model_file.string() << "]" << std::endl;

        if (model_file.string()[0] == '/')
            return "/";

        if (verbose)
            std::cout << "utheque: not an absolute path" << std::endl;

        // search current directory
        if (fs::exists(model_file))
            return fs::current_path().string();

        if (verbose)
            std::cout << "utheque: not found in current path [" << fs::current_path().string() << "]" << std::endl;

        // search <current_directory>/robots
        if (fs::exists(fs::path("utheque") / model_file))
            return (fs::current_path() / fs::path("utheque")).string();

        if (verbose)
            std::cout << "utheque: not found in current path/utheque [" << (fs::current_path() / fs::path("utheque")).string() << "]" << std::endl;

        // search $UTHEQUE_PATH
        const char* env = std::getenv("UTHEQUE_PATH");
        if (env != nullptr) {
            if (verbose)
                std::cout << "Utheque: $UTHEQUE_PATH: [" << env << "]" << std::endl;
            fs::path env_path(env);
            if (fs::exists(env_path / model_file))
                return env_path.string();
        }
        else if (verbose)
            std::cout << "Utheque: no $UTHEQUE_PATH" << std::endl;

        if (verbose)
            std::cout << "utheque: not found in $UTHEQUE_PATH" << std::endl;

        // search PREFIX/share/utheque
        fs::path system_path(prefix + "/share/utheque");
        if (fs::exists(system_path / model_file))
            return system_path.string();

        if (verbose)
            std::cout << "utheque: not found in [" << system_path.string() << "]" << std::endl;

        throw std::runtime_error(std::string("Utheque:: could not find: ") + filename);

        return std::string();
    }

    /// call directory() and put the filename back in the path
    /// @arg urdf or package name (e.g. talos/talos.urdf or talos_description)
    /// @arg prefix /usr/local/
    /// @return full path of the URDF file: (e.g. /usr/local/share/utheque/talos/talos.urdf)
    static std::string path(const std::string& filename, const std::string& prefix = UTHEQUE_DEFAULT_PREFIX, bool verbose = false)
    {
        namespace fs = boost::filesystem;
        auto file_dir = fs::path(directory(filename, prefix, verbose));
        auto model_file = file_dir / fs::path(boost::trim_copy(filename));
        return model_file.string();
    }
} // namespace utheque

#endif
