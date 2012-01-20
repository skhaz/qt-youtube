
#ifndef _Instance_h
#define _Instance_h



struct libvlc_instance_t;

class MonoInstance
{
    friend class Instance;

    MonoInstance();

    ~MonoInstance();

    libvlc_instance_t * m_vlc_instance;
};

class Instance
{
    public:
        Instance();

        operator libvlc_instance_t *() { return m_instance.m_vlc_instance; }

    private:
        static MonoInstance m_instance;
};

#endif
