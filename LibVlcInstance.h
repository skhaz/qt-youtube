#ifndef LIBVLCINSTANCE_H
#define LIBVLCINSTANCE_H

#include <vector>
#include <string>

struct libvlc_instance_t;

class LibVlcInstance
{
public:
    LibVlcInstance(const std::vector<std::string>& arguments);
    ~LibVlcInstance();

    operator libvlc_instance_t*();

private:
    libvlc_instance_t* m_instance;
};

#endif // LIBVLCINSTANCE_H
