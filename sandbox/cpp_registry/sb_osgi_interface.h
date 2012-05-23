#ifndef sb_osgi_interface_h__
#define sb_osgi_interface_h__

template<class T>
inline const char* osgi_interface_id()
{
    return 0;
}


#define OSGI_DECLARE_SERVICE_INTERFACE(IFace) \
    template<> \
    inline const char* osgi_interface_id<IFace*>() \
    { \
    return IFace ## _NAME; \
    }

#endif // sb_osgi_interface_h__
