#ifndef UTHEQUE_HPP_
#define UTHEQUE_HPP_

#include <string>
#include <exception>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp> // will move to std::filesystem

namespace utheque {
    static constexpr char* UTHEQUE_DEFAULT_PREFIX = "/usr/local";
    /// return the directory  where to find the urdf (or urdf package) from the utheque (URDF library)
    /// if start by /, do nothing
    /// otherwise, search (in this order): 
    ///    - in the current directory
    ///    - in <current_directory>/utheque/ 
    ///    - $UTHEQUE_PATH
    ///    - in <prefix>/share/utheque
    /// @arg urdf or package name (e.g. talos/talos.urdf or talos_description)
    /// @arg prefix /usr/local/
    /// @return the full (absolute) path where to find the URDF (e.g. /usr/local/share/utheque/)
    static std::string directory(const std::string& filename, const std::string prefix = UTHEQUE_DEFAULT_PREFIX)
    {
        namespace fs = boost::filesystem;
        fs::path model_file(boost::trim_copy(filename));
        if (model_file.string()[0] == '/')
            return "/";

        // search current directory
        if (fs::exists(model_file))
            return fs::current_path().string();

        // search <current_directory>/robots
        if (fs::exists(fs::path("utheque") / model_file))
            return (fs::current_path() / fs::path("utheque")).string();

        // search $UTHEQUE_PATH
        const char* env = std::getenv("UTHEQUE_PATH");
        if (env != nullptr) {
            fs::path env_path(env);
            if (fs::exists(env_path / model_file))
                return env_path.string();
        }

        // search PREFIX/share/utheque
        fs::path system_path(prefix + "/share/utheque");
        if (fs::exists(system_path / model_file))
            return system_path.string();

        throw std::runtime_error(std::string("Utheque::could not find :") + filename);

        return std::string();
    }

    /// call directory() and put the filename back in the path
    /// @arg urdf or package name (e.g. talos/talos.urdf or talos_description)
    /// @arg prefix /usr/local/
    /// @return full path of the URDF file: (e.g. /usr/local/share/utheque/talos/talos.urdf)
    static std::string path(const std::string& filename, const std::string& prefix = UTHEQUE_DEFAULT_PREFIX) {
        namespace fs = boost::filesystem;
        auto file_dir = fs::path(directory(filename, prefix));
        auto model_file = file_dir / fs::path(boost::trim_copy(filename));
        return model_file.string();
    }
}

#endif