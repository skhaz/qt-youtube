#include <algorithm>
#include <iterator>
#include <vlc/vlc.h>
#include "LibVlcInstance.h"

LibVlcInstance::LibVlcInstance(const std::vector<std::string> &arguments)
{
    std::vector<const char*> argv;
    std::transform(std::begin(arguments), std::end(arguments),
                   std::back_inserter(argv),
                   [](const std::string& str){ return str.c_str(); });
    m_instance = libvlc_new(arguments.size(), &argv[0]);
}

LibVlcInstance::~LibVlcInstance()
{
    libvlc_release(m_instance);
}

LibVlcInstance::operator libvlc_instance_t *()
{
    return m_instance;
}
