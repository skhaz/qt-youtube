#ifndef LIBVLCINSTANCE_H
#define LIBVLCINSTANCE_H

#include <vector>
#include <string>

struct libvlc_instance_t;

/// LibVlcInstance is a simple wrapper to libvlc_instance_t. It can be passed
/// as an argument to the library functions.
class LibVlcInstance
{
public:
    /// Creates a LibVlcInstance.
    LibVlcInstance(const std::vector<std::string>& arguments);

    /// Destroys a LibVlcInstance.
    ~LibVlcInstance();

    /// Returns the libvlc instance.
    operator libvlc_instance_t*();

private:
    /// Internal vlc instance.
    libvlc_instance_t* m_instance;
};

#endif // LIBVLCINSTANCE_H
