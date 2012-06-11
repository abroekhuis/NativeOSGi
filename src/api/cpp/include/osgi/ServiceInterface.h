#pragma once
#ifndef OSGI_SERVICEINTERFACE_H_
#define OSGI_SERVICEINTERFACE_H_

template<class T>
inline const char* osgi_service_interface_id()
{
    return 0;
}

#define OSGI_DECLARE_SERVICE_INTERFACE(IFace) \
    template<> \
    inline const char* osgi_service_interface_id<IFace*>() \
    { \
    return IFace ## _NAME; \
    }

#endif // OSGI_SERVICEINTERFACE_H_
